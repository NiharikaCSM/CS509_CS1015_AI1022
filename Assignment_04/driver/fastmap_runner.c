#include "fastmap_runner.h"
#include "../src/fastmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <wrapper.h>

#define FASTMAP_EPS 1e-9

static void failWithFormatError(const char *message) {
    fprintf(stderr, "Error: Invalid input file format - %s\n", message);
    exit(1);
}

static double **allocMatrix(int n) {
    double **m = malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++)
        m[i] = malloc(n * sizeof(double));
    return m;
}

static void freeMatrix(double **m, int n) {
    for (int i = 0; i < n; i++)
        free(m[i]);
    free(m);
}


static double **readFastMapFile(FILE *fp, int *outN, int *outK) {
    int n, k;
    if (fscanf(fp, "%d %d", &n, &k) != 2)
        failWithFormatError("file is empty or missing 'N K' on the first line.");

    if (n <= 0)
        failWithFormatError("number of objects (N) must be a positive integer.");
    if (k <= 0 || k >= n)
        failWithFormatError("target dimensionality (k) must be positive and less than N.");

    double **matrix = allocMatrix(n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (fscanf(fp, "%lf", &matrix[i][j]) != 1)
                failWithFormatError("expected an N x N distance matrix, but the file ended early or contained non-numeric data.");
            if (matrix[i][j] < 0.0)
                failWithFormatError("distances cannot be negative.");
        }
    }

    for (int i = 0; i < n; i++) {
        if (fabs(matrix[i][i]) > FASTMAP_EPS)
            failWithFormatError("diagonal entries of the distance matrix must be zero.");
        for (int j = i + 1; j < n; j++) {
            if (fabs(matrix[i][j] - matrix[j][i]) > 1e-6)
                failWithFormatError("distance matrix must be symmetric: d(i,j) must equal d(j,i).");
        }
    }

    *outN = n;
    *outK = k;
    return matrix;
}

int runFastMapFromFile(const char *filePath) {
    FILE *fp = fopen(filePath, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: could not open input file '%s'\n", filePath);
        return 1;
    }

    int n, k;
    double **distances = readFastMapFile(fp, &n, &k);
    fclose(fp);

    printf("Objects: %d, Target dimensions: %d\n", n, k);

    /* Seed the pivot-selection heuristic's randomness once per run. */
    srand((unsigned int)time(NULL));

    double startTime = getExecutionTime();
    FastMapResult result = runFastMap(distances, n, k);
    double endTime = getExecutionTime();

    /* Analysis only -- not part of the timed algorithm section. */
    double avgError = computeAverageDistanceError(distances, &result);

    printf("Algorithm: FastMap\n");
    printf("Target dimensions: %d\n", k);
    printf("Pivots per dimension:\n");
    for (int dim = 0; dim < k; dim++)
        printf("Dim %d: %d %d\n", dim + 1, result.pivotA[dim], result.pivotB[dim]);

    printf("Object coordinates:\n");
    for (int i = 0; i < n; i++) {
        printf("%d:", i);
        for (int dim = 0; dim < k; dim++)
            printf(" %f", result.coords[i][dim]);
        printf("\n");
    }

    printf("Average distance error: %f\n", avgError);
    printExecutionTime(startTime, endTime);

    freeFastMapResult(&result);
    freeMatrix(distances, n);

    return 0;
}