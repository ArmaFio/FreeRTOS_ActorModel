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


void __attribute__((noreturn))
handle(actor_handle self, actor_handle dest, uint32_t p0, uint32_t p1, uint32_t p2){
	send(self, dest, p0, p1, p2);
}

void disp_boot(actor_handle self, void *args){
	stored_msg *curr, *messages = (stored_msg*) args;
	self->handle = handle;
	do {
		curr = mailbox_pop(&messages);
		mailbox_push(&(self->mailbox), curr->p0, curr->p1, curr->p2, curr->dest);
	}while(messages!=NULL);
}



cat_cnt disp_cat = (cat_cnt) { .boot = disp_boot };

