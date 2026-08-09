#include "triangle_count.h"
#include <stdio.h>
#include <stdlib.h>

TriangleCountingResult runTriangleCounting(CSRGraph *graph, int printTriangles) {
    TriangleCountingResult result;
    result.totalTriangles = 0;
    result.triangles = NULL;
    result.triangleArrayCount = 0;
    result.printed = printTriangles;

    long long listCapacity = 0;
    if (printTriangles) {
        listCapacity = 64;
        result.triangles = malloc(sizeof(Triangle) * listCapacity);
    }

    long long rawCount = 0;

    for (int u = 0; u < graph->vertices; u++) {
        int start = graph->rowPtr[u];
        int end = graph->rowPtr[u + 1];
        int degree = end - start; //number of neighbors of vertex u

        for (int i = 0; i < degree; i++) {
            int v = graph->colIndex[start + i];
            //check if neighbors of u have an edge among themselves to form a triangle with u
            for (int j = i + 1; j < degree; j++) {
                int w = graph->colIndex[start + j];
                if (hasEdge(graph, v, w)) {
                    rawCount++;

                    // u<v check ensures each triangle is listed only once
                    if (printTriangles && u < v) {
                        //increase the capacity of array if it is already full
                        if (result.triangleArrayCount == listCapacity) {
                            listCapacity *= 2;
                            result.triangles = realloc(result.triangles, sizeof(Triangle) * listCapacity);
                        }
                        result.triangles[result.triangleArrayCount].v1 = u;
                        result.triangles[result.triangleArrayCount].v2 = v;
                        result.triangles[result.triangleArrayCount].v3 = w;
                        result.triangleArrayCount++;
                    }
                }
            }
        }
    }

    //triangles are counted three times (once at each vertex), so divide by 3 to get the actual count
    result.totalTriangles = rawCount / 3;
    return result;
}

void freeTriangleCountingResult(TriangleCountingResult *result) {
    free(result->triangles);
    result->triangles = NULL;
}

void printTriangleCountingResult(const TriangleCountingResult *result) {
    printf("Algorithm: Triangle Counting\n");
    printf("Total triangles: %lld\n", result->totalTriangles);
    if (result->printed) {
        printf("Triangles found:\n");
        for (long long i = 0; i < result->triangleArrayCount; i++) {
            printf("(%d, %d, %d)\n",
                   result->triangles[i].v1,
                   result->triangles[i].v2,
                   result->triangles[i].v3);
        }
    }
}
