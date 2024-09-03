#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

// Define pinos e constantes
#define LED_PIN 15 // Pin do LED (substitua pelo pino correto do seu LED AZUL)
#define ADC_PIN 26 // Pin do ADC conectado ao potenciômetro
#define ADC_THRESHOLD 2000 // Limite para acender o LED
#define ADC_READ_INTERVAL pdMS_TO_TICKS(300) // Intervalo de leitura do ADC em ticks

// Cria uma handle para a fila
QueueHandle_t adcQueue;

// Função da tarefa de leitura do ADC
void adcTask(void *pvParameters) {
    uint16_t adcValue;

    while (1) {
        // Lê o valor do ADC
        adcValue = adc_read();
        printf("%d\n", adcValue);

        // Envia o valor lido para a fila
        xQueueSend(adcQueue, &adcValue, portMAX_DELAY);

        // Espera pelo próximo ciclo de leitura
        vTaskDelay(ADC_READ_INTERVAL);
    }
}

// Função da tarefa de controle do LED
void ledTask(void *pvParameters) {
    uint16_t adcValue;

    while (1) {
        // Recebe o valor do ADC da fila
        if (xQueueReceive(adcQueue, &adcValue, portMAX_DELAY) == pdPASS) {
            // Verifica o valor do ADC e controla o LED
            if (adcValue > ADC_THRESHOLD) {
                gpio_put(LED_PIN, 1); // Acende o LED
            } else {
                gpio_put(LED_PIN, 0); // Apaga o LED
            }
        }
    }
}

int main() {
    // Inicializa a stdio
    stdio_init_all();

    // Inicializa o ADC
    adc_init();
    adc_gpio_init(ADC_PIN);
    adc_select_input(0);

    // Inicializa o GPIO do LED
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    // Cria a fila
    adcQueue = xQueueCreate(10, sizeof(uint16_t));
    if (adcQueue == NULL) {
        printf("Erro ao criar a fila.\n");
        return 1;
    }

    // Cria as tarefas
    xTaskCreate(adcTask, "ADC Task", 256, NULL, 1, NULL);
    xTaskCreate(ledTask, "LED Task", 256, NULL, 1, NULL);

    // Inicia o scheduler do FreeRTOS
    vTaskStartScheduler();

    // O código não deve chegar aqui
    while (1) {
        // Loop infinito caso o scheduler falhe
    }

    return 0;
}
