/*
 * tree.h
 *
 *  Created on: Jun 28, 2025
 *      Author: samue
 */

#ifndef SRC_TREE_H_
#define SRC_TREE_H_

typedef struct {
	actor_handle father;
	actor_handle sx;
	actor_handle dx;
	uint32_t sum;
} tree_st;

#endif /* SRC_TREE_H_ */
