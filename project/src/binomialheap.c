#include "binomialheap.h"

static BinomialHeapNode* mergeTree(BinomialHeap *heap, BinomialHeapNode *self, BinomialHeapNode *other) {
	if (heap->cmp(self->data,other->data)) {
		self->rank++;
		self->children->addLast(self->children, other);
		return self;
	} else {
		other->rank++;
		other->children->addLast(other->children, self);
		return other;
	}

}

static void merge(BinomialHeap *self, BinomialHeap *other) {
	int flag = 0;
	LinkedListNode *otherRoots = other->roots->head;
	while (otherRoots != NULL) {
		LinkedListNode *selfRoots = self->roots->head;
		if (selfRoots == NULL) {
			self->roots->addLast(self->roots, otherRoots->value);
			otherRoots = otherRoots->next;
			continue;
		}
		int i = 0;
		for (; selfRoots != NULL; selfRoots = selfRoots->next, i++) {
			if (((BinomialHeapNode *) selfRoots->value)->rank == ((BinomialHeapNode *) otherRoots->value)->rank) {
				flag = 1;
				selfRoots->value = self->mergeTree(self, selfRoots->value, otherRoots->value);

				if(selfRoots->next != NULL && ((BinomialHeapNode *)selfRoots->next->value)->rank == ((BinomialHeapNode *)selfRoots->value)->rank) {
					selfRoots->value = self->mergeTree(self, selfRoots->value, selfRoots->next->value);
					self->roots->removeIndex(self->roots, i+1);
				}
			}
			else if (((BinomialHeapNode *) selfRoots->value)->rank > ((BinomialHeapNode *) otherRoots->value)->rank) {
				break;
			}
		}
		if(flag == 0) {
			self->roots->addIndex(self->roots, i, otherRoots->value);
		}
		otherRoots = otherRoots->next;
		flag = 0;
	}



}

static void insert(BinomialHeap *self, void *value) {
	BinomialHeap *newHeap = newBinomialHeap(NULL);
	BinomialHeapNode *node = simpleAlloc(sizeof(BinomialHeapNode), false);

	node->data = value;
	node->rank = 0;
	node->children = newLinkedList();

	newHeap->roots->addFirst(newHeap->roots, node);

	self->merge(self, newHeap);
}

static BinomialHeapNode* findMinimum(BinomialHeap *self, int *index) {
	LinkedListNode *l = self->roots->head;
	BinomialHeapNode *min = l->value;
	for (int i = 0; l != NULL; l = l->next, i++) {
		if (min->data > ((BinomialHeapNode *) l->value)->data)
			min = l->value;
		*index = i;
	}
	return min;

}

static BinomialHeapNode* elementExists(BinomialHeap *self, void *value) {
	BinomialHeapNode *returned;
	if (self) {
		for (LinkedListNode *aux = self->roots->head; aux != NULL; aux = aux->next) {
			if((returned = self->elementExistsBinomialHeapNode(self, aux->value, value)) != NULL) {
				return returned;
			}
		}
	}
	return NULL;
}

static BinomialHeapNode* elementExistsBinomialHeapNode(BinomialHeap *self, BinomialHeapNode *node, void *value) {
	BinomialHeapNode *returned;
	for (LinkedListNode *aux = node->children->head; aux != NULL; aux = aux->next) {
		if((returned = self->elementExistsBinomialHeapNode(self, aux->value, value)) != NULL) {
			return returned;
		}
	}
	if(strcmp(((GraphNode *)node->data)->string, value)) {
		return node;
	}
	return NULL;
}

static BinomialHeapNode* deleteMinimum(BinomialHeap *self) {
	int *index;
	BinomialHeapNode *min = self->findMinimum(self, index);

	BinomialHeap *new = newBinomialHeap(NULL);
	new->roots = min->children;

	self->roots->removeIndex(self->roots, *index);
	self->merge(self, new);
	return min;

}

BinomialHeap* newBinomialHeap(const int (*cmp)(const void *child, const void *parent)) {
	BinomialHeap *binomialheap = simpleAlloc(sizeof(BinomialHeap), false);
	if (binomialheap == NULL) {
		NF_ERROR(errno, "Could not create a new binary heap");
		return NULL;
	}


	binomialheap->roots = newLinkedList();

	binomialheap->mergeTree = mergeTree;
	binomialheap->merge = merge;
	binomialheap->insert = insert;
	binomialheap->findMinimum = findMinimum;
	binomialheap->deleteMinimum = deleteMinimum;
	binomialheap->elementExists = elementExists;
	binomialheap->elementExistsBinomialHeapNode = elementExistsBinomialHeapNode;
	binomialheap->cmp = cmp;

	return binomialheap;
}

void destroyBinomialHeap(BinomialHeap *heap) {
	if (heap) {
		for (LinkedListNode *aux = heap->roots->head; aux != NULL; aux = aux->next) {
			destroyBinomialHeapNode(aux->value);
		}
		destroyList(heap->roots);
		simpleFree(heap);
	}
}

void destroyBinomialHeapNode(BinomialHeapNode *node) {
	for (LinkedListNode *aux = node->children->head; aux != NULL; aux = aux->next) {
		destroyBinomialHeapNode(aux->value);
	}
	destroyList(node->children);
	simpleFree(node);
}

void printBinomialHeap(BinomialHeap *heap, char* (valueToStr)(const void *value)) {
	if (heap) {
		for (LinkedListNode *aux = heap->roots->head; aux != NULL; aux = aux->next) {
			printBinomialHeapNode(aux->value, valueToStr);
		}
	}
}

void printBinomialHeapNode(BinomialHeapNode *node, char* (valueToStr)(const void *value)) {
	for (LinkedListNode *aux = node->children->head; aux != NULL; aux = aux->next) {
		printBinomialHeapNode(aux->value, valueToStr);
	}

	printf("rank: %d and value %s\n", node->rank, valueToStr(node->data));
}

