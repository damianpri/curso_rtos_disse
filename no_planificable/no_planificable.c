/*
 *  2 con C<T
pero con un factor de utilización del procesador mayor que 1
C1/T1+C2/T2>1
por ejemplo un 0,6+0,5 es > 1
de todas formas, en los dos casos C1<T1 y C2<T2
ej
C1=0,6
T1=1
C1/T1=0,6
otra podría ser C2=1
T2=2
C2/T2=0.5
este sistema no es planificable
pues C1/T1+C2/T2 > 1
 *
 * */

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

#define SYSCLOCK 50000000

void tarea_uno(void *pvParameters) {

	portTickType ui32WakeTime;
	uint32_t TiempoDeRepuesta;

	vTaskSetApplicationTaskTag( NULL, (pdTASK_HOOK_CODE) LED_ROJO);

	ui32WakeTime = 0;

	while(1) {

		//Delay
		ROM_SysCtlDelay(0.6*50000000/3); //C=0.6

		TiempoDeRepuesta = (xTaskGetTickCount() - ui32WakeTime) / portTICK_RATE_MS;

		if(TiempoDeRepuesta > 1000){
			UARTprintf("Tarea_UNO: Se perdio el deadline. Tiempo de Repuesta %d.\n",TiempoDeRepuesta);
		}

		vTaskDelayUntil(&ui32WakeTime, 1000 / portTICK_RATE_MS); //T=1seg

	}
}

void tarea_dos(void *pvParameters) {

	portTickType ui32WakeTime;
	uint32_t TiempoDeRepuesta;

	vTaskSetApplicationTaskTag( NULL, (pdTASK_HOOK_CODE) LED_VERDE);

	ui32WakeTime = 0;

	while(1) {

		//Delay
		ROM_SysCtlDelay((50000000/3)); //1 Seg  C2=1

		TiempoDeRepuesta = (xTaskGetTickCount() - ui32WakeTime) / portTICK_RATE_MS;

		if(TiempoDeRepuesta > 2000){
			UARTprintf("Tarea_DOS: Se perdio el deadline. Tiempo de Repuesta %d.\n",TiempoDeRepuesta);
		}

		vTaskDelayUntil(&ui32WakeTime, 2000 / portTICK_RATE_MS);

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

    xTaskCreate(tarea_uno, (signed portCHAR *)"TAREA_UNO", LEDTASKSTACKSIZE, NULL,
                       tskIDLE_PRIORITY + 2, NULL);

    xTaskCreate(tarea_dos, (signed portCHAR *)"TAREA_DOS", LEDTASKSTACKSIZE, NULL,
                           tskIDLE_PRIORITY + 1, NULL);

    //Note:Low priority numbers denote low priority tasks. The idle task has priority zero (tskIDLE_PRIORITY).

    vTaskStartScheduler();
}
