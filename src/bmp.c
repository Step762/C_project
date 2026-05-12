#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

Image create_empty_image(int width, int height) {
    Image img;

    img.width = width;
    img.height = height;
    img.data = NULL;

    if (width <= 0 || height <= 0) {
        return img;
    }

    img.data = (unsigned char *)calloc(width * height * 3, sizeof(unsigned char));

    return img;
}

void free_image(Image *img) {
    if (img == NULL) {
        return;
    }

    free(img->data);
    img->data = NULL;
    img->width = 0;
    img->height = 0;
}

void print_image_info(const Image *img) {
    if (img == NULL) {
        return;
    }

    printf("Image size: %d x %d\n", img->width, img->height);
}