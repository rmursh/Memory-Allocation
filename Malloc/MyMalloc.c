/**
 * Author: Razi Murshed
 * I.D - 260516333
 * Assignment 4, Fall 2015
 * Create your own Malloc
 */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "MyMalloc.h"
/**
 * Creating head and tail for double linked list
 */
FreeList *head;
FreeList *tail;
/**
 * Global variable declaration
 */
int current_allocation_strategy = 1;
int allocated_bytes = 0;
int space_free = 0;
int max_available_space = 0;
int number_of_malloc_calls = 0;
char *malloc_error; //constant for error reporting
int* top_program_break; //holds the current top of the program break

/**
 * A few useful macros in order to do computations quicker.
 */
#define TAG_LENGTH(length) (length >> 1)
#define FREE_TAG_LENGTH(pointer) (pointer & 0b1)
#define ADJUST_MALLOC(x) ((x/BLOCKSIZE + 1) * BLOCKSIZE)
#define INCREMENT_POINTER(pointer, length) (((char*)pointer) + length)
#define DECREMENT_POINTER(pointer, length) (((char*)pointer) - length)
#define CREATE_NEW_TAG(length,free) ((length << 1) + (free == 1 ? 0b0 : 0b1))

/**
 * Number of Bytes allocated
 */
int current_allocated_bytes()
{
	return (int) allocated_bytes;
}
/**
 * Free space available
 */
int current_free_space()
{
	return (int) space_free;
}
/**
 * Largest contiguous space available
 */
int current_largest_contiguous_space()
{
	return (int) max_available_space;
}

/**
 * Adds node to linked list
 */
void add_node(FreeList *newList)
{
	if (head != NULL) {
		head->previous_node = newList;
		newList->next_node = head;
		head = newList;
		newList->previous_node = NULL;
	}
	else {
		head = newList;
		tail = newList;
	}
}

/**
 * Removes a node from Linked list
 */
void remove_node(FreeList *iter) //removes a node from the doubly linked list
{
	if (iter->previous_node != NULL){
		iter->previous_node->next_node = iter->next_node;
	}
	if (iter->next_node != NULL){
		iter->next_node->previous_node = iter->previous_node;
	}
	if (iter == head){
		head = iter->next_node;
	}
	if (iter == tail){
		tail = iter->previous_node;
	}
	iter->next_node = NULL;
	iter->previous_node = NULL;
	iter = NULL;
}

/**
 * Checks largest block in the free list in order to update information as required
 */
void update_contiguous_block()
{
	int largest_avaliable = 0;
	FreeList *iter = head;
	while (iter != NULL) {
		int* update = (int*) iter;
		update = (int*) DECREMENT_POINTER(update, 4);
		int update_size = TAG_LENGTH(update[0]);
		if (update_size > largest_avaliable){
			largest_avaliable = update_size;
		}
		iter = iter->next_node;
	}
	max_available_space = largest_avaliable;
}

/**
 * Check if top block is free and update it.
 */
void update_top_empty_block()
{
	int* current_top = (int*) (top_program_break);
	current_top = (int*) DECREMENT_POINTER(current_top, 4);
	int top_tag = FREE_TAG_LENGTH(current_top[0]);
	if (top_tag == 0) {
		unsigned top_size = TAG_LENGTH(current_top[0]);

		if (top_size >= TOP_FREE)
		{
			current_top = (int*) DECREMENT_POINTER(current_top, (top_size - 4));
			sbrk(-END_DATA_SEGMENT);
			top_size -= END_DATA_SEGMENT;

			*current_top = CREATE_NEW_TAG(top_size, 1);
			current_top = (int*) INCREMENT_POINTER(current_top, (top_size - 4));

			*current_top = CREATE_NEW_TAG(top_size, 1);
			top_program_break = (int*) DECREMENT_POINTER(top_program_break, END_DATA_SEGMENT);

			space_free -= END_DATA_SEGMENT;
		}
	}
	update_contiguous_block();
}

/**
 * Creates a new block
 */
void* create_new_block(int size, int best_fit) {
	int total_size = ADJUST_MALLOC(size);
	//Check if  a new block is to be created
	if (best_fit == -1 || best_fit == 131073){
		//Can the total size be in a block of its own?
		if (total_size > (size + sizeof(FreeList) + EXTRA_BYTES)) {
			total_size = ADJUST_MALLOC(size);
		}
		//Can't so create a second block
		else {
			total_size += BLOCKSIZE;
		}

		int* begin = (int*) sbrk(total_size + EXTRA_BYTES);
		int* free_end = (int*) sbrk(0);
		allocated_bytes += size;
		top_program_break = free_end;

		*begin = CREATE_NEW_TAG(size, 0);
		begin = (int*) INCREMENT_POINTER(begin, 4);
		int* end = (int*) INCREMENT_POINTER(begin, size);
		*end = CREATE_NEW_TAG(size, 0);

		int* free_begin = (int*) INCREMENT_POINTER(end, 4);
		*free_begin = CREATE_NEW_TAG((total_size - size), 1);
		free_begin = (int*) INCREMENT_POINTER(free_begin, 4);

		FreeList *new_list = (FreeList*) free_begin; //add a new free block
		add_node(new_list);

		space_free += (total_size - size);
		free_end = (int*) INCREMENT_POINTER(free_begin, (total_size-(size + EXTRA_BYTES)));
		*free_end = CREATE_NEW_TAG((total_size - size), 1);

		update_contiguous_block();
		return (void*) begin;
	}
	//if block is specified
	else {

			FreeList *iter = head;
			while (iter != NULL) {
				int* temp = (int*) (iter);
				temp = (int*) DECREMENT_POINTER(temp, 4);
				unsigned avail_size = TAG_LENGTH(temp[0]);
				if (avail_size == best_fit){
					break;
				}
				else{
					iter = iter->next_node;
				}
			}
			int* temp = (int*) (iter);
			temp = (int*) DECREMENT_POINTER(temp, 4);
			unsigned available_size = TAG_LENGTH(temp[0]);

			//Fill up all the Space
			if (available_size - EXTRA_BYTES >= size && available_size > (size + sizeof(FreeList) + EXTRA_BYTES)) {
				int new_size = (available_size - (size + EXTRA_BYTES));
				int* begin = (int*) temp;
				int* free_end = (int*) INCREMENT_POINTER(temp, (available_size));
				*begin = CREATE_NEW_TAG(size, 0);
				begin = (int*) INCREMENT_POINTER(begin, 4);
				int* end = (int*) INCREMENT_POINTER(begin, size);
				*end = CREATE_NEW_TAG(size, 0);
				int* free_begin = (int*) INCREMENT_POINTER(end, 4);
				*free_begin = CREATE_NEW_TAG(new_size, 1);
				free_begin = (int*) INCREMENT_POINTER(free_begin, 4);
				remove_node(iter);
				FreeList *new_list = (FreeList*) free_begin;
				add_node(new_list);
				free_end = (int*) DECREMENT_POINTER(free_end, 4);
				*free_end = CREATE_NEW_TAG(new_size, 1);
				int val = TAG_LENGTH(free_end[0]);
				allocated_bytes += size;
				space_free -= (size + EXTRA_BYTES);
				update_contiguous_block();
				return (void*) begin;
			}
			//Dont split
			else if (available_size - EXTRA_BYTES >= size) {
				int* start = (int*) temp;
				*start = CREATE_NEW_TAG((available_size-EXTRA_BYTES), 0);
				start = (int*) INCREMENT_POINTER(start, 4);
				int* end = (int*) INCREMENT_POINTER(start, (available_size-8));
				*end = CREATE_NEW_TAG((available_size-EXTRA_BYTES), 0);
				remove_node(iter);
				allocated_bytes += (available_size - EXTRA_BYTES);
				space_free -= available_size;
				fprintf(stdout, " %d extra bytes allocated to Malloc\n", available_size - (size + EXTRA_BYTES));
				update_contiguous_block();
				return (void*) start;
			}
	}
}

/**
 * Carries out memory allocation
 */
void *my_malloc(int size) {
	if (number_of_malloc_calls == 0){
		malloc_error = (char*) sbrk(1028); //Setup error
	}
	number_of_malloc_calls++;
	if (size < 0) {
		malloc_error = "Space requested is negative!";
		return NULL;
	}
	//If strategy is first fit
	if (current_allocation_strategy == FIRST_FIT) {
		//find the first available free block that can fit requested size
		FreeList *iter = head;
		while (iter != NULL) {
			int* check = (int*) (iter);
			check = (int*) DECREMENT_POINTER(check, 4);
			unsigned available_size = TAG_LENGTH(check[0]);
			if (available_size - EXTRA_BYTES >= size){
				return create_new_block(size, available_size);
			}
			if (iter != NULL){
				iter = iter->next_node;
			}
		}
		//Create new block if none available currently
		return create_new_block(size, -1);
	}
	//If strategy is best fit
	else if (current_allocation_strategy == BEST_FIT) {
		FreeList *iter = head;
		unsigned best_fit = TOP_FREE + 1;
		//find the best fitting free block for requested memory
		while (iter != NULL)
		{
			int* check = (int*) (iter);
			check = (int*) DECREMENT_POINTER(check, 4);
			unsigned available_size = TAG_LENGTH(check[0]);
			if (available_size - EXTRA_BYTES == size)
				return create_new_block(size, available_size);
			else if (available_size - EXTRA_BYTES >= size && available_size <= best_fit)
				best_fit = available_size;
			if (iter != NULL)
				iter = iter->next_node;
		}
		return create_new_block(size, best_fit);
	}
	malloc_error = "Could not malloc"; //Check for problems while trying to malloc
	return NULL;
}

/**
 * Deallocates block of memory
 */
void my_free(void *ptr) {
	int* recently_freed = (int*) (ptr); //Gets information about the size
	recently_freed = (int*) DECREMENT_POINTER(recently_freed, 4);

	int free_size = TAG_LENGTH(recently_freed[0]);

	allocated_bytes -= free_size;
	space_free += (free_size + EXTRA_BYTES);

	int* check_bottom = (int*) DECREMENT_POINTER(recently_freed, 4);
	int* check_top = (int*) INCREMENT_POINTER(recently_freed, (free_size + EXTRA_BYTES));

	int is_bottom_free = FREE_TAG_LENGTH(check_bottom[0]);
	int is_top_free = FREE_TAG_LENGTH(check_top[0]);
	//if malloc starts at last block dont free any more blocks
	int bottom_size = TAG_LENGTH(check_bottom[0]);
	if (bottom_size == 0){
		is_bottom_free = -1;
	}
	//Check if top and bottom blocks are available to use
	if (!is_bottom_free && !is_top_free)
	{
		int size_of_bottom = TAG_LENGTH(check_bottom[0]);
		check_bottom = (int*) DECREMENT_POINTER(check_bottom, (size_of_bottom - EXTRA_BYTES));
		int size_of_top = TAG_LENGTH(check_top[0]);
		check_top = (int*) INCREMENT_POINTER (check_top, 4);
		FreeList *bottom_remaining = (FreeList*) (check_bottom);
		FreeList *top_remaining = (FreeList*) (check_top);
		remove_node(bottom_remaining);
		remove_node(top_remaining);
		//updating properties
		free_size += (size_of_bottom + size_of_top + EXTRA_BYTES);
		check_bottom = (int*) DECREMENT_POINTER(check_bottom, 4);
		check_top = (int*) INCREMENT_POINTER(check_top, (size_of_top - 8));
		*check_bottom = CREATE_NEW_TAG(free_size, 1);
		*check_top = CREATE_NEW_TAG(free_size, 1);
		int* begin = (int*) INCREMENT_POINTER(check_bottom, 4);
		FreeList *new = (FreeList*) begin;
		add_node(new);
	}
	//if only the bottom block is free
	else if (!is_bottom_free)
	{
		int size_of_bottom = TAG_LENGTH(check_bottom[0]);
		check_bottom = (int*) DECREMENT_POINTER(check_bottom, (size_of_bottom - EXTRA_BYTES));

		FreeList *bottom_remaining = (FreeList*) (check_bottom); //remove the previous free list node
		remove_node(bottom_remaining);

		check_bottom = (int*) DECREMENT_POINTER(check_bottom, 4);
		recently_freed = (int*) INCREMENT_POINTER(recently_freed, (free_size + 4));

		free_size += (size_of_bottom + EXTRA_BYTES); //update the size and tag information
		*check_bottom = CREATE_NEW_TAG(free_size, 1);
		*recently_freed = CREATE_NEW_TAG(free_size, 1);

		int* begin = (int*) INCREMENT_POINTER(check_bottom, 4);

		FreeList *new_list = (FreeList*) begin;
		add_node(new_list); //add the new free list node to the linked list
	}
	//if only the top block is free
	else if (!is_top_free)
	{
		int top_size = TAG_LENGTH(check_top[0]);
		check_top = (int*) INCREMENT_POINTER (check_top, (4));
		FreeList *rem_top = (FreeList*) (check_top);
		remove_node(rem_top);
		//update information
		check_top = (int*) INCREMENT_POINTER(check_top, (top_size - EXTRA_BYTES));
		free_size += (top_size + EXTRA_BYTES);
		*recently_freed = CREATE_NEW_TAG(free_size, 1);
		*check_top = CREATE_NEW_TAG(free_size, 1);
		int* begin = (int*) INCREMENT_POINTER(recently_freed, 4);
		FreeList *new_list = (FreeList*) begin;
		add_node(new_list);
	}
	//If nothing is free, free this block and add node
	else {
		*recently_freed = CREATE_NEW_TAG((free_size + EXTRA_BYTES), 1);
		recently_freed = (int*) INCREMENT_POINTER(recently_freed, (free_size + 4));
		*recently_freed = CREATE_NEW_TAG((free_size + EXTRA_BYTES), 1);

		int* start = (int*) DECREMENT_POINTER(recently_freed, free_size);
		FreeList *new = (FreeList*) start;
		add_node(new);
	}
	update_top_empty_block();
}
/**
 * Allocates the policy
 */
void my_mallopt(int policy)
{
	if (policy == FIRST_FIT){
		current_allocation_strategy = FIRST_FIT;
	}
	else if (policy == BEST_FIT){
		current_allocation_strategy = BEST_FIT;
	}
	else {
		fprintf(stderr, "Please enter a valid policy!!");
		return;
	}
}
/**
 * Gives the status of the malloc information
 */
void my_mallinfo()
{
	fprintf(stdout, "Number of bytes currently allocated -> %d\n", allocated_bytes);
	fprintf(stdout, "Free space currently available -> %d\n", space_free);
	fprintf(stdout, "Largest contiguous free space currently available -> %d\n",max_available_space);
	fprintf(stdout, "Total Mallocs requested -> %d\n", number_of_malloc_calls);
	fprintf(stdout, "Current policy applied -> %d\n", current_allocation_strategy);

}


