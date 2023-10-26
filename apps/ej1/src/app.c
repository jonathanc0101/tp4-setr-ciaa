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
			gpioWrite( CIAA_BOARD_LED, gpioRead( CIAA_BOARD_BUTTON ) );
        }
}
