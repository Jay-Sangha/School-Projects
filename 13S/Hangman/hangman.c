#include "hangman_helpers.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LOSING_MISTAKE 6

int main(int c, char *word[]) {

    int mistakes = 0;
    char guess;
    char gletters[26] = { 0 };
    char sword[256] = { 0 };
    char *w = sword;
    int len = (int) strlen(word[1]);
    strncpy(sword, word[1], (size_t) len);
    char lets[26] = { 0 };
    char *p = lets;

    if (c > 2) {
        printf("wrong number of arguments\n"
               "usage: ./hangman <secrete word or phrase>\n"
               "if the secret is multiple words, you must quote it\n");
        exit(1);
    }

    if (validate_secret(word[1]) == false) {
        exit(1);
    }

    for (int i = 0; i < len; ++i) {
        if (is_lowercase_letter(*w) == true) {
            *w = '_';
        }
        w++;
    }

    while (mistakes < LOSING_MISTAKE) {

        printf("%s", CLEAR_SCREEN);
        printf("%s\n\n", arts[mistakes]);
        printf("    Phrase: %s\n", sword);

        printf("Eliminated: ");
        for (int i = 0; i < 26; i++) {
            if (gletters[i] == 1) {
                printf("%c", 'a' + i);
            }
        }

        printf("\n\n");

        do {
            guess = read_letter();
        } while (!is_lowercase_letter(guess) || string_contains_character(sword, guess)
                 || string_contains_character(lets, guess));

        if (string_contains_character(word[1], guess) == true) {
            for (int i = 0; i < len; ++i) {
                if (guess == word[1][i]) {
                    sword[i] = guess;
                }
            }
        }

        else {

            if (string_contains_character(lets, guess) == false) {

                gletters[guess - 'a'] = 1;
                mistakes++;
                for (int i = 0; i < 26; ++i) {
                    if (gletters[i] == 1) {
                        *p = (char) i + 'a';
                        p++;
                    }
                }
            }
        }

        if (strcmp(sword, word[1]) == 0) {
            break;
        }
    }

    printf("%s", CLEAR_SCREEN);
    printf("%s\n\n", arts[mistakes]);

    if (mistakes >= LOSING_MISTAKE) {
        printf("    Phrase: %s\n", sword);
        printf("Eliminated: ");
        for (int i = 0; i < 26; i++) {
            if (gletters[i] == 1) {
                printf("%c", 'a' + i);
            }
        }
        printf("\n\nYou lose! The secret phrase was: %s\n", word[1]);
    } else {
        printf("    Phrase: %s\n", sword);
        printf("Eliminated: ");
        for (int i = 0; i < 26; i++) {
            if (gletters[i] == 1) {
                printf("%c", 'a' + i);
            }
        }

        printf("\n\nYou win! The secret phrase was: %s\n", word[1]);
    }

    return 0;
}
