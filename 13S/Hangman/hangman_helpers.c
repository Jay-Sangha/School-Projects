#include "hangman_helpers.h"

bool is_lowercase_letter(char c) {

    if (c >= 'a' && c <= 'z') {
        return true;
    } else {
        return false;
    }
}

bool validate_secret(const char *secret) {

    int len = (int) strlen(secret);

    if (len > 256) {
        printf("the secret phrase is over 256 characters\n");
        return false;
    }

    for (int i = 0; i < len; ++i) {
        if ((is_lowercase_letter(secret[i]) || string_contains_character(punctuation, secret[i]))
            == false) {

            fprintf(stdout,
                "invalid character: '%c'\n"
                "the secret phrase must contain "
                "only lowercase letters, spaces, hyphens, and apostrophes\n",
                secret[i]);
            return false;
        }
    }

    return true;
}

bool string_contains_character(const char *s, char c) {

    if (strchr(s, c) != NULL) {
        return true;
    } else {
        return false;
    }
}

char read_letter(void) {
    char guess;
    printf("Guess a letter: ");
    guess = (char) getchar();

    while (guess == '\n') {
        guess = (char) getchar();
    }

    return guess;
}
