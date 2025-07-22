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
#include "fork.h"

extern forkArgs_t *forkargs;

/* When a task begins its execution, it calls jump_to_next to set a jump buffer : in that moment its stack is still ~empty
 * Each time an actor performs a send/forward it makes a noreturnjump to reset the stack to that state freeing some space
 * The state is maintained using a ThreadLocalStoragePointer to keep a pointer to the message that is being sent
 */

void __attribute__((noreturn))
noreturnjump(){
	void *args = NULL;
	args = pvTaskGetThreadLocalStoragePointer(NULL, 1);
	send_message(args);
	while(1){};
}


void jump_to_next() {
    jmp_buf *jump_buf_ptr= pvPortMalloc(sizeof(jmp_buf));
    if (setjmp(*jump_buf_ptr) == 0) {
    	vTaskSetThreadLocalStoragePointer(NULL, 0, jump_buf_ptr);
    } else {
    	noreturnjump();
    }
}

/*
 * This ends a task: if there's a son that can delete it the task suspends: this way it will be externally freed by the son who will also free its TCB
 * This is done to optimize memory efficiency
 */
void end(){
	jmp_buf *jmp = pvTaskGetThreadLocalStoragePointer(NULL, 0);
	int isBeingDeleted = (int)(intptr_t) pvTaskGetThreadLocalStoragePointer(NULL, 2);
	vPortFree(jmp);
	if (isBeingDeleted == 1 && forkargs->sonHasExecuted == 0){
		vTaskSuspend(NULL);
	}
	else{
		if (isBeingDeleted == 1 && forkargs->sonHasExecuted == 1){
			vPortFree(forkargs);
		}
		vTaskDelete(NULL);
	}
}
