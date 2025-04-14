/*
 * actor_factory.c
 *
 *  Created on: Apr 5, 2025
 *      Author: samue
 */


#include "handles.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include <stdint.h>
#include <stdlib.h>
#include "actor_meta.h"

actor_handle actor_spawn(catalogue_entry e, void *params) {
    /* Allocate an actor descriptor object from the allocation pool, then
       initialize it */
    actor_handle new_actor = malloc(sizeof(actor_obj));
    new_actor->lock = xSemaphoreCreateMutex();
    new_actor->mailbox = NULL;
    /* Set actor-specific parameters by running its boot procedure */
    e->boot(new_actor, params);

    return new_actor;
}

void actor_retire(actor_handle a) {
    /* Call destructor */
    a->des(a);
    free(a);
    return;
}
