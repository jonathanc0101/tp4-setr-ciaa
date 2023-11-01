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


/** Structure to initialize task*/
typedef struct {
	int led;
	int duration;
	int period;
	int led_is_on;
	int task_number;
} task_config_t;

gpioMap_t leds[] = {PWM7,PWM8,PWM9};
int amount_of_leds = 3;
int BASE_TICKS_FOR_UNIT = 1000;

task_config_t tasks_config[] = {
	{PWM7, 1, 3, 0, 0},  // Primer elemento
	{PWM8, 1, 4, 0, 1},  // Segundo elemento
	{PWM9, 2, 6, 0, 2}   // Tercer elemento
};

int amount_of_tasks = 3;

int HALF_INTENSITY = 10;
int FULL_INTENSITY = 255;

void task(void *p);
void enable_pwm_with_leds(){
    // configurar los pwm
    pwmConfig(0,PWM_ENABLE);

    for(int i = 0; i<amount_of_leds; i++){
        pwmConfig(leds[i],PWM_ENABLE_OUTPUT);
    }
}
void do_some_work(int duration, int led, int task_number){
	//does "some work" for (duration)*BASE_TICKS_FOR_UNIT
	TickType_t starting_time = xTaskGetTickCount();

	int max_differences = duration * BASE_TICKS_FOR_UNIT;

	int difference_counter = 0;

	while(difference_counter < max_differences){
		if(starting_time != xTaskGetTickCount()){
			difference_counter++;
			starting_time = xTaskGetTickCount();
		}
		pwmWrite(led,FULL_INTENSITY);
		tasks_config[task_number].led_is_on = 1;
	}
}

void manage_leds(int task_number){
	for(int i = 0; i < amount_of_tasks; i++){
		if(i!=task_number && tasks_config[i].led_is_on){
			pwmWrite(tasks_config[i].led,HALF_INTENSITY);
		}
	}
}

int main( void )
{
        // Inicializar y configurar la placa de desarrollo
        boardConfig();
        enable_pwm_with_leds();

        xTaskCreate( task, "T1", 256, &tasks_config[0], configMAX_PRIORITIES - 1, NULL );
        xTaskCreate( task, "T2", 256, &tasks_config[1], configMAX_PRIORITIES - 2, NULL );
        xTaskCreate( task, "T3", 256, &tasks_config[2], configMAX_PRIORITIES - 3, NULL );
        vTaskStartScheduler();
        for(;;);
}

void task(void *p)
{
		task_config_t* config_ptr = (task_config_t*)p;
		TickType_t xPeriod = config_ptr->period*BASE_TICKS_FOR_UNIT;
		TickType_t xLastWakeTime = 0;

		while( TRUE ) {
			manage_leds(config_ptr->task_number);
			do_some_work(config_ptr->duration, config_ptr->led, config_ptr->task_number);

			pwmWrite( config_ptr->led, 0);
			config_ptr->led_is_on = 0;

			vTaskDelayUntil( &xLastWakeTime, xPeriod);
        }
}
