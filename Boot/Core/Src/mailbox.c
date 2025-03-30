/*
 * mailbox.c
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

void mailbox_push (stored_msg **mailbox, uint32_t p0, uint32_t p1, uint32_t p2){
	stored_msg *message = malloc(sizeof(stored_msg)), *curr;
	message->p0 = p0;
	message->p1 = p1;
	message->p2 = p2;
	message->prio = uxTaskPriorityGet(NULL);
	message->next=NULL;

	if (*mailbox == NULL || (*mailbox)->prio < message->prio){
		message->next = *mailbox;
		*mailbox = message;
		return;
	}

	curr = *mailbox;

	for(; curr->next != NULL ; curr = curr->next){
		if(curr->next->prio < message->prio){
			message->next = curr->next;
			curr->next = message;
			return;
		}
	}

	curr->next=message;
};

stored_msg* mailbox_pop(stored_msg **mailbox){
	stored_msg *next_msg = *mailbox;
	*mailbox = (*mailbox)->next;
	return next_msg;
}

