/*
** EPITECH PROJECT, 2019
** c project malloc
** File description:
** made by Martin Vantalon and Thibault FOUCOU
*/

#include "../include/malloc.h"

header_t *head = NULL;
header_t *tail = NULL;

void *malloc(size_t size)
{
    void *p = sbrk(0);
    void *request = sbrk(size);

    if (request == (void*) -1)
        return NULL;
    else
        assert(p == request);
    return p;
}