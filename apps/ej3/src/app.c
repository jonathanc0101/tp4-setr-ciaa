/*
 * main.c
 *
 *  Created on: 13 oct. 2022
 *      Author: fep
 */

#include "app.h"         // <= Su propia cabecera (opcional)
#include "sapi.h"        // <= Biblioteca sAPI

int mapValue(int value) {
    // Asegura que el valor esté en el rango [0, 100]
    if (value < 0) {
        value = 0;
    } else if (value > 100) {
        value = 100;
    }

    // Realiza el mapeo a [0, 255]
    return (int)((float)value / 100.0 * 255);
}

int main( void )
{
        // Inicializar y configurar la placa de desarrollo
        boardConfig();
        pwmConfig(0,PWM_ENABLE);

        gpioMap_t leds[] = {PWM7,PWM8,PWM9};
        gpioMap_t botones[] = {TEC2,TEC3,TEC4};
        int contadores[] = {0,0,0};

        for(int i = 0; i<3; i++){
            pwmConfig(leds[i],PWM_ENABLE_OUTPUT);
        }


        while( TRUE ) {

        	int j = 0;

        	while(j<3){
        		if(!gpioRead(botones[j]) ){
        			contadores[j] += 10;
        			contadores[j] = contadores[j] % 100;
        		}

				pwmWrite(leds[j],mapValue(contadores[j]));

        		j++;
        	}

			delay( 100 );
        }
}
