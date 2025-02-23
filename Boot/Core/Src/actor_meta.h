/*
 * actor_meta.h
 *
 *  Created on: Feb 23, 2025
 *      Author: armaf
 */
#ifndef SRC_ACTOR_META_H_
#define SRC_ACTOR_META_H_

#include "handles.h"

typedef struct actor_obj{

	TaskHandle_t curr;
	msghand send;

	void *state;

	void (*des)(struct actor_obj *);

} actor_obj;

#endif/*SRC_ACTOR_META_ */
