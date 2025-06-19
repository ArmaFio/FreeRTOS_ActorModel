/*
 * utils.c
 *
 *  Created on: May 11, 2025
 *      Author: armaf
 */

#include "FreeRTOS.h"
#include "task.h"
#include "setjmp.h"
#include "utils.h"
#include "send.h"



void __attribute__((noreturn))
noreturnjump(){
	void *args = NULL;
	args = pvTaskGetThreadLocalStoragePointer(NULL, 1);
	send_message(args);
	while(1){};
}

// Wrapper del task

void jump_to_next() {
    jmp_buf *jump_buf_ptr= pvPortMalloc(sizeof(jmp_buf));
    if (setjmp(*jump_buf_ptr) == 0) {
    	vTaskSetThreadLocalStoragePointer(NULL, 0, jump_buf_ptr);
    } else {
    	noreturnjump();
    }
}

void end(){
	jmp_buf *jmp = pvTaskGetThreadLocalStoragePointer(NULL, 0);
	int isBeingDeleted = (int)(intptr_t) pvTaskGetThreadLocalStoragePointer(NULL, 2);
	vPortFree(jmp);
	if (isBeingDeleted == 1)
		vTaskSuspend(NULL);
	else
		vTaskDelete(NULL);
}
