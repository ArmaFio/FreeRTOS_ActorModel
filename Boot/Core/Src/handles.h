/*
 * handles.h
 *
 *  Created on: Feb 23, 2025
 *      Author: armaf
 */

#ifndef SRC_HANDLES_H_
#define SRC_HANDLES_H_

#include <stdint.h>

/* Catalogue entry */
typedef struct cat_cnt *catalogue_entry;
typedef struct actor_obj *actor_handle;
typedef void (*msghand)(actor_handle, actor_handle, uint32_t, uint32_t, uint32_t); /*(self, destination, p0,p1,p2) (self and destination will often coincide) */
typedef void (*bootf)(actor_handle, void *);

#endif/* SRC_HANDLES_H_ */
