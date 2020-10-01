#include "linkedlist.h"

static LinkedListNode* createNode(void *value) {
	LinkedListNode *node = simpleAlloc(sizeof(LinkedListNode), false);
	if(node == NULL) {
		NF_ERROR(errno, "Could not create a new linked list node");
		return NULL;
	}

	node->value = value;
	node->next = NULL;

	return node;
}

static void addFirstNode(LinkedList *self, LinkedListNode *node) {
	self->head = node;
	self->last = node;
}

static bool addIndex(LinkedList *self, const unsigned int index, void *value) {
	if(self && index >= 0 && index <= self->size) {
		LinkedListNode *node = createNode(value);

		if(node) {
			if(!self->head) {
				addFirstNode(self, node);
			}
			else {
				if(index == 0) {
					node->next = self->head;
					self->head = node;
				}
				else if(index == self->size) {
					self->last->next = node;
					self->last = node;
				}
				else {
					LinkedListNode *inode = self->head;
					for(unsigned int i = 1; i < index; i++, inode = inode->next);

					LinkedListNode *nextNode = inode->next;
					inode->next = node;
					node->next = nextNode;
				}
			}

			self->size += 1;
			return true;
		}
	}

	return false;
}

static bool addFirst(LinkedList *self, void *value) {
	if(self) {
		return addIndex(self, 0, value);
	}

	return false;
}

static bool addLast(LinkedList *self, void *value) {
	if(self) {
		return addIndex(self, self->size, value);
	}

	return false;
}

static void removeOnlyNode(LinkedList *self) {
	simpleFree(self->head);
	self->head = NULL;
	self->last = NULL;
}

static bool removeIndex(LinkedList *self, const unsigned int index) {
	if(self && self->size && index >= 0 && index < self->size) {
		if(self->size == 1) {
			removeOnlyNode(self);
		}
		else {
			LinkedListNode *node = NULL;

			if(index == 0) {
				node = self->head;
				self->head = self->head->next;
			}
			else if(index == self->size - 1) {
				node = self->last;
				LinkedListNode *inode = self->head;
				for(; inode->next != node; inode = inode->next);
				inode->next = NULL;
				self->last = inode;
			}
			else {
				LinkedListNode *inode = self->head;
				for(unsigned int i = 1; i < index; i++, inode = inode->next);
				node = inode->next;
				inode->next = inode->next->next;
			}

			simpleFree(node);
		}

		self->size -= 1;
		return true;
	}

	return false;
}

static bool removeFirst(LinkedList *self) {
	if(self && self->size) {
		return removeIndex(self, 0);
	}

	return false;
}

static bool removeLast(LinkedList *self) {
	if(self && self->size) {
		return removeIndex(self, self->size - 1);
	}

	return false;
}

static void printList(const LinkedList *self, char* (valueToStr)(const void *value)) {
	if(self) {
		LinkedListNode *node = self->head;

		printf("HEAD -> ");

		for(; node; node = node->next) {
			printf("%s -> ", valueToStr(node->value));
		}

		printf("TAIL\n");
	}
}

LinkedList* newLinkedList() {
	LinkedList *list = simpleAlloc(sizeof(LinkedList), false);
	if(list == NULL) {
		NF_ERROR(errno, "Could not create a new linked list");
		return NULL;
	}

	list->size = 0;
	list->head = NULL;
	list->last = NULL;

	list->addFirst = *addFirst;
	list->addIndex = *addIndex;
	list->addLast = *addLast;
	list->removeFirst = *removeFirst;
	list->removeIndex = *removeIndex;
	list->removeLast = *removeLast;

	list->printList = *printList;

	return list;
}

void destroyList(LinkedList *list) {
	while(removeFirst(list));
	simpleFree(list);
}

