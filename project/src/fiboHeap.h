#ifndef _FIBO_HEAP_H_
#define _FIBO_HEAP_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fiboNode.h"

typedef struct{
    int key;     // Indicates the minimum
    void* value; //Arbitrary client data
} HeapData;


FibonacciHeap* heap_init();
int is_empty(FibonacciHeap* H);

//Returns a pointer to the element inserted, for decrease_key and delete
FibonacciElement* heap_insert(FibonacciHeap** H, int key, void* value);

//Don't call either of these on the empty heap
HeapData  heap_min(FibonacciHeap* H);
HeapData  heap_extract_min(FibonacciHeap** H);

FibonacciHeap* heap_union(FibonacciHeap* H1, FibonacciHeap* H2);

//newKey must be less than elem's old key
void  heap_decrease_key(FibonacciHeap** H, FibonacciElement* x, int newKey);
void  heap_delete(FibonacciHeap** H, FibonacciElement* x);

HeapData  elem_data(FibonacciElement* x);
void  elem_set_value(FibonacciElement* x, void* newValue);


void heap_free(FibonacciHeap** H);

#endif
