#include "binheap.h"

static void heapPush(BinaryHeap *self, void *value) {
	if (self->count + 1 >= self->size) {
		return;
	}
	BinaryHeapNode *binheapNode = simpleAlloc(sizeof(BinaryHeapNode), false);

	binheapNode->data = value;
	binheapNode->index = self->count;
	/*binheapNode->lNode = NULL;
	binheapNode->rNode = NULL;
	binheapNode->pNode = self->heap[((((self->count) + 1) >> 1) - 1)];*/

	//self->root[self->count] = binheapNode;
	self->heap[self->count] = *binheapNode;

	self->count++;

	self->binheap_siftup(self, self->count - 1);
}

static void* heapPeek(BinaryHeap *self) {
	if (self->count == 0) {
		return NULL;
	}
	return self->heap[0].data;
}

static void* heapPop(BinaryHeap *self) {
	if (self->count == 0) {
		return NULL;
	}
	self->count--;
	void *popped = (void *) &self->heap[0];
	/*falta fazer pop do array*/

	self->heap[0] = self->heap[self->count];

	self->binheap_siftdown(self, 0);

	return popped;
}

/*static void heapify(BinaryHeap *self, int index) {
	int left, right, smallest;

	if (self->count < 2 * index + 1) {
		left = -1;
		right = -1;
	} else if (self->count == 2 * index + 1) {
		left = 2 * index + 1;
		right = -1;
	} else {
		left = 2 * index + 1;
		right = 2 * index + 2;
	}
	smallest = index;

	if (self->heap[left] < self->heap[smallest]) {
		smallest = left;
	}
	if (self->heap[right] < self->heap[smallest]) {
		smallest = right;
	}

	if (smallest != index) {
		BinaryHeapNode aux = self->heap[index];
		self->heap[index] = self->heap[smallest];
		self->heap[smallest] = aux;
	}

	heapify(self, left);
	heapify(self, right);

}*/

static void binheap_siftdown(BinaryHeap *self, int index) {
	int left, right, smallest = index;

	left = 2 * index + 1;
	right = 2 * index + 2;

	if (left < self->count && self->cmp((BinaryHeapNode *) &self->heap[left], (BinaryHeapNode *) &self->heap[index]) > 0)
		smallest = left;
	if (right < self->count && self->cmp((BinaryHeapNode *) &self->heap[right], (BinaryHeapNode *) &self->heap[index]) > 0)
		if (self->cmp((BinaryHeapNode *) &self->heap[right], (BinaryHeapNode *) &self->heap[left]) > 0)
			smallest = right;
	if (smallest == index)
		return;

	BinaryHeapNode aux = self->heap[index];
	self->heap[index] = self->heap[smallest];
	self->heap[smallest] = aux;

	self->binheap_siftdown(self, smallest);
}

static void binheap_siftup(BinaryHeap *self, int index) {
	int p;

	if (index == 0) {
		return;
	}

	p = ((((index) + 1) >> 1) - 1);

	if (self->cmp((BinaryHeapNode *) &self->heap[index], (BinaryHeapNode *) &self->heap[p]) <= 0) {
		return;
	}

	BinaryHeapNode aux = self->heap[index];
	self->heap[index] = self->heap[p];
	self->heap[p] = aux;

	self->binheap_siftup(self, p);
}

BinaryHeap* newHeap(size_t size, const int (*cmp)(const void *child, const void *parent)) {
	BinaryHeap *binheap = simpleAlloc(sizeof(BinaryHeap), false);
	if (binheap == NULL) {
		NF_ERROR(errno, "Could not create a new binary heap");
		return NULL;
	}

	binheap->count = 0;
	binheap->size = size;

	binheap->heap = simpleAlloc(sizeof(BinaryHeapNode) * size, false);
	if (binheap->heap == NULL) {
		NF_ERROR(errno, "Could not create a new binary heap array");
		return NULL;
	}

	binheap->root = simpleAlloc(sizeof(BinaryHeapNode) * size, false);
		if (binheap->root == NULL) {
			NF_ERROR(errno, "Could not create a new binary heap array");
			return NULL;
		}

	binheap->heapPeek = *heapPeek;
	binheap->heapPop = *heapPop;
	binheap->heapPush = *heapPush;
	binheap->binheap_siftup = *binheap_siftup;
	binheap->binheap_siftdown = *binheap_siftdown;
	binheap->cmp = cmp;

	return binheap;
}

void destroyHeap(BinaryHeap *heap) {
	if (heap) {
		for (int index = 0; index < heap->count; index++) {
			//simpleFree(heap->heap[index]);
		}
		simpleFree(heap->heap);
		simpleFree(heap);
	}
}
