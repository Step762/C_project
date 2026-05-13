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

    Image image = load_bmp(input_filename);

    if (image.data == NULL) {
        printf("Failed to load BMP image\n");
        return 1;
    }

    printf("BMP image loaded successfully\n");
    print_image_info(&image);
    print_first_pixel(&image);

    free_image(&image);

    return 0;
}