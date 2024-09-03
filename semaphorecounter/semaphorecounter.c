#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "pico/stdlib.h"


#define LED_PIN 14
#define LED_PINTWO 13
#define BUTTON_PIN 15


SemaphoreHandle_t xCountingSemaphore;

void vButtonTask(void *pvParameters) {

    while (1) {

        if (gpio_get(BUTTON_PIN) == 0) {
            xSemaphoreGive(xCountingSemaphore);

            // Aguarda um pouco para evitar múltiplas contagens com uma única pressão de botão
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void vLedTask(void *pvParameters) {

    while (1) {

        if (xSemaphoreTake(xCountingSemaphore, portMAX_DELAY) == pdTRUE) {
                gpio_put(LED_PIN, 1);
                gpio_put(LED_PINTWO, 1);
                vTaskDelay(pdMS_TO_TICKS(1000));
                gpio_put(LED_PIN, 0);
                gpio_put(LED_PINTWO, 0);


        }
    }
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


    xCountingSemaphore = xSemaphoreCreateCounting(10, 0);


    xTaskCreate(vButtonTask, "Button Task", 256, NULL, 1, NULL);
    xTaskCreate(vLedTask, "LED Task", 256, NULL, 1, NULL);




    vTaskStartScheduler();


    while (1) {}
}

int main() {
    app_main();
    return 0;
}
