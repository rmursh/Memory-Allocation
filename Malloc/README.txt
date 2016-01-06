/**
 * Author: Razi Murshed
 * I.D - 260516333
 * Assignment 4, Fall 2015
 * Create your own Malloc
 */

NOTES:

*The program break requires 8 extra bytes as written in code for the the first set of tags.The number of bytes allocated does not use these extrab ytes or it does not affect their size. We have to account forthese bytes every time sbrk() is called to allocate space. Consequently, for every full sized block that sbrk() allocates, 8 extra bytes have to be included.

*The maximum possible error reported can be 1024 bytes with another 4 bytes that are used as a buffer.

*The free blocks is implemented as a doubly linked list with start and end pointers.

*The test outputs are given in the file "MyMallocResult.txt"

*Console outputs are given in the file "MallocConsoleOutput.txt"

*To compile use the Makefile

*To run use the executable generated.




