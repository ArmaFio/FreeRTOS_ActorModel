/*
 * tokring.c
 *
 *  Created on: Jun 18, 2025
 *      Author: armaf
 */



#include "catalogue.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdint.h>
#include <stdlib.h>
#include "actor_meta.h"
#include "send.h"
#include "actor_factory.h"
#include "fork.h"
#include "mailbox.h"
#include "utils.h"
#include "stdio.h"
#include "tokring.h"



static void __attribute__((noreturn))
handle(actor_handle self, actor_handle dest, uint32_t p0, uint32_t p1, uint32_t p2) {
	if (p0 > 0)
		send (((ring_st *)self->state)->succ, self, p0-1 , 0, 0);
	else{
		(((ring_st *)self->state)-> val) ++;
		xSemaphoreGive(self->lock);
		actor_fork(self);
		end();
	}
	while(1){}
}

void tokring_boot(actor_handle self, void *args) {
	boot_args *arg = (boot_args *) args;
	self->handle = handle;
	self->state = pvPortMalloc(sizeof(ring_st));
	((ring_st *)self->state)-> val = 0;
	if(arg-> actor_number > 0){
		if (arg -> first == NULL){
			arg-> actor_number -= 1;
			arg -> first = self;
		}
		if (arg-> actor_number> 0){
			arg -> actor_number -= 1;
			((ring_st *)self->state) -> succ = actor_spawn(tokring, arg);
		}
	}
	else{
		((ring_st *)self->state) -> succ = arg->first;
		free (arg);
	}
}

cat_cnt tokr_cat = (cat_cnt) { .boot = tokring_boot };
