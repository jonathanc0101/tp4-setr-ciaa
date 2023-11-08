/*
 * main.c
 *
 *  Created on: 13 oct. 2022
 *      Author: fep
 */

#include "FreeRTOS.h"
#include "task.h"
#include "app.h"         // <= Su propia cabecera (opcional)
#include "task_types.h"
#include "sapi.h"        // <= Biblioteca sAPI

#define system_number 3 //SYSTEM TO CHANGE, options: 1,2 or 3

#if system_number == 1
	#include "system_1.h"
#endif

#if system_number == 2
	#include "system_2.h"
#endif

#if system_number == 3
	#include "system_3.h"
#endif


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

void task(void *p)
{
		task_config_t* config_ptr = (task_config_t*)p;
		TickType_t xPeriod = config_ptr->period*BASE_TICKS_FOR_UNIT;
		TickType_t xLastWakeTime = 0;

		int instance = 0;
		while( TRUE ) {
			instance++;
			TickType_t starting_time = xTaskGetTickCount();

			sprintf(mensaje,"T%d\t%d [S] %d\r\n",config_ptr->task_number, instance, starting_time);
			uartWriteString(UART_USB, mensaje);

			do_some_work(*config_ptr);

			TickType_t ending_time = xTaskGetTickCount();
			sprintf(mensaje,"T%d\t%d [E] %d\r\n",config_ptr->task_number, instance, ending_time);
			uartWriteString(UART_USB, mensaje);


			vTaskDelayUntil( &xLastWakeTime, xPeriod);
        }
}

int main( void )
{
        // Inicializar y configurar la placa de desarrollo
        boardConfig();
        uartConfig(UART_USB,9600);

		uartWriteString(UART_USB, "Tareas en proceso de creación\n\r");

    	for(int i = 0; i < AMOUNT_OF_TASKS; i++){

    		xTaskCreate( task, "T", 256, &system[i], configMAX_PRIORITIES - 1 - i, NULL );
    	}

    	uartWriteString(UART_USB, "Tareas creadas \n\r");

        vTaskStartScheduler();
        for(;;);
}



