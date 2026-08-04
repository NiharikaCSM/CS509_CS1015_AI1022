#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "wrapper.h"

FILE *openFile(char *filename) {
    FILE *inputFile = fopen(filename, "r");
    if (!inputFile) {
        printf("Error : Input file not found.\n");
        exit(1);
    }
    return inputFile;
}

double getExecutionTime(void) {
    return (double)clock() / CLOCKS_PER_SEC;
}

void printExecutionTime(double startTime, double endTime) {
    double ms = (endTime - startTime) * 1000;
    printf("Execution time: %.6f ms\n", ms);
}