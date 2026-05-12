#ifndef BMP_H
#define BMP_H

typedef struct {
    int width;
    int height;
    unsigned char *data;
} Image;

Image create_empty_image(int width, int height);
void free_image(Image *img);
void print_image_info(const Image *img);

#endif