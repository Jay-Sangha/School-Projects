#include "ll.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

LL *list_create() {
    LL *l = (LL *) malloc(sizeof(LL));
    if (l == NULL) {
        return NULL;
    }
    l->head = NULL;
    l->tail = NULL;
    return l;
}

bool list_add(LL *l, item *i) {
    Node *n = (Node *) malloc(sizeof(Node));
    if (n == NULL) {
        return false;
    }
    n->data = *i;
    n->next = NULL;
    if (l->head == NULL) {
        l->head = n;
        l->tail = n;

    } else {
        l->tail->next = n;
    }

    l->tail = n;

    return true;
}

item *list_find(LL *l, bool (*cmp)(item *, item *), item *i) {
    Node *n = l->head;
    while (n != NULL) {
        if (cmp(&n->data, i)) {
            return &n->data;
        }
        n = n->next;
    }
    return NULL;
}

void list_remove(LL *l, bool (*cmp)(item *, item *), item *i) {

    Node *active = l->head;
    Node *last = NULL;

    while (active != NULL) {
        if (cmp(&active->data, i)) {
            if (last != NULL) {
                last->next = active->next;
            } else {
                l->head = active->next;
            }

            if (active == l->tail) {
                l->tail = last;
            }

            free(active);
            break;
        }

        last = active;
        active = active->next;
    }
}

void list_destroy(LL **l) {

    if (*l == NULL) {
        return;
    }

    Node *next;
    Node *active = (*l)->head;

    while (active != NULL) {
        next = active->next;
        free(active);
        active = next;
    }

    (*l)->head = NULL;
    free(*l);

    *l = NULL;
}
