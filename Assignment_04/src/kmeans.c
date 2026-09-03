#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "kmeans.h"

static double squaredDistance(const double *a, const double *b, int d) {
    double sum = 0.0;
    for (int i = 0; i < d; i++) {
        double diff = a[i] - b[i];
        sum += diff * diff;
    }
    return sum;
}

/* Returns the index of the centroid nearest to `point`. */
static int nearestCentroid(const double *point, const double *centroids, int k, int d) {
    int best = 0;
    double bestDist = squaredDistance(point, centroids, d);
    for (int c = 1; c < k; c++) {
        double dist = squaredDistance(point, centroids + (size_t)c * d, d);
        if (dist < bestDist) {
            bestDist = dist;
            best = c;
        }
    }
    return best;
}

KMeansResult runKMeans(PointSet *points, int k, int maxIterations, double tolerance) {
    int n = points->n;
    int d = points->d;

    KMeansResult res;
    res.n = n;
    res.d = d;
    res.k = k;
    res.iterations = 0;
    res.converged = 0;
    res.assignments = malloc((size_t)n * sizeof(int));
    res.centroids = malloc((size_t)k * d * sizeof(double));

    /* Initial centroids: first k input points, in input order (reproducible). */
    for (int c = 0; c < k; c++)
        memcpy(res.centroids + (size_t)c * d, points->coords + (size_t)c * d, d * sizeof(double));

    /* -1 so the very first assignment pass always counts as "changed". */
    for (int i = 0; i < n; i++)
        res.assignments[i] = -1;

    double *newCentroids = malloc((size_t)k * d * sizeof(double));
    int *clusterCounts = malloc((size_t)k * sizeof(int));

    for (int iter = 0; iter < maxIterations; iter++) {
        int anyChanged = 0;

        /* Assignment step: nearest centroid for every point, using this
         * iteration's centroids (previous iteration's update, or the
         * initial centroids on the first pass). */
        for (int i = 0; i < n; i++) {
            int nearest = nearestCentroid(points->coords + (size_t)i * d, res.centroids, k, d);
            if (nearest != res.assignments[i]) {
                anyChanged = 1;
                res.assignments[i] = nearest;
            }
        }

        /* Update step: recompute each centroid as the mean of its points. */
        memset(newCentroids, 0, (size_t)k * d * sizeof(double));
        memset(clusterCounts, 0, (size_t)k * sizeof(int));

        for (int i = 0; i < n; i++) {
            int c = res.assignments[i];
            clusterCounts[c]++;
            for (int j = 0; j < d; j++)
                newCentroids[(size_t)c * d + j] += points->coords[(size_t)i * d + j];
        }

        double maxShift = 0.0;
        for (int c = 0; c < k; c++) {
            if (clusterCounts[c] == 0) {
                /* Empty cluster: keep its previous centroid unchanged. */
                memcpy(newCentroids + (size_t)c * d, res.centroids + (size_t)c * d, d * sizeof(double));
                continue;
            }
            for (int j = 0; j < d; j++)
                newCentroids[(size_t)c * d + j] /= clusterCounts[c];

            double shift = sqrt(squaredDistance(newCentroids + (size_t)c * d, res.centroids + (size_t)c * d, d));
            if (shift > maxShift)
                maxShift = shift;
        }

        memcpy(res.centroids, newCentroids, (size_t)k * d * sizeof(double));
        res.iterations++;

        if (!anyChanged || maxShift <= tolerance) {
            res.converged = 1;
            break;
        }
    }

    free(newCentroids);
    free(clusterCounts);

    /* Final WCSS against the final centroids/assignments. */
    res.wcss = 0.0;
    for (int i = 0; i < n; i++) {
        int c = res.assignments[i];
        res.wcss += squaredDistance(points->coords + (size_t)i * d, res.centroids + (size_t)c * d, d);
    }

    return res;
}

void freeKMeansResult(KMeansResult *res) {
    free(res->assignments);
    free(res->centroids);
}