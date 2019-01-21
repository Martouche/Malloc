/*
** EPITECH PROJECT, 2019
** c project malloc
** File description:
** made by Martin Vantalon
*/

#include "../include/malloc.h"

struct header_t *head, *tail;

void *malloc(size_t size)
{
	void *block;
	block = sbrk(size);
	if (block == (void*) -1)
		return NULL;
	return block;
}

struct header_t *get_free_block(size_t size)
{
	struct header_t *curr = head;
	while (curr) {
		if (curr->is_free != 0 && curr->size >= size)
			return curr;
		curr = curr->next;
	}
	return NULL;
}
