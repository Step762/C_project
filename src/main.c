#include <stdio.h>

#include "bmp.h"

void print_usage(const char *program_name) {
    printf("Usage: %s <input.bmp> <output.bmp>\n", program_name);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        print_usage(argv[0]);
        return 1;
    }

    const char *input_filename = argv[1];
    const char *output_filename = argv[2];

    printf("Image compression project: SVD and JPEG\n");
    printf("Input file: %s\n", input_filename);
    printf("Output file: %s\n", output_filename);

    int width;
    int height;
    int bits_per_pixel;

    if (!read_bmp_info(input_filename, &width, &height, &bits_per_pixel)) {
        printf("Failed to read BMP information\n");
        return 1;
    }

    printf("BMP information:\n");
    printf("Width: %d\n", width);
    printf("Height: %d\n", height);
    printf("Bits per pixel: %d\n", bits_per_pixel);

    if (bits_per_pixel != 24) {
        printf("Warning: this project will work only with 24-bit BMP images\n");
    }

    Image test_image = create_empty_image(width, height);

    if (test_image.data == NULL) {
        printf("Failed to create image structure\n");
        return 1;
    }

    print_image_info(&test_image);
    free_image(&test_image);

    return 0;
}