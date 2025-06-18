/*
 * mailbox.h
 *
 *  Created on: Apr 5, 2025
 *      Author: samue
 */

#ifndef SRC_MAILBOX_H_
#define SRC_MAILBOX_H_


void mailbox_push (stored_msg **mailbox, uint32_t p0, uint32_t p1, uint32_t p2, actor_handle dest);

stored_msg* mailbox_pop(stored_msg **mailbox);

void mailbox_push_array(stored_msg *array, size_t max_size, size_t *current_size,
                      uint32_t p0, uint32_t p1, uint32_t p2, actor_handle dest);
#endif /* SRC_MAILBOX_H_ */
