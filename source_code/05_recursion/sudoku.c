#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

typedef int8_t num_t;

typedef struct table
{
    num_t x[9][9];
} table_t;

table_t import_table(FILE *file)
{
    table_t table;

    char line[256];
    char c;

    int i, j, c_index;

    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 9; j++)
        {
            table.x[i][j] = -1;
        }
    }

    for (i = 0; i < 9; i++)
    {
        if (fgets(line, 256, file) == NULL)
        {
            return table;
        }
        j = 0;
        c_index = 0;

        while ((c = line[c_index++]) && (j < 9))
        {
            if ((c >= '1') && (c <= '9'))
            {
                table.x[i][j++] = c - '0';
            }
            else if ((c == '.') || (c == '0'))
            {
                table.x[i][j++] = 0;
            }
        }

        if (j < 9)
        {
            for (j = 0; j < 9; j++)
            {
                table.x[i][j] = -1;
            }
            i--;
        }
    }

    return table;
}

void print_table(const table_t *table, FILE *file)
{
    const char sep[] = "+-----+-----+-----+\n";
    num_t n;

    for (int i = 0; i < 9; i++)
    {
        if (i % 3 == 0)
        {
            fputs(sep, file);
        }
        for (int j = 0; j < 9; j++)
        {
            fputc((j % 3 == 0) ? '|' : ' ', file);
            n = table->x[i][j];
            fputc((n == 0) ? '.' : ((n == -1) ? 'x' : (char)(table->x[i][j] + '0')), file);
        }
        fputs("|\n", file);
    }
    fputs(sep, file);
}

bool is_num_valid(const table_t *t, const num_t num, const int i, const int j)
{
    if (num == 0)
        return true;

    int i_sq = (i / 3) * 3;
    int j_sq = (j / 3) * 3;

    for (int k = 0; k < 9; k++)
    {
        if (t->x[i][k] == num)
            return false;
        if (t->x[k][j] == num)
            return false;
        if (t->x[i_sq + k / 3][j_sq + k % 3] == num)
            return false;
    }

    return true;
}

bool fill_table(table_t *t, const int i, const int j)
{
    int next_i, next_j;

    next_i = i + (j + 1) / 9;
    next_j = (j + 1) % 9;

    if (i >= 9)
        return true;

    if (t->x[i][j] != 0)
        return fill_table(t, next_i, next_j);

    for (num_t n = 1; n <= 9; n++)
    {
        if (is_num_valid(t, n, i, j))
        {
            t->x[i][j] = n;
            if (fill_table(t, next_i, next_j))
                return true;
            t->x[i][j] = 0;
        }
    }

    return false;
}

int main(int argc, char const *argv[])
{
    table_t t;
    if (argc < 2)
    {
        t = import_table(stdin);
    }
    else
    {
        FILE *file;
        file = fopen(argv[1], "r");
        t = import_table(file);
        fclose(file);
    }

    fputs("imported table:\n", stdout);
    print_table(&t, stdout);
    if (fill_table(&t, 0, 0))
    {
        fputs("solved table:\n", stdout);
        print_table(&t, stdout);
    }
    else
    {
        fputs("unsolvable!", stdout);
    }

    return 0;
}
