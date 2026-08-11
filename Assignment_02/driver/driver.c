#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "triangle_count_runner.h"
#include "betweenness_runner.h"
#include "connected_comp_runner.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Please provide input in correct format\n");
        return 1;
    }

    const char *path = argv[1];
    const char *algorithm = argv[2];

    if (strcmp(algorithm, "tc") == 0) {
        runTriangleCountingFromFile(path);
    } 
    else if (strcmp(algorithm, "bc") == 0) {
        runBetweennessCentralityFromFile(path);
    } 
    else if (strcmp(algorithm, "cc") == 0) {
        runConnectedComponentsFromFile(path);
    } 
    else {
        fprintf(stderr, "Error: Unknown method '%s'. Use 'tc', 'bc', or 'cc'.\n", algorithm);
        return EXIT_FAILURE;
    }

    return 0;
}