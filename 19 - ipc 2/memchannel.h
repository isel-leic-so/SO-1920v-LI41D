#pragma once

#include <stdlib.h>
#include <stdatomic.h>
#include <semaphore.h>

#define MC_IN
#define MC_OUT

#define MEMCH_MAX_NAME_LEN 128

typedef unsigned char byte;

typedef struct memch {
	size_t elem_size;
	size_t capacity;
	char name[MEMCH_MAX_NAME_LEN];
	
	sem_t has_space;
	sem_t has_items;
	
	atomic_long iput;
	atomic_long iget;
	
	// type_with_elem_size items[capacity];
	byte data[1];
} memch_t;

memch_t * mc_create(size_t elem_size, size_t capacity, const char * name);

memch_t * mc_open(const char * name);

void mc_put(memch_t * mc, MC_IN  void * item);

void mc_get(memch_t * mc, MC_OUT void * item);

void mc_close(memch_t * mc);

void mc_destroy(memch_t * mc);
