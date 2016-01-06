/**
 * Author: Razi Murshed
 * I.D - 260516333
 * Assignment 4, Fall 2015
 * Create your own Malloc
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "MyMalloc.h"

/**
 * Some useful Macros defining sizes of blocks that can be malloced to test our implementation
 */
#define TWO_KB 2048
#define FOUR_KB 4096
#define FIFTEEN_KB 15000

/**
 * Tests implementation of the malloc API
 */
int main(int argc, char* argv[]) {
	FILE *output = fopen("MyMallocResult.txt", "w");
	my_mallinfo();
	my_mallopt(1);
	int* indicate_failure = my_malloc(-10);
	fprintf(output, "%s\r\n", malloc_error);
	int* test_two_kb_allocation = my_malloc(TWO_KB);
	int i = 0, j = 0;
	for (i = 0; i < TWO_KB / sizeof(int); i++) {
		test_two_kb_allocation[i] = i + j;
		j++;
	}
	my_mallinfo();
	fprintf(output, "Buffer Number 1 -> %p\r\n", test_two_kb_allocation);
	if (current_allocated_bytes() == TWO_KB){
		fprintf(output, "Successfully Allocated Requested number of Bytes!\r\n");
	}
	else{
		fprintf(output, "Error, could not allocate number of bytes\r\n");
	}
	if (current_free_space() == (BLOCKSIZE - TWO_KB)){
		fprintf(output, "Successfully freed requested bytes.\r\n");
	}
	else{
		fprintf(output, "Error, could not free requested bytes\r\n");
	}
	if (current_largest_contiguous_space() == (BLOCKSIZE - TWO_KB)){
		fprintf(output, "Successfully found largest contiguous space\r\n");
	}
	else{
		fprintf(output, "Error, largest contiguous space could not be found!\r\n");
	}
	int* test_four_kb_allocation = my_malloc(FOUR_KB);
	fprintf(output, "Buffer Number 2 -> %p\r\n", test_four_kb_allocation);

	if (current_allocated_bytes() == TWO_KB + FOUR_KB){
		fprintf(output, "Successfully Allocated Requested number of Bytes!\r\n");
	}
	else{
		fprintf(output, "Error, could not allocate number of bytes\r\n");
	}
	if (current_free_space() == (BLOCKSIZE - (TWO_KB + FOUR_KB + EXTRA_BYTES))){
		fprintf(output, "Successfully freed requested bytes.\r\n");
	}
	else{
		fprintf(output, "Error, could not free requested bytes\r\n");
	}

	if (current_largest_contiguous_space() == (BLOCKSIZE - (TWO_KB + FOUR_KB + EXTRA_BYTES))){
		fprintf(output, "Successfully found largest contiguous space\r\n");
	}
	else{
		fprintf(output, "Error, largest contiguous space could not be found!\r\n");
	}
	my_mallinfo();

	j = 0;
	for (i = 0; i < FOUR_KB / sizeof(int); i++) {
		test_four_kb_allocation[i] = i + j;
		j++;
	}
	my_free(test_two_kb_allocation);

	if (current_allocated_bytes() == FOUR_KB){
		fprintf(output, "Successfully Allocated Requested number of Bytes!\r\n");
	}
	else{
		fprintf(output, "Error, could not allocate number of bytes\r\n");
	}

	if (current_free_space() == (BLOCKSIZE - FOUR_KB)){
		fprintf(output, "Successfully freed requested bytes.\r\n");
	}
	else{
		fprintf(output, "Error, could not free requested bytes\r\n");
	}

	if (current_largest_contiguous_space() == (BLOCKSIZE - (TWO_KB + FOUR_KB + EXTRA_BYTES))){
		fprintf(output, "Successfully found largest contiguous space\r\n");
    }
	else{
		fprintf(output, "Error, largest contiguous space could not be found!\r\n");
	}
	my_mallinfo();
	int* test_additional_two_kb = my_malloc(TWO_KB);
	j = 0;
	for (i = 0; i < TWO_KB / sizeof(int); i++) {
		test_additional_two_kb[i] = i + j;
		j++;
	}
	fprintf(output, "Buffer Number 3 -> %p\r\n", test_additional_two_kb);
	if (current_allocated_bytes() == TWO_KB + FOUR_KB){
		fprintf(output, "Successfully Allocated Requested number of Bytes!\r\n");
	}
	else{
		fprintf(output, "Error, could not allocate number of bytes\r\n");
	}

	if (current_free_space() == (BLOCKSIZE - (TWO_KB + FOUR_KB + EXTRA_BYTES))){
		fprintf(output, "Successfully freed requested bytes.\r\n");
	}
	else{
		fprintf(output, "Error, could not free requested bytes\r\n");
	}

	if (current_largest_contiguous_space() == (BLOCKSIZE - (TWO_KB + FOUR_KB + EXTRA_BYTES))){
		fprintf(output, "Successfully found largest contiguous space\r\n");
	}
	else{
		fprintf(output, "Error, largest contiguous space could not be found!\r\n");
	}
	my_mallinfo();

	my_free(test_additional_two_kb);
	if (current_allocated_bytes() == FOUR_KB){
		fprintf(output, "Successfully Allocated Requested number of Bytes!\r\n");
	}
	else{
		fprintf(output, "Error, could not allocate number of bytes\r\n");
	}

	if (current_free_space() == (BLOCKSIZE - FOUR_KB)){
		fprintf(output, "number of bytes freed succesful.\r\n");
	}
	else{
		fprintf(output, "Error, could not free requested bytes\r\n");
	}

	if (current_largest_contiguous_space() == (BLOCKSIZE - (TWO_KB + FOUR_KB + EXTRA_BYTES))){
		fprintf(output, "Successfully found largest contiguous space\r\n");
	}
	else{
		fprintf(output, "Error, largest contiguous space could not be found!\r\n");
	}
	my_mallinfo();

	int* test_fifteen_kb = my_malloc(FIFTEEN_KB);
	j = 0;
	for (i = 0; i < FIFTEEN_KB / sizeof(int); i++) {
		test_fifteen_kb[i] = i + j;
		j++;
	}
	fprintf(output, "Buffer Number 4 -> %p\r\n", test_fifteen_kb);

	my_mallinfo();
	if (current_allocated_bytes() == FOUR_KB + FIFTEEN_KB){
		fprintf(output, "Successfully Allocated Requested number of Bytes!\r\n");
	}
	else{
		fprintf(output, "Error, could not allocate number of bytes\r\n");
	}

	if (current_free_space() == (2 * (BLOCKSIZE) - (FOUR_KB + FIFTEEN_KB))){
		fprintf(output, "Successfully freed requested bytes.\r\n");
	}
	else{
		fprintf(output, "Error, could not free requested bytes\r\n");
	}
	if (current_largest_contiguous_space() == (BLOCKSIZE - (TWO_KB + FOUR_KB + EXTRA_BYTES))){
		fprintf(output, "Successfully found largest contiguous space\r\n");
	}
	else{
		fprintf(output, "Error, largest contiguous space could not be found!\r\n");
	}
	my_free(test_four_kb_allocation);

	if (current_allocated_bytes() == FIFTEEN_KB){
		fprintf(output, "Successfully Allocated Requested number of Bytes!\r\n");
	}
	else{
		fprintf(output, "Error, could not allocate number of bytes\r\n");
	}

	if (current_free_space() == (2 * (BLOCKSIZE) + EXTRA_BYTES - FIFTEEN_KB)){
		fprintf(output, "Successfully freed requested bytes.\r\n");
	}
	else{
		fprintf(output, "Error, could not free requested bytes\r\n");
	}

	if (current_largest_contiguous_space() == (BLOCKSIZE + EXTRA_BYTES)){
		fprintf(output, "Successfully found largest contiguous space\r\n");
	}
	else{
		fprintf(output, "Error, largest contiguous space could not be found!\r\n");
	}
	my_mallinfo();

	my_free(test_fifteen_kb);
	if (current_allocated_bytes() == 0){
		fprintf(output, "Successfully Allocated Requested number of Bytes!\r\n");
	}
	else{
		fprintf(output, "Error, could not allocate number of bytes\r\n");
	}

	if (current_free_space() == (2 * (BLOCKSIZE) + 16)){
		fprintf(output, "Successfully freed requested bytes.\r\n");
	}
	else{
		fprintf(output, "Error, could not free requested bytes\r\n");
	}

	if (current_largest_contiguous_space() == (2 * (BLOCKSIZE) + 16)){
		fprintf(output, "Successfully found largest contiguous space\r\n");
	}
	else{
		fprintf(output, "Error, largest contiguous space could not be found!\r\n");
	}
	my_mallinfo();

	fclose(output);
	return 0;
}
