#include <stdio.h>
#include <stdlib.h>

/*
 * Function: alloc_2d_array
 * ------------------------
 * dynamically allocate 2d array of chosen dimensions
 * 
 * input:
 * - size_t n - number of rows
 * - size_t m - number of collumns
 * 
 * return value:
 * - 2d array
 * - NULL if the allocation fails
 */
int **alloc_2d_array(size_t n, size_t m)
{
    int **p;

    // allocate pointer array to point to start of each row
    // if malloc doesn't work:
    // - return NULL
    p = (int **)malloc(n * sizeof(int *));
    if (p == NULL)
        return NULL;
    // allocate all rows in one block and save it
    // to first index of previously allocated array
    // if malloc does't work:
    // - free previously allocated pointer array
    // - return NULL
    p[0] = (int *)malloc(n * m * sizeof(int));
    if (p == NULL)
    {
        free(p);
        return NULL;
    }
    // fill the rest of pointer array to point to first column of each row
    for (size_t i = 0; i < n; i++)
    {
        p[i] = p[0] + i * sizeof(int);
    }
    // return pointer to the pointer array
    return p;
}

/*
 * Function: free_2d_array
 * -----------------------
 * frees a memory allocated to the 2d array
 * 
 * input:
 * - int **p - pointer to the array to be freed
 */
void free_2d_array(int **p)
{
    // free the memory allocated for data
    free(p[0]);
    // then free the memory allocated for pointers
    free(p);
}

int main(int argc, char const *argv[])
{
    const size_t n = 3,
                 m = 4;

    int **p;
    p = alloc_2d_array(n, m);
    if (p == NULL)
        return 1;

    printf("p    = %p\n", p);
    for (size_t i = 0; i < n; i++)
    {
        printf("p[%d] = %p\n", i, p[i]);
    }

    free_2d_array(p);

    return 0;
}
