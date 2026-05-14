#include <stddef.h>

#include "processing.h"

void convert_to_grayscale(Image *img) {
    if (img == NULL || img->data == NULL) {
        return;
    }

    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            int index = (y * img->width + x) * 3;

            unsigned char r = img->data[index];
            unsigned char g = img->data[index + 1];
            unsigned char b = img->data[index + 2];

            unsigned char gray = (unsigned char)(0.299 * r + 0.587 * g + 0.114 * b);

            img->data[index] = gray;
            img->data[index + 1] = gray;
            img->data[index + 2] = gray;
        }
    }
}