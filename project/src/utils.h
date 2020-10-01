#ifndef UTILS_H
#define UTILS_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <error.h>
#include <math.h>
#include <limits.h>
#include <inttypes.h>
#include <time.h>
#include <omp.h>

#include "macros.h"

/**
 * Tests if a C-style string is null or empty
 * @param in str 	string to test
 * @return 			whether str is null or empty
 **/
bool nullOrEmpty(const char *str);

/**
 * Determines the corresponding C-style string length for an integer
 * without converting it
 * @param in number 	integer to calculate C-style string length
 * @return 				length of number as a C-style string
 **/
unsigned short intStrLength(const int number);

/**
 * Determines the maximum C-style string length for an array of integers
 * without converting them
 * @param in numbers 	array of integers to calculate maximum C-style string length
 * @param in size		size of the array
 * @return 				maximum length of a C-style string length corresponding to an integer in the array
 **/
unsigned short intStrMaxLength(const int *numbers, const size_t size);

/**
 * Allocates an array with the specified byte size and initializes its memory to a clean state
 *	@param in bytes 		size of the array in bytes
 *  @param in exitOnFail 	whether to terminate the program on failure
 *	@return 				pointer to array starting position
 **/
void* simpleAlloc(const size_t bytes, const bool exitOnFail);

/**
 *	Frees the memory allocated to a pointer
 * 	@param in array 	pointer to memory to be freed
 *	@return 			whether array was freed
 **/
bool simpleFree(void *ptr);

/**
 * Opens a valid file system file
 * @param in filePath 		file path
 * @param in mode			mode to open the file as [r, w, a, r+, w+, a+]
 * @param in exitOnFail 	whether to terminate the program on failure
 * @return 					file handle to the requested file or NULL
 **/
FILE* openFile(const char *filePath, const char *mode, const bool exitOnFail);

/**
 * Closes a previously opened file handle
 * @param in file 			file handle to close
 * @param in exitOnFail 	whether to terminate the program on failure
 * @return 					boolean value indicating success or failure
 **/
bool closeFile(FILE *file, const bool exitOnFail);

/**
 * Reads a file given a format as fscanf would and stores the result
 * on the variadicly supplied variables
 * @param in file 			file handle to read
 * @param in exitOnFail 	whether to terminate the program on failure
 * @param in format 		format to match against the file
 * @param variadic in 		variables to store the matched results [one per format type specifier]
 * @return 					boolean value indicating success or failure
 **/
bool readFile(FILE *file, const bool exitOnFail, const char *format, ...);

/**
 * Prints a char array as if it were a bit array
 * @param in bitArray 		array to print
 * @param in size 			array size in number of bits
 * @param in bitsLine 		number of bits per line
 * @return 					void
 **/
void printCharBitArray(const char *bitArray, const size_t size, const size_t bitsLine);

#endif
