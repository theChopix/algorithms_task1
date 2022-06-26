#ifndef READ_H_
#define READ_H_

#include <stdio.h>
#include "error.h"

#define DIMENSIONS 2

void readDimensions(int *r, int *c)
{
    if (scanf("%d %d\r", r, c) != DIMENSIONS) {
        exitError(0);
    }
}

void readMatrix(int r, int c, char *g)
{
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            if (scanf("%c", &(g[i * c + j])) != 1) {
                g = NULL;
                return;
            }
        }
        scanf("\r\n");
    }
    if (!g)
        exitError(2);
}

#endif