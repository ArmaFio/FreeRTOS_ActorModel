/*
 * handles.h
 *
 *  Created on: Feb 23, 2025
 *      Author: armaf
 */

#ifndef SRC_HANDLES_H_
#define SRC_HANDLES_H_

typedef struct actor_obj *actor_handle;
typedef void (*msghand)(actor_handle, uint32_t, uint32_t, uint32_t);
typedef void (*bootf)(actor_handle, void *);

#endif/* SRC_HANDLES_H_ */
