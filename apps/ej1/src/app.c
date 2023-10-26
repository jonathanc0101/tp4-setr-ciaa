/*
 * main.c
 *
 *  Created on: 13 oct. 2022
 *      Author: fep
 */

#include "app.h"         // <= Su propia cabecera (opcional)
#include "sapi.h"        // <= Biblioteca sAPI

int main( void )
{
        // Inicializar y configurar la placa de desarrollo
        boardConfig();

        while( TRUE ) {
			gpioWrite( LEDB, !gpioRead( TEC1 ));
			gpioWrite( LED1, !gpioRead( TEC2));
			gpioWrite( LED2, !gpioRead( TEC3));
			gpioWrite( LED3, !gpioRead( TEC4));

			delay( 10 );
        }
}
