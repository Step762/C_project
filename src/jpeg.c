#include <stdio.h>
#include <stddef.h>
#include <math.h>

#include "jpeg.h"

#define PI 3.14159265358979323846

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

static double dct_coefficient(int index) {
    if (index == 0) {
        return 1.0 / sqrt(2.0);
    }

    return 1.0;
}

void forward_dct(double input[JPEG_BLOCK_SIZE][JPEG_BLOCK_SIZE],
                 double output[JPEG_BLOCK_SIZE][JPEG_BLOCK_SIZE]) {
    for (int v = 0; v < JPEG_BLOCK_SIZE; v++) {
        for (int u = 0; u < JPEG_BLOCK_SIZE; u++) {
            double sum = 0.0;

            for (int y = 0; y < JPEG_BLOCK_SIZE; y++) {
                for (int x = 0; x < JPEG_BLOCK_SIZE; x++) {
                    double pixel = input[y][x] - 128.0;

                    double cos_x = cos(((2 * x + 1) * u * PI) / 16.0);
                    double cos_y = cos(((2 * y + 1) * v * PI) / 16.0);

                    sum += pixel * cos_x * cos_y;
                }
            }

            output[v][u] = 0.25 * dct_coefficient(u) * dct_coefficient(v) * sum;
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

void print_dct_block(double block[JPEG_BLOCK_SIZE][JPEG_BLOCK_SIZE]) {
    printf("DCT coefficients for first block:\n");

    for (int y = 0; y < JPEG_BLOCK_SIZE; y++) {
        for (int x = 0; x < JPEG_BLOCK_SIZE; x++) {
            printf("%8.1f ", block[y][x]);
        }

        printf("\n");
    }
}