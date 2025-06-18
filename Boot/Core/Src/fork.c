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

void next_message_process(void *actor){
	stored_msg *msg, msg1;
	actor_handle self = (actor_handle) actor;
	vTaskDelay(1);
	jump_to_next();
	xSemaphoreTake(self->lock, portMAX_DELAY);
	msg = mailbox_pop(&(self->mailbox));
	vTaskPrioritySet(NULL, msg->prio);
	msg1 = *msg;
	vPortFree(msg);
	self->handle(self, msg1.dest, msg1.p0, msg1.p1, msg1.p2);
}

void actor_fork(actor_handle self){
	TaskHandle_t xHandle = NULL;
	UBaseType_t prio = uxTaskPriorityGet(NULL);

	if(self->mailbox!=NULL)
	/* Create the task, storing the handle. */
		xTaskCreate(
					next_message_process,       /* Function that implements the task. */
					"NEXT",          			/* Text name for the task. */
					256,      					/* Stack size in words, not bytes. */
					self,    					/* Parameter passed into the task. */
					prio,						/* Priority at which the task is created. */
					&xHandle );      			/* Used to pass out the created task's handle. */
}

