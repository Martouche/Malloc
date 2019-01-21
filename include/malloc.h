/*
** EPITECH PROJECT, 2019
** c project malloc.h
** File description:
** made by Martin Vantalon
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

struct header_s {
	size_t size;
	int is_free;
	struct header_t *next;
    struct header_t *prev;
} header_t;

#endif