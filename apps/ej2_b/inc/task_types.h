
#include "sapi.h"        // <= Biblioteca sAPI

#ifndef TASK_CONFIG_h
#define TASK_CONFIG_h

/** Structure to initialize task*/
typedef struct {
	TickType_t wcet;
	TickType_t period;
	TickType_t deadline;
	int task_number;
	bool is_locking;
} task_config_t;


#define BASE_TICKS_FOR_UNIT 1000

#endif

