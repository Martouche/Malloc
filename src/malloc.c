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
    void *p;

    p = sbrk(0);
    if (sbrk(size) == (void*) -1)
        return NULL;
    return p;
}