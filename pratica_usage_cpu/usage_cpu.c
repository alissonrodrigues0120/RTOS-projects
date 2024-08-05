#include "FreeRTOS.h"
#include "task.h"
#include "pico/stdlib.h"
#include "stdio.h"


TaskHandle_t cpuConsumingTaskHandler;

static unsigned long int tick_counter = 0;    






void vApplicationIdleHook(void){
    unsigned long int tick = xTaskGetTickCount();
    while (xTaskGetTickCount() == tick);
    tick_counter++;                        
}

void taskCPUusage(void *pvParameters){
    unsigned long int idle_tick_last, ticks;
    idle_tick_last = tick_counter = 0;
    for (;;) {
        vTaskDelay(3000 / portTICK_PERIOD_MS);
        if (tick_counter > idle_tick_last)
            ticks = tick_counter - idle_tick_last;          
        else
            ticks = 0xFFFFFFFF - idle_tick_last + tick_counter;
        ticks /= 3;

        
        printf("%lu idle ticks p/s (de %lu)\n", ticks, configTICK_RATE_HZ);

        
        unsigned long int cpu_usage = ((configTICK_RATE_HZ - ticks) * 100) / configTICK_RATE_HZ;
        printf("Usage CPU: %lu%%\n", cpu_usage);

        
        idle_tick_last = tick_counter;
    }
}


void taskUseCPU(void *pvParameters){
    unsigned int i, j;
    for (;;) {
        for (i = 0, j = 0; i < 100000; i++){
            j *= i + 10;
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void taskUseCPU2(void *pvParameters){
    unsigned int i, j;
    for (;;) {
        for (i = 0, j = 0; i < 100000; i++){
            j *= i + 10;
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void taskUseCPU3(void *pvParameters){
    unsigned int i, j;
    for (;;) {
        for (i = 0, j = 0; i < 100000; i++){
            j *= i + 10;
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void taskUseCPU4(void *pvParameters){
    unsigned int i, j;
    for (;;) {
        for (i = 0, j = 0; i < 100000; i++){
            j *= i + 10;
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

int main(void){
    stdio_init_all();
    xTaskCreate(taskCPUusage, "Task CPU Usage", configMINIMAL_STACK_SIZE * 4, NULL, 1, NULL);
    xTaskCreate(taskUseCPU, "TaskUseCPU", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    xTaskCreate(taskUseCPU2, "TaskUseCPU2", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    xTaskCreate(taskUseCPU3, "TaskUseCPU3", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    xTaskCreate(taskUseCPU4, "TaskUseCPU4", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    
    vTaskStartScheduler();

    while (true){}
}
