#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#define BUF_SIZE 256

#define N_GUESSES 9
char hangman_ascii[N_GUESSES + 1][BUF_SIZE];

typedef struct hangman
{
    char word[BUF_SIZE];
    char hidden[BUF_SIZE];
    char guessed[BUF_SIZE];
    int rem_guesses;
} hangman_t;

void strip_end(char str[], char c)
{
    int str_len = strlen(str);

    if (str[str_len - 1] == c)
    {
        str[str_len - 1] = '\0';
    }
}

void make_uppercase(char str[])
{
    int str_len = strlen(str);

    for (int i = 0; i < str_len; i++)
    {
        str[i] = (str[i] >= 'a' && str[i] <= 'z') ? str[i] + ('A' - 'a') : str[i];
    }
}

int get_hangman_ascii()
{
    FILE *file;
    int n;
    if ((file = fopen("hangman_ascii.txt", "r")) == NULL)
    {
        return -1;
    }
    fflush(file);

    for (int i = 0; i < N_GUESSES + 1; i++)
    {
        hangman_ascii[i][0] = 0;
        do
        {
            n = strlen(hangman_ascii[i]);
            if (fgets(hangman_ascii[i] + n, BUF_SIZE - n, file) == 0)
            {
                printf("hangman ascii fail");
                return -2;
            }
        } while (strcmp(hangman_ascii[i] + n, "\n") != 0);
    }

    fclose(file);
    return 0;
}

int get_word(char word[], const char file_name[], const int n_words)
{
    FILE *file;
    int rand_n;

    if ((file = fopen(file_name, "r")) == NULL)
        return -1;

    rand_n = rand() % n_words;

    for (int i = 0; i <= rand_n; i++)
    {
        if ((fgets(word, BUF_SIZE, file) == NULL))
        {
            fclose(file);
            return -2;
        }
    }

    strip_end(word, '\n');
    make_uppercase(word);

    fclose(file);
    return 0;
}

void print_hangman(hangman_t *hm)
{
    char buf[BUF_SIZE];

    printf(hangman_ascii[hm->rem_guesses]);

    for (int i = 0; i < strlen(hm->hidden); i++)
    {
        buf[2 * i] = hm->hidden[i];
        buf[2 * i + 1] = ' ';
    }
    buf[2 * strlen(hm->hidden)] = '\0';

    printf("\n%s\n", buf);

    for (int i = 0; i < strlen(hm->guessed); i++)
    {
        buf[2 * i] = hm->guessed[i];
        buf[2 * i + 1] = ' ';
    }
    buf[2 * strlen(hm->guessed)] = '\0';

    printf("guessed: %s\n", buf);
}

int start_hangman(hangman_t *hm)
{
    get_word(hm->word, "3000_words.txt", 3000);

    for (int i = 0; i < strlen(hm->word); i++)
    {
        hm->hidden[i] = (hm->word[i] >= 'A' && hm->word[i] <= 'Z') ? '_' : hm->word[i];
    }

    for (int i = 0; i < BUF_SIZE; i++)
    {
        hm->guessed[i] = 0;
    }

    hm->rem_guesses = N_GUESSES;

    return 0;
}

int make_guess(hangman_t *hm)
{
    char line[BUF_SIZE];
    char c;
    bool correct_guess;

    fgets(line, BUF_SIZE, stdin);

    strip_end(line, '\n');
    make_uppercase(line);

    if (strlen(line) > 1)
    {
        if (strcmp(line, hm->word) == 0)
        {
            strcpy(hm->hidden, line);
            return 0;
        }
        else
        {
            hm->rem_guesses--;
            return 3;
        }
    }
    else
    {
        c = line[0];
        for (int i = 0; i < strlen(hm->guessed); i++)
        {
            if (hm->guessed[i] == c)
            {
                return 2;
            }
        }

        hm->guessed[strlen(hm->guessed)] = c;

        correct_guess = false;

        for (int i = 0; i < strlen(hm->word); i++)
        {
            if (hm->word[i] == c)
            {
                correct_guess = true;
                hm->hidden[i] = c;
            }
        }

        if (!correct_guess)
        {
            hm->rem_guesses--;
            return 4;
        }
        else if (strcmp(hm->hidden, hm->word) == 0)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }
}

int play_hangman(void)
{
    hangman_t hm;
    int rv;

    start_hangman(&hm);

    printf("Welcome to hangman!\n");
    printf("Guess a word with %d characters.\n", strlen(hm.word));

    print_hangman(&hm);

    while (hm.rem_guesses > 0)
    {
        rv = make_guess(&hm);
        switch (rv)
        {
        case 0:
            print_hangman(&hm);
            printf("Congratulations, you guessed the word '%s' correctly!\n", hm.word);
            return 1;
            break;

        case 1:
            printf("Letter '%c' is in the word.\n", hm.guessed[strlen(hm.guessed) - 1]);
            break;

        case 2:
            printf("Letter already guessed.\n");
            break;

        case 3:
            printf("Wrong word guess, %d guesses remaining.\n", hm.rem_guesses);
            break;

        case 4:
            printf("Wrong letter guess, %d guesses remaining.\n", hm.rem_guesses);
            break;

        default:
            break;
        }

        print_hangman(&hm);
    }

    printf("Out of guesses, you lose, the word was '%s'!\n", hm.word);

    return 0;
}

int main(int argc, char const *argv[])
{
    srand(time(NULL));

    if (get_hangman_ascii() < 0)
    {
        return 1;
    }

    play_hangman();

    return 0;
}
