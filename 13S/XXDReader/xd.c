#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE (size_t) 16

/**
vo
**/

int main(int arg1, char *arg2[]) {
    char buffer[BUFFER_SIZE];
    int offset = 0;
    int file = 0;
    size_t rbytes = 0;
    ssize_t reading = 0;

    if (arg1 > 1) {
        file = open(arg2[1], O_RDONLY, 0);

        if (file == -1) {
            exit(1);
        }

    }

    else {
        file = STDIN_FILENO;
    }

    while ((reading = read(file, buffer + rbytes, BUFFER_SIZE - rbytes)) > 0) {

        rbytes += (size_t) reading;

        if (rbytes == BUFFER_SIZE) {

            printf("%08x: ", offset);
            for (size_t i = 0; i < rbytes; i++) {
                printf("%02x", (unsigned char) buffer[i]);
                if (i % 2 == 1) {
                    printf(" ");
                }
            }

            for (size_t i = 0; i < BUFFER_SIZE; i++) {
                if (buffer[i] < 32 || buffer[i] >= 127) {
                    buffer[i] = '.';
                }
            }

            printf(" %.16s\n", buffer);

            rbytes = 0;

            offset += 16;
        }
    }

    if (rbytes > 0) {

        printf("%08x: ", offset);

        for (size_t i = 0; i < rbytes; ++i) {
            printf("%02x", (unsigned char) buffer[i]);
            if (i % 2 == 1) {
                printf(" ");
            }
        }

        if (rbytes < BUFFER_SIZE) {
            for (size_t i = rbytes; i < BUFFER_SIZE; ++i) {
                printf("  ");
                if (i % 2 == 1) {
                    printf(" ");
                }
            }
        }

        printf(" ");

        for (size_t i = 0; i < rbytes; i++) {
            if (buffer[i] >= 32 && buffer[i] <= 126)
                putchar(buffer[i]);
            else
                putchar('.');
        }

        printf("\n");
    }

    close(file);

    return 0;
}
