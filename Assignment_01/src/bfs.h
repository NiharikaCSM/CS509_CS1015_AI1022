#ifndef BFS_H
#define BFS_H
#include "csr_conversion.h"

typedef struct {
    int *traversalOrder;   // vertices in the order they were visited
    int traversalCount;    // number of vertices actually visited
    int *distances;        // minimum edge-count distance from source; -1 if unreachable
} BFSResult;

// Runs BFS on a CSR graph starting from the given source vertex.
BFSResult runBFS(CSRGraph *graph, int source);

// Frees memory held by a BFSResult
void freeBFSResult(BFSResult *result);

#endif