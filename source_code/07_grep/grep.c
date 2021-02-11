#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define BUF_SIZE 256

#define C_RED "\x1b[31m"
#define C_DEFAULT "\x1b[0m"

// command line arguments (must be passed in this order):
// 1) file name (defaults to stdin if not provided)
// 2) -c to enable colored output
int main(int argc, char const *argv[])
{
    char line[BUF_SIZE];
    char *line_ptr;
    char *find;
    char *first_find;
    char const *search;

    bool color_escape = false;

    int start_index = 1;
    FILE *file;

    if ((file = fopen(argv[start_index], "r")) != NULL)
    {
        start_index++;
    }
    else
    {
        file = stdin;
    }

    if (strcmp(argv[start_index], "-c") == 0)
    {
        color_escape = true;
        start_index++;
    }

    while (fgets(line, BUF_SIZE, file))
    {
        line_ptr = line;

        do
        {
            first_find = NULL;
            for (int i = start_index; i < argc; i++)
            {
                find = strstr(line_ptr, argv[i]);
                if (find != NULL && (find < first_find || first_find == NULL))
                {
                    first_find = find;
                    search = argv[i];
                }
            }

            if (first_find != NULL)
            {
                printf("%.*s%s%s%s", first_find - line_ptr, line_ptr,
                       (color_escape) ? C_RED : "", search,
                       (color_escape) ? C_DEFAULT : "");
                line_ptr = first_find + strlen(search);
            }

        } while (first_find != NULL);

        if (line_ptr != line)
        {
            printf("%s", line_ptr);
        }
    }

    return 0;
}
