#ifndef BMP_H
#define BMP_H

typedef struct {
    int width;
    int height;
    unsigned char *data;
} Image;

Image create_empty_image(int width, int height);
Image copy_image(const Image *src);
void free_image(Image *img);
void print_image_info(const Image *img);

int read_bmp_info(const char *filename, int *width, int *height, int *bits_per_pixel);
Image load_bmp(const char *filename);
int save_bmp(const char *filename, const Image *img);

void print_first_pixel(const Image *img);

#endif