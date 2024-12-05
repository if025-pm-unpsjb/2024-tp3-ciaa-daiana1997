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


#define BUFFER 25

int main( void )
{
		char mensaje[ BUFFER ];
		int messageIndex;
		uint8_t receivedByte;
		bool_t entrada;

        uartConfig( UART_USB, 9600);

        // Inicializar y configurar la placa de desarrollo
        boardConfig();

        while (TRUE) {

                	entrada = uartReadByte( UART_USB, &receivedByte);
                	if (entrada == FALSE)
                		continue;

                	mensaje[messageIndex++] = receivedByte;
                	if (receivedByte == '\r') {
                		mensaje[messageIndex++] = '\0\n';
                		uartWriteString(UART_USB, mensaje);
                		messageIndex = 0;
                		for (int i = 0; i < 25; i++) {
                			mensaje[i] = 0;
                		}
                	}

                }



        for(;;);
}

