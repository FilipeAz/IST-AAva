#ifndef UNFND_H
#define UNFND_H

typedef struct _disjoint_set *DisjointSet;

DisjointSet makeSet(int);
void unionSet(DisjointSet, int, int);
int sameSet(DisjointSet, int, int);
int findSet(DisjointSet, int);

#endif /* UNFND_H */
