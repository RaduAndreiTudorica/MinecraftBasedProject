#include "chunk.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

static const int TOTAL_DIRECTIONS = 6;
static const int TEN = 10;
char*** chunk_rotate_y(
    char*** chunk, int width, int height, int depth) {
    int new_width = depth;
    int new_height = height;
    int new_depth = width;

    char ***new_chunk = malloc(new_width * sizeof(char**));
    for (int x = 0; x < new_width; x++) {
        new_chunk[x] = malloc(new_height * sizeof(char*));
        for (int y = 0; y < new_height; y++) {
            new_chunk[x][y] = malloc(new_depth * sizeof(char));
        }
    }
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            for (int z = 0 ; z < depth; z++) {
                new_chunk[depth - 1 - z][y][x] = chunk[x][y][z];
            }
        }
    }

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            free(chunk[x][y]);
        }
        free(chunk[x]);
    }
    free(chunk);

    return new_chunk;
}

int verification(int x, int y, int z, int width, int height, int depth) {
    return x < 0 || x >= width || y < 0 || y >= height || z < 0 || z >= depth;
}

int drop(char ***chunk, int x, int y, int z) {
    int new_y = y - 1;
    int aux = 0;
    while (new_y >= 0 && chunk[x][new_y][z] == 0) {
        new_y--;
        aux++;
    }
    return aux;
}

int *realloc_vect(int *vect, int *capacity) {
    int new_capacity = (*capacity) * 2;
    int *new_vect = realloc(vect, new_capacity * sizeof(int));
    if (new_vect == NULL) {
        free(vect);
        exit(EXIT_FAILURE);
    }
    *capacity = new_capacity;
    return new_vect;
}

void fill_object(char ***chunk, bool ***visited, int **total_drops, int *index, int *capacity,
                 int width, int height, int depth, int x, int y, int z, char initial_block, int object_type) {
    if (verification(x, y, z, width, height, depth) || chunk[x][y][z] == 0 || visited[x][y][z]) {
        return;
    }
    if (object_type == 2 && chunk[x][y][z] != initial_block) {
        return;
    }
    visited[x][y][z] = true;

    if (*index >= *capacity) {
        *total_drops = realloc_vect(*total_drops, capacity);
    }

    if (object_type == 1) {
        if (y && chunk[x][y - 1][z]) {
            (*total_drops)[(*index)++] = INT_MAX;

        } else {
            (*total_drops)[(*index)++] = drop(chunk, x, y, z);
        }
    } else if (object_type == 2) {
        if (y && chunk[x][y - 1][z] == initial_block) {
            (*total_drops)[(*index)++] = INT_MAX;
        } else {
            (*total_drops)[(*index)++] = drop(chunk, x, y, z);
        }
    }

    const int directions[6][3] = {{1, 0, 0}, {-1, 0, 0}, {0, 1, 0}, {0, -1, 0}, {0, 0, 1}, {0, 0, -1}};
    for (int i = 0; i < TOTAL_DIRECTIONS; i++) {
        fill_object(chunk, visited, total_drops, index, capacity, width, height, depth, x + directions[i][0],
            y + directions[i][1], z + directions[i][2], initial_block, object_type);
    }
}

int compare_ints(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

void chunk_copy(char ***chunk, char ***new_chunk, int width, int height, int depth) {
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            for (int z = 0; z < depth; z++) {
                chunk[x][y][z] = new_chunk[x][y][z];
            }
        }
    }
}

void chunk_reset(char ***chunk, int width, int height, int depth) {
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            for (int z = 0; z < depth; z++) {
                chunk[x][y][z] = 0;
            }
        }
    }
}

void fall(char ***chunk, char ***new_chunk, bool ***visited, int width, int height, int depth,
    int x, int y, int z, int fall_length) {
    if (verification(x , y, z, width, height, depth) || chunk[x][y][z] == 0 || visited[x][y][z] == false) {
        return;
    }
    visited[x][y][z] = false;


    if (fall_length <= y) {
        new_chunk[x][y - fall_length][z] = chunk[x][y][z];
    }

    const int directions[6][3] = {{1, 0, 0}, {-1, 0, 0}, {0, 1, 0}, {0, -1, 0}, {0, 0, 1}, {0, 0, -1}};
    for (int i = 0; i < TOTAL_DIRECTIONS; i++) {
        fall(chunk, new_chunk, visited, width, height, depth, x + directions[i][0],
            y + directions[i][1], z + directions[i][2], fall_length);
    }
}

void revisit(char ***chunk, bool ***visited, int width, int height, int depth, int x, int y, int z) {
    if (verification(x, y, z, width, height, depth) || chunk[x][y][z] == 0 || visited[x][y][z]) {
        return;
    }
    visited[x][y][z] = true;
    const int directions[6][3] = {{1, 0, 0}, {-1, 0, 0}, {0, 1, 0}, {0, -1, 0}, {0, 0, 1}, {0, 0, -1}};
    for (int i = 0; i < TOTAL_DIRECTIONS; i++) {
        revisit(chunk, visited , width, height, depth, x + directions[i][0],
            y + directions[i][1], z + directions[i][2]);
    }
}

int new_height_calc(char ***chunk, int width, int height, int depth) {
    int y = 0;
    for (y = height - 1; y >= 0; y--) {
        for (int x = 0; x < width; x++) {
            for (int z = 0; z < depth; z++) {
                if (chunk[x][y][z]) {
                    return y + 1;
                }
            }
        }
    }
    return 0;
}

char  ***realoc_chunk(char ***chunk, int width, int height, int depth) {
    char ***new_chunk = malloc(width * sizeof *new_chunk);
    for (int x = 0; x < width; x++) {
        new_chunk[x] = malloc(height * sizeof **new_chunk);
        for (int y = 0; y < height; y++) {
            new_chunk[x][y] = malloc(depth * sizeof ***new_chunk);
            for (int z = 0; z < depth; z++) {
                new_chunk[x][y][z] = chunk[x][y][z];
            }
        }
    }
    return new_chunk;
}

char*** chunk_apply_gravity(
    char*** chunk, int width, int height, int depth, int* new_height) {

    int capacity = TEN;
    int *total_drops = malloc(capacity * sizeof(int));
    if (!total_drops) {
        exit(EXIT_FAILURE);
    }


    char ***new_chunk = malloc(width * sizeof *new_chunk);
    for (int x = 0; x < width; x++) {
        new_chunk[x] = malloc(height * sizeof **new_chunk);
        for (int y = 0; y < height; y++) {
            new_chunk[x][y] = malloc(depth * sizeof ***new_chunk);
            for (int z = 0; z < depth; z++) {
                new_chunk[x][y][z] = 0;
            }
        }
    }

    bool ***visited = malloc(width * sizeof(*visited));
    for (int x = 0; x < width; x++) {
        visited[x] = malloc(height * sizeof(**visited));
        for (int y = 0; y < height; y++) {
            visited[x][y] = calloc(depth, sizeof(***visited));
        }
    }

    int object_type = 1;
    while (object_type <= 2) {
        int ok = 1;
        while (ok) {
            ok = 0;
            for (int y = 0; y < height; y++) {
                for (int z = 0; z < depth; z++) {
                    for (int x = 0; x < width; x++) {
                        if (!visited[x][y][z] && chunk[x][y][z] != 0) {
                            int index = 0;
                            char initial_block = chunk[x][y][z];
                            fill_object(chunk, visited, &total_drops, &index, &capacity,
                                        width, height, depth, x, y, z, initial_block, object_type);
                            qsort(total_drops, index, sizeof(int), compare_ints);
                            if (total_drops[0]) {
                                ok = 1;
                            }
                            fall(chunk, new_chunk, visited, width, height,
                                depth, x, y, z, total_drops[0]);
                        }
                    }
                }
            }
            chunk_copy(chunk, new_chunk, width, height, depth);
            chunk_reset(new_chunk, width, height, depth);
        }
        object_type++;
    }

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            free(visited[x][y]);
        }
        free(visited[x]);
    }
    free(visited);

    free(total_drops);

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            free(new_chunk[x][y]);
        }
        free(new_chunk[x]);
    }
    free(new_chunk);

    *new_height = new_height_calc(chunk, width, height, depth);

    char ***reallocated_chunk = realoc_chunk(chunk, width, *new_height, depth);

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            free(chunk[x][y]);
        }
        free(chunk[x]);
    }
    free(chunk);

    return reallocated_chunk;
}
