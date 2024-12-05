/*
 * main.c
 *
 *  Created on: 13 oct. 2022
 *      Author: fep
 */

#include "FreeRTOS.h"
#include "task.h"
#include "app.h"         // <= Su propia cabecera (opcional)
#include "sapi.h"        // <= Biblioteca sAPI
#include "semphr.h"


static void task( void* pvParameters );

gpioMap_t botones[] = {TEC1, TEC2, TEC3, TEC4};
gpioMap_t led[] = {CIAA_BOARD_LED, LED1, LED2, LED3};

int main( void )
{
        // Inicializar y configurar la placa de desarrollo
        boardConfig();

       /* Creates the periodic tasks. */
       xTaskCreate( task, "T1", 256, NULL, configMAX_PRIORITIES -1, NULL );

       vTaskStartScheduler();


        for(;;);
}

void task ()
{

	TickType_t pxPreviousWakeTime = 0;
	int period = 500;
	  int valorLed;
	  for(;;) {

	         for (int i = 0; i < 4; i++) {

	        	if (gpioRead(botones[i]) == 0) {
	        		gpioToggle(led[i]);
	        		if (gpioRead(botones[i]) == 1) {
	        			gpioToggle(led[i]);
	        		}
	        	}

	        	valorLed = gpioRead(botones[i]);

	          }


	  vTaskDelayUntil( &pxPreviousWakeTime, period );
	  }
	  vTaskDelete(NULL);
}
