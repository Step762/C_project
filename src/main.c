#include <stdio.h>
#include <string.h>

#include "bmp.h"
#include "processing.h"
#include "metrics.h"

void print_usage(const char *program_name) {
    printf("Usage: %s <mode> <input.bmp> <output.bmp>\n", program_name);
    printf("Modes:\n");
    printf("  grayscale   convert image to grayscale and calculate PSNR\n");
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        print_usage(argv[0]);
        return 1;
    }

    const char *mode = argv[1];
    const char *input_filename = argv[2];
    const char *output_filename = argv[3];

    printf("Image compression project: SVD and JPEG\n");
    printf("Mode: %s\n", mode);
    printf("Input file: %s\n", input_filename);
    printf("Output file: %s\n", output_filename);

    if (strcmp(mode, "grayscale") != 0) {
        printf("Unknown mode: %s\n", mode);
        print_usage(argv[0]);
        return 1;
    }

    Image original = load_bmp(input_filename);

    if (original.data == NULL) {
        printf("Failed to load BMP image\n");
        return 1;
    }

    Image processed = copy_image(&original);

    if (processed.data == NULL) {
        printf("Failed to copy image\n");
        free_image(&original);
        return 1;
    }

    printf("BMP image loaded successfully\n");
    print_image_info(&original);

    convert_to_grayscale(&processed);

    printf("Image converted to grayscale\n");

    double psnr = calculate_psnr(&original, &processed);
    printf("PSNR: %.2f dB\n", psnr);

    if (!save_bmp(output_filename, &processed)) {
        printf("Failed to save BMP image\n");
        free_image(&original);
        free_image(&processed);
        return 1;
    }

    printf("BMP image saved successfully\n");

    free_image(&original);
    free_image(&processed);

    return 0;
}