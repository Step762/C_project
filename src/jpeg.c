#include <stdio.h>
#include <stddef.h>

#include "jpeg.h"

int is_valid_jpeg_size(const Matrix *matrix) {
    if (matrix == NULL || matrix->data == NULL) {
        return 0;
    }

    if (matrix->width % JPEG_BLOCK_SIZE != 0) {
        return 0;
    }

    if (matrix->height % JPEG_BLOCK_SIZE != 0) {
        return 0;
    }

    return 1;
}

void print_jpeg_blocks_info(const Matrix *matrix) {
    if (matrix == NULL || matrix->data == NULL) {
        return;
    }

    int blocks_x = matrix->width / JPEG_BLOCK_SIZE;
    int blocks_y = matrix->height / JPEG_BLOCK_SIZE;
    int total_blocks = blocks_x * blocks_y;

    printf("JPEG block size: %d x %d\n", JPEG_BLOCK_SIZE, JPEG_BLOCK_SIZE);
    printf("Blocks by width: %d\n", blocks_x);
    printf("Blocks by height: %d\n", blocks_y);
    printf("Total blocks: %d\n", total_blocks);
}

void extract_block(const Matrix *matrix, int block_x, int block_y, double block[JPEG_BLOCK_SIZE][JPEG_BLOCK_SIZE]) {
    int start_x = block_x * JPEG_BLOCK_SIZE;
    int start_y = block_y * JPEG_BLOCK_SIZE;

    for (int y = 0; y < JPEG_BLOCK_SIZE; y++) {
        for (int x = 0; x < JPEG_BLOCK_SIZE; x++) {
            int image_x = start_x + x;
            int image_y = start_y + y;

            block[y][x] = matrix->data[image_y * matrix->width + image_x];
        }
    }
}

void print_block(double block[JPEG_BLOCK_SIZE][JPEG_BLOCK_SIZE]) {
    printf("First 8x8 block values:\n");

    for (int y = 0; y < JPEG_BLOCK_SIZE; y++) {
        for (int x = 0; x < JPEG_BLOCK_SIZE; x++) {
            printf("%6.1f ", block[y][x]);
        }

        printf("\n");
    }
}