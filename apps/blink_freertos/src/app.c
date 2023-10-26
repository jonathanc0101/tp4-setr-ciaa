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

void task(void *p);

int main( void )
{
        // Inicializar y configurar la placa de desarrollo
        boardConfig();

        xTaskCreate( task, "T1", 256, NULL, configMAX_PRIORITIES - 1, NULL );
        vTaskStartScheduler();
        for(;;);
}

void task(void *p)
{
		TickType_t xPeriod = 500;
		int onoff = 0;
        while( TRUE ) {
        		onoff = !onoff;
                gpioWrite( CIAA_BOARD_LED, onoff );
                vTaskDelay( xPeriod );
        }
}
