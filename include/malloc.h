/*
** EPITECH PROJECT, 2019
** c project malloc.h
** File description:
** made by Martin Vantalon and Thibault FOUCOU
*/

#ifndef MALLOC_H_
#define MALLOC_H_

#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>

typedef struct header_s {
    size_t size;
    int is_free;
    struct header_s *next;
    struct header_s *prev;
} header_t;

void *malloc(size_t size);
void *realloc(void *ptr, size_t size);
void free(void *ptr);
#endif
