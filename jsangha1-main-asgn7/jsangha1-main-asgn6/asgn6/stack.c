#include "stack.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct stack {
    uint32_t capacity;
    uint32_t top;
    uint32_t *items;
} Stacks;

Stack *stack_create(uint32_t capacity) {
    Stack *s = (Stack *) malloc(sizeof(Stack));
    s->capacity = capacity;
    s->top = 0;
    s->items = calloc(s->capacity, sizeof(uint32_t));
    // We created our stack, return it!
    return s;
}

void stack_free(Stack **sp) {
    // sp is a double pointer, so we have to check if it, // or the pointer it points to is null.
    if (sp != NULL && *sp != NULL) {
        // memory for the array of items first,
        // as that was also dynamically allocated!
        // not be able to access the array to free it.
        if ((*sp)->items) {
            free((*sp)->items);
            (*sp)->items = NULL;
        }

        // Free memory allocated for the stack
        free(*sp);
    }
    if (sp != NULL) {
        // Set the pointer to null! This ensures we dont ever do a double free!
        *sp = NULL;
    }
}

bool stack_push(Stack *s, uint32_t val) {
    if (stack_full(s)) {
        return false;
    }
    // Set val
    s->items[s->top] = val;
    // Move the top of the stack
    s->top++;
    return true;
}

bool stack_pop(Stack *s, uint32_t *val) {
    if (stack_empty(s)) {
        return false;
    }

    s->top--;
    *val = s->items[(s->top)];
    return true;
}

bool stack_peek(const Stack *s, uint32_t *val) {
    if (!stack_empty(s)) {
        *val = s->items[s->top - 1];
        return true;
    }

    return false;
}

bool stack_empty(const Stack *s) {
    if (s->top == 0) {
        return true;
    }

    return false;
}

bool stack_full(const Stack *s) {
    if (s->top == s->capacity) {
        return true;
    }

    return false;
}

uint32_t stack_size(const Stack *s) {
    return s->top;
}

void stack_copy(Stack *dst, const Stack *src) {
    for (uint32_t i = 0; i < src->capacity; ++i) {
        dst->items[i] = src->items[i];
    }

    dst->top = src->top;

    return;
}

void stack_print(const Stack *s, FILE *f, char *vals[]) {
    for (uint32_t i = 0; i < s->top; i += 1) {
        fprintf(f, "%s\n", vals[s->items[i]]);
    }
}
