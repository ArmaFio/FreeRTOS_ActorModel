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


void __attribute__((noreturn))
send_message(actor_handle dest, uint32_t p0, uint32_t p1, uint32_t p2){
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

void __attribute__((noreturn))
send(actor_handle dest, actor_handle self, uint32_t p0, uint32_t p1, uint32_t p2){

	//actor_fork(self);

	//xSemaphoreGive(dest->lock);

	TaskStatus_t xTaskDetails;
	vTaskGetInfo( /* The handle of the task being queried. */
	                  NULL,
	                  /* The TaskStatus_t structure to complete with information
	                     on xTask. */
	                  &xTaskDetails,
	                  /* Include the stack high water mark value in the
	                     TaskStatus_t structure. */
	                  pdTRUE,
	                  /* Include the task state in the TaskStatus_t structure. */
	                  eInvalid );

	StackType_t *StackBase = xTaskDetails.pxStackBase;

	uintptr_t aligned_sp = (uintptr_t)StackBase & ~0x7; // Align at 8 byte

	// Use register variables to bind parameters to specific registers
	register actor_handle dest_reg asm("r0") = dest;
	register uint32_t p0_reg asm("r1") = p0;
	register uint32_t p1_reg asm("r2") = p1;
	register uint32_t p2_reg asm("r3") = p2;
	register uintptr_t aligned_sp_reg asm("r4") = aligned_sp;
	register void (*send_message_reg)(actor_handle dest, uint32_t p0, uint32_t p1, uint32_t p2) asm("r5") = send_message;

	asm volatile (
	    "msr psp, r4     \n\t" // Set PSP to aligned_sp (r4)
	    "blx r5           \n\t" // Call send_message (r5)
	    :
	    : "r" (dest_reg), "r" (p0_reg), "r" (p1_reg), "r" (p2_reg),
	      "r" (aligned_sp_reg), "r" (send_message_reg)\
	);

	while(1){}
}

