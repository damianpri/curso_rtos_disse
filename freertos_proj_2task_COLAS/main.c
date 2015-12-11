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
#include "queue.h"

#define LEDTASKSTACKSIZE        128         // Stack size in words


#define LEDS (GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3)
#define LED_ROJO GPIO_PIN_1
#define LED_AZUL GPIO_PIN_2
#define LED_VERDE GPIO_PIN_3

#define SYSCLOCK 50000000

// Global Queue Handle
xQueueHandle qh = 0;

void tarea_tx(void *pvParameters) {


	 int myInt = 0;
	 portTickType ui32WakeTime;
	 ui32WakeTime = 0;


	 while (1) {
		myInt++;
		if (!xQueueSend(qh, &myInt, 0)) {  // Cambiar 0 por // portMAX_DELAY y queda bloqueada hasta que la Cola libere
			UARTprintf("Fallo el Envio");
		} else {
			UARTprintf("Envio: %u\n", myInt);
		}
		vTaskDelayUntil(&ui32WakeTime, 1000 / portTICK_RATE_MS);
	}

}

void tarea_rx(void *pvParameters) {

	portTickType ui32WakeTime;

	ui32WakeTime = 0;

	int myInt = 0;

	while(1)
	    {
	        if(!xQueueReceive(qh, &myInt, 0)) { // portMAX_DELAY
	        	UARTprintf("Fallo Recepcion");
	        }
	        else {
	        	UARTprintf("Recibo: %u\n", myInt);
	        }
	        vTaskDelayUntil(&ui32WakeTime, 3000 / portTICK_RATE_MS);
	    }
}



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



int main(void) {
	
	ConfigClock();

	ConfigBotonesYLeds();

	ConfigUART();
	qh = xQueueCreate(5, sizeof(int));

    xTaskCreate(tarea_tx, (signed portCHAR *)"TAREA_UNO", LEDTASKSTACKSIZE, NULL,
    						tskIDLE_PRIORITY + 1, NULL);

    xTaskCreate(tarea_rx, (signed portCHAR *)"TAREA_DOS", LEDTASKSTACKSIZE, NULL,
                           	tskIDLE_PRIORITY + 1, NULL);




    //Note:Low priority numbers denote low priority tasks. The idle task has priority zero (tskIDLE_PRIORITY).

    vTaskStartScheduler();
}
