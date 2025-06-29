#include <stdlib.h>
#include "chunk.h"


static const int MAX_NEIGHBOURS = 18;

char*** chunk_shell(
    char*** chunk, int width, int height, int depth,
    char target_block, char shell_block) {

    char ***new_chunk = malloc(width * sizeof(char**));
    if (chunk == NULL) {
        exit(1);
    }
    for (int x = 0; x < width; x++) {
        new_chunk[x] = malloc(height * sizeof(char*));
        if (new_chunk[x] == NULL) {
            for (int i = 0; i < x; i++) {
                free(new_chunk[i]);
            }
            free(new_chunk);
            exit(1);
        }
        for (int y = 0; y < height; y++) {
            new_chunk[x][y] = malloc(depth * sizeof(char));
            if (new_chunk[x][y] == NULL) {
                for (int i = 0; i < x; i++) {
                    for (int j = 0; j < y; j++) {
                        free(new_chunk[i][j]);
                    }
                    free(new_chunk[i]);
                }
                free(new_chunk);
            }
            for (int z = 0; z < depth; z++) {
                new_chunk[x][y][z] = chunk[x][y][z];
            }
        }
    }

    int neighbours[][3]={
        { 1,  0,  0}, {-1,  0,  0},
        { 0,  1,  0}, { 0, -1,  0},
        { 0,  0,  1}, { 0,  0, -1},
        { 1,  1,  0}, {-1, -1,  0},
        { 1, -1,  0}, {-1,  1,  0},
        { 1,  0,  1}, {-1,  0, -1},
        { 1,  0, -1}, {-1,  0,  1},
        { 0,  1,  1}, { 0, -1, -1},
        { 0,  1, -1}, { 0, -1,  1}
    };

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            for (int z = 0; z < depth; z++) {
                if (chunk[x][y][z] == target_block) {
                    for (int i = 0; i < MAX_NEIGHBOURS; i++) {
                        int new_x = x + neighbours[i][0];
                        int new_y = y + neighbours[i][1];
                        int new_z = z + neighbours[i][2];

                        if (new_x >= 0 && new_y >= 0 && new_z >= 0
                            && new_x < width && new_y < height && new_z < depth) {
                            if (new_chunk[new_x][new_y][new_z] != target_block) {
                                new_chunk[new_x][new_y][new_z] = shell_block;
                            }
                        }
                    }
                }
            }
        }
    }
    return new_chunk;
}

int verify(int x, int y, int z, int width, int height, int depth) {
    return x < 0 || x >= width || y < 0 || y >= height ||
        z < 0 || z >= depth;
}

void fill(char*** chunk, int width, int height, int depth,
    int x, int y, int z, char block, char initial_block) {
    if (verify(x, y, z, width, height, depth)) {
        return;
    }

    if (chunk[x][y][z] == initial_block) {
        chunk[x][y][z] = block;
        fill(chunk, width, height, depth, x - 1, y, z, block, initial_block);
        fill(chunk, width, height, depth, x + 1, y, z, block, initial_block);
        fill(chunk, width, height, depth, x, y, z - 1, block, initial_block);
        fill(chunk, width, height, depth, x, y, z + 1, block, initial_block);
    }
}

char*** chunk_fill_xz(
    char*** chunk, int width, int height, int depth,
    int x, int y, int z, char block) {

    if (verify(x, y, z, width, height, depth)) {
        return chunk;
    }
    char initial_block = chunk[x][y][z];

    fill(chunk, width, height, depth, x, y, z, block, initial_block);

    return chunk;
}

void fill_space(char*** chunk, int width, int height, int depth,
    int x, int y, int z, char block, char initial_block) {

    if (verify(x, y, z, width, height, depth)) {
        return;
    }
    if (chunk[x][y][z] == initial_block) {
        chunk[x][y][z] = block;
        fill_space(chunk, width, height, depth, x + 1, y, z, block, initial_block);
        fill_space(chunk, width, height, depth, x - 1, y, z, block, initial_block);
        fill_space(chunk, width, height, depth, x, y + 1, z, block, initial_block);
        fill_space(chunk, width, height, depth, x, y - 1, z, block, initial_block);
        fill_space(chunk, width, height, depth, x, y, z + 1, block, initial_block);
        fill_space(chunk, width, height, depth, x, y, z - 1, block, initial_block);
    }
}

char*** chunk_fill(
    char*** chunk, int width, int height, int depth,
    int x, int y, int z, char block) {

    if (verify(x, y, z, width, height, depth)) {
        return chunk;
       }

    char initial_block = chunk[x][y][z];

    fill_space(chunk, width, height, depth, x, y, z, block, initial_block);

    return chunk;
}
