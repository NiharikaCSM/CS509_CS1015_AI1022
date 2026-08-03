#ifndef WRAPPER_H
#define WRAPPER_H

#include <stdio.h>

FILE *openFile(char *filename);
double getExecutionTime(void);
void printExecutionTime(double startTime, double endTime);

#endif