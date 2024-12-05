/*
 * main.c
 *
 *  Created on: 13 oct. 2022
 *      Author: fep
 */

#include "../../tp4-echo/inc/app.h"         // <= Su propia cabecera (opcional)

#include "FreeRTOS.h"
#include "task.h"
#include "sapi.h"        // <= Biblioteca sAPI
#include "semphr.h"

void task(void*);

gpioMap_t botones[] = {TEC1, TEC2, TEC3, TEC4};
gpioMap_t led[] = {CIAA_BOARD_LED, LED1, LED2, LED3};

int main( void )
{

		//uint8_t receivedByte;
		//bool_t input;



        uartConfig( UART_USB, 9600);

        // Inicializar y configurar la placa de desarrollo
        boardConfig();

        uartInterrupt(UART_USB, TRUE);
        uartCallbackSet(UART_USB , UART_RECEIVE ,task,NULL);

        for(;;);
}

void task(void*)
{
	uint8_t valorLed;

	valorLed = uartRxRead( UART_USB);
	valorLed = valorLed - '1';
	gpioToggle(led[valorLed]);


}
