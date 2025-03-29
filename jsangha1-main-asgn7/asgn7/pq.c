#include "pq.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct ListElement ListElement;

struct ListElement {
    Node *tree;
    ListElement *next;
};

struct PriorityQueue {
    ListElement *list;
};

PriorityQueue *pq_create(void) {
    PriorityQueue *pq = calloc(1, sizeof(PriorityQueue));

    if (pq == NULL) {
        return NULL;
    }

    else {
        return pq;
    }
}

void pq_free(PriorityQueue **q) {

    free(*q);
    *q = NULL;
}

bool pq_is_empty(PriorityQueue *q) {
    if (q->list == NULL) {
        return true;

    }

    else {
        return false;
    }
}

bool pq_size_is_1(PriorityQueue *q) {
    if (pq_is_empty(q)) {
        return false;
    }
    if (q->list->next == NULL) {
        return true;
    }

    return false;
}

bool pq_less_than(ListElement *e1, ListElement *e2) {
    if (e1->tree->weight < e2->tree->weight) {
        return true;
    }

    else if (e1->tree->weight > e2->tree->weight) {
        return false;
    }

    return e1->tree->symbol < e2->tree->symbol;
}

void enqueue(PriorityQueue *q, Node *tree) {
    ListElement *e = calloc(1, sizeof(ListElement));

    if (e == NULL) {
        return;
    }

    e->tree = tree;
    e->next = NULL;

    if (pq_is_empty(q)) {
        q->list = e;

    }

    else if (pq_less_than(e, q->list)) {
        e->next = q->list;
        q->list = e;

    }

    else {
        ListElement *hold = q->list;

        while (hold->next != NULL && !pq_less_than(e, hold->next)) {
            hold = hold->next;
        }

        e->next = hold->next;
        hold->next = e;
    }
}

Node *dequeue(PriorityQueue *q) {
    ListElement *i = q->list;
    Node *s = i->tree;

    q->list = i->next;

    free(i);

    return s;
}

void pq_print(PriorityQueue *q) {
    assert(q != NULL);
    ListElement *e = q->list;
    int position = 1;

    while (e != NULL) {
        if (position++ == 1) {
            printf("=============================================\n");
        }

        else {
            printf("---------------------------------------------\n");
        }

        node_print_tree(e->tree);
        e = e->next;
    }

    printf("=============================================\n");
}
