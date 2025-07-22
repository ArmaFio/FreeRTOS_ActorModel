/*
 * fork.c
 *
 *  Created on: Apr 5, 2025
 *      Author: samue
 */

#include "FreeRTOS.h"
#include "semphr.h"
#include <stdint.h>
#include "actor_meta.h"
#include "mailbox.h"
#include "utils.h"
#include <stdlib.h>
#include "fork.h"


forkArgs_t *forkargs = NULL;

void next_message_process(void *args){
	forkArgs_t *arg = (forkArgs_t *) args;
	stored_msg *msg, msg1;
	actor_handle self = (actor_handle) arg->actor;
	if (arg->taskToDelete!= NULL){
		if (eTaskGetState(arg-> taskToDelete) == eSuspended){
			vTaskDelete(arg->taskToDelete);
			vPortFree(arg);
			arg = NULL;
		}
		else
			arg -> sonHasExecuted = 1;
	}
	else
		vPortFree(arg);
	jump_to_next();
	xSemaphoreTake(self->lock, portMAX_DELAY);
	if (self->mailbox==NULL)
		end();
	msg = mailbox_pop(&(self->mailbox));
	vTaskPrioritySet(NULL, msg->prio);
	msg1 = *msg;
	vPortFree(msg);
	vTaskSetThreadLocalStoragePointer(NULL, 2, (void *)(intptr_t)0);
	self->handle(self, msg1.dest, msg1.p0, msg1.p1, msg1.p2);
}

void actor_fork(actor_handle self){
	forkArgs_t *args;
	if(self->mailbox!=NULL){
		TaskHandle_t xHandle = NULL, selfHandle = xTaskGetCurrentTaskHandle();
		UBaseType_t prio = uxTaskPriorityGet(NULL);
		int isBeingDeleted = (int)(intptr_t) pvTaskGetThreadLocalStoragePointer(NULL, 2);
		if (isBeingDeleted == 0 || forkargs -> sonHasExecuted == 1){
			forkargs = pvPortMalloc(sizeof(forkArgs_t));
			args = forkargs;
			args->taskToDelete = selfHandle;
		    vTaskSetThreadLocalStoragePointer(NULL, 2, (void *)(intptr_t)1);
		}
		else {
			args = pvPortMalloc(sizeof(forkArgs_t));
			args->taskToDelete = NULL;
		}
		args->sonHasExecuted = 0;
		args->actor = self;
		xTaskCreate(
						next_message_process,       /* Function that implements the task. */
						"NEXT",          			/* Text name for the task. */
						256,      					/* Stack size in words, not bytes. */
						args,    					/* Parameter passed into the task. */
						prio,						/* Priority at which the task is created. */
						&xHandle );

		/* Create the task, storing the handle. */

	}/* Used to pass out the created task's handle. */
}

