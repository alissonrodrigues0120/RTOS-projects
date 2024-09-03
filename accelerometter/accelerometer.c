#include "FreeRTOS.h"
#include "stdio.h"
#include "stdlib.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "pico/stdlib.h"


QueueHandle_t accelerometerQueue;
static SemaphoreHandle_t mutex;


int16_t elementax, elementay, elementaz;


// Função para gerar dados aleatórios
void generate_random_acceleration(int16_t *ax, int16_t *ay, int16_t *az) {
    *ax = rand() % 1024 - 512; // Gera valores entre -512 e 511
    *ay = rand() % 1024 - 512;
    *az = rand() % 1024 - 512;
}

// Tarefa para gerar dados simulados do acelerômetro
void vTaskSimulateAccelerometer(void *pvParameters) {

    while(1) {
        if(xSemaphoreTake(mutex, 0) == pdTRUE){
        generate_random_acceleration(&elementax, &elementay, &elementaz);
        printf("Loading...\n\n");
        xSemaphoreGive(mutex);
        vTaskDelay(pdMS_TO_TICKS(50));  // Aguarda 50ms
        }
    }
}

// Tarefa para processar dados do acelerômetro
void vTaskProcessData(void *pvParameters) {
    while(1) {
        if(xSemaphoreTake(mutex, 0)==pdTRUE){
              // Processa os dados simulados
              printf("X: %d, Y: %d, Z: %d\n", elementax, elementay, elementaz);
              xSemaphoreGive(mutex);
              vTaskDelay(pdMS_TO_TICKS(1000));
      }
    }
}

int main() {
    stdio_init_all();

    accelerometerQueue = xQueueCreate(10, sizeof(int16_t));
    mutex = xSemaphoreCreateMutex();

    xTaskCreate(vTaskSimulateAccelerometer, "SimulateAccelerometer", 256, NULL, 2, NULL);
    xTaskCreate(vTaskProcessData, "ProcessData", 256, NULL, 1, NULL);

    vTaskStartScheduler();

    while (1) {}
}
