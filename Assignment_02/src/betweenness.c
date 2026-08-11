#include "betweenness.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

BetweennessCentralityResult runBetweennessCentrality(const CSRGraph *graph) {
    BetweennessCentralityResult result;
    int numOfVertices = graph->vertices;
    result.vertices = numOfVertices;
    result.centrality = calloc(numOfVertices, sizeof(double)); //initialize centrality to 0 for all vertices


    int *dist = malloc(sizeof(int) * numOfVertices); //distance from source vertex s to each vertex v
    double *sigma = malloc(sizeof(double) * numOfVertices); //records distinct shortest paths from source vertex s to each vertex v
    double *delta = malloc(sizeof(double) * numOfVertices); //records how many paths v has been a part of so far
    int *queue = malloc(sizeof(int) * numOfVertices); //for BFS run
    int *stack = malloc(sizeof(int) * numOfVertices); //records order in which vertices were visited for backward run

    //records number of predecessor of end vertex of path
    int *predCount = malloc(sizeof(int) * numOfVertices);

    //records all the predecessor's of end vertex of path
    int **predList = malloc(sizeof(int *) * numOfVertices);

    for (int w = 0; w < numOfVertices; w++) {
        int degree = graph->rowPtr[w + 1] - graph->rowPtr[w];
        //allocate size according to degree (no need to grow dynamically)
        predList[w] = malloc(sizeof(int) * (degree > 0 ? degree : 1));
    }

    //source loop
    for (int s = 0; s < numOfVertices; s++) {
        //finding path to each vertex
        for (int v = 0; v < numOfVertices; v++) {
            dist[v] = -1; //initially vertex not reached
            sigma[v] = 0.0; 
            delta[v] = 0.0;
            predCount[v] = 0;
        }
        dist[s] = 0;
        sigma[s] = 1.0; //initially only one path from source to itself

        //running BFS using queue
        int qHead = 0, qTail = 0;
        queue[qTail++] = s;
        int stackTop = 0;

        while (qHead < qTail) {
            int v = queue[qHead++];
            stack[stackTop++] = v;

            int start = graph->rowPtr[v];
            int end = graph->rowPtr[v + 1];
            for (int e = start; e < end; e++) {
                int w = graph->colIndex[e];
                //if w is not yet visited
                if (dist[w] < 0) {
                    dist[w] = dist[v] + 1;
                    queue[qTail++] = w;
                }
                //if v is a predecessor of w
                if (dist[w] == dist[v] + 1) {
                    sigma[w] += sigma[v]; //w's total path count grows by however many ways there were to reach v
                    predList[w][predCount[w]++] = v; //record v as one of w's predecessors, for the backward phase to use.
                }
            }
        }

        //backward run using stack
        while (stackTop > 0) {
            int w = stack[--stackTop];
            for (int p = 0; p < predCount[w]; p++) {
                int v = predList[w][p];
                //(sigma[v] / sigma[w]) -> what fraction of w's shortest paths specifically went through v?
                delta[v] += (sigma[v] / sigma[w]) * (1.0 + delta[w]);
            }
            if (w != s) {
                result.centrality[w] += delta[w];
            }
        }
    }

    for (int w = 0; w < numOfVertices; w++) free(predList[w]);
    free(predList);
    free(predCount);
    free(stack);
    free(queue);
    free(delta);
    free(sigma);
    free(dist);

    for (int v = 0; v < numOfVertices; v++) {
        //dividing because path is counted twice for undirected graphs
        result.centrality[v] /= 2.0;
    }

    return result;
}

void freeBetweennessCentralityResult(BetweennessCentralityResult *result) {
    free(result->centrality);
    result->centrality = NULL;
}

void printBetweennessCentralityResult(const BetweennessCentralityResult *result) {
    printf("Algorithm: Betweenness Centrality\n");
    printf("Vertex Centrality\n");
    for (int v = 0; v < result->vertices; v++) {
        printf("%d %.2f\n", v, result->centrality[v]);
    }
}