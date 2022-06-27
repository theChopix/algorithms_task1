#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "allocation.h"
#include "error.h"
#include "print.h"
#include "read.h"

#define MAX(x, y) ((x > y) ? x : y)
#define MIN(x, y) ((x < y) ? x : y)

#define GETSHAPE(lu, ld, ru, rd, b) (2 * MIN(lu, ru) + 2 * MIN(ld, rd) + b + 2)

#define DOT '.'
#define CIRCLE 'o'
#define CROSS 'X'
#define isDOT(x) (x == '.')

h_matrices_t *prepareHelpMatrices(int r, int c)
{
    h_matrices_t *h = allocateHelpStruct();

    h->clean_up = allocateIntMatrix(r, c);
    h->dirty_up = allocateIntMatrix(r, c);
    h->clean_down = allocateIntMatrix(r, c);
    h->dirty_down = allocateIntMatrix(r, c);
    h->clean_right = allocateIntMatrix(r, c);
    h->dirty_right = allocateIntMatrix(r, c);

    return h;
}

void computeUpMatrices(int r, int c, char *g, h_matrices_t *h)
{
    /* data structure for counting circles during iteration through dirty-up
     * matrix ; each column has to keep this information */
    int *circle_counter = allocateIntArray(c);

    /* helpful refactoring of matrices we are going to need in this function */
    int *clean = h->clean_up;
    int *dirty = h->dirty_up;

    for (int i = 0; i < c; i++) {
        /* setting each element of first row of matrices to 0 -> there are no
         * other elements over these*/
        clean[i] = 0;
        dirty[i] = 0;
        circle_counter[i] = 0;
    }

    for (int i = 1; i < r; i++) {
        for (int j = 0; j < c; j++) {

            /* indexes of previous and current element in matrices according to
             * iterating through up-typed matrices */
            int prev = (i - 1) * c + j;
            int curr = i * c + j;

            /* in question what to fill in 'current' elements of helpful
             * matrices we have to know 'previous' element in given matrix */
            switch (g[prev]) {
            case DOT:
                /* in case of '.' it is trivial -> we increment value on
                 * previous element in both matrices */
                clean[curr] = clean[prev] + 1;
                dirty[curr] = dirty[prev] + 1;
                break;

            case CIRCLE:
                /* in case of 'o' we are setting current element of clean matrix
                   to 0 and dirty's one too if circle_counter has not recorded
                   circle till then if so (circle_counter[column] == 1) we are
                   back-iterating till we find another not-dot-char*/
                clean[curr] = 0;
                if (circle_counter[j] == 0) {
                    dirty[curr] = dirty[prev] + 1;
                    circle_counter[j] += 1;
                } else {
                    int prev_row = i - 2;
                    dirty[curr] = 1;
                    while (prev_row >= 0 && isDOT(g[prev_row * c + j])) {
                        dirty[curr] += 1;
                        prev_row -= 1;
                    }
                }
                break;

            case CROSS:
                /* in case of 'X' also trivial -> both current elements setting
                 * to 0 and circle_counter of concerned column setting to 0 to
                 * start new circle counting */
                clean[curr] = 0;
                dirty[curr] = 0;
                circle_counter[j] = 0;
                break;

            default:
                exitError(3);
            }
        }
    }

    deallocateIntArray(&circle_counter);
}

void computeDownMatrices(int r, int c, char *g, h_matrices_t *h)
{
    /* same approach as in computeUpMatrices but with different iterating... */
    int *circle_counter = allocateIntArray(c);

    int *clean = h->clean_down;
    int *dirty = h->dirty_down;

    int last_row = (r - 1) * c;
    for (int i = 0; i < c; i++) {
        clean[last_row + i] = 0;
        dirty[last_row + i] = 0;
        circle_counter[i] = 0;
    }

    for (int i = r - 2; i >= 0; i--) {
        for (int j = 0; j < c; j++) {

            int prev = (i + 1) * c + j;
            int curr = i * c + j;

            switch (g[prev]) {
            case DOT:
                clean[curr] = clean[prev] + 1;
                dirty[curr] = dirty[prev] + 1;
                break;

            case CIRCLE:
                clean[curr] = 0;
                if (circle_counter[j] == 0) {
                    dirty[curr] = dirty[prev] + 1;
                    circle_counter[j] += 1;
                } else {
                    int prev_row = i + 2;
                    dirty[curr] = 1;
                    while (prev_row < r && isDOT(g[prev_row * c + j])) {
                        dirty[curr] += 1;
                        prev_row += 1;
                    }
                }
                break;

            case CROSS:
                clean[curr] = 0;
                dirty[curr] = 0;
                circle_counter[j] = 0;
                break;

            default:
                exitError(3);
            }
        }
    }

    deallocateIntArray(&circle_counter);
}

void computeRightMatrices(int r, int c, char *g, h_matrices_t *h)
{
    /* same approach as in computeUpMatrices but with different iterating... */
    int *circle_counter = allocateIntArray(r);

    int *clean = h->clean_right;
    int *dirty = h->dirty_right;

    for (int i = 0; i < r; i++)
        circle_counter[i] = 0;

    int end_of_row = c - 1;
    for (int i = 0; i < r; i++) {
        for (int j = end_of_row; j >= 0; j--) {

            int prev = i * c + (j + 1);
            int curr = i * c + j;

            if (j == end_of_row) {
                clean[curr] = 0;
                dirty[curr] = 0;
                continue;
            }

            switch (g[prev]) {
            case DOT:
                clean[curr] = clean[prev] + 1;
                dirty[curr] = dirty[prev] + 1;
                break;

            case CIRCLE:
                clean[curr] = 0;
                if (circle_counter[i] == 0) {
                    dirty[curr] = dirty[prev] + 1;
                    circle_counter[i] += 1;
                } else {
                    int prev_col = j + 2;
                    dirty[curr] = 1;
                    while (prev_col < c && isDOT(g[i * c + prev_col])) {
                        dirty[curr] += 1;
                        prev_col += 1;
                    }
                }
                break;

            case CROSS:
                clean[curr] = 0;
                dirty[curr] = 0;
                circle_counter[i] = 0;
                break;

            default:
                exitError(3);
            }
        }
    }

    deallocateIntArray(&circle_counter);
}

int getLargestCleanShape(int col, int pos_row, int pos_col, h_matrices_t *h)
{
    /* same approach as in getLargestDirtyShape but only with 'clean' values... */
    int ret = 0;
    int tmp_ret = 0;

    int position = pos_row * col + pos_col;

    int left_wing_up = h->clean_up[position];
    int left_wing_down = h->clean_down[position];

    int right_wing_up, right_wing_down;
    if (left_wing_up > 0 && left_wing_down > 0) {

        for (int central = 1; central + 1 < h->clean_right[position];
             central++) {

            right_wing_up = h->clean_up[position + (central + 1)];
            right_wing_down = h->clean_down[position + (central + 1)];

            if (right_wing_up > 0 && right_wing_down > 0) {
                tmp_ret = GETSHAPE(left_wing_up, left_wing_down, right_wing_up,
                                   right_wing_down, central);
                if (tmp_ret > ret)
                    ret = tmp_ret;
            }
        }
    }

    return ret;
}

int getLargestDirtyShape(int col, int pos_row, int pos_col, h_matrices_t *h)
{
    int ret = 0;
    int tmp_ret = 0;

    /* position of left-wing-central-wing 'joint' */
    int position = pos_row * col + pos_col;

    int left_up_clean = h->clean_up[position];
    int left_down_clean = h->clean_down[position];
    int left_up_dirty = h->dirty_up[position];
    int left_down_dirty = h->dirty_down[position];

    int right_up_clean, right_down_clean, right_up_dirty, right_down_dirty;

    /* stands for sizes of : left wing upper part, left wing lower part,
        right wing upper part, right wing lower part */
    /* from these variables (and size of central wing) is then computed the
     * whole size of shape (via GET_SHAPE macro func) */
    int l_wing_u, l_wing_d, r_wing_u, r_wing_d;

    /* circle in LEFT wing in UPper part */
    if (left_up_dirty > left_up_clean && left_down_clean) {

        l_wing_u = left_up_dirty;
        l_wing_d = left_down_clean;

        for (int central = 1; central < h->clean_right[position];
             central++) {
            r_wing_u = h->clean_up[position + (central + 1)];
            r_wing_d = h->clean_down[position + (central + 1)];
            if (r_wing_u > 0 && r_wing_d > 0) {
                tmp_ret =
                    GETSHAPE(l_wing_u, l_wing_d, r_wing_u, r_wing_d, central);
                if (tmp_ret > ret)
                    ret = tmp_ret;
            }
        }
    }

    /* circle in LEFT wing in LOWer part */
    if (left_down_dirty > left_down_clean && left_up_clean) {

        l_wing_u = left_up_clean;
        l_wing_d = left_down_dirty;

        for (int central = 1; central < h->clean_right[position];
             central++) {
            r_wing_u = h->clean_up[position + (central + 1)];
            r_wing_d = h->clean_down[position + (central + 1)];
            if (r_wing_u > 0 && r_wing_d > 0) {
                tmp_ret =
                    GETSHAPE(l_wing_u, l_wing_d, r_wing_u, r_wing_d, central);
                if (tmp_ret > ret)
                    ret = tmp_ret;
            }
        }
    }

    if (left_up_clean > 0 && left_down_clean > 0) {

        l_wing_u = left_up_clean;
        l_wing_d = left_down_clean;

        for (int central = 1; central < h->dirty_right[position];
             central++) {

            right_up_clean = h->clean_up[position + (central + 1)];
            right_down_clean = h->clean_down[position + (central + 1)];

            if (central + 1 <= h->clean_right[position]) {

                right_up_dirty = h->dirty_up[position + (central + 1)];
                right_down_dirty = h->dirty_down[position + (central + 1)];

                /* circle in RIGHT wing in UPper part */

                if (right_up_dirty > right_up_clean && right_down_clean > 0) {
                    r_wing_u = right_up_dirty;
                    r_wing_d = right_down_clean;

                    tmp_ret = GETSHAPE(l_wing_u, l_wing_d, r_wing_u, r_wing_d,
                                       central);
                    if (tmp_ret > ret)
                        ret = tmp_ret;
                }

                /* circle in RIGHT wing in LOWer part */

                if (right_down_dirty > right_down_clean && right_up_clean > 0) {
                    r_wing_u = right_up_clean;
                    r_wing_d = right_down_dirty;

                    tmp_ret = GETSHAPE(l_wing_u, l_wing_d, r_wing_u, r_wing_d,
                                       central);
                    if (tmp_ret > ret)
                        ret = tmp_ret;
                }

                /* without circle (clean shape) */

                if (right_up_clean > 0 && right_down_clean > 0) {
                    r_wing_u = right_up_clean;
                    r_wing_d = right_down_clean;

                    tmp_ret = GETSHAPE(l_wing_u, l_wing_d, r_wing_u, r_wing_d,
                                       central);
                    if (tmp_ret > ret)
                        ret = tmp_ret;
                }

            } else {

                /* circle in CENTRAL wing (or the 'joint' of central and right
                 * wing) */

                if (right_up_clean > 0 && right_down_clean > 0) {
                    r_wing_u = right_up_clean;
                    r_wing_d = right_down_clean;

                    tmp_ret = GETSHAPE(l_wing_u, l_wing_d, r_wing_u, r_wing_d,
                                       central);
                    if (tmp_ret > ret)
                        ret = tmp_ret;
                }
            }
        }
    }
    return ret;
}

void cleanUp(char *g, h_matrices_t *h)
{
    deallocateCharMatrix(&g);
    deallocateHelpStruct(&h);
}

int main(int argc, char *argv[])
{
    int ret = 0;

    int rows, cols;
    readDimensions(&rows, &cols);

    char *given_matrix = allocateCharMatrix(rows, cols);
    readMatrix(rows, cols, given_matrix);

    h_matrices_t *helpful_matrices = prepareHelpMatrices(rows, cols);

    computeUpMatrices(rows, cols, given_matrix, helpful_matrices);
    computeDownMatrices(rows, cols, given_matrix, helpful_matrices);
    computeRightMatrices(rows, cols, given_matrix, helpful_matrices);

    int result = 0;
    int tmp_result = 0;

    /* iterating through positions where we can put 'joint' of left-wing and
     * central-wing; these are according to rules all without these in
     * first & last row and these in last and penultimate column */
    int offset_row = rows - 1;
    int offset_col = cols - 2;

    for (int i = 1; i < offset_row; i++) {
        for (int j = 0; j < offset_col; j++) {
            int curr = i * cols + j;
            switch (given_matrix[curr]) {
            case DOT:
                /* in case of '.' there can be circle in the rest of shape */
                tmp_result = getLargestDirtyShape(cols, i, j, helpful_matrices);
                break;
            case CIRCLE:
                /* in case of 'o' there cannot be circle in the rest of shape */
                tmp_result = getLargestCleanShape(cols, i, j, helpful_matrices);
                break;
            }
            if (tmp_result > result)
                result = tmp_result;
        }
    }

    printf("%d\n", result);

    cleanUp(given_matrix, helpful_matrices);
    return ret;
}
