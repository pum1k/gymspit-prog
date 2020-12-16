#include <stdio.h>
#include <string.h>

// one of the possible solutions to this task

int count_uppercase(char s[])
{
    int n;

    for (size_t i = 0; i < strlen(s); i++)
    {
        if ('A' <= s[i] && s[i] <= 'Z')
        {
            n++;
        }
    }

    return n;
}

int count_lowercase(char s[])
{
    int n;

    for (size_t i = 0; i < strlen(s); i++)
    {
        if ('a' <= s[i] && s[i] <= 'z')
        {
            n++;
        }
    }

    return n;
}

int count_digits(char s[])
{
    int n;

    for (size_t i = 0; i < strlen(s); i++)
    {
        if ('0' <= s[i] && s[i] <= '9')
        {
            n++;
        }
    }

    return n;
}

int main()
{
    char str[256];

    gets(str);
    printf("uppercase: %d\n", count_uppercase(str));
    printf("lowercase: %d\n", count_lowercase(str));
    printf("digits: %d\n", count_digits(str));

    return 0;
}
