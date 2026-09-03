#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kmeans_runner.h"
#include "fastmap_runner.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Please provide input in correct format\n");
        return 1;
    }

    const char *path = argv[1];
    const char *algorithm = argv[2];

    if (strcmp(algorithm, "kmeans") == 0) {
        runKMeansFromFile(path);

    } 
    // else if (strcmp(algorithm, "fastmap") == 0) {
    //     runFastMapFromFile(path);

    // } 
    else {
        fprintf(stderr, "Error: unknown algorithm '%s'\n", algorithm);
        return EXIT_FAILURE;
    }

    return 0;
}