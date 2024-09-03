#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "pico/stdlib.h"


#define LED_PIN 14
#define LED_PINTWO 13
#define BUTTON_PIN 15


typedef struct {
    bool ledState;
} LedCommand_t;

// Handle para a fila e o semáforo
QueueHandle_t xLedQueue;
SemaphoreHandle_t xButtonSemaphore;

void vButtonTask(void *pvParameters) {
    LedCommand_t xLedCommand;

    while (1) {
        // Espera o semáforo ser liberado pela interrupção
        if (xSemaphoreTake(xButtonSemaphore, portMAX_DELAY) == pdTRUE) {
            // Prepara a mensagem para enviar pela fila
            xLedCommand.ledState = true;


            xQueueSend(xLedQueue, &xLedCommand, 0);
        }
    }
}

void vLedTask(void *pvParameters) {
    LedCommand_t xReceivedCommand;

    while (1) {
        // Espera receber uma mensagem da fila
        if (xQueueReceive(xLedQueue, &xReceivedCommand, portMAX_DELAY) == pdTRUE) {
            // Verifica o comando recebido e controla o LED
            if (xReceivedCommand.ledState) {
                gpio_put(LED_PIN, 1);
                gpio_put(LED_PINTWO, 1);
                vTaskDelay(pdMS_TO_TICKS(500));
                gpio_put(LED_PIN, 0);
                gpio_put(LED_PINTWO, 0);

            }
        }
    }
}

void vButtonISR(uint gpio, uint32_t events) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    xSemaphoreGiveFromISR(xButtonSemaphore, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void app_main(void) {

    stdio_init_all();


    gpio_init(LED_PIN);
    gpio_init(LED_PINTWO);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_set_dir(LED_PINTWO, GPIO_OUT);

    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);


    xLedQueue = xQueueCreate(10, sizeof(LedCommand_t));
    xButtonSemaphore = xSemaphoreCreateBinary();


    xTaskCreate(vButtonTask, "Button Task", 256, NULL, 1, NULL);
    xTaskCreate(vLedTask, "LED Task", 256, NULL, 1, NULL);


    gpio_set_irq_enabled_with_callback(BUTTON_PIN, GPIO_IRQ_EDGE_FALL, true, &vButtonISR);


    vTaskStartScheduler();


    while (1) {}
}

int main() {
    app_main();
    return 0;
}
