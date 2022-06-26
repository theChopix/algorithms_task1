#ifndef PRINT_H_
#define PRINT_H_

#include <stdio.h>
#include "allocation.h"

void printIntMatrix(int row, int col, int *matrix)
{
    printf("Printing Int Matrix\n");
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            printf(" %d ", matrix[i * col + j]);
        }
        printf("\n");
    }
    printf("\n");
}

void printCharMatrix(int row, int col, char *matrix)
{
    printf("Printing Char Matrix\n");
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            printf(" %c ", matrix[i * col + j]);
        }
        printf("\n");
    }
    printf("\n");
}

void printAllMatrices(int row, int col, char *given, h_matrices_t *h) {
    printf("given\n");
    printCharMatrix(row, col, given);

    printf("clean_up\n");
    printIntMatrix(row, col, h->clean_up);

    printf("dirty_up\n");
    printIntMatrix(row, col, h->dirty_up);

    printf("clean_down\n");
    printIntMatrix(row, col, h->clean_down);
    
    printf("dirty_down\n");
    printIntMatrix(row, col, h->dirty_down);
    
    printf("clean_right\n");
    printIntMatrix(row, col, h->clean_right);
    
    printf("dirty_right\n");
    printIntMatrix(row, col, h->dirty_right);
}

#endif