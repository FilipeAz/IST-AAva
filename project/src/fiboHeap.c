#include "fiboHeap.h"

#include <stdio.h>
#include <limits.h>
#include <assert.h>

#include "fiboNode.h"

FibonacciHeap* heap_init(){
    return NULL;
}

FibonacciElement* heap_add(FibonacciHeap** H, fibonacci_heap_node* newNode);

FibonacciElement* heap_insert(FibonacciHeap** H, int key, void* value){
    fibonacci_heap_node* newNode = node_init(key, value);
    return heap_add(H, newNode);
}

int is_empty(FibonacciHeap* H){
    return H == NULL;
}

FibonacciElement* heap_add(FibonacciHeap** H, fibonacci_heap_node* newNode){
    assert(H);
    assert(newNode);
    fibonacci_heap_node* oldNode = *H;
    newNode->parent = NULL;
    newNode->hasLostKid = 0;
    if (oldNode){ //nonempty heap
        node_add(oldNode, newNode);
        if (oldNode->key > newNode->key){ //new smallest
            *H = newNode;
        }
    }else{ //previously empty heap
        newNode->left = newNode;
        newNode->right = newNode;
        *H = newNode;
    }
    return newNode;
}

HeapData  heap_min(FibonacciHeap* H){
    assert(H);
    HeapData d;
    fibonacci_heap_node* head = H;
    d.key = head->key;
    d.value = head->value;
    return d;
}

void  heap_consolidate(FibonacciHeap** H);
void  heap_match_degrees(FibonacciHeap** H, fibonacci_heap_node** A, fibonacci_heap_node* x);
fibonacci_heap_node* heap_link(FibonacciHeap** H, fibonacci_heap_node* x, fibonacci_heap_node* y);
void  heap_remove_from(FibonacciHeap** H, fibonacci_heap_node* x);

HeapData  heap_extract_min(FibonacciHeap** H){
    assert(H && *H);
    fibonacci_heap_node* z = *H;
    HeapData d = elem_data(z);
    fibonacci_heap_node* first = z->kid;
    heap_remove_from(H, z);
    freeNode(z);
    if (first){
        fibonacci_heap_node* current = first->right;
        while (current != first){
            current->parent = NULL;
            current = current->right;
        }
        first->parent = NULL;
        *H = heap_union(*H, first);
    }
    heap_consolidate(H);
    return d;
}

void  heap_remove_from(FibonacciHeap** H, fibonacci_heap_node* x){
    assert(!x->parent);
    if (x->right == x){
        *H = NULL;
    }else{
        x->left->right = x->right;
        x->right->left = x->left;
        *H = x->right;
    }
    x->left = x;
    x->right = x;
    x->parent = NULL;
}
void  heap_consolidate(FibonacciHeap** H){
    fibonacci_heap_node* x = *H;
    if (!x) return;
    fibonacci_heap_node** A = calloc(100, sizeof(fibonacci_heap_node));
    memset(A, '\0', 100);
    assert(x->degree >= 0);
    fibonacci_heap_node* last = x->left;
    while(x != last){
        fibonacci_heap_node* next = x->right;
        heap_match_degrees(H, A, x);
        x = next;
    }
    heap_match_degrees(H, A, last);
    *H = heap_init();
    for (int i=0; i<100; i++){
        if (A[i]){
            heap_add(H, A[i]);
        }
    }
    free(A);
}

void heap_match_degrees(FibonacciHeap** H, fibonacci_heap_node** A, fibonacci_heap_node* x){
    int d = x->degree;
    while(A[d]){
        if (d > 99){
            exit(1);
        }
        fibonacci_heap_node* y = A[d];
        if (y != x){
            x = heap_link(H, x, y);
            A[d] = NULL;
            d++;
        }else{
            break;
        }
    }
    A[d] = x;
}
fibonacci_heap_node* heap_link(FibonacciHeap** H, fibonacci_heap_node* x, fibonacci_heap_node* y){
    assert(x);
    assert(y);
    assert(x->degree == y->degree);
    if (x->key > y->key){
        return heap_link(H, y, x);
    }
    heap_remove_from(H, y);
    if (x->kid){
        fibonacci_heap_node* z = x->kid;
        y->right = z;
        y->left = z->left;
        z->left->right = y;
        z->left = y;
    }
    y->parent = x;
    x->kid = y;
    x->degree++;
    y->hasLostKid = 0;
    return x;
}

FibonacciHeap* heap_union(FibonacciHeap* H1, FibonacciHeap* H2){
    if(!H1) return H2;
    if(!H2) return H1;
    if (heap_min(H2).key < heap_min(H1).key){
        return heap_union(H2, H1);
    }

    fibonacci_heap_node* H1first = H1;
    fibonacci_heap_node* H1last = H1first->left;
    fibonacci_heap_node* H2first = H2;
    fibonacci_heap_node* H2last = H2first->left;

    H1last->right = H2first;
    H2first->left = H1last;
    H2last->right = H1first;
    H1first->left = H2last;

    return H1first;
}

void  heap_decrease_key(FibonacciHeap** H, FibonacciElement* x, int newKey){
    assert(H && *H);
    assert(x && x->key >= newKey);
    x->key = newKey;
    if(x->parent && x->parent->key > newKey){
        if (x->left == x){
            assert(x->parent->degree == 2);
            x->parent->kid = NULL;
        }else{
            assert(x->parent->degree > 2);
            x->left->right = x->right;
            x->right->left = x->left;
            x->parent->kid = x->left;
        }
        x->parent->degree--;
        heap_add(H, x);
        if (! x->parent->hasLostKid){
            x->parent->hasLostKid = 1;
        }else{
            heap_decrease_key(H, x->parent, x->parent->key);
        }

    }else{
        if (newKey < (*H)->key){
            assert(!x->parent);
            *H = x;
        }
    }
}

void  heap_delete(FibonacciHeap** H, FibonacciElement* x){
    heap_decrease_key(H, x, INT_MIN);
    heap_extract_min(H);
}

HeapData  elem_data(FibonacciElement* x){
    assert(x);
    HeapData d;
    d.key = x->key;
    d.value = x->value;
    return d;
}

void heap_free(FibonacciHeap** H){
    fibonacci_heap_node* header = *H;
    fibonacci_heap_node* first = header;
    if (header){
        while(header != first){
            fibonacci_heap_node* next = header->right;
            node_kill(header);
            header = next;
        }
    }
    *H = NULL;
}

