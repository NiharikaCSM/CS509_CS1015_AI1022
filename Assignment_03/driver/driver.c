#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gradient_descent_runner.h"
#include "maxflow_mincut_runner.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Please provide input in correct format\n");
        return 1;
    }

    const char *path = argv[1];
    const char *algorithm = argv[2];

    if (strcmp(algorithm, "gd") == 0) {
        runGradientDescentFromFile(path);

    } else if (strcmp(algorithm, "maxflow") == 0) {
       runMaxflowMincutFromFile(path);

    } else {
        fprintf(stderr, "Error: unknown algorithm '%s'\n", algorithm);
        return EXIT_FAILURE;
    }

    return 0;
}