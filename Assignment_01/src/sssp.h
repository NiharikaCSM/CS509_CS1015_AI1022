#ifndef SSSP_H
#define SSSP_H

#include "csr_conversion.h"

#define SSSP_INF -1  /* marks unreachable vertices in distOut[] */

void sssp_dijkstra(const CSRGraph *graph, int source, int *distOut);

#endif