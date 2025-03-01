/*
 * test.c
 *
 *  Created on: Mar 1, 2025
 *      Author: samue
 */
#include "actor_meta.h"
#include "catalogue.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdint.h>
#include <stdlib.h>

static void __attribute__((noreturn))
handle(actor_handle self, uint32_t p0, uint32_t p1, uint32_t p2) {
	self->state += 1;
	vTaskDelete(NULL);
	while(1){}
}

void test_boot(actor_handle self, void *args) {
	self = malloc(sizeof(actor_obj) + sizeof(stored_msg)*20);

	self->send = handle;
	self->state = 0; // Use the state pointer directly as a counter
	self->sizeb = 20;
}

cat_cnt test_cat = (cat_cnt) { .boot = test_boot };

