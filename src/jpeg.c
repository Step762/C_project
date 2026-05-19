#include <stdio.h>
#include <stddef.h>
#include <math.h>

#include "jpeg.h"

#define PI 3.14159265358979323846

static int quantization_table[JPEG_BLOCK_SIZE][JPEG_BLOCK_SIZE] = {
    {16, 11, 10, 16, 24, 40, 51, 61},
    {12, 12, 14, 19, 26, 58, 60, 55},
    {14, 13, 16, 24, 40, 57, 69, 56},
    {14, 17, 22, 29, 51, 87, 80, 62},
    {18, 22, 37, 56, 68, 109, 103, 77},
    {24, 35, 55, 64, 81, 104, 113, 92},
    {49, 64, 78, 87, 103, 121, 120, 101},
    {72, 92, 95, 98, 112, 100, 103, 99}
};

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

static void insert_block(Matrix *matrix, int block_x, int block_y, double block[JPEG_BLOCK_SIZE][JPEG_BLOCK_SIZE]) {
    int start_x = block_x * JPEG_BLOCK_SIZE;
    int start_y = block_y * JPEG_BLOCK_SIZE;

    for (int y = 0; y < JPEG_BLOCK_SIZE; y++) {
        for (int x = 0; x < JPEG_BLOCK_SIZE; x++) {
            int image_x = start_x + x;
            int image_y = start_y + y;

            matrix->data[image_y * matrix->width + image_x] = block[y][x];
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

void inverse_dct(double input[JPEG_BLOCK_SIZE][JPEG_BLOCK_SIZE],
                 double output[JPEG_BLOCK_SIZE][JPEG_BLOCK_SIZE]) {
    for (int y = 0; y < JPEG_BLOCK_SIZE; y++) {
        for (int x = 0; x < JPEG_BLOCK_SIZE; x++) {
            double sum = 0.0;

            for (int v = 0; v < JPEG_BLOCK_SIZE; v++) {
                for (int u = 0; u < JPEG_BLOCK_SIZE; u++) {
                    double cos_x = cos(((2 * x + 1) * u * PI) / 16.0);
                    double cos_y = cos(((2 * y + 1) * v * PI) / 16.0);

                    sum += dct_coefficient(u) * dct_coefficient(v) *
                           input[v][u] * cos_x * cos_y;
                }
            }

            output[y][x] = 0.25 * sum + 128.0;
        }
    }
}

void quantize_block(double input[JPEG_BLOCK_SIZE][JPEG_BLOCK_SIZE],
                    int output[JPEG_BLOCK_SIZE][JPEG_BLOCK_SIZE],
                    double quality) {
    for (int y = 0; y < JPEG_BLOCK_SIZE; y++) {
        for (int x = 0; x < JPEG_BLOCK_SIZE; x++) {
            double q = quantization_table[y][x] * quality;

            if (q < 1.0) {
                q = 1.0;
            }

            output[y][x] = (int)round(input[y][x] / q);
        }
    }
}

void dequantize_block(int input[JPEG_BLOCK_SIZE][JPEG_BLOCK_SIZE],
                      double output[JPEG_BLOCK_SIZE][JPEG_BLOCK_SIZE],
                      double quality) {
    for (int y = 0; y < JPEG_BLOCK_SIZE; y++) {
        for (int x = 0; x < JPEG_BLOCK_SIZE; x++) {
            double q = quantization_table[y][x] * quality;

            if (q < 1.0) {
                q = 1.0;
            }

            output[y][x] = input[y][x] * q;
        }
    }
}

int count_zero_coefficients(int block[JPEG_BLOCK_SIZE][JPEG_BLOCK_SIZE]) {
    int count = 0;

    for (int y = 0; y < JPEG_BLOCK_SIZE; y++) {
        for (int x = 0; x < JPEG_BLOCK_SIZE; x++) {
            if (block[y][x] == 0) {
                count++;
            }
        }
    }

    return count;
}

Matrix apply_jpeg_compression(const Matrix *input, double quality, JpegStats *stats) {
    Matrix output;

    output.width = 0;
    output.height = 0;
    output.data = NULL;

    if (stats != NULL) {
        stats->total_coefficients = 0;
        stats->zero_coefficients = 0;
    }

    if (!is_valid_jpeg_size(input)) {
        return output;
    }

    output = create_matrix(input->width, input->height);

    if (output.data == NULL) {
        return output;
    }

    int blocks_x = input->width / JPEG_BLOCK_SIZE;
    int blocks_y = input->height / JPEG_BLOCK_SIZE;

    for (int by = 0; by < blocks_y; by++) {
        for (int bx = 0; bx < blocks_x; bx++) {
            double block[JPEG_BLOCK_SIZE][JPEG_BLOCK_SIZE];
            double dct_block[JPEG_BLOCK_SIZE][JPEG_BLOCK_SIZE];
            int quantized_block[JPEG_BLOCK_SIZE][JPEG_BLOCK_SIZE];
            double dequantized_block[JPEG_BLOCK_SIZE][JPEG_BLOCK_SIZE];
            double restored_block[JPEG_BLOCK_SIZE][JPEG_BLOCK_SIZE];

            extract_block(input, bx, by, block);

            forward_dct(block, dct_block);
            quantize_block(dct_block, quantized_block, quality);

            if (stats != NULL) {
                stats->total_coefficients += JPEG_BLOCK_SIZE * JPEG_BLOCK_SIZE;
                stats->zero_coefficients += count_zero_coefficients(quantized_block);
            }

            dequantize_block(quantized_block, dequantized_block, quality);
            inverse_dct(dequantized_block, restored_block);

            insert_block(&output, bx, by, restored_block);
        }
    }

    return output;
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

void print_quantized_block(int block[JPEG_BLOCK_SIZE][JPEG_BLOCK_SIZE]) {
    printf("Quantized DCT coefficients:\n");

    for (int y = 0; y < JPEG_BLOCK_SIZE; y++) {
        for (int x = 0; x < JPEG_BLOCK_SIZE; x++) {
            printf("%5d ", block[y][x]);
        }

        printf("\n");
    }
}

void print_restored_block(double block[JPEG_BLOCK_SIZE][JPEG_BLOCK_SIZE]) {
    printf("Restored block after inverse DCT:\n");

    for (int y = 0; y < JPEG_BLOCK_SIZE; y++) {
        for (int x = 0; x < JPEG_BLOCK_SIZE; x++) {
            printf("%6.1f ", block[y][x]);
        }

        printf("\n");
    }
}