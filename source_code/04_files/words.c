#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int count_words(char *s)
{
    int n;
    for (int i = 0; s[i] != 0; i++)
    {
        if (s[i] == ' ' || s[i] == '\n')
        {
            n++;
        }
    }
    return n;
}

char *find_next_word(char *s)
{
    int i = 0;
    while (1)
    {
        if (s[i] == ' ' || s[i] == '\n')
        {
            s[i] = '\0';
            return &(s[i + 1]);
        }
        else if (s[i] == '\0')
        {
            return NULL;
        }
        i++;
    }
}

void find_words(FILE *fd)
{
    char first_line[256];
    char buffer[256];

    if (!fgets(first_line, 256, fd))
    {
        return;
    }

    int n_words = count_words(first_line);

    if (n_words < 1)
    {
        return;
    }

    char **words = (char **)malloc(sizeof(char *) * n_words);

    words[0] = first_line;

    for (int i = 1; i < n_words; i++)
    {
        words[i] = find_next_word(words[i - 1]);
    }

    find_next_word(words[n_words - 1]);

    // for (int i = 0; i < n_words; i++) {
    //     printf("%s\n", words[i]);
    // }
    int i_line = 2;
    while (fgets(buffer, 256, fd))
    {
        for (int i = 0; i < n_words; i++)
        {
            if (strstr(buffer, words[i]))
            {
                printf("line %d, word '%s': %s", i_line, words[i], buffer);
                break;
            }
        }
        i_line++;
    }

    free(words);
}

int main(int argc, char const *argv[])
{
    FILE *fd = fopen(argv[1], "r");

    find_words(fd);

    fclose(fd);

    return 0;
}
