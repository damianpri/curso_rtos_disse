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

extern xSemaphoreHandle recurso_mutex;

void tarea_roja(void *pvParameters) {

	portTickType ui32WakeTime;
	uint32_t TiempoDeRepuesta;
	uint32_t CantidadDePeriodos = 0;

	vTaskSetApplicationTaskTag( NULL, (pdTASK_HOOK_CODE) LED_ROJO);

	ui32WakeTime = 0;

	while(1) {

		++CantidadDePeriodos;

		UARTprintf("TAREA_ROJA[%d]: Ejecutando.\n",CantidadDePeriodos);

		//Delay
		ROM_SysCtlDelay(50000000/3);

		xSemaphoreTake(recurso_mutex, portMAX_DELAY);
		UARTprintf("TAREA_ROJA[%d]: Recurso alojado.\n",CantidadDePeriodos);

		//Delay
		ROM_SysCtlDelay(2*50000000/3);

		UARTprintf("TAREA_ROJA[%d]: Recurso liberado.\n",CantidadDePeriodos);
		xSemaphoreGive(recurso_mutex);


		TiempoDeRepuesta = xTaskGetTickCount() / portTICK_RATE_MS;

		if(TiempoDeRepuesta > CantidadDePeriodos*10000){
			UARTprintf("TAREA_ROJA[%d]: Se perdio el deadline. Tiempo de Repuesta %d.\n",CantidadDePeriodos,TiempoDeRepuesta);
		}

		vTaskDelayUntil(&ui32WakeTime, 10000 / portTICK_RATE_MS);

	}
}

void tarea_verde(void *pvParameters) {

	portTickType ui32WakeTime;
	uint32_t TiempoDeRepuesta;
	uint32_t CantidadDePeriodos = 0;

	vTaskSetApplicationTaskTag( NULL, (pdTASK_HOOK_CODE) LED_VERDE);

	ui32WakeTime = 0;

	while(1) {

		++CantidadDePeriodos;

		UARTprintf("TAREA_VERDE[%d]: Ejecutando.\n",CantidadDePeriodos);

		//Delay
		ROM_SysCtlDelay(4*50000000/3);

		TiempoDeRepuesta = xTaskGetTickCount() / portTICK_RATE_MS;

		if(TiempoDeRepuesta > CantidadDePeriodos*15000){
			UARTprintf("TAREA_VERDE[%d]: Se perdio el deadline. Tiempo de Repuesta %d.\n",CantidadDePeriodos,TiempoDeRepuesta);
		}

		vTaskDelayUntil(&ui32WakeTime, 15000 / portTICK_RATE_MS);

	}
}

void tarea_azul(void *pvParameters) {

	portTickType ui32WakeTime;
	uint32_t TiempoDeRepuesta;
	uint32_t CantidadDePeriodos = 0;

	vTaskSetApplicationTaskTag( NULL, (pdTASK_HOOK_CODE) LED_AZUL);

	ui32WakeTime = 0;

	while(1) {

		++CantidadDePeriodos;

		UARTprintf("TAREA_AZUL[%d]: Ejecutando.\n",CantidadDePeriodos);

		xSemaphoreTake(recurso_mutex, portMAX_DELAY);
		UARTprintf("TAREA_AZUL[%d]: Recurso alojado.\n",CantidadDePeriodos);

		//Delay
		ROM_SysCtlDelay(8*50000000/3);

		UARTprintf("TAREA_AZUL[%d]: Recurso liberado.\n",CantidadDePeriodos);
		xSemaphoreGive(recurso_mutex);


		//Delay
		ROM_SysCtlDelay(2*50000000/3);

		TiempoDeRepuesta = xTaskGetTickCount() / portTICK_RATE_MS;

		if(TiempoDeRepuesta > CantidadDePeriodos*35000){
			UARTprintf("TAREA_AZUL[%d]: Se perdio el deadline. Tiempo de Repuesta %d.\n",CantidadDePeriodos,TiempoDeRepuesta);
		}

		vTaskDelayUntil(&ui32WakeTime, 35000 / portTICK_RATE_MS);

	}
}
