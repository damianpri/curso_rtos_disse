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

	ui32WakeTime = 0;

	while(1) {

		//Prendo Led
		ROM_GPIOPinWrite(GPIO_PORTF_BASE, LEDS, LED_ROJO);

		//Delay
		ROM_SysCtlDelay(50000000/3); //1 Seg

		//Apago Led
		ROM_GPIOPinWrite(GPIO_PORTF_BASE, LEDS, ~LEDS);

		//ROM_SysCtlDelay(50000000/3); //1 Seg
		//vTaskDelay(1000);
		//
		//vTaskDelayUntil(&ui32WakeTime, 500 / portTICK_RATE_MS); //Periodo 500ms

		vTaskDelayUntil(&ui32WakeTime, 1000 / portTICK_RATE_MS); //Periodo 2000ms 2SEG
	}
}



void ConfigClock(void) {
	// Set the clocking to run at 50 MHz from the PLL.
	ROM_SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

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


    xTaskCreate(tarea_uno, (signed portCHAR *)"TAREA_UNO", LEDTASKSTACKSIZE, NULL,
                       tskIDLE_PRIORITY + 1, NULL);



    //Note:Low priority numbers denote low priority tasks. The idle task has priority zero (tskIDLE_PRIORITY).

    vTaskStartScheduler();
}
