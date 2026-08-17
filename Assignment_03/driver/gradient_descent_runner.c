#include "gradient_descent_runner.h"
#include "gradient_descent.h"
#include "wrapper.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define REPEAT_RUNS 100

static void failGD(const char *message) {
    fprintf(stderr, "Error: Invalid input file format - %s\n", message);
    exit(1);
}


static void expectKeyword(FILE *file, const char *expected) {
    char token[64];
    if (fscanf(file, "%63s", token) != 1) {
        fprintf(stderr,
            "Error: Invalid input file format - expected the keyword '%s' "
            "but the file ended early.\n", expected);
        exit(1);
    }
    if (strcmp(token, expected) != 0) {
        fprintf(stderr,
            "Error: Invalid input file format - expected the keyword '%s' "
            "but found '%s' instead (check the coefficient count and line "
            "order against the required format).\n", expected, token);
        exit(1);
    }
}

void runGradientDescentFromFile(const char *path) {
    FILE *file = fopen(path, "r");
    if (!file) {
        printf("Error: could not open input file\n");
        exit(EXIT_FAILURE);
    }

    //DEGREE
    expectKeyword(file, "DEGREE");
    int degree;
    if (fscanf(file, "%d", &degree) != 1) {
        failGD("expected an integer degree after 'DEGREE'.");
    }
    if (degree < 1) {
        failGD("degree must be a positive integer (a degree-0 polynomial has no minimum to search for).");
    }

    //COEFFICIENTS (exactly degree + 1 values)
    expectKeyword(file, "COEFFICIENTS");
    double *coefficients = malloc(sizeof(double) * (degree + 1));
    for (int i = 0; i <= degree; i++) {
        if (fscanf(file, "%lf", &coefficients[i]) != 1) {
            fprintf(stderr,
                "Error: Invalid input file format - expected %d coefficients "
                "(degree + 1) after 'COEFFICIENTS', but the file ran out "
                "after %d.\n", degree + 1, i);
            free(coefficients);
            exit(1);
        }
    }

    // INITIAL_X
    expectKeyword(file, "INITIAL_X");
    double initialX;
    if (fscanf(file, "%lf", &initialX) != 1) {
        free(coefficients);
        failGD("expected a numeric value after 'INITIAL_X'.");
    }

    //LEARNING_RATE 
    expectKeyword(file, "LEARNING_RATE");
    double learningRate;
    if (fscanf(file, "%lf", &learningRate) != 1) {
        free(coefficients);
        failGD("expected a numeric value after 'LEARNING_RATE'.");
    }
    if (learningRate <= 0.0) {
        free(coefficients);
        failGD("learning rate must be positive.");
    }

    //TOLERANCE 
    expectKeyword(file, "TOLERANCE");
    double tolerance;
    if (fscanf(file, "%lf", &tolerance) != 1) {
        free(coefficients);
        failGD("expected a numeric value after 'TOLERANCE'.");
    }
    if (tolerance <= 0.0) {
        free(coefficients);
        failGD("tolerance must be positive.");
    }

    //MAX_ITERATIONS 
    expectKeyword(file, "MAX_ITERATIONS");
    long long maxIterations;
    if (fscanf(file, "%lld", &maxIterations) != 1) {
        free(coefficients);
        failGD("expected an integer value after 'MAX_ITERATIONS'.");
    }
    if (maxIterations <= 0) {
        free(coefficients);
        failGD("MAX_ITERATIONS must be a positive integer.");
    }

    fclose(file);

    GDResult result = runGradientDescent(coefficients, degree, initialX, learningRate, tolerance, maxIterations);

    double startTime = getExecutionTime();
    for (int r = 0; r < REPEAT_RUNS; r++) {
        runGradientDescent(coefficients, degree, initialX, learningRate, tolerance, maxIterations);
    }
    double endTime = getExecutionTime();

    double totalElapsed = endTime - startTime;
    double perRunEnd = startTime + (totalElapsed / REPEAT_RUNS);

    printGDResult(&result);
    printExecutionTime(startTime, perRunEnd);

    free(coefficients);
}