#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_

#include "utils.h"

typedef struct linked_list LinkedList;
typedef struct linked_list_node LinkedListNode;

struct linked_list_node {
	void *value;
	LinkedListNode *next;
};

struct linked_list {
	unsigned int size;
	LinkedListNode *head;
	LinkedListNode *last;

	bool (*addFirst)(LinkedList *self, void *value);
	bool (*addIndex)(LinkedList *self, const unsigned int index, void *value);
	bool (*addLast)(LinkedList *self, void *value);
	bool (*removeFirst)(LinkedList *self);
	bool (*removeIndex)(LinkedList *self, const unsigned int index);
	bool (*removeLast)(LinkedList *self);

	void (*printList)(const LinkedList *self, char* (valueToStr)(const void* value));
};

LinkedList* newLinkedList();

void destroyList(LinkedList *list);

#endif
