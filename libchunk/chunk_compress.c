#include "chunk.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


enum {
    MAX_SHORT_RUN = 32,
    MAX_LONG_RUN = 4096,
    CAPACITY = 16,
    FIVE_BITS_SIFT = 5,
    SIX_BITS_SIFT = 6,
    EIGHT_BITS_SIFT = 8,
    MASK1 = 0x20,
    MASK2 = 0xF,
    MASK3 = 0xFF,
    MASK4 = 0x1F
};

unsigned char* chunk_encode(
     char*** chunk, int width, int height, int depth, int* length) {
    int size = width * height * depth;
    int capacity = CAPACITY;
    char *run = malloc(size * sizeof(char));
    unsigned char *compressed = malloc(capacity * sizeof(unsigned char));

    int k = 0;
    for (int y = 0; y < height; y++) {
        for (int z = 0; z < depth; z++) {
            for (int x = 0; x < width; x++) {
                run[k++] = chunk[x][y][z];
            }
        }
    }

    int i = 0;
    int index = 0;
    while (i < size) {
        int run_length = 0;
        while (i + run_length < size && run[i + run_length] == run[i]) {
            run_length++;
        }

        if (index + 1 >= capacity) {
            capacity *= 2;
            unsigned char* temp = realloc(compressed, capacity);
            if (!temp) {
                free(run);
                free(compressed);
                return NULL;
            }
            compressed = temp;
        }

        if (run_length < MAX_SHORT_RUN) {
            compressed[index++] = run[i] << SIX_BITS_SIFT | run_length;
        } else if (run_length < MAX_LONG_RUN) {
            compressed[index++] = run[i] << SIX_BITS_SIFT | MASK1 | run_length >> EIGHT_BITS_SIFT;
            compressed[index++] = run_length & MASK3;
        } else {
            int aux = run_length;
            while (aux >= MAX_LONG_RUN) {
                compressed[index++] = run[i] << SIX_BITS_SIFT | MASK1 | MASK2;
                compressed[index++] = MASK3;
                aux -= MAX_LONG_RUN - 1;
            }
            if (aux < MAX_SHORT_RUN) {
                compressed[index++] = run[i] << SIX_BITS_SIFT | aux;
            } else {
                compressed[index++] = run[i] << SIX_BITS_SIFT | MASK1 | aux >> EIGHT_BITS_SIFT;
                compressed[index++] = aux & MASK3;
            }
        }
        i += run_length;
    }
    free(run);
    *length = index;
    return compressed;
}

void convert_to_int_array(unsigned char* code, int code_size, unsigned char* output, int* output_size) {
    int bits_per_int = 1;
    *output_size = code_size / bits_per_int;

    for (int i = 0; i < *output_size; i++) {
        int value = 0;

        for (int j = 0; j < bits_per_int; j++) {
            value = (value << 1) | code[i * bits_per_int + j];
        }
        output[i] = value;
    }
}

char*** chunk_decode(
    unsigned char* code, int width, int height, int depth) {

    char*** chunk = malloc(width * sizeof(char**));
    for (int x = 0; x < width; x++) {
        chunk[x] = malloc(height * sizeof(char*));
        for (int y = height - 1; y >= 0; y--) {
            chunk[x][y] = malloc(depth * sizeof(char));
        }
    }

    int total_blocks = width * height * depth, blocks_down = 0;
    int i = 0, index = 0;
    char *run = malloc((total_blocks + 1) * sizeof(char));
    while (blocks_down < total_blocks) {
        bool choice = (code[i] & MASK1) >> FIVE_BITS_SIFT;
        char block_type = (char)(code[i] >> SIX_BITS_SIFT);
        int blocks_number = 0;
        if (choice == false) {
            blocks_number = (int)code[i] & MASK4;
            i++;
        } else if (choice == true) {
            blocks_number = ((int)code[i] & MASK2) << EIGHT_BITS_SIFT | code[i + 1];
            i += 2;
        }
        blocks_down += blocks_number;
        while (blocks_number--) {
            run[index++] = block_type;
        }
    }
    i = 0;
    for (int y = 0; y < height; y++) {
        for (int z = 0; z < depth; z++) {
            for (int x = 0; x < width; x++) {
                chunk[x][y][z] = run[i++];
            }
        }
    }
    free(run);
    return chunk;
}
