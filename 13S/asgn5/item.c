#include "item.h"

#include <stdio.h>
#include <string.h>

// implementation of the cmp() function on items, for when items are ints
bool cmp(item *a, item *b) {
    return strcmp(a->key, b->key) == 0;
}
