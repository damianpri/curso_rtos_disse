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

#define C_TAREA_ROJA 500
#define T_TAREA_ROJA 2000
#define C_TAREA_VERDE 500
#define T_TAREA_VERDE 4000
#define SYSCLOCK 50000000

extern xSemaphoreHandle led_rgb_mutex;
extern xSemaphoreHandle uart_mutex;

void tarea_roja(void *pvParameters) {

	portTickType ui32WakeTime;

	ui32WakeTime = 0;

	while(1) {

		UARTprintf("TAREA_ROJA: Recursos LedRGB antes de alojar.\n");
		xSemaphoreTake(led_rgb_mutex, portMAX_DELAY);
		UARTprintf("TAREA_ROJA: Recursos LedRGB despues de alojar.\n");
		ROM_GPIOPinWrite(GPIO_PORTF_BASE, LEDS, LED_ROJO);

		UARTprintf("TAREA_ROJA: Recursos UART antes de alojar.\n");

		//Delay
		ROM_SysCtlDelay(0.250*50000000/3);

		xSemaphoreTake(uart_mutex, portMAX_DELAY);
		UARTprintf("TAREA_ROJA: Recursos UART despues de alojar.\n");

		//Delay
		ROM_SysCtlDelay(0.250*50000000/3);  //250ms

		xSemaphoreGive(uart_mutex);

		ROM_GPIOPinWrite(GPIO_PORTF_BASE, LEDS, ~LEDS);
		xSemaphoreGive(led_rgb_mutex);

		vTaskDelayUntil(&ui32WakeTime, 1000 / portTICK_RATE_MS); //	vTaskDelayUntil(&ui32WakeTime, 1000 / portTICK_RATE_MS); //Periodo 2000ms 2SEG

	}
}

void tarea_verde(void *pvParameters) {

	portTickType ui32WakeTime;
	ui32WakeTime = 0;

	while(1) {
		xSemaphoreTake(uart_mutex, portMAX_DELAY);
		UARTprintf("TAREA_VERDE: Recurso UART alojado. Por alojar recurso LedRGB.\n");

		//Delay
		ROM_SysCtlDelay(1*50000000/3); //1Seg

		UARTprintf("TAREA_VERDE: Recurso LED antes de alojar.\n");
		xSemaphoreTake(led_rgb_mutex, portMAX_DELAY);
		UARTprintf("TAREA_VERDE: Recurso LED Despues de alojar.\n");
		ROM_GPIOPinWrite(GPIO_PORTF_BASE, LEDS, LED_VERDE);

		//Delay
		ROM_SysCtlDelay(1*50000000/3);
		ROM_GPIOPinWrite(GPIO_PORTF_BASE, LEDS, ~LEDS);
		xSemaphoreGive(led_rgb_mutex);
		xSemaphoreGive(uart_mutex);
		vTaskDelayUntil(&ui32WakeTime, 4000 / portTICK_RATE_MS);
	}
}

