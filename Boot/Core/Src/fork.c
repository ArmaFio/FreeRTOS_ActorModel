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


void next_message_process(void *actor){
	stored_msg *msg;
	actor_handle self = (actor_handle) actor;
	xSemaphoreTake(self->lock, portMAX_DELAY);
	msg = mailbox_pop(&(self->mailbox));
	vTaskPrioritySet(NULL, msg->prio);
	self->handle(self, msg->dest, msg->p0, msg->p1, msg->p2);
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

