#include "stdafx.h"

void PrintResults(const int* x, int nrows, int ncols, int* row_sums, int* col_sums)
{
    for (int i = 0; i < nrows; i++)
    {
        for (int j = 0; j < ncols; j++)
            printf("%5d ", x[i* ncols + j]);
        printf(" -- %5d\n", row_sums[i]);
    }
    printf("\n");

    for (int i = 0; i < ncols; i++)
        printf("%5d ", col_sums[i]);
    printf("\n");
}
