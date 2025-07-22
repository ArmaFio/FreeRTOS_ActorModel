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
#include <stdio.h>

typedef struct {
    uint32_t p0, p1, p2;
    actor_handle dest;
} SendArgs_t;

void __attribute__((noreturn))
send_message(void *param){
	SendArgs_t *arg = (SendArgs_t *)param;
	actor_handle dest = arg->dest;
	uint32_t p0 = arg->p0, p1 = arg->p1, p2 = arg->p2;
	//vTaskSetThreadLocalStoragePointer(NULL, 1, NULL);
	vPortFree(arg);
	if(dest->mailbox == NULL){
		if(xSemaphoreTake(dest->lock, (TickType_t) 0) == pdTRUE){
			dest->handle(dest, dest, p0, p1, p2);
		}
		else{
			mailbox_push(&(dest->mailbox), p0, p1, p2, dest);
			end();
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
    SendArgs_t *args = pvPortMalloc(sizeof(*args));
    args->dest = dest;
    args->p0   = p0;
    args->p1   = p1;
    args->p2   = p2;

    jmp_buf *buf = (jmp_buf *)pvTaskGetThreadLocalStoragePointer(NULL, 0);

    vTaskSetThreadLocalStoragePointer(NULL, 1, args);
    xSemaphoreGive(self->lock);
    actor_fork(self);
    longjmp(*buf, 1);

    while (1) {} // fallback
}

void __attribute__((noreturn))
forward(actor_handle dest, actor_handle self, uint32_t p0, uint32_t p1, uint32_t p2){
	size_t size;
    SendArgs_t *args = pvPortMalloc(sizeof(*args));
    args->dest = dest;
    args->p0   = p0;
    args->p1   = p1;
    args->p2   = p2;

    jmp_buf *buf = (jmp_buf *)pvTaskGetThreadLocalStoragePointer(NULL, 0);

    vTaskSetThreadLocalStoragePointer(NULL, 1, args);
    xSemaphoreGive(self->lock);
	size = xPortGetFreeHeapSize();
	printf("%d", size);
    actor_fork(self);
	if(self->mailbox == NULL){
		actor_retire(self);
	}
    longjmp(*buf, 1);

    while (1) {} // fallback
}


void __attribute__((noreturn))
multiple_send(actor_handle self, stored_msg *messages){
	size_t size;
	stored_msg firstmsg;
	if(self!=NULL){
		actor_fork(self);
		xSemaphoreGive(self->lock);
	}
	actor_handle disp = actor_spawn(dispatcher, messages);
	size = xPortGetFreeHeapSize();
	xSemaphoreTake(disp->lock, portMAX_DELAY);
	printf("%d", size);
	stored_msg *first = mailbox_pop(&(disp->mailbox));
	firstmsg = *first;
	vPortFree(first);
	disp->handle(disp, firstmsg.dest, firstmsg.p0, firstmsg.p1, firstmsg.p2);
	while(1){};
}

