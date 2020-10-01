#include <limits.h>
#include <stdlib.h>

#include "unfnd.h"

struct _disjoint_set {
	int size;
	struct {
		int pi, rank;
	} my[1];
};

DisjointSet makeSet(int size) {
	DisjointSet set;
	int i;

	if (size == 0)
		return NULL;
	set = malloc(sizeof(int) + 2 * sizeof(int) * size);
	if (set == NULL)
		return NULL;
	set->size = size;
	for (i = 0; i < size; i++) {
		set->my[i].pi = i;
		set->my[i].rank = 0;
	}
	return set;
}

int findSet(DisjointSet set, int i) {
	if (set == NULL || i < 0 || i >= set->size)
		return UINT_MAX;
	for (; i != set->my[i].pi; i = set->my[i].pi)
		set->my[i].pi = set->my[set->my[i].pi].pi;
	return i;
}

int sameSet(DisjointSet set, int i, int j) {
	//int i_root, j_root;

	if (set == NULL || i < 0 || j < 0 || i >= set->size || j >= set->size)
		return 0;
	return findSet(set, i) == findSet(set, j);
}

void unionSet(DisjointSet set, int i, int j) {
	int i_root, j_root;

	if (set == NULL || i < 0 || j < 0 || i >= set->size || j >= set->size)
		return;
	i_root = findSet(set, i);
	j_root = findSet(set, j);
	if (i_root == j_root)
		return;
	if (set->my[i_root].rank > set->my[j_root].rank)
		set->my[j_root].pi = i_root;
	else if (set->my[i_root].rank < set->my[j_root].rank)
		set->my[i_root].pi = j_root;
	else { /* set->my[i_root].rank == set->my[j_root].rank */
		set->my[i_root].pi = j_root;
		set->my[j_root].rank++;
	}
}
