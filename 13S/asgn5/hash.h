#ifndef _HASH

#define _HASH

#include "ll.h"

typedef struct Hashtable Hashtable;

struct Hashtable {
    char *key;
    struct Hashtable *next;
    int val;
};

Hashtable *hash_create(void);

bool hash_put(Hashtable *, char *key, int val);

int *hash_get(Hashtable *, char *key);

void hash_destroy(Hashtable **);

#endif
