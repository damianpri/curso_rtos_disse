#include <stdint.h>
#include <stdbool.h>

#include "driverlib/sysctl.h"
#include "driverlib/rom.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/uart.h"


#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"

#include "utils/uartstdio.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#define LEDTASKSTACKSIZE        128         // Stack size in words

#define LEDS (GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3)
#define LED_ROJO GPIO_PIN_1
#define LED_AZUL GPIO_PIN_2
#define LED_VERDE GPIO_PIN_3
#define BOTONES (GPIO_PIN_0 | GPIO_PIN_4)
#define BOTON_IZQUIERDO GPIO_PIN_4
#define BOTON_DERECHO GPIO_PIN_0

void ConfigClock(void) {
	// Set the clocking to run at 50 MHz from the PLL.
	ROM_SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
}

void ConfigUART(void) {
    //
    // Enable the GPIO Peripheral used by the UART.
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //
    // Enable UART0
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    //
    // Configure GPIO Pins for UART mode.
    //
    ROM_GPIOPinConfigure(GPIO_PA0_U0RX);
    ROM_GPIOPinConfigure(GPIO_PA1_U0TX);
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // Use the internal 16MHz oscillator as the UART clock source.
    //
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

    //
    // Initialize the UART for console I/O.
    //
    UARTStdioConfig(0, 115200, 16000000);
}

void ConfigBotonesYLeds(void) {
    // Enable the GPIO port to which the pushbuttons and leds are connected.
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    // Unlock PF0 so we can change it to a GPIO input
    // Once we have enabled (unlocked) the commit register then re-lock it
    // to prevent further changes.  PF0 is muxed with NMI thus a special case.
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;

    // Set each of the button GPIO pins as an input with a pull-up.
    ROM_GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4, GPIO_DIR_MODE_IN);
    ROM_GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4,
                         GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    // Set each of the button GPIO pins as an input with a pull-up.
    ROM_GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_DIR_MODE_OUT);
    ROM_GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3,
                         GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
}

xSemaphoreHandle uart_mutex;

void tarea_uart_1(void *pvParameters) {

	portTickType ui32WakeTime = 0;

	while(1) {

		xSemaphoreTake(uart_mutex, portMAX_DELAY);
		UARTprintf("Este es el texto que se imprime en la tarea 1.\n");
		xSemaphoreGive(uart_mutex);

		vTaskDelayUntil(&ui32WakeTime, 5 / portTICK_RATE_MS);

	}
}

void tarea_uart_2(void *pvParameters) {

	portTickType ui32WakeTime = 0;

	while(1) {


		xSemaphoreTake(uart_mutex, portMAX_DELAY);
		UARTprintf("Este es el texto que se imprime en la tarea 2.\n");
		xSemaphoreGive(uart_mutex);

		vTaskDelayUntil(&ui32WakeTime, 15 / portTICK_RATE_MS);

	}
}

int main(void) {
	
	ConfigClock();
	ConfigUART();
	ConfigBotonesYLeds();

	uart_mutex = xSemaphoreCreateMutex();

    xTaskCreate(tarea_uart_1, (signed portCHAR *)"tarea_uart_1", LEDTASKSTACKSIZE, NULL,
                       tskIDLE_PRIORITY + 2, NULL);
    xTaskCreate(tarea_uart_2, (signed portCHAR *)"tarea_uart_2", LEDTASKSTACKSIZE, NULL,
                       tskIDLE_PRIORITY + 1, NULL);

    vTaskStartScheduler();
}
