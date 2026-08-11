#ifndef BETWEENNESS_CENTRALITY_H
#define BETWEENNESS_CENTRALITY_H

#include "graph_helper.h"

typedef struct {
    int vertices;
    double *centrality; //raw (unnormalized) betweenness centrality per vertex
} BetweennessCentralityResult;

BetweennessCentralityResult runBetweennessCentrality(const CSRGraph *graph);

void freeBetweennessCentralityResult(BetweennessCentralityResult *result);
void printBetweennessCentralityResult(const BetweennessCentralityResult *result);

#endif