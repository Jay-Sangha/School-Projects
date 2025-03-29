#include "bitwriter.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct BitWriter {
    FILE *underlying_stream;
    uint8_t byte;
    uint8_t bit_pos;
};

BitWriter *bit_write_open(const char *filename) {
    BitWriter *buf = calloc(1, sizeof(BitWriter));
    FILE *f = fopen(filename, "wb");
    if (f == NULL) {
        free(buf);
        return NULL;
    }

    else {
        buf->underlying_stream = f;
        buf->byte = 0;
        buf->bit_pos = 0;

        return buf;
    }
}

void bit_write_close(BitWriter **pbuf) {
    if (*pbuf != NULL) {
        if ((*pbuf)->bit_pos > 0) {
            fputc((*pbuf)->byte, (*pbuf)->underlying_stream);
        }
        fclose((*pbuf)->underlying_stream);
        free(*pbuf);
        *pbuf = NULL;
    }
}

void bit_write_bit(BitWriter *buf, uint8_t bit) {
    if (buf->bit_pos > 7) {
        fputc(buf->byte, buf->underlying_stream);
        buf->byte = 0;
        buf->bit_pos = 0;
    }

    buf->byte |= (bit & 1) << buf->bit_pos;
    buf->bit_pos += 1;
}

void bit_write_uint8(BitWriter *buf, uint8_t x) {
    for (int i = 0; i < 8; i++) {
        bit_write_bit(buf, (x >> i) & 1);
    }
}

void bit_write_uint16(BitWriter *buf, uint16_t x) {
    for (int i = 0; i < 16; i++) {
        bit_write_bit(buf, (x >> i) & 1);
    }
}

void bit_write_uint32(BitWriter *buf, uint32_t x) {
    for (int i = 0; i < 32; i++) {
        bit_write_bit(buf, (x >> i) & 1);
    }
}
