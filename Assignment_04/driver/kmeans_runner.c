#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>  /* clock() is plain ANSI C - no feature-test macro needed */

#include "kmeans.h"
#include "kmeans_runner.h"

static void failWithFormatError(const char *message) {
    fprintf(stderr, "Error: Invalid input file format - %s\n", message);
}

void runKMeansFromFile(const char *path) {
    FILE *fp = fopen(path, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: could not open input file '%s'\n", path);
        return;
    }

    /* ---- Header: N D K ---- */
    int n, d, k;
    if (fscanf(fp, "%d %d %d", &n, &d, &k) != 3) {
        failWithFormatError("expected 'N D K' on the first line");
        fclose(fp);
        return;
    }
    if (n <= 0) {
        failWithFormatError("N (number of points) must be positive");
        fclose(fp);
        return;
    }
    if (d <= 0) {
        failWithFormatError("D (number of dimensions) must be positive");
        fclose(fp);
        return;
    }
    if (k <= 0) {
        failWithFormatError("K (number of clusters) must be positive");
        fclose(fp);
        return;
    }
    if (k > n) {
        failWithFormatError("K cannot be greater than N");
        fclose(fp);
        return;
    }

    /* ---- N*D coordinate values ---- */
    double *coords = malloc((size_t)n * d * sizeof(double));
    for (int i = 0; i < n * d; i++) {
        if (fscanf(fp, "%lf", &coords[i]) != 1) {
            failWithFormatError("expected N*D coordinate values, but the file ended early or contained non-numeric data");
            free(coords);
            fclose(fp);
            return;
        }
    }

    /* ---- MAX_ITERATIONS n ---- */
    char keyword[32];
    int maxIterations;
    if (fscanf(fp, "%31s %d", keyword, &maxIterations) != 2) {
        failWithFormatError("expected a 'MAX_ITERATIONS n' line");
        free(coords);
        fclose(fp);
        return;
    }
    if (strcasecmp(keyword, "MAX_ITERATIONS") != 0) {
        failWithFormatError("expected the keyword 'MAX_ITERATIONS'");
        free(coords);
        fclose(fp);
        return;
    }
    if (maxIterations <= 0) {
        failWithFormatError("MAX_ITERATIONS must be positive");
        free(coords);
        fclose(fp);
        return;
    }

    /* ---- TOLERANCE epsilon ---- */
    double tolerance;
    if (fscanf(fp, "%31s %lf", keyword, &tolerance) != 2) {
        failWithFormatError("expected a 'TOLERANCE epsilon' line");
        free(coords);
        fclose(fp);
        return;
    }
    if (strcasecmp(keyword, "TOLERANCE") != 0) {
        failWithFormatError("expected the keyword 'TOLERANCE'");
        free(coords);
        fclose(fp);
        return;
    }
    if (tolerance <= 0) {
        failWithFormatError("TOLERANCE must be positive");
        free(coords);
        fclose(fp);
        return;
    }

    fclose(fp);

    /* ---- Timed region: algorithm only (assignment + update, all iterations). ---- */
    PointSet points;
    points.n = n;
    points.d = d;
    points.coords = coords;

    clock_t start = clock();

    KMeansResult result = runKMeans(&points, k, maxIterations, tolerance);

    clock_t end = clock();
    double timeMs = ((double)(end - start) * 1000.0) / CLOCKS_PER_SEC;

    /* ---- Print result (Section 7.3 format). Not timed. ---- */
    printf("Algorithm: K-Means Clustering\n");
    printf("K: %d\n", k);

    printf("Point assignments:\n");
    for (int i = 0; i < n; i++)
        printf("%d %d\n", i, result.assignments[i]);

    printf("Final centroids:\n");
    for (int c = 0; c < k; c++) {
        printf("%d:", c);
        for (int j = 0; j < d; j++)
            printf(" %f", result.centroids[(size_t)c * d + j]);
        printf("\n");
    }

    printf("WCSS: %f\n", result.wcss);
    printf("Iterations: %d\n", result.iterations);
    printf("Converged: %s\n", result.converged ? "true" : "false");
    printf("Execution time: %.4f ms\n", timeMs);

    freeKMeansResult(&result);
    free(coords);
}