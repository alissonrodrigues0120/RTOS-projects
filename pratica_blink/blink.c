#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include "pico/stdlib.h"
#define mainRUN_ON_CORE 0
#define LED_PIN 15
#define LED_TWO 14




void taskone(void *pvParameters){
	(void) pvParameters;
        for (;;) {

            gpio_put(LED_TWO, 1);

            vTaskDelay(500);

            gpio_put(LED_TWO, 0);

            vTaskDelay(500);
   }


}


void tasktwo(void *pvParameters){
        (void) pvParameters;
        for(int i =0;i <= 2;i++){
                vTaskDelay(500);
                gpio_put(LED_PIN, 1);
                vTaskDelay(500);
		gpio_put(LED_PIN, 0);

	}

	vTaskDelete(NULL);

	

}







void main() {
	stdio_init_all();
	gpio_init(PICO_DEFAULT_LED_PIN);
	gpio_init(LED_PIN);
        gpio_set_dir(LED_PIN, GPIO_OUT);
	gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

	xTaskCreate(taskone, "Task 1", 128, NULL, 1, NULL);
	xTaskCreate(tasktwo, "Task 2", 128, NULL, 1, NULL);

	vTaskStartScheduler();




}
