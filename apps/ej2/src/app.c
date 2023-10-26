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
        uartConfig(UART_USB,9600);

    	char *mensaje = (char*)malloc(40 * sizeof(char));

        gpioMap_t botones[] = {TEC1,TEC2,TEC3,TEC4};

        while( TRUE ) {

        	int j = 0;

        	while(j<4 && gpioRead(botones[j])){
        		j++;
        	}

        	if(j==4){
        		;
        	}else{
            	sprintf(mensaje,"Se presionó el botón %d\n",j+1);
        		uartWriteString(UART_USB, mensaje);
        	}

			delay( 10 );
        }
}
