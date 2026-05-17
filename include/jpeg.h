#ifndef JPEG_H
#define JPEG_H

#include "matrix.h"

#define JPEG_BLOCK_SIZE 8

int is_valid_jpeg_size(const Matrix *matrix);
void print_jpeg_blocks_info(const Matrix *matrix);

void extract_block(const Matrix *matrix, int block_x, int block_y, double block[JPEG_BLOCK_SIZE][JPEG_BLOCK_SIZE]);

void forward_dct(double input[JPEG_BLOCK_SIZE][JPEG_BLOCK_SIZE],
                 double output[JPEG_BLOCK_SIZE][JPEG_BLOCK_SIZE]);

void inverse_dct(double input[JPEG_BLOCK_SIZE][JPEG_BLOCK_SIZE],
                 double output[JPEG_BLOCK_SIZE][JPEG_BLOCK_SIZE]);

void print_block(double block[JPEG_BLOCK_SIZE][JPEG_BLOCK_SIZE]);
void print_dct_block(double block[JPEG_BLOCK_SIZE][JPEG_BLOCK_SIZE]);
void print_restored_block(double block[JPEG_BLOCK_SIZE][JPEG_BLOCK_SIZE]);

#endif