/*
 * utils.c
 *
 *  Created on: May 11, 2025
 *      Author: armaf
 */

#include "FreeRTOS.h"
#include "task.h"

void end(){
	//void *new_psp = pvTaskGetThreadLocalStoragePointer(NULL, 1);
	//__asm volatile ("msr psp, %0" :: "r" (new_psp));
	vTaskDelete(NULL);
}
