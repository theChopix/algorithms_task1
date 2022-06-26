#ifndef ALLOCATION_H_
#define ALLOCATION_H_

#include <stdio.h>
#include <stdlib.h>
#include "error.h"

typedef struct {
    int *clean_up;
    int *dirty_up;
    int *clean_down;
    int *dirty_down;
    int *clean_right;
    int *dirty_right;
} h_matrices_t;

char *allocateCharMatrix(int r, int c)
{
    char *m = (char *)malloc(sizeof(char) * r * c);
    if (!m)
        exitError(1);
    return m;
}

int *allocateIntMatrix(int r, int c)
{
    int *m = (int *)malloc(sizeof(int) * r * c);
    if (!m)
        exitError(1);
    return m;
}

h_matrices_t *allocateHelpStruct() 
{
    h_matrices_t *h = malloc(sizeof(h_matrices_t));
    if (!h) 
        exitError(1);
    return h;
}

int *allocateIntArray(int l) 
{
    int *a = (int *)malloc(sizeof(int) * l);
    if (!a) 
        exitError(1);
    return a;
}

void deallocateCharMatrix(char **m) {
    if (*m) 
        free(*m);
}

void deallocateIntMatrix(int **m) {
    if (*m) 
        free(*m);
}

void deallocateIntArray(int **m) {
    if (*m)
        free(*m);
}

void deallocateHelpStruct(h_matrices_t **h) {
    deallocateIntMatrix(&(*h)->clean_up);
    deallocateIntMatrix(&(*h)->dirty_up);
    deallocateIntMatrix(&(*h)->clean_down);
    deallocateIntMatrix(&(*h)->dirty_down);
    deallocateIntMatrix(&(*h)->clean_right);
    deallocateIntMatrix(&(*h)->dirty_right);

    if (*h)  
        free(*h);
}

#endif