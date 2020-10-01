#ifndef BINHEAP_H
#define BINHEAP_H

#include "utils.h"

typedef struct binary_heap BinaryHeap;
typedef struct binary_heap_node BinaryHeapNode;

struct binary_heap_node {
	void *data;
	
	size_t index;
	/*BinaryHeapNode *pNode;
	BinaryHeapNode *rNode;
	BinaryHeapNode *lNode;*/
};

struct binary_heap {
	size_t size;
	size_t count;
	BinaryHeapNode *root;
	
	BinaryHeapNode *heap;
	
	void (*heapPush)(BinaryHeap *heap, void *value);

	void* (*heapPeek)(BinaryHeap *heap);

	void* (*heapPop)(BinaryHeap *heap);

	void (*binheap_siftdown)(BinaryHeap *heap, int index);

	void (*binheap_siftup)(BinaryHeap *heap, int index);
	
	const int (*cmp)(const void *child, const void *parent);
};

BinaryHeap* newHeap(size_t size, const int (*cmp)(const void *child, const void *parent));

void destroyHeap(BinaryHeap *heap);

#endif


