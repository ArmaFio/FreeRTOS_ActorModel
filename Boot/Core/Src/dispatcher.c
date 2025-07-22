/*
 * dispatcher.c
 *
 *  Created on: May 11, 2025
 *      Author: armaf
 */

#include "catalogue.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdint.h>
#include <stdlib.h>
#include "actor_meta.h"
#include "send.h"
#include "mailbox.h"

/*Dispatcher boots pushing into its mailbox the list of messages to be forwarded, then sends it one by one to their actual destination.
 *Used to send multiple messages from a single actor
 */

void __attribute__((noreturn))
handle(actor_handle self, actor_handle dest, uint32_t p0, uint32_t p1, uint32_t p2){
	forward(dest, self, p0, p1, p2);
}

static void des(actor_handle self){}

void disp_boot(actor_handle self, void *args){
	self->des = des;
	stored_msg *curr, *messages = (stored_msg*) args;
	self->handle = handle;
	do {
		curr = mailbox_pop(&messages);
		mailbox_push(&(self->mailbox), curr->p0, curr->p1, curr->p2, curr->dest);
		vPortFree(curr);
	}while(messages!=NULL);
}


cat_cnt disp_cat = (cat_cnt) { .boot = disp_boot };

