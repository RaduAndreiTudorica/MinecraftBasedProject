# MinecraftBasedProject
*Second project pclp1*

Minecraft is an open-world video game where players can build structures in a world made of blocks (cubes) arranged in a three-dimensional grid.

Since Minecraft is trending again, an architect has decided to recreate the buildings they design in the game and needs your help: they want a program that automatically 
generates grand architectural structures.

We define a chunk as a 3D matrix of blocks (represented by char values), with each block having three coordinates:

x (from 0 to width - 1)

y (from 0 to height - 1)

z (from 0 to depth - 1)

An example chunk is illustrated in the image:
![image](https://github.com/user-attachments/assets/342a0590-6f18-4801-88ec-6ebab39ffb8f)

A chunk is represented by a vector (array) with width elements.
Each element chunk[x] represents a plane parallel to the yOz plane, and is a pointer to a vector with height elements.
Further, chunk[x][y] represents a row – within the chunk[x] plane – parallel to the Oz axis, and is a pointer to a vector with depth elements.
Finally, chunk[x][y][z] describes the block located at coordinates (x, y, z), according to the following encoding, for which we have symbolic constants:

0 = air (BLOCK_AIR)

1 = grass (BLOCK_GRASS)

2 = wood (BLOCK_WOOD)

3 = stone (BLOCK_STONE)

![image](https://github.com/user-attachments/assets/ee081a90-38ff-4f3c-9fa4-c72328888b63)


**Part A – Structure Generation**
For Part A, you will implement the functions defined in the file libchunk/chunk_gen.c.

Task 1 (4 points) – Add Block
  Implement the function chunk_place_block, which receives as parameters a chunk of dimensions width, height, and depth, three coordinates, and a block type:

    char*** chunk_place_block(
        char*** chunk, int width, int height, int depth,
        int x, int y, int z, char block);
  The function returns the chunk obtained by placing the specified block at the given coordinates.
  
![image](https://github.com/user-attachments/assets/aed10253-fc75-40ba-a699-e86c136e9ec6)


**Task 2 (6 points) – Fill Cuboid**
  Implement the function chunk_fill_cuboid:

    char*** chunk_fill_cuboid(
        char*** chunk, int width, int height, int depth,
        int x0, int y0, int z0, int x1, int y1, int z1, char block);
  The function receives the coordinates of two points – (x0, y0, z0) and (x1, y1, z1) – which represent opposite corners of a rectangular cuboid, along with a block type.

  It returns the chunk obtained by filling the cuboid with blocks of the specified type.

![image](https://github.com/user-attachments/assets/7a7698f9-0d1a-4823-b5eb-76e29e074a77)

**Task 3 (6 points) – Fill Sphere**
  Implement the function chunk_fill_sphere, which places blocks of a given type at all positions strictly inside the sphere defined by the given center and radius.
  The function should return the modified chunk.

    char*** chunk_fill_sphere(
        char*** chunk, int width, int height, int depth,
        int x, int y, int z, double radius, char block);
  For example, if the sphere has center (1, 1, 2) and radius 1.2, then the block at (1, 1, 3) will be modified by the function, because its distance from the center is 1.
  However, the block at (0, 0, 2) will remain unchanged, because its distance from the center is √2 ≈ 1.41, which is greater than 1.2.

  ![image](https://github.com/user-attachments/assets/87812ed8-617a-45a7-83c9-c006b3567300)

**Part B – Processing**
*To solve Part B, you will complete the functions found in the file libchunk/chunk_process.c.*

**Task 4 (6 points) – Shell Around a Block**
  You must implement the function chunk_shell:


    char*** chunk_shell(
        char*** chunk, int width, int height, int depth,
        char target_block, char shell_block);
  For every block of type target_block, the function builds a shell made of shell_block.
  The target_block itself is not replaced with the shell block, but other blocks may be overwritten if needed.

  The shell of a block includes:

  1.All direct neighbors (e.g., (x+1, y, z) or (x, y-1, z))

  2.All edge neighbors (e.g., (x+1, y, z+1) or (x, y-1, z-1))

  3.But not the corner neighbors (e.g., (x+1, y+1, z+1))

  ![image](https://github.com/user-attachments/assets/b0be6dfb-5fba-495d-a18d-ade593a535df)

**Task 5 (8 points) – Fill in the xOz Plane**
  You will implement the function chunk_fill_xz:

    char*** chunk_fill_xz(
        char*** chunk, int width, int height, int depth,
        int x, int y, int z, char block);
  If the position (x, y, z) contains a block of type B, then the function places block at all positions in the xOz plane that are reachable by moving one block at a time, 
  parallel to the Ox and Oz axes, starting from (x, y, z) and only passing through blocks of type B.

![image](https://github.com/user-attachments/assets/1f4fbf6a-7078-4b1e-baeb-21931d5fce23)

**Task 6 (10 points) – Fill in Space**
  You must implement the function chunk_fill, which has the same prototype as chunk_fill_xz:

    char*** chunk_fill(
        char*** chunk, int width, int height, int depth,
        int x, int y, int z, char block);
  The function places block at all positions in 3D space that are reachable by moving one block at a time along any axis, starting from (x, y, z), and only passing through 
  blocks that are identical to the one at the starting position.

![image](https://github.com/user-attachments/assets/56f91358-66a0-4a88-8098-e227a42d2b9d)


**Part C – Transformations**
*For Part C, you will work in the file libchunk/chunk_transform.c.*

**Task 7 (8 points) – Rotate Around the Oy Axis**
  You will implement the function chunk_rotate_y, which receives a chunk and returns the chunk rotated 90 degrees around the Oy axis (i.e., in the xOz plane).
  When viewed from above, the rotation is counterclockwise (i.e., trigonometric direction).

    char*** chunk_rotate_y(
        char*** chunk, int width, int height, int depth);

![image](https://github.com/user-attachments/assets/16ad60d0-078e-424e-9ec4-2e7cdf0421f9)

**Task 8 (12 points) – Gravity**
  For this task, you need to implement the function chunk_apply_gravity:

    char*** chunk_apply_gravity(
        char*** chunk, int width, int height, int depth, int* new_height);
  We define a body as a set of connected blocks of the same type (but different from air).
  In other words, if you apply the chunk_fill function on a coordinate containing a non-air block, it will fill exactly the blocks belonging to the same body as that block.

  The chunk_apply_gravity function causes all bodies present in the chunk to fall downward, until no body can move further — either because it reaches the bottom of the \
  chunk (y = 0), or because another body is directly beneath it.

  After applying gravity, there may be empty (air) planes at the top of the chunk — those that are entirely made of air and parallel to the xOz plane.
  These must be removed. For example, if the top 2 xOz planes consist only of air blocks, the returned chunk will exclude them, and the output parameter new_height will be 
  set to height - 2.

  Example  
  In the example below, there are seven bodies (four cubes, two rings, and one rod).
  After gravity is applied, the top 7 xOz planes will be removed, as they contain only air.

![image](https://github.com/user-attachments/assets/4e8bed75-6f91-4a64-b91e-e87fc94f4c21)

**Part D – Compression**
*For Part D, you will complete the two functions in the file libchunk/chunk_compress.c.*

**Task 9 (10 points) – Storing a Chunk**
  In the 3D matrix representation, large chunks consume significant amounts of memory.
  For example, a chunk of dimensions 20 × 20 × 20 requires storing the block type for 8,000 blocks.

  We can reduce memory usage by noting that adjacent blocks are often identical.
  Instead of storing the same block type multiple times, we will keep only: the block type, and the number of consecutive occurrences.

  This follows the lossless compression scheme called Run-Length Encoding (RLE).

  To begin, we will flatten the 3D matrix by assigning a unique number to each block, ranging from 0 to width × height × depth - 1, based on the block’s position in the 
  order: y, then z, then x.

  An example is illustrated below (layers separated for clarity).

  ![image](https://github.com/user-attachments/assets/e36b892a-536e-45f5-b930-b1e41acce1fa)


  While traversing the blocks in the specified order, we count consecutive occurrences (called runs) of the same block type.
  In the previous example, there are 5 runs:

    (grass, 4)

    (stone, 5)

    (air, 8)

    (grass, 1)

    (air, 9)

  We want to store this information in an unsigned char array (each unsigned char has 8 bits), encoding the runs one after another, so that we can later reconstruct the 
  chunk.

  If a block is one of the four possible block types, then it corresponds to a number from 0 to 3 — which means it can be represented using 2 bits: b1b0 (the binary digits 
  of the block number).

  Also, the run length n (i.e., the number of consecutive blocks of that type) can be represented in binary using q bits.

  Thus, we store each run in the form (block, n) using the following bit format:

  bb = the 2 bits encoding the block type

  nn...n = the bits encoding the run length
  
![{75B86E6E-FE29-485A-9160-36EB44E26DBA}](https://github.com/user-attachments/assets/628dbe4a-3a8d-4a1a-b234-5b2134cf00f7)

  If 32 ≤ n < 4096, the most significant 4 bits of n are stored in the first unsigned char, and the remaining bits in the second unsigned char.

  If a run has n ≥ 4096, it is split into: a first run with n₁ = 4095, and a second run with n₂ = n - 4095, and the process repeats recursively for the remaining count.

![image](https://github.com/user-attachments/assets/4c8093c2-e028-4f1c-b31e-d642d5dc1153)

  A chunk described by a single run (wood, 8192) is represented by a vector of five unsigned char values.

![image](https://github.com/user-attachments/assets/7cfb70ec-cdb0-421a-9f31-2ad29cfe20c0)

  For this task, you must implement the function chunk_encode:

    unsigned char* chunk_encode(
        char*** chunk, int width, int height, int depth,
        int* length);

**Task 10 (10 points) – Recovering a Chunk**
  Once chunks have been encoded using RLE (Run-Length Encoding), they occupy less memory.
  However, if we want to visualize them or apply operations to them, they must first be converted back to their 3D matrix representation.

  Function to implement:

    char*** chunk_decode(
        unsigned char* code, int width, int height, int depth);

**Task 11 (10 points) – Valgrind**
  For this task, you must:

  Have full points on Tasks 7, 8, 9, and 10

  Have no memory leaks when those tasks are checked using Valgrind

  Valgrind will be run with the following command:
  
    valgrind --tool=memcheck --leak-check=full --error-exitcode=1 <program>
