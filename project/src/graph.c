#include "graph.h"

void createNode(Graph *graph, char *string, int index) {
	graph->nodes[index].string = simpleAlloc(sizeof(char) * strlen(string) + sizeof(char), false);
	strcpy(graph->nodes[index].string, string);
	graph->nodes[index].edges = newLinkedList();
	graph->nodes[index].cheapestCost = UINT_MAX;
	graph->nodes[index].cheapestConnection = NULL;
}

void createEdge(Graph *graph, int source, int destination, int weight) {
	Edge *edge1 = simpleAlloc(sizeof(Edge), false);
	Edge *edge2 = simpleAlloc(sizeof(Edge), false);
	if (edge1 == NULL) {
		NF_ERROR(errno, "Could not create a new edge");
		return;
	}
	if (edge2 == NULL) {
		NF_ERROR(errno, "Could not create a new edge");
		return;
	}

	edge1->weight = weight;
	edge2->weight = weight;
	edge1->source = source;
	edge2->source = destination;
	edge1->destination = destination;
	edge2->destination = source;

	graph->nodes[source].edges->addFirst(graph->nodes[source].edges, edge1);
	graph->nodes[destination].edges->addFirst(graph->nodes[destination].edges, edge2);
	graph->edges->addLast(graph->edges, edge1);
}

Graph* newGraph(int size) {
	Graph *graph = simpleAlloc(sizeof(Graph), false);

	if (graph == NULL) {
		NF_ERROR(errno, "Could not create a new graph");
		return NULL;
	}

	graph->nodes = simpleAlloc(sizeof(GraphNode) * size, false);

	if (graph->nodes == NULL) {
		NF_ERROR(errno, "Could not create nodes");
		return NULL;
	}

	graph->size = size;
	graph->edges = newLinkedList();

	graph->createNode = *createNode;
	graph->createEdge = *createEdge;



	return graph;
}

void destroyGraph(Graph *graph) {

	simpleFree(graph->nodes);
	simpleFree(graph);
}
