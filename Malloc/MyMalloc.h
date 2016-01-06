/**
 * Author: Razi Murshed
 * I.D - 260516333
 * Assignment 4, Fall 2015
 * Create your own Malloc
 */
#ifndef _MALLOC_API_H_
#define _MALLOC_API_H_

extern char *malloc_error;

/**
 * Useful constants
 */
#define FIRST_FIT 1
#define BEST_FIT 2
#define BLOCKSIZE 16384
#define TOP_FREE 131072
#define END_DATA_SEGMENT 20000
#define EXTRA_BYTES 8

//struct for free blocks
typedef struct FreeListNode {
	struct FreeListNode *previous_node;
	struct FreeListNode *next_node;

} FreeList;

void *my_malloc(int size);
void my_free(void *ptr);
void my_mallopt(int policy);
void my_mallinfo();
int current_free_space();
int current_allocated_bytes();
int current_largest_contiguous_space();

#endif
