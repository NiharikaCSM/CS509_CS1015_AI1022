#ifndef TRIANGLE_COUNT_H
#define TRIANGLE_COUNT_H

#include "graph_helper.h"

typedef struct {
    int v1, v2, v3; //the three vertices of one triangle, a < b < c
} Triangle;

typedef struct {
    long long totalTriangles; //total number of triangles in the graph 
    Triangle *triangles;   //triangle array, populated only if `printTriangles` is 1
    long long triangleArrayCount; //number of entries actually stored in `triangles`
    int printed;              //1 if `triangles` was populated, 0 otherwise 
} TriangleCountingResult;

TriangleCountingResult runTriangleCounting(CSRGraph *graph, int printTriangles);

void freeTriangleCountingResult(TriangleCountingResult *result);

void printTriangleCountingResult(const TriangleCountingResult *result);

#endif 
