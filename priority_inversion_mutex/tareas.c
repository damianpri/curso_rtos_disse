#include <stdint.h>
#include <stdbool.h>

#include "driverlib/rom.h"
#include "driverlib/gpio.h"

#include "inc/hw_memmap.h"

#include "utils/uartstdio.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#define LEDS (GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3)
#define LED_ROJO GPIO_PIN_1
#define LED_AZUL GPIO_PIN_2
#define LED_VERDE GPIO_PIN_3

extern xSemaphoreHandle uart_mutex;

void tarea_roja(void *pvParameters) {

	portTickType ui32WakeTime;

	vTaskSetApplicationTaskTag( NULL, (pdTASK_HOOK_CODE) LED_ROJO);

	ui32WakeTime = 0;

	while(1) {

		//Delay
		ROM_SysCtlDelay(50000000/3);

		xSemaphoreTake(uart_mutex, portMAX_DELAY);
		UARTprintf("TAREA_ROJA: Recurso UART alojado./n");

		//Delay
		ROM_SysCtlDelay(2*50000000/3);

		xSemaphoreGive(uart_mutex);

		vTaskDelayUntil(&ui32WakeTime, 10000 / portTICK_RATE_MS);

	}
}

void tarea_verde(void *pvParameters) {

	portTickType ui32WakeTime;

	vTaskSetApplicationTaskTag( NULL, (pdTASK_HOOK_CODE) LED_VERDE);

	ui32WakeTime = 0;

	while(1) {

		//Delay
		ROM_SysCtlDelay(4*50000000/3);

		vTaskDelayUntil(&ui32WakeTime, 15000 / portTICK_RATE_MS);

	}
}

void tarea_azul(void *pvParameters) {

	portTickType ui32WakeTime;

	vTaskSetApplicationTaskTag( NULL, (pdTASK_HOOK_CODE) LED_AZUL);

	ui32WakeTime = 0;

	while(1) {

		xSemaphoreTake(uart_mutex, portMAX_DELAY);
		UARTprintf("TAREA_AZUL: Recurso UART alojado./n");

		//Delay
		ROM_SysCtlDelay(8*50000000/3);

		xSemaphoreGive(uart_mutex);

		//Delay
		ROM_SysCtlDelay(2*50000000/3);

		vTaskDelayUntil(&ui32WakeTime, 35000 / portTICK_RATE_MS);

	}
}
