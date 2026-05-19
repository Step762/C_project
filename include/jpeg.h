#ifndef JPEG_H
#define JPEG_H

#include "matrix.h"

#define JPEG_BLOCK_SIZE 8

typedef struct {
    int total_coefficients;
    int zero_coefficients;
} JpegStats;

int is_valid_jpeg_size(const Matrix *matrix);
void print_jpeg_blocks_info(const Matrix *matrix);

void extract_block(const Matrix *matrix, int block_x, int block_y, double block[JPEG_BLOCK_SIZE][JPEG_BLOCK_SIZE]);

void forward_dct(double input[JPEG_BLOCK_SIZE][JPEG_BLOCK_SIZE],
                 double output[JPEG_BLOCK_SIZE][JPEG_BLOCK_SIZE]);

void inverse_dct(double input[JPEG_BLOCK_SIZE][JPEG_BLOCK_SIZE],
                 double output[JPEG_BLOCK_SIZE][JPEG_BLOCK_SIZE]);

void quantize_block(double input[JPEG_BLOCK_SIZE][JPEG_BLOCK_SIZE],
                    int output[JPEG_BLOCK_SIZE][JPEG_BLOCK_SIZE],
                    double quality);

void dequantize_block(int input[JPEG_BLOCK_SIZE][JPEG_BLOCK_SIZE],
                      double output[JPEG_BLOCK_SIZE][JPEG_BLOCK_SIZE],
                      double quality);

int count_zero_coefficients(int block[JPEG_BLOCK_SIZE][JPEG_BLOCK_SIZE]);

Matrix apply_jpeg_compression(const Matrix *input, double quality, JpegStats *stats);

void print_block(double block[JPEG_BLOCK_SIZE][JPEG_BLOCK_SIZE]);
void print_dct_block(double block[JPEG_BLOCK_SIZE][JPEG_BLOCK_SIZE]);
void print_quantized_block(int block[JPEG_BLOCK_SIZE][JPEG_BLOCK_SIZE]);
void print_restored_block(double block[JPEG_BLOCK_SIZE][JPEG_BLOCK_SIZE]);

#endif