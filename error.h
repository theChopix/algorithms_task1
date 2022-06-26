#ifndef ERROR_H_
#define ERROR_H_

#include <stdio.h>

#define ERROR 1

char errors[4][100] = {
    "Wrong arguments [number of rows & number of cols] provided!",
    "Error during dynamic allocation!",
    "Wrong arguments [matrix of char] provided!",
    "Wrong arguments [chars in matrix] provided!"};

void exitError(int error_num)
{
    fprintf(stderr, "Error: %s\n", errors[error_num]);
    exit(ERROR);
}

#endif 