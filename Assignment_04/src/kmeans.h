#ifndef KMEANS_H
#define KMEANS_H

/* A set of N points in D-dimensional space, flattened row-major:
 * point i's coordinate j is coords[i*d + j]. */
typedef struct {
    int n;
    int d;
    double *coords;
} PointSet;

/* Result of running K-Means on a PointSet. */
typedef struct {
    int n;
    int d;
    int k;

    int *assignments;      /* size n: cluster id (0..k-1) assigned to each point */
    double *centroids;     /* size k*d, flattened row-major like coords */

    double wcss;           /* within-cluster sum of squared distances, final */
    int iterations;        /* number of assignment/update passes actually run */
    int converged;         /* 1 if stopped by no-change/tolerance, 0 if hit maxIterations */
} KMeansResult;

/* Runs K-Means clustering.
 * points        : input data (not modified)
 * k             : number of clusters (1 <= k <= points->n)
 * maxIterations : hard cap on assignment/update passes
 * tolerance     : stop when the largest centroid shift (Euclidean distance)
 *                 between consecutive iterations is <= tolerance
 * Centroids are initialized to the first k input points, in input order.
 * Caller must call freeKMeansResult() when done. */
KMeansResult runKMeans(PointSet *points, int k, int maxIterations, double tolerance);

void freeKMeansResult(KMeansResult *res);

#endif