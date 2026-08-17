#ifndef GRADIENT_DESCENT_H
#define GRADIENT_DESCENT_H

typedef struct {
    int degree;
    double finalX;
    double finalFx;
    long long iterations;
    int converged; // 1 if |f'(x)| <= tolerance was reached, 0 if maxIterations ran out first
} GDResult;


double evaluatePolynomial(const double *coefficients, int degree, double x);

double evaluateDerivative(const double *coefficients, int degree, double x);

GDResult runGradientDescent(const double *coefficients, int degree,
                             double initialX, double learningRate,
                             double tolerance, long long maxIterations);

void printGDResult(const GDResult *result);

#endif 