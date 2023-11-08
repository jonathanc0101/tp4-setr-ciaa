/*
 * main.c
 *
 *  Created on: 13 oct. 2022
 *      Author: fep
 */

//freertos includes
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

//custom includes
#include "app.h"         // <= Su propia cabecera (opcional)
#include "task_types.h"
#include "sapi.h"        // <= Biblioteca sAPI

#include "system.h"


static SemaphoreHandle_t lock = NULL;

char mensaje[40];

void do_some_work(task_config_t task_config){
	//does "some work" for (task_config.wcet)*BASE_TICKS_FOR_UNIT
	TickType_t starting_time = xTaskGetTickCount();

	int max_differences = task_config.wcet * BASE_TICKS_FOR_UNIT;

	int difference_counter = 0;

	while(difference_counter < max_differences){
		if(starting_time != xTaskGetTickCount()){
			difference_counter++;
			starting_time = xTaskGetTickCount();
		}
	}

}

void locking_task(void *p)
{
		task_config_t* config_ptr = (task_config_t*)p;
		TickType_t xPeriod = config_ptr->period*BASE_TICKS_FOR_UNIT;
		TickType_t xLastWakeTime = 0;

		int instance = 0;
		while( TRUE ) {
			instance++;

			sprintf(mensaje,"T%d\ti:%d\tlocks\tt:%d\r\n",config_ptr->task_number, instance, xTaskGetTickCount());
			uartWriteString(UART_USB, mensaje);

			//toma mutex
			xSemaphoreTake(lock,portMAX_DELAY);

			sprintf(mensaje,"T%d\ti:%d\t[S]\tt:%d\r\n",config_ptr->task_number, instance, xTaskGetTickCount());
			uartWriteString(UART_USB, mensaje);

			do_some_work(*config_ptr);

			sprintf(mensaje,"T%d\ti:%d\t[E]\tt:%d\r\n",config_ptr->task_number, instance, xTaskGetTickCount());
			uartWriteString(UART_USB, mensaje);

			//libera mutex
			xSemaphoreGive(lock);

			vTaskDelayUntil( &xLastWakeTime, xPeriod);
        }
}

void non_locking_task(void *p)
{
		task_config_t* config_ptr = (task_config_t*)p;
		TickType_t xPeriod = config_ptr->period*BASE_TICKS_FOR_UNIT;
		TickType_t xLastWakeTime = 0;

		int instance = 0;
		while( TRUE ) {
			instance++;

			sprintf(mensaje,"T%d\ti:%d\t[S]\tt:%d\r\n",config_ptr->task_number, instance, xTaskGetTickCount());
			uartWriteString(UART_USB, mensaje);

			do_some_work(*config_ptr);

			sprintf(mensaje,"T%d\ti:%d\t[E]\tt:%d\r\n",config_ptr->task_number, instance, xTaskGetTickCount());
			uartWriteString(UART_USB, mensaje);

			vTaskDelayUntil( &xLastWakeTime, xPeriod);
        }
}

int main( void )
{
        // Inicializar y configurar la placa de desarrollo
        boardConfig();
        uartConfig(UART_USB,9600);

        //lock = xSemaphoreCreateBinary(); //semaphore without priority inheritance
        lock = xSemaphoreCreateMutex(); //mutex with priority inheritance

        xSemaphoreGive(lock);

        if( lock != NULL ){
        	int count = uxSemaphoreGetCount(lock);
        	uartWriteString(UART_USB, "The semaphore was created successfully and can be used.\r\n");

            sprintf(mensaje, "Semaphore has count: %d\n\r", count);
    		uartWriteString(UART_USB, mensaje);
	   }


        sprintf(mensaje, "Creating %d tasks...\n\r", AMOUNT_OF_TASKS);
		uartWriteString(UART_USB, mensaje);

    	for(int i = 0; i < AMOUNT_OF_TASKS; i++){
    		if(system[i].is_locking){
    			xTaskCreate( locking_task, "T", 256, &system[i], configMAX_PRIORITIES - 1 - i, NULL );
    		}else{
    			xTaskCreate( non_locking_task, "T", 256, &system[i], configMAX_PRIORITIES - 1 - i, NULL );
    		}
    	}

    	uartWriteString(UART_USB, "Tasks created\n\r");

        vTaskStartScheduler();
        for(;;);
}



