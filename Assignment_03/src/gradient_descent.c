#include "gradient_descent.h"
#include <stdio.h>
#include <math.h>

//evaluates the input polynomial using horner's method
double evaluatePolynomial(const double *coefficients, int degree, double x) {
    double result = coefficients[degree];
    for (int i = degree - 1; i >= 0; i--) {
        result = result * x + coefficients[i];
    }
    return result;
}

//calculating derivative using horner's method
double evaluateDerivative(const double *coefficients, int degree, double x) {
    //derivative of constatn
    if (degree == 0) return 0.0;

    double result = degree * coefficients[degree];
    for (int i = degree - 1; i >= 1; i--) {
        result = result * x + i * coefficients[i];
    }
    return result;
}


GDResult runGradientDescent(const double *coefficients, int degree, double initialX, double learningRate,
                            double tolerance, long long maxIterations) {

    GDResult result;
    result.degree = degree;

    double x = initialX;
    long long iterations = 0;
    int converged = 0;
    
    //run algorithm till max iterations or till it converges
    for (; iterations < maxIterations; iterations++) {
        double fprime = evaluateDerivative(coefficients, degree, x);
        if (fabs(fprime) <= tolerance) {
            converged = 1;
            break;
        }
        x = x - learningRate * fprime;
    }

    result.finalX = x;
    result.finalFx = evaluatePolynomial(coefficients, degree, x);
    result.iterations = iterations;
    result.converged = converged;

    return result;
}

void printGDResult(const GDResult *result) {
    printf("Algorithm: Gradient Descent\n");
    printf("Degree: %d\n", result->degree);
    printf("Final x: %.6f\n", result->finalX);
    printf("Final f(x): %.6f\n", result->finalFx);
    printf("Iterations: %lld\n", result->iterations);
    printf("Converged: %s\n", result->converged ? "true" : "false");
}