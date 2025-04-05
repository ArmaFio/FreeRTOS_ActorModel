/*
 * actor_factory.h
 *
 *  Created on: Apr 5, 2025
 *      Author: samue
 */

#ifndef SRC_ACTOR_FACTORY_H_
#define SRC_ACTOR_FACTORY_H_

actor_handle actor_spawn(catalogue_entry e, void *params);

void *actor_retire(actor_handle a);

#endif /* SRC_ACTOR_FACTORY_H_ */
