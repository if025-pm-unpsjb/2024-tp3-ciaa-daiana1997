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

/* Tasks periods. */
#define TASK1_PERIOD 	4000
#define TASK2_PERIOD 	6000
#define TASK3_PERIOD 	90000
#define TASK4_PERIOD 	11000


/* Tasks WCETs. */
#define TASK1_WCET		1000
#define TASK2_WCET		2000
#define TASK3_WCET		1000
#define TASK4_WCET		2000



struct xTaskStruct {
	TickType_t wcet;
	TickType_t period;
	//gpioMap_t led;
	pwmMap_t led;
};

typedef struct xTaskStruct xTask;


xTask task1 = { TASK1_WCET, TASK1_PERIOD, CIAA_BOARD_LED };
xTask task2 = { TASK2_WCET, TASK2_PERIOD, PWM7 };
xTask task3 = { TASK3_WCET, TASK3_PERIOD, PWM8 };
xTask task4 = { TASK4_WCET, TASK4_PERIOD, PWM9 };


void task(void *p);

SemaphoreHandle_t s;

int main( void )
{
        // Inicializar y configurar la placa de desarrollo
        boardConfig();

        pwmConfig(0, PWM_ENABLE);
        pwmConfig(PWM7, PWM_ENABLE_OUTPUT);
        pwmConfig(PWM8, PWM_ENABLE_OUTPUT);
        pwmConfig(PWM9, PWM_ENABLE_OUTPUT);
     //   pwmWrite(PWM7, 10);


    	s = xSemaphoreCreateMutex();


       /* xTaskCreate( task, "T1", 256, NULL, configMAX_PRIORITIES - 1, NULL ); */

        /* Creates the periodic tasks. */
           xTaskCreate( task, "T1", configMINIMAL_STACK_SIZE + 50, (void*) &task1, configMAX_PRIORITIES - 1, NULL );
           xTaskCreate( task, "T2", configMINIMAL_STACK_SIZE + 50, (void*) &task2, configMAX_PRIORITIES - 2, NULL );
           xTaskCreate( task, "T3", configMINIMAL_STACK_SIZE + 50, (void*) &task3, configMAX_PRIORITIES - 3, NULL );
           xTaskCreate( task, "T4", configMINIMAL_STACK_SIZE + 50, (void*) &task4, configMAX_PRIORITIES - 4, NULL );


        vTaskStartScheduler();


        for(;;);
}


static void vBusyWait( TickType_t ticks )
{
    TickType_t elapsedTicks = 0;
    TickType_t currentTick = 0;
    while ( elapsedTicks < ticks ) {
        currentTick = xTaskGetTickCount();
        while ( currentTick == xTaskGetTickCount() ) {
            __asm__("nop");
        }
        elapsedTicks++;
    }
}

void task( void *pvParameters )
{

	unsigned int uxReleaseCount = 0;
	TickType_t pxPreviousWakeTime = 0;
	xTask *task = (xTask*) pvParameters;
	int valuePwm;

	for( ;; )
	{

		if (task->led == CIAA_BOARD_LED){
			gpioWrite(task->led , 1);
		}
		else{
		pwmWrite(task->led, 255);
		}



        valuePwm = pwmRead(PWM7);

        if ((valuePwm == 255) && (task->led != PWM7)){
        	pwmWrite(PWM7, 5);
        }

        valuePwm = pwmRead(PWM8);

        if ((valuePwm == 255) && (task->led != PWM8)){
            pwmWrite(PWM8, 5);
        }

        valuePwm = pwmRead(PWM9);
        if ((valuePwm == 255) && (task->led != PWM9)){
            pwmWrite(PWM9, 5);
        }

        vBusyWait( task->wcet );

        if(task->led == CIAA_BOARD_LED){
        	 gpioWrite(task->led , 0);
        }
        else{
        	pwmWrite(task->led ,0);
        }

        valuePwm = pwmRead(PWM7);

        if ((valuePwm == 5) && (task->led != PWM7)){
         pwmWrite(PWM7, 255);
        }

        valuePwm = pwmRead(PWM8);

        if ((valuePwm == 5) && (task->led != PWM8)){
        pwmWrite(PWM8, 255);
        }

        valuePwm = pwmRead(PWM9);
        if ((valuePwm == 5) && (task->led != PWM9)){
          pwmWrite(PWM9, 255);
        }

        vTaskDelayUntil( &pxPreviousWakeTime, task->period );
        uxReleaseCount += 1;

	}


	vTaskDelete( NULL );

}
