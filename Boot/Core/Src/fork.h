/*
 * fork.h
 *
 *  Created on: Apr 5, 2025
 *      Author: samue
 */

#ifndef SRC_FORK_H_
#define SRC_FORK_H_


void next_message_process(void *actor);

void actor_fork(actor_handle self);

typedef struct {
	actor_handle actor;
	TaskHandle_t taskToDelete;
	int sonHasExecuted;
}forkArgs_t;

#endif /* SRC_FORK_H_ */
