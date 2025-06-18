/*
 * test.c
 *
 *  Created on: Mar 1, 2025
 *      Author: samue
 */
#include "catalogue.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdint.h>
#include <stdlib.h>
#include "actor_meta.h"
#include "send.h"
#include "fork.h"
#include "mailbox.h"
#include "utils.h"
#include "stdio.h"


static void __attribute__((noreturn))
handle(actor_handle self, actor_handle dest, uint32_t p0, uint32_t p1, uint32_t p2) {
	stored_msg *list;
	if( (int) self->state < 100){
		self->state += 1;
		if ( (int) self->state == 1){
			for (int i=0; i<100; i++){
				mailbox_push(&list, i, i, i, self);
			}
			multiple_send(self,list);
		}
		else{
			xSemaphoreGive(self->lock);
			end();
		}
	}
	else{
		xSemaphoreGive(self->lock);
		actor_fork(self);
		end();
	}
		while(1){}
}


void test_boot(actor_handle self, void *args) {
	//TODO Factory has to initialize lock and mailbox
	self->handle = handle;
	self->state = 0; // Use the state pointer directly as a counter
}

cat_cnt test_cat = (cat_cnt) { .boot = test_boot };

