#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Function: count_words
 * ---------------------
 * Calculates number of words in the string
 * (word must end with either a space or newline character)
 * 
 * input:
 * - char *s - string to be counted
 * 
 * return value:
 * - number of words
 */
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

/*
 * Function: find_next_word
 * ------------------------
 * Finds the end of first word in the string
 * (word must end with either a space or newline character),
 * places null-character after it and returns the address of first character
 * after the terminating null-character it created.
 * 
 * Returns NULL if it reaches end of string before ond of word.
 * 
 * This function only assumes one word terminating character after a word.
 * 
 * input:
 * - char *s - string in which we search for the word
 * 
 * return value:
 * - address at which the next word starts
 * - NULL if there are no other words
 */
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

/*
 * Function: find_words
 * --------------------
 * Reads a file inputed in fd.
 * Finds single words in the first line and stores them,
 * then prints every line in which is at least one of the stored words
 * from the first line.
 * 
 * Each line is printed with a corresponding line number
 * (counting from one, 
 * including the first line from which are the searched words)
 * and the first word found on that line
 * 
 * input:
 * - FILE *fd - file to be read
 */
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
    // showcase code for the functions above

    FILE *fd = fopen(argv[1], "r");

    find_words(fd);

    fclose(fd);

    return 0;
}
