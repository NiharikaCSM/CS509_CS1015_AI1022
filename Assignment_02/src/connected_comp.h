#ifndef CONNECTED_COMPONENTS_H
#define CONNECTED_COMPONENTS_H

#include "csr_conversion.h"

typedef struct {
    int numVertices;
    int numComponents;
    int *component;   // component[i] = component id of vertex i
} ConnectedComponentsResult;

ConnectedComponentsResult runConnectedComponents(const CSRGraph *graph);
void printConnectedComponentsResult(const ConnectedComponentsResult *result);
void freeConnectedComponentsResult(ConnectedComponentsResult *result);

#endif 