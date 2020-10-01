#include "linkedlist.h"
#include "binheap.h"
#include "graph.h"
#include "unfnd.h"
#include "binomialheap.h"

/*char* valueToStr(const void *value) {
	return (int*) value;
}*/

const int cmp(const void *child, const void *parent) {
	BinaryHeapNode *ch = (BinaryHeapNode *) child;
	BinaryHeapNode *p = (BinaryHeapNode *) parent;



	if (ch->data > p->data) {
		return 1;
	} else if (ch->data < p->data) {
		return -1;
	} else {
		return 0;
	}
}

const int cmpBinomial(const void *first, const void *second) {
	BinomialHeapNode *fst = (BinomialHeapNode *) first;
	BinomialHeapNode *sec = (BinomialHeapNode *) second;



	if (((GraphNode *)fst->data)->cheapestCost > ((GraphNode *)sec->data)->cheapestCost) {
		return 1;
	} else if (((GraphNode *)fst->data)->cheapestCost < ((GraphNode *)sec->data)->cheapestCost) {
		return -1;
	} else {
		return 0;
	}
}



int cmpSource(const void *value1, const void *value2) {
	Edge *edge1 = *(Edge **) value1;
	Edge *edge2 = *(Edge **) value2;

	int minEdge1 = edge1->source < edge1->destination ? edge1->source : edge1->destination;
	int minEdge2 = edge2->source < edge2->destination ? edge2->source : edge2->destination;


	if (minEdge1 > minEdge2) {
		return 1;
	} else if (minEdge1 < minEdge2) {
		return -1;
	}

	int maxEdge1 = edge1->source > edge1->destination ? edge1->source : edge1->destination;
	int maxEdge2 = edge2->source > edge2->destination ? edge2->source : edge2->destination;

	if (maxEdge1 > maxEdge2) {
		return 1;
	} else if (maxEdge1 < maxEdge2) {
		return -1;
	}
	return 0;

}

int cmpEdges(const void *value1, const void *value2) {
	Edge *edge1 = *(Edge **) value1;
	Edge *edge2 = *(Edge **) value2;

	if (edge1->weight > edge2->weight) {
		return 1;
	} else if (edge1->weight < edge2->weight) {
		return -1;
	}

	int minEdge1 = edge1->source < edge1->destination ? edge1->source : edge1->destination;
	int minEdge2 = edge2->source < edge2->destination ? edge2->source : edge2->destination;



	if (minEdge1 > minEdge2) {
		return 1;
	} else if (minEdge1 < minEdge2) {
		return -1;
	}

	int maxEdge1 = edge1->source > edge1->destination ? edge1->source : edge1->destination;
	int maxEdge2 = edge2->source > edge2->destination ? edge2->source : edge2->destination;

	if (maxEdge1 > maxEdge2) {
		return 1;
	} else if (maxEdge1 < maxEdge2) {
		return -1;
	}
	return 0;

}

LinkedList* prim(Graph *graph) {
	LinkedList *F = newLinkedList();
	BinomialHeap *Q = newBinomialHeap(cmpBinomial);
	BinomialHeapNode *min;
	for(int i = 0; i < graph->size; i++) {
		Q->insert(Q, &graph->nodes[i]);
	}

	while(Q->roots->head != NULL) {
		min = Q->deleteMinimum(Q);
		F->addLast(F, min->data);

		if(((GraphNode*)min->data)->cheapestConnection != NULL) {
			F->addLast(F, ((GraphNode*)min->data)->cheapestConnection);
		}
		for(LinkedListNode *aux = ((GraphNode*)min->data)->edges->head; aux != NULL; aux = aux->next) {

			BinomialHeapNode *w = Q->elementExists(Q, graph->nodes[((Edge*)aux->value)->destination].string);

			if (w != NULL && ((Edge *)aux->value)->weight < ((GraphNode*)w->data)->cheapestCost) {
				((GraphNode*)w->data)->cheapestCost = ((Edge *)aux->value)->weight;
				((GraphNode*)w->data)->cheapestConnection = ((Edge *)aux->value);
			}
		}
	}
	return F;
}

LinkedList* kruskal(Graph *graph) {
	LinkedList *solution = newLinkedList();
	Edge* allEdges[graph->edges->size];

	DisjointSet set = makeSet(graph->size);


	int i = 0;
	for (LinkedListNode *node = graph->edges->head; node != NULL; node = node->next, i++) {
		allEdges[i] = (Edge*) node->value;
	}


	qsort(allEdges, graph->edges->size, sizeof(Edge*), cmpEdges);


	for (int i = 0; i < graph->edges->size; i++) {
		if (!sameSet(set, allEdges[i]->source, allEdges[i]->destination)) {
			solution->addLast(solution, allEdges[i]);
			unionSet(set, allEdges[i]->source, allEdges[i]->destination);
		}
	}
	return solution;
}

int main(int arc, char **argv) {

	Graph *graph;

	int nstrings;
	int k;


	scanf("%d\n", &nstrings);

	graph = newGraph(nstrings);

	for (int i = 0; i < nstrings; i++) {
		char string[2048];
		scanf("%s\n", string);
		graph->createNode(graph, string, i);
	}

	scanf("%d\n", &k);



	for (int i = 0; i < nstrings; i++) {
		for (int j = i + 1; j < nstrings; j++) {
			int dist = 0;
			for (int b = 0; graph->nodes[i].string[b] != '\0'; b++) {
				if (graph->nodes[i].string[b] != graph->nodes[j].string[b]) {
					dist++;
				}
			}

			if (dist <= k) {
				graph->createEdge(graph, i, j, dist);
			}
		}
	}





	//KRUSKAL----------------------------------------------------------------------------------------------------------------

	LinkedList *spanningTree = kruskal(graph);

	//PRIM-------------------------------------------------------------------------------------------------------------------

	//LinkedList *spanningTree = prim(graph);

	Edge* finalEdges[spanningTree->size];

	int i = 0;
	for (LinkedListNode *node = spanningTree->head; node != NULL; node = node->next, i++) {
		finalEdges[i] = (Edge*) node->value;
	}

	qsort(finalEdges, spanningTree->size, sizeof(Edge*), cmpSource);

	for (i = 0; i < spanningTree->size; i++) {
		printf("%d %d\n", finalEdges[i]->source + 1, finalEdges[i]->destination + 1);
	}

	destroyList(spanningTree);

	destroyGraph(graph);

	return 0;
}

