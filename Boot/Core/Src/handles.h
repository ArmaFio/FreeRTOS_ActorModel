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
typedef void (*msghand)(actor_handle, uint32_t, uint32_t, uint32_t); /*(actor_handle,p0,p1,p2)*/
typedef void (*bootf)(actor_handle, void *);

#endif/* SRC_HANDLES_H_ */
