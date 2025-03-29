#include "bitreader.h"
#include "bitwriter.h"
#include "pq.h"

#include <stdio.h>
#include <stdlib.h>

Node *stack_pop(Node *stack[64], int *top) {
    if (*top < 0) {
        exit(1);
    }

    Node *hold = stack[*top];
    (*top)--;

    return hold;
}

void stack_push(Node *stack[64], Node *i, int *top) {
    if (*top >= 64) {
        exit(1);
    }

    (*top)++;
    stack[*top] = i;
}

void dehuff_decompress_file(FILE *fout, BitReader *inbuf) {
    uint8_t type1 = bit_read_uint8(inbuf);
    uint8_t type2 = bit_read_uint8(inbuf);
    uint32_t filesize = bit_read_uint32(inbuf);
    uint16_t num_leaves = bit_read_uint16(inbuf);

    Node *stack[64];
    int top = -1;

    assert(type1 == 'H');
    assert(type2 == 'C');

    uint16_t num_nodes = 2 * num_leaves - 1;
    Node *node = NULL;

    for (int i = 0; i < num_nodes; i++) {
        uint8_t bit = bit_read_bit(inbuf);

        if (bit == 1) {
            uint8_t symbol = bit_read_uint8(inbuf);
            node = node_create(symbol, 0);
        }

        else {
            node = node_create(0, 0);
            node->right = stack_pop(stack, &top);
            node->left = stack_pop(stack, &top);
        }

        stack_push(stack, node, &top);
    }

    Node *code_tree = stack_pop(stack, &top);

    for (uint32_t i = 0; i < filesize; i++) {
        node = code_tree;

        while (true) {
            uint8_t bit = bit_read_bit(inbuf);

            if (bit == 0) {
                node = node->left;
            }

            else {
                node = node->right;
            }

            if (node != NULL && node->right == NULL && node->left == NULL) {
                break;
            }
        }

        fputc(node->symbol, fout);
    }

    node_free(&code_tree);
}

int main(int argc, char *argv[]) {
    const char *in = argv[2];
    const char *out = argv[4];

    if (argc != 5) {
        return 0;
    }

    BitReader *infile = bit_read_open(in);

    FILE *outfile = fopen(out, "wb");

    if (infile == NULL || outfile == NULL) {
        return 1;
    }

    dehuff_decompress_file(outfile, infile);

    fclose(outfile);

    bit_read_close(&infile);

    return 0;
}
