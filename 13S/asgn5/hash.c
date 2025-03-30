#include "hash.h"

#include <stdlib.h>
#include <string.h>

#define buckets 200

Hashtable *hash_create(void) {

    Hashtable *htable = malloc(sizeof(Hashtable) * buckets);

    if (!htable) {
        return NULL;
    }

    for (int i = 0; i < buckets; ++i) {
        htable[i].key = NULL;
        htable[i].next = NULL;
    }

    return htable;
}

bool hash_put(Hashtable *htable, char *key, int val) {
    if (!htable || !key) {
        return false;
    }

    unsigned int hash = 0;

    for (char *i = key; *i; ++i) {
        hash += (unsigned char) *i;
    }

    int bindex = hash % buckets;
    Hashtable *n_node = malloc(sizeof(Hashtable));
    if (!n_node) {
        return false;
    }

    n_node->key = malloc(strlen(key) + 1);
    if (!n_node->key) {
        free(n_node);
        return false;
    }

    strcpy(n_node->key, key);
    n_node->val = val;

    Hashtable *active_node = &htable[bindex];

    while (active_node->next) {
        active_node = active_node->next;
    }

    active_node->next = n_node;
    n_node->next = NULL;

    return true;
}

void hash_destroy(Hashtable **htable) {
    if (!htable || !*htable) {
        return;
    }

    for (int i = 0; i < buckets; ++i) {
        Hashtable *active_node = (*htable)[i].next;

        while (active_node) {
            Hashtable *holder = active_node;
            active_node = active_node->next;

            free(holder->key);
            free(holder);
        }
    }

    free(*htable);

    *htable = NULL;
}

int *hash_get(Hashtable *htable, char *key) {
    if (!htable || !key) {
        return false;
    }

    unsigned int hash = 0;

    for (char *i = key; *i; ++i) {
        hash += (unsigned char) *i;
    }

    int bindex = hash % buckets;

    Hashtable *active_node = htable[bindex].next;

    while (active_node) {
        if (strcmp(active_node->key, key) == 0) {
            return &(active_node->val);
        }

        active_node = active_node->next;
    }
    return NULL;
}
