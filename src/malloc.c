/*
** EPITECH PROJECT, 2019
** c project malloc
** File description:
** made by Martin Vantalon and Thibault FOUCOU
*/

#include "../include/malloc.h"

header_t *head = NULL;
header_t *tail = NULL;

header_t *get_free_block(size_t size)
{
	header_t *curr;

	while (curr) {
		if (curr->is_free != 0 && curr->size >= size)
			return curr;
		curr = curr->next;
	}
	return NULL;
}

void *my_malloc(size_t size)
{
	void *block;
    header_t *header;

    block = sbrk(size);
    if (!size)
        return NULL;
    if (block == (void*) -1)
        return NULL;
	header = get_free_block(size);
    if (header) {
        header->is_free = 0;
        return (void*)(header + 1);
    }
	return block;
}
