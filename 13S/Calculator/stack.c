#include "stack.h"

#include "mathlib.h"
#include "messages.h"

#include <stdbool.h>
#include <stdio.h>

int first = -1;
double stack[STACK_CAPACITY] = { 0 };
int stack_size = 0;

void stack_print(void) {
    if (stack_size == 0) {
        return;
    }

    printf("%.10f", stack[0]);

    for (int i = 1; i < stack_size; i++) {
        printf(" %.10f", stack[i]);
    }
}

void stack_clear(void) {
    stack_size = 0;
    first = -1;
}

bool stack_peek(double *item) {
    if (stack_size == 0) {
        return false;
    }

    else {

        *item = stack[first];

        return true;
    }
}

bool stack_pop(double *item) {
    if (stack_size == 0) {
        return false;
    }

    else {

        *item = stack[first];

        first--;

        stack_size--;
        return true;
    }
}

bool stack_push(double item) {

    if (stack_size >= STACK_CAPACITY) {

        fprintf(stderr, ERROR_NO_SPACE, item);

        return false;

    }

    else {

        stack_size++;

        first++;

        stack[first] = item;

        return true;
    }
}
