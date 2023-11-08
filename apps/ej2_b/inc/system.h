#include "task_types.h"

#ifndef SYSTEM_H
#define SYSTEM_H

static task_config_t system[] = {
	{ 1, 5, 5, 0, true},
	{ 1, 7, 7, 1, false},
	{ 3, 19, 19, 2, true}
};

#define AMOUNT_OF_TASKS 3

#endif
