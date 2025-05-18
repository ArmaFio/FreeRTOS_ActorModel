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



// Wrapper del task
void jump_to_next() {
    jmp_buf *jump_buf_ptr = pvPortMalloc(sizeof(jmp_buf));
    void *args = NULL;


    if (setjmp(*jump_buf_ptr) == 0) {
    	vTaskSetThreadLocalStoragePointer(NULL, 0, *jump_buf_ptr);
    	return;
    } else {
    	args = pvTaskGetThreadLocalStoragePointer(NULL, 1);
    	send_message(args);
    }

    end();
}

void end(){
	jmp_buf *jmp = pvTaskGetThreadLocalStoragePointer(NULL, 0);
	vPortFree(jmp);
	vTaskDelete(NULL);
}
