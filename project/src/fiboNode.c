#include "fiboNode.h"

fibonacci_heap_node* node_init(int key, void* value){
    fibonacci_heap_node* newNode = malloc(sizeof(fibonacci_heap_node));
    newNode->left = newNode->right = newNode;
    newNode->key = key;
    newNode->value = value;
    newNode->parent = NULL;
    newNode->kid = NULL;
    newNode->hasLostKid = 0;
    newNode->degree = 0;
    return newNode;
}

void freeNode(fibonacci_heap_node* toFree){
    toFree->degree = -1;
    free(toFree);
}

void node_kill(fibonacci_heap_node* toKill){
    fibonacci_heap_node* kid = toKill->kid;
    if (kid){
        kid->left->right = NULL;
        while(kid->right != NULL){
            kid = kid->right;
            node_kill(kid->left);
        }
        node_kill(kid);
    }
    freeNode(toKill);
}

void node_add(fibonacci_heap_node* old, fibonacci_heap_node* newRight){
    fibonacci_heap_node* oldRight = old->right;
    assert(old != newRight);
    assert(oldRight != newRight);
    old->right = newRight;
    oldRight->left = newRight;
    newRight->left = old;
    newRight->right = oldRight;
}
