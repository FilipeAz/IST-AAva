#ifndef _FIBO_NODE_H_
#define _FIBO_NODE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct fibonacci_node fibonacci_heap_node;
struct fibonacci_node{
  fibonacci_heap_node* left;
  fibonacci_heap_node* right;
  fibonacci_heap_node* parent; // NULL iff this node is a root
  fibonacci_heap_node* kid; // Any kid
  int key;
  void* value;
  int hasLostKid;
  int degree;
};

typedef fibonacci_heap_node FibonacciHeap;
typedef fibonacci_heap_node FibonacciElement;

fibonacci_heap_node* node_init(int key, void* value);

void  freeNode(fibonacci_heap_node* toFree);
void  node_kill(fibonacci_heap_node* toKill);

void node_add(fibonacci_heap_node* old, fibonacci_heap_node* newRight);

#endif
