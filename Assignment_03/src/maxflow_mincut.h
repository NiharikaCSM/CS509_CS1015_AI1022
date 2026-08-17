#ifndef MAXFLOW_MINCUT_H
#define MAXFLOW_MINCUT_H

#include "csr_conversion.h"

/* Result of running Dinic's algorithm on a CSR graph. */
typedef struct {
    long long maxFlow;          /* = min cut capacity */

    int sourceSideCount;
    int *sourceSide;            /* vertices reachable from source in final residual graph */

    int sinkSideCount;
    int *sinkSide;               /* all other vertices */

    int cutEdgeCount;
    int *cutFrom;                /* parallel arrays: cutFrom[i] -> cutTo[i], capacity cutCap[i] */
    int *cutTo;
    int *cutCap;
} MaxflowResult;

/* Runs Dinic's max-flow / min-cut on the given CSR graph.
 * graph  : input graph (CSR, directed, positive capacities in edgeWeights)
 * source : source vertex
 * sink   : sink vertex
 * Returns a MaxflowResult. Caller must call freeMaxflowResult() when done. */
MaxflowResult dinicMaxflow(CSRGraph *graph, int source, int sink);

/* Frees all heap memory inside a MaxflowResult. */
void freeMaxflowResult(MaxflowResult *res);

#endif