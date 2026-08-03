#ifndef DFS_H
#define DFS_H

#include "csr_conversion.h"

typedef struct {
    int *traversalOrder;
    int traversalCount;
} DFSResult;

DFSResult runDFS(CSRGraph *graph, int source);

void freeDFSResult(DFSResult *result);

#endif