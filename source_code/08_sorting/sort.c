#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define C_RED "\x1b[31m"
#define C_GREEN "\x1b[32m"
#define C_DEFAULT "\x1b[0m"

void swap(int *i, int *j);

void shuffle_array(int array[], int size);
int *get_random_array(int size);

void print_array(int array[], int size);
void bar_print_array(int *array_in, int size_in, int hl1, int hl2, char hl_color[]);
void fast_bar_print(int hl1, int hl2, char hl_color[]);

void selection_sort(int array[], int size);

void heapify(int array[], int heap_size, int i);
void heap_sort(int array[], int size);

int partition(int array[], int low, int high);
void quick_sort_rec(int array[], int low, int high);
void quick_sort(int array[], int size);

int main(int argc, char const *argv[])
{
    int size;
    int *array;

    if (argc < 2)
    {
        return 1;
    }
    size = atoi(argv[1]);
    printf("size: %d\n", size);
    array = get_random_array(size);

    if (array == NULL)
    {
        return 2;
    }

    srand(7);

    bar_print_array(array, size, -1, -1, "");

    selection_sort(array, size);
    // heap_sort(array, size);
    // quick_sort(array, size);

    bar_print_array(array, size, -1, -1, "");
    free(array);
    return 0;
}

void swap(int *i, int *j)
{
    int tmp;

    tmp = *i;
    *i = *j;
    *j = tmp;
}

void shuffle_array(int array[], int size)
{
    int i, j, tmp;

    for (i = size - 1; i > 0; i--)
    {
        j = rand() % (i + 1);

        swap(&array[i], &array[j]);
    }
}

int *get_random_array(int size)
{
    int *array;

    if ((array = (int *)malloc(sizeof(int) * size)) == NULL)
    {
        return NULL;
    }

    for (int i = 0; i < size; i++)
    {
        array[i] = i + 1;
    }

    shuffle_array(array, size);

    return array;
}

void print_array(int array[], int size)
{
    for (int i = 0; i < size; i++)
    {
        printf("%c%d%c", (i == 0) ? '[' : ' ', array[i], (i == size - 1) ? ']' : ',');
    }
    putc('\n', stdout);
}

void bar_print_array(int *array_in, int size_in, int hl1, int hl2, char hl_color[])
{
    static int *array = NULL;
    static int size = 0;

    if (array_in != NULL)
    {
        array = array_in;
        size = size_in;
    }

    if (array == NULL)
    {
        return;
    }

    for (int j = 0; j < size + 5; j++)
    {
        putc('-', stdout);
    }
    putc('\n', stdout);
    for (int i = 0; i < size; i++)
    {
        printf("%s%3d |", ((i == hl1) || (i == hl2)) ? hl_color : "", array[i]);
        for (int j = 0; j < array[i]; j++)
        {
            putc('=', stdout);
        }
        printf("%s\n", ((i == hl1) || (i == hl2)) ? C_DEFAULT : "");
    }
}

void fast_bar_print(int hl1, int hl2, char hl_color[])
{
    bar_print_array(NULL, 0, hl1, hl2, hl_color);
}

void selection_sort(int array[], int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = i + 1; j < size; j++)
        {
            if (array[i] > array[j])
            {
                swap(&array[i], &array[j]);
            }
        }
    }
}

void heapify(int array[], int heap_size, int i)
{
    int l = 2 * i + 1;
    int r = 2 * i + 2;
    int max_i = i;

    // check if left child exists
    if (l < heap_size)
    {
        // check if left child is larger then parent
        if (array[max_i] < array[l])
        {
            max_i = l;
        }
    }

    // check if right child exists
    if (r < heap_size)
    {
        // check if right is maximum
        if (array[max_i] < array[r])
        {
            max_i = r;
        }
    }

    if (max_i != i)
    {
        swap(&array[max_i], &array[i]);
        heapify(array, heap_size, max_i);
    }
}

void heap_sort(int array[], int size)
{
    for (int i = size / 2 - 1; i >= 0; i--)
    {
        heapify(array, size, i);
    }

    for (int i = size - 1; i > 0; i--)
    {
        swap(&array[0], &array[i]);
        heapify(array, i, 0);
    }
}

int partition(int array[], int low, int high)
{
    int i = low;
    // pivot is last element = array[high-1]
    for (int j = low; j < high - 1; j++)
    {
        if (array[j] <= array[high - 1])
        {
            swap(&array[i], &array[j]);
            i++;
        }
    }

    swap(&array[i], &array[high - 1]);
    return i;
}

void quick_sort_rec(int array[], int low, int high)
{
    if (low >= high - 1)
        return;

    int pivot_i = partition(array, low, high);

    quick_sort_rec(array, low, pivot_i);
    quick_sort_rec(array, pivot_i + 1, high);
}

void quick_sort(int array[], int size)
{
    quick_sort_rec(array, 0, size);
}
