#ifndef FASTMAP_H
#define FASTMAP_H

typedef struct {
    int n;            // number of objects                              
    int k;             // target embedding dimensionality                
    double **coords;   // coords[i][dim] = coordinate of object i on axis dim (n x k) 
    int *pivotA;        // pivotA[dim] = first pivot object chosen for dimension dim (size k) 
    int *pivotB;        // pivotB[dim] = second pivot object chosen for dimension dim (size k) 
} FastMapResult;

FastMapResult runFastMap(double **distances, int n, int k);


void freeFastMapResult(FastMapResult *result);

// Computes the average absolute error between the original pairwise distances and the Euclidean distances between the embedded coordinates.
double computeAverageDistanceError(double **distances, FastMapResult *result);

#endif