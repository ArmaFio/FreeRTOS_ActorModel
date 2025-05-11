/*
 * actor_meta.h
 *
 *  Created on: Feb 23, 2025
 *      Author: armaf
 */
#ifndef SRC_ACTOR_META_H_
#define SRC_ACTOR_META_H_

#include "handles.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include <stdint.h>

typedef struct stored_msg{
	actor_handle dest;
	uint32_t p0;
	uint32_t p1;
	uint32_t p2;
	UBaseType_t prio;
	struct stored_msg *next;
}stored_msg;

typedef struct actor_obj{

	SemaphoreHandle_t lock;
	msghand handle;
	void *state;
	stored_msg *mailbox;
	void (*des)(struct actor_obj *);

} actor_obj;

/* Actor catalogue entry */
typedef struct cat_cnt {
    bootf boot; // Pointer to actor-specific boot (init) function
} cat_cnt;

#endif/*SRC_ACTOR_META_ */
