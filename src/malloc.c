/*
** EPITECH PROJECT, 2019
** c project malloc
** File description:
** made by Martin Vantalon and Thibault FOUCOU
*/

#include "malloc.h"

void *my_malloc(size_t size)
{
	void *block;

	block = sbrk(size);
	if (block == (void*) -1)
		return NULL;
	return block;
}

/*header_t *get_free_block(size_t size)
{
	header_t *curr;

	while (curr) {
		if (curr->is_free != 0 && curr->size >= size)
			return curr;
		curr = curr->next;
	}
	return NULL;
}*/