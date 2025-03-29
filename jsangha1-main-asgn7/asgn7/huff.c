#include "bitreader.h"
#include "bitwriter.h"
#include "node.h"
#include "pq.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct Code {
    uint64_t code;
    uint8_t code_len;
} Code;

void huff_write_tree(BitWriter *outbuf, Node *node) {
    if (node->left == NULL) {
        bit_write_bit(outbuf, 1);
        bit_write_uint8(outbuf, node->symbol);

    }

    else {
        huff_write_tree(outbuf, node->left);
        huff_write_tree(outbuf, node->right);
        bit_write_bit(outbuf, 0);
    }
}

uint32_t fill_histogram(FILE *fin, uint32_t *histogram) {
    uint32_t filesize = 0;

    for (int i = 0; i < 256; i++) {
        histogram[i] = 0;
    }

    ++histogram[0x00];
    ++histogram[0xff];

    int byte;

    while ((byte = fgetc(fin)) != EOF) {
        ++histogram[byte];
        ++filesize;
    }

    return filesize;
}

Node *create_tree(uint32_t *histogram, uint16_t *num_leaves) {
    PriorityQueue *q = pq_create();

    for (int i = 0; i < 256; i++) {
        if (histogram[i] != 0) {
            Node *n = node_create((uint8_t) i, histogram[i]);

            enqueue(q, n);
            (*num_leaves)++;
        }
    }

    while (!pq_size_is_1(q) && !pq_is_empty(q)) {
        Node *left = dequeue(q);
        Node *right = dequeue(q);
        Node *n2 = node_create(0, left->weight + right->weight);

        n2->left = left;
        n2->right = right;

        enqueue(q, n2);
    }

    Node *huff = dequeue(q);

    pq_free(&q);

    return huff;
}

void fill_code_table(Code *code_table, Node *node, uint64_t code, uint8_t code_length) {
    if (node->left != NULL && node->right != NULL) {
        fill_code_table(code_table, node->left, code, code_length + 1);
        code |= (uint64_t) 1 << code_length;
        fill_code_table(code_table, node->right, code, code_length + 1);

    }

    else {
        code_table[node->symbol].code = code;
        code_table[node->symbol].code_len = code_length;
    }
}

void huff_compress_file(BitWriter *outbuf, FILE *fin, uint32_t filesize, uint16_t num_leaves,
    Node *code_tree, Code *code_table) {
    bit_write_uint8(outbuf, 'H');
    bit_write_uint8(outbuf, 'C');
    bit_write_uint32(outbuf, filesize);
    bit_write_uint16(outbuf, num_leaves);
    huff_write_tree(outbuf, code_tree);

    while (true) {
        int b = fgetc(fin);

        if (b == EOF) {
            break;
        }

        uint64_t code = code_table[b].code;
        uint8_t code_length = code_table[b].code_len;

        for (int i = 0; i < code_length; ++i) {
            bit_write_bit(outbuf, (code & 1));
            code >>= 1;
        }
    }

    node_free(&code_tree);
}

int main(int argc, char *argv[]) {
    const char *in = argv[2];
    const char *out = argv[4];

    if (argc != 5) {
        return 0;
    }

    FILE *infile = fopen(in, "rb");

    BitWriter *outfile = bit_write_open(out);

    uint32_t hist[256];
    uint32_t size = fill_histogram(infile, hist);

    uint16_t leaves = 0;

    Node *tree = create_tree(hist, &leaves);

    Code table[256];

    fill_code_table(table, tree, 0, 0);

    if (fseek(infile, 0, 0) != 0) {
        fclose(infile);
        exit(1);
    }

    huff_compress_file(outfile, infile, size, leaves, tree, table);

    fclose(infile);

    bit_write_close(&outfile);

    return 0;
}
