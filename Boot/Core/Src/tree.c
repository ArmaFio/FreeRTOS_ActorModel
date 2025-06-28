/*
 * tree.c
 *
 *  Created on: Jun 28, 2025
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
#include "tree.h"
#include "actor_factory.h"
#include <inttypes.h>




static void __attribute__((noreturn))
handle(actor_handle self, actor_handle dest, uint32_t p0, uint32_t p1, uint32_t p2) { /*p0 exponent, p1 type of message, p2 sum*/
	if(p1 == 0){/*0 if must generate children, 1 if receiving from children*/
		if(p0 > 0){
			((tree_st*)(self->state))->sx = actor_spawn(tree, self);
			((tree_st*)(self->state))->dx = actor_spawn(tree, self);
			stored_msg *list;
			mailbox_push(&list, p0-1, 0, 0, ((tree_st*)(self->state))->sx);
			mailbox_push(&list, p0-1, 0, 0, ((tree_st*)(self->state))->dx);
			multiple_send(self,list);
		}else{
			if(((tree_st *)(self->state))->father != NULL){
				send(((tree_st *)(self->state))->father, self, 0,1,1);
			}else{
				((tree_st *)(self->state))->sum = 1;
			}
		}
	}else{
		if(((tree_st *)(self->state))->sum == 0){
			((tree_st *)(self->state))->sum = p2;
		}else{
			((tree_st *)(self->state))->sum = ((tree_st *)(self->state))->sum + p2;
			if(((tree_st *)(self->state))->father != NULL){
				actor_retire(((tree_st *)(self->state))->sx);
				actor_retire(((tree_st *)(self->state))->dx);
				send(((tree_st *)(self->state))->father, self, 0,1,((tree_st *)(self->state))->sum);
			}else{
				printf("Value: %" PRIu32 "\r\n", ((tree_st *)(self->state))->sum = 0);
			}
		}
	}
	xSemaphoreGive(self->lock);
	end();
	while(1){};
}

static void des(actor_handle self){
	vPortFree(self->state);
}

void tree_boot(actor_handle self, void *args) {
	self->handle = handle;
	self->state = pvPortMalloc(sizeof(tree_st));
	((tree_st *)(self->state))->sum = 0;
	((tree_st *)(self->state))->sx = NULL;
	((tree_st *)(self->state))->dx = NULL;
	((tree_st *)(self->state))->father = (actor_handle)args;
	self->des = des;
}

cat_cnt tree_cat = (cat_cnt) { .boot = tree_boot };
