#ifndef GRAPH_HELPER_H
#define GRAPH_HELPER_H

#include "csr_conversion.h" 

//sorts the neighbor lists of each vertex in ascending order using qsort
void sortNeighborLists(CSRGraph *graph);

//binary search for whether directed edge exists in the CSR structure
int hasEdge(CSRGraph *graph, int u, int v);

//checks that the input file actually describes a proper undirected graph i.e. every edge is listed on both ends
void validateUndirectedSymmetry(CSRGraph *graph);

#endif