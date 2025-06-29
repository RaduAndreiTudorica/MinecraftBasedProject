#include "chunk.h"
#include <math.h>

char*** chunk_place_block(
    char*** chunk, int width, int height, int depth,
    int x, int y, int z, char block) {
    if (x > 0 && x < width && y > 0 && y < height &&
        z > 0 && z < depth) {
        chunk[x][y][z] = block;
    }
    return chunk;
}

int minim(int a, int b) {
    return a < b ? a : b;
}

int maxim(int a, int b) {
    return a > b ? a : b;
}

char*** chunk_fill_cuboid(
    char*** chunk, int width, int height, int depth,
    int x0, int y0, int z0, int x1, int y1, int z1, char block) {
    int min_x = minim(x0, x1), max_x = maxim(x0, x1);
    int min_y = minim(y0, y1), max_y = maxim(y0, y1);
    int min_z = minim(z0, z1), max_z = maxim(z0, z1);


    for (int x = min_x; x <= max_x; x++) {
        if (x < 0 || x >= width) continue;
        for (int y = min_y; y <= max_y; y++) {
            if (y < 0 || y >= height) continue;
            for (int z = min_z; z <= max_z; z++) {
                if (z < 0 || z >= depth) continue;
                chunk[x][y][z] = block;
            }
        }
    }
    return chunk;
}

double dist(int x0, int y0, int z0, int x1, int y1, int z1) {
    return sqrt(pow(x0 - x1, 2) + pow(y0 - y1, 2) +
        pow(z0 - z1, 2));
}

char*** chunk_fill_sphere(
    char*** chunk, int width, int height, int depth,
    int x, int y, int z, double radius, char block) {

    for (int i_x = 0; i_x < width; i_x++) {
        for (int i_y = 0; i_y < height; i_y++) {
            for (int i_z = 0; i_z < depth; i_z++) {
                if (dist(x, y, z, i_x, i_y, i_z) < radius) {
                    chunk[i_x][i_y][i_z] = block;
                }
            }
        }
    }
    return chunk;
}
