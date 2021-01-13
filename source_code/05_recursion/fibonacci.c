#include <stdio.h>
#include <stdlib.h>

void fibonacci(int n)
{
    int a = 0, b = 1, b_old;
    for (int i = 0; i < n; i++)
    {
        printf("%d\n", b);
        b_old = b;
        b += a;
        a = b_old;
    }
}

int fib(int n)
{
    if (n < -1)
        return -1;

    static size_t mem_size = 0;
    static int *mem = NULL;
    static int calls = 0;

    if (n == -1)
    {
        free(mem);
        return calls;
    }

    calls++;

    if (mem == NULL)
    {
        mem_size = n + 1;
        mem = (int *)malloc(sizeof(int) * mem_size);
        if (mem == NULL)
        {
            return -2;
        }

        for (int i = 0; i < n + 1; i++)
        {
            mem[i] = 0;
        }
    }
    else if (mem_size < n + 1)
    {
        int *tmp;
        tmp = (int *)realloc(mem, sizeof(int) * (n + 1));
        if (tmp == NULL)
        {
            return -3;
        }
        else
        {
            mem = tmp;
        }

        for (int i = mem_size; i < n + 1; i++)
        {
            mem[i] = 0;
        }

        mem_size = n + 1;
    }

    if (n < 2)
    {
        return 1;
    }

    int rv;

    if (mem[n] == 0)
    {
        rv = fib(n - 1) + fib(n - 2);
        mem[n] = rv;
    }
    else
    {
        rv = mem[n];
    }

    return rv;
}

void fibonacci_rec(int n)
{
    int fib_rv;
    for (int i = 0; i < n; i++)
    {
        fib_rv = fib(i);
        if (fib_rv >= 0)
        {
            printf("%d\n", fib_rv);
        }
        else
        {
            // TODO: handle error
            printf("exceptional return value: %d", fib_rv);
        }
    }
}

int main(int argc, char *argv[])
{

    int n;

    if (scanf("%d", &n) < 1)
    {
        return 1;
    }

    // 1 1 2 3 5 8 13 21 34 55 89 ...
    fibonacci_rec(n);

    printf("calls: %d\n", fib(-1));

    return 0;
}
