#ifndef BINOMIALHEAP_H
#define BINOMIALHEAP_H

#include "utils.h"
#include "linkedlist.h"
#include "graph.h"

typedef struct binomial_heap BinomialHeap;
typedef struct binomial_heap_node BinomialHeapNode;

struct binomial_heap_node {
	void *data;

	int rank;
	size_t index;

	LinkedList *children;

};

struct binomial_heap {
	size_t size;

	LinkedList *roots;


	BinomialHeapNode* (*mergeTree)(BinomialHeap *heap, BinomialHeapNode *self, BinomialHeapNode *other);

	void (*merge)(BinomialHeap *heap, BinomialHeap *other);

	void (*insert)(BinomialHeap *heap, void *value);

	BinomialHeapNode* (*findMinimum)(BinomialHeap *heap, int *index);

	BinomialHeapNode* (*deleteMinimum)(BinomialHeap *heap);

	BinomialHeapNode* (*elementExists)(BinomialHeap *self, void *value);

	BinomialHeapNode* (*elementExistsBinomialHeapNode)(BinomialHeap *self, BinomialHeapNode *node, void *value);

	const int (*cmp)(const void *child, const void *parent);
};

BinomialHeap* newBinomialHeap(const int (*cmp)(const void *child, const void *parent));

void destroyBinomialHeap(BinomialHeap *heap);

void destroyBinomialHeapNode(BinomialHeapNode *node);

void printBinomialHeap(BinomialHeap *heap, char* (valueToStr)(const void *value));

void printBinomialHeapNode(BinomialHeapNode *node, char* (valueToStr)(const void *value));

#endif

