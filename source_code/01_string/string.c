#include <stdio.h>
#include <string.h>

/*
 * Function: my_strlen
 * -------------------
 * returns length of a null-terminated string
 * null character is not included in the length
 * 
 * input:
 * - char *s - pointer to the string (to first byte in null-terminated string)
 * 
 * return value:
 * - length of the entered string (excluding the null character at the end)
 */
size_t my_strlen(char *s)
{
    size_t i = 0;
    while (s[i] != '\0')
    {
        i++;
    }
    return i;
}

/*
 * Function: my_strcpy
 * ------------------- 
 * copies string from source string to a destination string
 * (overwrites destination string)
 * 
 * input:
 * - char *dest - pointer to the destination string
 * - char *src - pointer to the source string
 */
void my_strcpy(char *dest, char *src)
{
    size_t src_len;

    src_len = my_strlen(src);

    for (size_t i = 0; i < src_len; i++)
    {
        dest[i] = src[i];
    }

    dest[src_len] = '\0';
}

/* 
 * Function: my_strcat
 * -------------------
 * copies a source string at the end of destination string 
 * (does not overwrite contents destination string)
 * 
 * input:
 * - char *dest - pointer to the destination string
 * - char *src - pointer to the source string
 */
void my_strcat(char *dest, char *src)
{
    my_strcpy(dest + my_strlen(dest), src);
}

/* 
 * Function: my_strcmp
 * -------------------
 * compares two strings and returns integer based on their comparison
 * 
 * input:
 * - char *s1 - pointer to the first string
 * - char *s2 - pointer to the second string
 * 
 * return value:
 * - 0 if both strings are identical
 * - negative number (-1) if the the first different character has smaller
 *   ascii value in the first string
 * - positive number (1) if the the first different character has smaller
 *   ascii value in the second string
 */
int my_strcmp(char *s1, char *s2)
{
    for (int i = 0; s1[i] != '\0' || s2[i] != '\0'; i++)
    {
        if (s1[i] < s2[i])
            return -1;
        else if (s1[i] > s2[i])
            return 1;
    }
    return 0;
}

int main(int argc, char const *argv[])
{
    // showcase code for the functions above
    char s1[256];
    char s2[10000];

    s2[0] = '\0';

    while (1)
    {
        gets(s1);
        if (my_strcmp(s1, "") == 0)
            break;
        my_strcat(s2, s1);
        my_strcat(s2, " ");
        printf("curr string: ");
        puts(s2);
    }

    return 0;
}
