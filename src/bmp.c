#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

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

int read_bmp_info(const char *filename, int *width, int *height, int *bits_per_pixel) {
    FILE *file = fopen(filename, "rb");

    if (file == NULL) {
        printf("Cannot open file: %s\n", filename);
        return 0;
    }

    unsigned char signature[2];

    fread(signature, sizeof(unsigned char), 2, file);

    if (signature[0] != 'B' || signature[1] != 'M') {
        printf("File is not a BMP image\n");
        fclose(file);
        return 0;
    }

    int32_t bmp_width;
    int32_t bmp_height;
    uint16_t bmp_bits_per_pixel;

    fseek(file, 18, SEEK_SET);
    fread(&bmp_width, sizeof(int32_t), 1, file);

    fseek(file, 22, SEEK_SET);
    fread(&bmp_height, sizeof(int32_t), 1, file);

    fseek(file, 28, SEEK_SET);
    fread(&bmp_bits_per_pixel, sizeof(uint16_t), 1, file);

    fclose(file);

    *width = bmp_width;
    *height = bmp_height;
    *bits_per_pixel = bmp_bits_per_pixel;

    return 1;
}