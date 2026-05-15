#ifndef MATRIX_H
#define MATRIX_H

#include "bmp.h"

typedef struct {
    int width;
    int height;
    double *data;
} Matrix;

Matrix create_matrix(int width, int height);
void free_matrix(Matrix *matrix);

Matrix image_to_grayscale_matrix(const Image *img);
Image matrix_to_image(const Matrix *matrix);

#endif