#include "bitreader.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct BitReader {
    FILE *underlying_stream;
    uint8_t byte;
    uint8_t bit_pos;
};

BitReader *bit_read_open(const char *filename) {
    BitReader *reader = calloc(1, sizeof(BitReader));

    FILE *f = fopen(filename, "rb");

    if (f == NULL) {
        free(reader);
        return NULL;
    }

    reader->underlying_stream = f;
    reader->byte = 0;
    reader->bit_pos = 8;

    return reader;
}

void bit_read_close(BitReader **pbuf) {
    if (*pbuf != NULL) {
        fclose((*pbuf)->underlying_stream);
        free(*pbuf);
        *pbuf = NULL;
    }
}

uint8_t bit_read_bit(BitReader *buf) {
    if (buf->bit_pos > 7) {
        int hold = fgetc(buf->underlying_stream);

        buf->byte = (uint8_t) hold;
        buf->bit_pos = 0;
    }

    uint8_t val = (buf->byte >> buf->bit_pos) & 1;

    buf->bit_pos += 1;

    return val;
}

uint8_t bit_read_uint8(BitReader *buf) {
    uint8_t byte = 0x00;

    for (int i = 0; i < 8; i++) {
        uint8_t bit = bit_read_bit(buf);
        byte |= (bit << i);
    }

    return byte;
}

uint16_t bit_read_uint16(BitReader *buf) {
    uint16_t word = 0x0000;

    for (int i = 0; i < 16; i++) {
        uint16_t bit = bit_read_bit(buf);
        word |= (bit << i);
    }
    return word;
}

uint32_t bit_read_uint32(BitReader *buf) {
    uint32_t word = 0x00000000;
    for (int i = 0; i < 32; i++) {
        uint32_t bit = bit_read_bit(buf);
        word |= (bit << i);
    }

    return word;
}
