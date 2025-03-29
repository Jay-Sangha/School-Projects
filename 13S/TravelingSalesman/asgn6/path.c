#include "path.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct path {
    uint32_t total_weight;
    Stack *vertices;
} Path;

Path *path_create(uint32_t capacity) {
    Path *p = (Path *) malloc(sizeof(Path));
    p->total_weight = 0;
    p->vertices = stack_create(capacity);

    return p;
}

void path_free(Path **pp) {
    if (*pp) {
        if ((*pp)->vertices) {
            free((*pp)->vertices);
            (*pp)->vertices = NULL;
        }

        free(*pp);
    }

    if (pp != NULL) {
        *pp = NULL;
    }

    return;
}

uint32_t path_vertices(const Path *p) {
    return stack_size(p->vertices);
}

uint32_t path_distance(const Path *p) {

    if (p == NULL || stack_size(p->vertices) < 2) {
        return 0;
    }

    else {
        return p->total_weight;
    }
}

void path_add(Path *p, uint32_t val, const Graph *g) {
    if (stack_full(p->vertices)) {
        return;
    } else {

        uint32_t i = 0;

        if (path_vertices(p) > 0) {
            stack_peek(p->vertices, &i);
            p->total_weight += graph_get_weight(g, i, val);
        }
        stack_push(p->vertices, val);
    }
}

uint32_t path_remove(Path *p, const Graph *g) {
    uint32_t i = 0;
    uint32_t x = 0;
    if (path_vertices(p) > 0) {

        stack_pop(p->vertices, &i);

        if (path_vertices(p) > 0) {
            stack_peek(p->vertices, &x);
            p->total_weight -= graph_get_weight(g, i, x);

        }

        else {
            p->total_weight = 0;
        }
    }

    return i;
}

void path_copy(Path *dst, const Path *src) {

    dst->total_weight = src->total_weight;
    stack_copy(dst->vertices, src->vertices);
    return;
}

void path_print(const Path *p, FILE *f, const Graph *g) {

    stack_print(p->vertices, f, graph_get_names(g));
}
