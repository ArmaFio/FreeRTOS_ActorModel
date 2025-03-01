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

typedef struct{
	uint32_t p0;
	uint32_t p1;
	uint32_t p2;
	uint32_t prio;
}stored_msg;

typedef struct actor_obj{

	SemaphoreHandle_t lock;
	msghand send;
	uint32_t sizeb;
	void *state;

	void (*des)(struct actor_obj *);

	stored_msg mailbox[];

} actor_obj;

/* Actor catalogue entry */
typedef struct cat_cnt {
    bootf boot; // Pointer to actor-specific boot (init) function
} cat_cnt;

#endif/*SRC_ACTOR_META_ */
