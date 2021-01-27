#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

/* 
 * AUTOPLAY 0 - try the problem yourself
 * AUTOPLAY 1 - automatically solve the problem 
 */
#define AUTOPLAY 1

typedef struct hanoi
{
    int *a;
    int *b;
    int *c;
    int n;
} hanoi_t;

hanoi_t *constr_hanoi(int n, hanoi_t *cpy)
{
    hanoi_t *h;

    if ((h = (hanoi_t *)malloc(sizeof(hanoi_t))) == NULL)
    {
        return NULL;
    }

    h->n = n;

    if ((h->a = (int *)malloc(sizeof(int) * n)) == NULL)
    {
        free(h);
        return NULL;
    }

    if ((h->b = (int *)malloc(sizeof(int) * n)) == NULL)
    {
        free(h->a);
        free(h);
        return NULL;
    }

    if ((h->c = (int *)malloc(sizeof(int) * n)) == NULL)
    {
        free(h->b);
        free(h->a);
        free(h);
        return NULL;
    }

    for (int i = 0; i < n; i++)
    {
        if (cpy == NULL)
        {
            h->a[i] = (i + 1);
            h->b[i] = 0;
            h->c[i] = 0;
        }
        else
        {
            h->a[i] = cpy->a[i];
            h->b[i] = cpy->b[i];
            h->c[i] = cpy->c[i];
        }
    }

    return h;
}

void free_hanoi(hanoi_t *h)
{
    free(h->a);
    free(h->b);
    free(h->c);
    free(h);
}

void get_hanoi_disk_str(char s[], const int w, const int n, char c)
{
    for (int i = 0; i <= n; i++)
    {
        s[n - i] = (i <= w) ? c : ' ';
        s[n + i] = (i <= w) ? c : ' ';
    }

    s[2 * n + 1] = '\0';
}

void print_hanoi_row(hanoi_t *h, int r, FILE *f)
{
    char s[256];
    assert(2 * h->n < 256);

    if (r == -2)
    {
        get_hanoi_disk_str(s, 0, h->n, 'A');
        fprintf(f, "%s ", s);
        get_hanoi_disk_str(s, 0, h->n, 'B');
        fprintf(f, "%s ", s);
        get_hanoi_disk_str(s, 0, h->n, 'C');
        fprintf(f, "%s\n", s);
    }
    else if (r == -1)
    {
        get_hanoi_disk_str(s, 0, h->n, '|');
        fprintf(f, "%s ", s);
        get_hanoi_disk_str(s, 0, h->n, '|');
        fprintf(f, "%s ", s);
        get_hanoi_disk_str(s, 0, h->n, '|');
        fprintf(f, "%s\n", s);
    }
    else
    {
        get_hanoi_disk_str(s, h->a[r], h->n, h->a[r] ? '=' : '|');
        fprintf(f, "%s ", s);
        get_hanoi_disk_str(s, h->b[r], h->n, h->b[r] ? '=' : '|');
        fprintf(f, "%s ", s);
        get_hanoi_disk_str(s, h->c[r], h->n, h->c[r] ? '=' : '|');
        fprintf(f, "%s\n", s);
    }
}

void print_hanoi(hanoi_t *h, FILE *f)
{
    for (int i = -2; i < h->n; i++)
    {
        print_hanoi_row(h, i, f);
    }
    fputc('\n', f);
}

bool move_disk(hanoi_t *h, char from, char to)
{
    int *ar[3];
    int from_i, to_i;
    int from_r = -1, to_r = -1;
    int x;

    assert(from >= 'a' && from <= 'c');
    assert(to >= 'a' && to <= 'c');

    if (from == to)
        return false;

    ar[0] = h->a;
    ar[1] = h->b;
    ar[2] = h->c;

    from_i = from - 'a';
    to_i = to - 'a';

    for (int r = 0; r < h->n; r++)
    {
        if (ar[from_i][r] > 0)
        {
            from_r = r;
            break;
        }
    }

    if (from_r == -1)
        return false;

    for (int r = 0; r < h->n; r++)
    {
        if (ar[to_i][r] > 0)
        {
            to_r = r;
            break;
        }
    }

    x = ar[from_i][from_r];

    if (to_r > 0)
    {
        if (x > ar[to_i][to_r])
            return false;
    }
    else
    {
        to_r = h->n;
    }

    ar[from_i][from_r] = 0;
    ar[to_i][to_r - 1] = x;

    return true;
}

void solve_hanoi(hanoi_t *h, int d, char src, char tar, char aux)
{
    static int calls = 0;
    if (d == 0)
        return;

    solve_hanoi(h, d - 1, src, aux, tar);

    move_disk(h, src, tar);

    printf("\ncall #%d - depth %d, src %c, tar %c, aux %c\n", ++calls, d, src, tar, aux);
    print_hanoi(h, stdout);

    solve_hanoi(h, d - 1, aux, tar, src);
}

int main(int argc, char const *argv[])
{
    hanoi_t *h;
    int n;
    printf("enter size: ");
    while (scanf("%d", &n) < 1)
    {
        printf("wrong size scan!\n");
        printf("enter size: ");
        fflush(stdin);
    }
    fflush(stdin);

    h = constr_hanoi(n, NULL);

    if (h == NULL)
    {
        printf("const failed\n");
        return 1;
    }
    printf("\n");
    print_hanoi(h, stdout);

#if AUTOPLAY
    solve_hanoi(h, n, 'a', 'c', 'b');
    free_hanoi(h);
    return 0;
#else  /* AUTOPLAY 0 */
    char c1, c2;
    bool rv;

    while (scanf("%c%c", &c1, &c2) >= 2)
    {
        fflush(stdin);
        if (c1 == 'q')
            break;
        printf("moving from '%c' to '%c'\n", c1, c2);
        rv = move_disk(h, c1, c2);
        if (rv == false)
        {
            printf("invalid move!\n");
        }
        else
        {
            print_hanoi(h, stdout);
        }
    }

    free_hanoi(h);
    return 0;
#endif /* AUTOPLAY */
}
