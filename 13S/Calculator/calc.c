#include "mathlib.h"
#include "messages.h"
#include "operators.h"
#include "stack.h"

#include <getopt.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {

    int opt;

    while ((opt = getopt(argc, argv, "hm")) != -1) {
        switch (opt) {
        case 'h': printf(USAGE, "./calc"); return 0;

        case '?': break;
        }
    }

    while (1) {
        char *saveptr;
        bool error = false;
        char expr[1024];

        fprintf(stderr, "> ");

        if (fgets(expr, sizeof(expr), stdin) == NULL) {
            return 0;
        }

        size_t len = strlen(expr);

        if (len > 0 && expr[len - 1] == '\n') {
            expr[len - 1] = '\0';
        }

        const char *token = strtok_r(expr, " ", &saveptr);

        while (token != NULL && !error) {
            double hold;
            if (parse_double(token, &hold)) {
                if (!stack_push(hold)) {
                    error = true;
                    break;
                }
            } else {
                if (strlen(token) == 1) {
                    switch (token[0]) {
                    case '+':
                        if (!apply_binary_operator(operator_add)) {
                            fprintf(stderr, ERROR_BINARY_OPERATOR);
                            error = true;
                        }

                        break;

                    case '-':
                        if (!apply_binary_operator(operator_sub)) {
                            fprintf(stderr, ERROR_BINARY_OPERATOR);
                            error = true;
                        }

                        break;
                        //error = !!apply_binary_operator(operator_sub); break;

                    case '*':
                        if (!apply_binary_operator(operator_mul)) {
                            fprintf(stderr, ERROR_BINARY_OPERATOR);
                            error = true;
                        }

                        break;
                        ///error = !!apply_binary_operator(operator_mul); break;

                    case '/':
                        if (!apply_binary_operator(operator_div)) {
                            fprintf(stderr, ERROR_BINARY_OPERATOR);
                            error = true;
                        }

                        break;

                        ///error = !!apply_binary_operator(operator_div); break;

                    case '%':
                        if (!apply_binary_operator(fmod)) {
                            fprintf(stderr, ERROR_BINARY_OPERATOR);
                            error = true;
                        }

                        break;
                        ///error = !apply_binary_operator(fmod); break;

                    case 's':
                        if (!apply_unary_operator(Sin)) {

                            fprintf(stderr, ERROR_UNARY_OPERATOR);

                            error = true;
                        }

                        break;
                        ///error = !apply_unary_operator(Sin); break;

                    case 'c':

                        if (!apply_unary_operator(Cos)) {

                            fprintf(stderr, ERROR_UNARY_OPERATOR);

                            error = true;
                        }

                        break;
                        ///error = !apply_unary_operator(Cos); break;

                    case 't':
                        if (!apply_unary_operator(Tan)) {

                            fprintf(stderr, ERROR_UNARY_OPERATOR);

                            error = true;
                        }

                        break;
                        ////error = !apply_unary_operator(Tan); break;

                    case 'a':
                        if (!apply_unary_operator(Abs)) {

                            fprintf(stderr, ERROR_UNARY_OPERATOR);

                            error = true;
                        }

                        break;
                        ///error = !apply_unary_operator(Abs);

                    case 'r':

                        if (!apply_unary_operator(Sqrt)) {

                            fprintf(stderr, ERROR_UNARY_OPERATOR);

                            error = true;
                        }
                        ////error = !apply_unary_operator(Sqrt);
                        break;

                    default:
                        fprintf(stderr, ERROR_BAD_CHAR, *token);
                        error = true;
                        break;
                    }
                }

                else {
                    fprintf(stderr, ERROR_BAD_STRING, token);
                    error = true;
                    break;
                }
            }
            token = strtok_r(NULL, " ", &saveptr);
        }

        if (error == false) {
            stack_print();
            printf("%c", '\n');
        }

        stack_clear();
    }

    return 0;
}
