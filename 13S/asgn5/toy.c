#include "ll.h"

#include <stdio.h>

#define MAGICN 457

int main() {
    LL *l = list_create();
    for (int i = 0; i < 1000; i++) {
        list_add(l, &i);
    }

    item i = MAGICN;
    list_remove(l, cmp, &i);

    int *ret;
    for (int i = 0; i < 1000; i++) {
        if (i != MAGICN) {
            ret = list_find(l, cmp, &i);
            if (ret == NULL) {
                printf("something went wrong. I inserted %d into the list but then could not find "
                       "it.\n",
                    i);
                return 1;
            }
        }
    }
    list_destroy(&l);
    return 0;
}
