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

Image load_bmp(const char *filename) {
    Image img;
    img.width = 0;
    img.height = 0;
    img.data = NULL;

    FILE *file = fopen(filename, "rb");

    if (file == NULL) {
        printf("Cannot open file: %s\n", filename);
        return img;
    }

    unsigned char signature[2];
    fread(signature, sizeof(unsigned char), 2, file);

    if (signature[0] != 'B' || signature[1] != 'M') {
        printf("File is not a BMP image\n");
        fclose(file);
        return img;
    }

    int32_t width;
    int32_t height;
    uint16_t bits_per_pixel;
    uint32_t pixel_offset;

    fseek(file, 10, SEEK_SET);
    fread(&pixel_offset, sizeof(uint32_t), 1, file);

    fseek(file, 18, SEEK_SET);
    fread(&width, sizeof(int32_t), 1, file);

    fseek(file, 22, SEEK_SET);
    fread(&height, sizeof(int32_t), 1, file);

    fseek(file, 28, SEEK_SET);
    fread(&bits_per_pixel, sizeof(uint16_t), 1, file);

    if (bits_per_pixel != 24) {
        printf("Only 24-bit BMP files are supported now\n");
        fclose(file);
        return img;
    }

    img = create_empty_image(width, height);

    if (img.data == NULL) {
        printf("Cannot allocate memory for image\n");
        fclose(file);
        return img;
    }

    int row_size = width * 3;
    int padding = (4 - (row_size % 4)) % 4;

    fseek(file, pixel_offset, SEEK_SET);

    for (int y = height - 1; y >= 0; y--) {
        for (int x = 0; x < width; x++) {
            unsigned char b;
            unsigned char g;
            unsigned char r;

            fread(&b, sizeof(unsigned char), 1, file);
            fread(&g, sizeof(unsigned char), 1, file);
            fread(&r, sizeof(unsigned char), 1, file);

            int index = (y * width + x) * 3;

            img.data[index] = r;
            img.data[index + 1] = g;
            img.data[index + 2] = b;
        }

        fseek(file, padding, SEEK_CUR);
    }

    fclose(file);

    return img;
}

void print_first_pixel(const Image *img) {
    if (img == NULL || img->data == NULL) {
        return;
    }

    printf("First pixel RGB: %d %d %d\n",
           img->data[0],
           img->data[1],
           img->data[2]);
}