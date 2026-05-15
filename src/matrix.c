#include <stdlib.h>
#include <stddef.h>

#include "matrix.h"

Matrix create_matrix(int width, int height) {
    Matrix matrix;

    matrix.width = width;
    matrix.height = height;
    matrix.data = NULL;

    if (width <= 0 || height <= 0) {
        return matrix;
    }

    matrix.data = (double *)calloc(width * height, sizeof(double));

    return matrix;
}

void free_matrix(Matrix *matrix) {
    if (matrix == NULL) {
        return;
    }

    free(matrix->data);
    matrix->data = NULL;
    matrix->width = 0;
    matrix->height = 0;
}

Matrix image_to_grayscale_matrix(const Image *img) {
    Matrix matrix;

    matrix.width = 0;
    matrix.height = 0;
    matrix.data = NULL;

    if (img == NULL || img->data == NULL) {
        return matrix;
    }

    matrix = create_matrix(img->width, img->height);

    if (matrix.data == NULL) {
        return matrix;
    }

    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            int image_index = (y * img->width + x) * 3;
            int matrix_index = y * img->width + x;

            unsigned char r = img->data[image_index];
            unsigned char g = img->data[image_index + 1];
            unsigned char b = img->data[image_index + 2];

            matrix.data[matrix_index] = 0.299 * r + 0.587 * g + 0.114 * b;
        }
    }

    return matrix;
}

Image matrix_to_image(const Matrix *matrix) {
    Image img;

    img.width = 0;
    img.height = 0;
    img.data = NULL;

    if (matrix == NULL || matrix->data == NULL) {
        return img;
    }

    img = create_empty_image(matrix->width, matrix->height);

    if (img.data == NULL) {
        return img;
    }

    for (int y = 0; y < matrix->height; y++) {
        for (int x = 0; x < matrix->width; x++) {
            int matrix_index = y * matrix->width + x;
            int image_index = (y * matrix->width + x) * 3;

            double value = matrix->data[matrix_index];

            if (value < 0.0) {
                value = 0.0;
            }

            if (value > 255.0) {
                value = 255.0;
            }

            unsigned char gray = (unsigned char)value;

            img.data[image_index] = gray;
            img.data[image_index + 1] = gray;
            img.data[image_index + 2] = gray;
        }
    }

    return img;
}