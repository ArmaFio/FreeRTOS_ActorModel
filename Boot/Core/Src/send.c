/*
 * send.c
 *
 *  Created on: Mar 30, 2025
 *      Author: armaf
 */

#include "catalogue.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdint.h>
#include <stdlib.h>
#include "actor_meta.h"
#include "mailbox.h"
#include "fork.h"
#include "send.h"
#include "actor_factory.h"
#include "utils.h"
#include "setjmp.h"

typedef struct {
    uint32_t p0, p1, p2;
    actor_handle dest;
} SendArgs_t;

void __attribute__((noreturn))
send_message(void *param){
	SendArgs_t *a = (SendArgs_t *)param;
	actor_handle dest = a->dest;
	uint32_t p0 = a->p0, p1 = a->p1, p2 = a->p2;

	if(dest->mailbox == NULL){
		if(xSemaphoreTake(dest->lock, (TickType_t) 0) == pdTRUE){
			dest->handle(dest, dest, p0, p1, p2);
		}
	}
	else{
		mailbox_push(&(dest->mailbox), p0, p1, p2, dest);
		end();
	}
	while(1){};
}

void __attribute__((noreturn))
send(actor_handle dest, actor_handle self, uint32_t p0, uint32_t p1, uint32_t p2){
	//xSemaphoreGive(self->lock);
	actor_fork(self);

	SendArgs_t *args = pvPortMalloc(sizeof(*args));
	args->dest = dest;
	args->p0   = p0;
	args->p1   = p1;
	args->p2   = p2;
	jmp_buf *buf = (jmp_buf *)pvTaskGetThreadLocalStoragePointer(NULL, 0);

	vTaskSetThreadLocalStoragePointer(NULL, 1, args);

	longjmp(*buf, 1);

	while(1){}
}

void __attribute__((noreturn))
multiple_send(actor_handle self, stored_msg *messages){
	actor_fork(self);
	xSemaphoreGive(self->lock);
	actor_handle disp = actor_spawn(dispatcher, messages);
	xSemaphoreTake(disp->lock, portMAX_DELAY);
	stored_msg *first = mailbox_pop(&(disp->mailbox));
	disp->handle(disp, first->dest, first->p0, first->p1, first->p2);
	while(1){};
}

