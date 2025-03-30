#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 256

#include "hash.h"

int main() {
    char buf[BUFFER_SIZE];

    Hashtable *table = hash_create();
    size_t unique = 0;
    size_t len = 0;

    while (!feof(stdin) && !ferror(stdin)) {

        if (fgets(buf, sizeof(buf), stdin) == NULL) {
            break;
        }

        len = strlen(buf);

        if (len > 0 && buf[len - 1] == '\n') {
            buf[len - 1] = '\0';
            --len;
        }

        if (hash_get(table, buf) == NULL) {
            hash_put(table, buf, 0);
            ++unique;
        }
    }

    printf("%zu\n", unique);

    hash_destroy(&table);

    return 0;
}
