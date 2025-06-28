/*
 * tokring.h
 *
 *  Created on: Jun 18, 2025
 *      Author: armaf
 */

#ifndef SRC_TOKRING_H_
#define SRC_TOKRING_H_

#include "FreeRTOS.h"

typedef enum { SET_TOKS, START, FWD, RETIRE } ring_msg;

typedef struct {
	actor_handle succ;
	uint32_t val;
	uint32_t tokens;
	uint8_t flags;
} ring_st;

typedef struct {
	actor_handle first;
	uint32_t actor_number;
} boot_args;

#endif /* SRC_TOKRING_H_ */
