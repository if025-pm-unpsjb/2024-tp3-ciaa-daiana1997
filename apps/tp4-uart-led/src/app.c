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



int main( void )
{

		uint8_t receivedByte;
		bool_t input;

		gpioMap_t botones[] = {TEC1, TEC2, TEC3, TEC4};
		gpioMap_t led[] = {CIAA_BOARD_LED, LED1, LED2, LED3};
		int valorLed;

        uartConfig( UART_USB, 9600);

        // Inicializar y configurar la placa de desarrollo
        boardConfig();

        while (TRUE) {

                	input = uartReadByte( UART_USB, &receivedByte);
                	if (input == FALSE)
                	    continue;


                    uartWriteString(UART_USB, receivedByte);

                    valorLed = receivedByte - '1';
                    gpioToggle(led[valorLed]);

        }


        for(;;);
}
