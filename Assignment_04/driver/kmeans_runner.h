#ifndef KMEANS_RUNNER_H
#define KMEANS_RUNNER_H

/* Reads a K-Means input file (Section 7.1 format), validates it, runs
 * K-Means clustering, and prints the result in the Section 7.3 format.
 *
 * path: path to the input .txt file.
 * On any invalid/missing file, prints an error to stderr and returns
 * without touching stdout's result format. */
void runKMeansFromFile(const char *path);

#endif