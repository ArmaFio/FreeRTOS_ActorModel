/*
 * send.h
 *
 *  Created on: Apr 13, 2025
 *      Author: armaf
 */

#include "handles.h"
#include "actor_meta.h"

#ifndef SRC_SEND_H_
#define SRC_SEND_H_

void __attribute__((noreturn))
send_message(void *args);

void __attribute__((noreturn))
send(actor_handle dest, actor_handle self, uint32_t p0, uint32_t p1, uint32_t p2);

void __attribute__((noreturn))
multiple_send(actor_handle self, stored_msg *list);

#endif /* SRC_SEND_H_ */
