#include "fastmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define FASTMAP_EPS 1e-9

/* Convenience accessor -- distance from an object to itself is always 0. */
static double dist(double **d, int i, int j) {
    if (i == j) return 0.0;
    return d[i][j];
}

/* Finds the object farthest from 'from' under the current distance matrix. */
static int farthestFrom(double **d, int n, int from) {
    int best = from;
    double bestDist = -1.0;
    for (int i = 0; i < n; i++) {
        if (i == from) continue;
        double di = dist(d, from, i);
        if (di > bestDist) {
            bestDist = di;
            best = i;
        }
    }
    return best;
}


static void choosePivots(double **d, int n, int *outA, int *outB) {
    int b = rand() % n;
    int a = b;
    for (int iter = 0; iter < 2; iter++) {
        a = farthestFrom(d, n, b);
        b = farthestFrom(d, n, a);
    }
    *outA = a;
    *outB = b;
}

FastMapResult runFastMap(double **distances, int n, int k) {
    FastMapResult result;
    result.n = n;
    result.k = k;

    result.coords = malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++)
        result.coords[i] = malloc(k * sizeof(double));

    result.pivotA = malloc(k * sizeof(int));
    result.pivotB = malloc(k * sizeof(int));


    double **work = malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++) {
        work[i] = malloc(n * sizeof(double));
        for (int j = 0; j < n; j++)
            work[i][j] = distances[i][j];
    }

    for (int dim = 0; dim < k; dim++) {
        int a, b;
        choosePivots(work, n, &a, &b);
        result.pivotA[dim] = a;
        result.pivotB[dim] = b;

        double dab = dist(work, a, b);

        if (dab < FASTMAP_EPS) {
            /* Remaining objects are indistinguishable on this axis. */
            for (int i = 0; i < n; i++)
                result.coords[i][dim] = 0.0;
            continue;
        }

        /* Project every object onto the (a, b) line via the law of cosines. */
        for (int i = 0; i < n; i++) {
            double dai = dist(work, a, i);
            double dbi = dist(work, b, i);
            double xi = (dai * dai + dab * dab - dbi * dbi) / (2.0 * dab);
            result.coords[i][dim] = xi;
        }

        /* Deflate: remove this dimension's contribution from every pair. */
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                double xi = result.coords[i][dim];
                double xj = result.coords[j][dim];
                double sq = work[i][j] * work[i][j] - (xi - xj) * (xi - xj);
                if (sq < 0.0) sq = 0.0; /* guard against floating-point drift */
                double newDist = sqrt(sq);
                work[i][j] = newDist;
                work[j][i] = newDist;
            }
        }
    }

    for (int i = 0; i < n; i++)
        free(work[i]);
    free(work);

    return result;
}

void freeFastMapResult(FastMapResult *result) {
    if (result == NULL) return;
    for (int i = 0; i < result->n; i++)
        free(result->coords[i]);
    free(result->coords);
    free(result->pivotA);
    free(result->pivotB);
    result->coords = NULL;
    result->pivotA = NULL;
    result->pivotB = NULL;
}

double computeAverageDistanceError(double **distances, FastMapResult *result) {
    int n = result->n;
    int k = result->k;
    if (n < 2) return 0.0;

    double totalError = 0.0;
    long pairCount = 0;

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            double sq = 0.0;
            for (int dim = 0; dim < k; dim++) {
                double diff = result->coords[i][dim] - result->coords[j][dim];
                sq += diff * diff;
            }
            double embedded = sqrt(sq);
            totalError += fabs(embedded - distances[i][j]);
            pairCount++;
        }
    }

    return pairCount > 0 ? totalError / pairCount : 0.0;
}