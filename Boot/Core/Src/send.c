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

//TODO Stack handling

static void __attribute__((noreturn))
send(actor_handle dest, uint32_t p0, uint32_t p1, uint32_t p2){
	if(dest->mailbox == NULL){
		if(xSemaphoreTake(dest->lock, (TickType_t) 0) == pdTRUE){
			dest->handle(dest, p0, p1, p2);
		}
	}
	else{
		mailbox_push(&(dest->mailbox), p0, p1, p2);
		vTaskDelete(NULL);
	}
	while(1){};
}

