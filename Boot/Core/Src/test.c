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

static void __attribute__((noreturn))
handle(actor_handle self, uint32_t p0, uint32_t p1, uint32_t p2) {
	if( (int) self->state < 1 ){
		self->state += 1;
		xSemaphoreGive(self->lock);
		//vTaskDelete(NULL);
		send(self, self, 0, 0, 0);
	}
	else{
		xSemaphoreGive(self->lock);
		vTaskDelete(NULL);
	}
	while(1){}
}

void test_boot(actor_handle self, void *args) {
	//TODO Factory has to initialize lock and mailbox
	self->handle = handle;
	self->state = 0; // Use the state pointer directly as a counter
}

cat_cnt test_cat = (cat_cnt) { .boot = test_boot };

