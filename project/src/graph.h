#ifndef GRAPH_H_
#define GRAPH_H_

#include "utils.h"
#include "string.h"
#include "linkedlist.h"

typedef struct edge Edge;
typedef struct graph Graph;
typedef struct graph_node GraphNode;

struct edge {
	int weight;
	int source;
	int destination;
};

struct graph_node {
	char *string;
	LinkedList *edges;

	//ONLY USED IN PRIM
	Edge *cheapestConnection;
	int cheapestCost;
};

struct graph {
	int size;

	GraphNode *nodes;
	LinkedList *edges;

	void (*createNode)(Graph *graph, char *string, int index);
	void (*createEdge)(Graph *graph, int first_index, int second_index, int weight);


};

Graph* newGraph(int size);

void destroyGraph(Graph *graph);

#endif





