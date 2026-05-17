#include <stdio.h>
#include <string.h>

#include "bmp.h"
#include "processing.h"
#include "metrics.h"
#include "matrix.h"
#include "jpeg.h"

void print_usage(const char *program_name) {
    printf("Usage: %s <mode> <input.bmp> <output.bmp>\n", program_name);
    printf("Modes:\n");
    printf("  grayscale   convert image to grayscale and calculate PSNR\n");
    printf("  blocks      show information about 8x8 blocks\n");
}

int run_grayscale_mode(const char *input_filename, const char *output_filename) {
    Image original = load_bmp(input_filename);

    if (original.data == NULL) {
        printf("Failed to load BMP image\n");
        return 1;
    }

    printf("BMP image loaded successfully\n");
    print_image_info(&original);

    Matrix brightness = image_to_grayscale_matrix(&original);

    if (brightness.data == NULL) {
        printf("Failed to create brightness matrix\n");
        free_image(&original);
        return 1;
    }

    Image processed = matrix_to_image(&brightness);

    if (processed.data == NULL) {
        printf("Failed to convert matrix to image\n");
        free_matrix(&brightness);
        free_image(&original);
        return 1;
    }

    printf("Image converted to grayscale matrix\n");

    double psnr = calculate_psnr(&original, &processed);
    printf("PSNR: %.2f dB\n", psnr);

    if (!save_bmp(output_filename, &processed)) {
        printf("Failed to save BMP image\n");
        free_image(&processed);
        free_matrix(&brightness);
        free_image(&original);
        return 1;
    }

    printf("BMP image saved successfully\n");

    free_image(&processed);
    free_matrix(&brightness);
    free_image(&original);

    return 0;
}

int run_blocks_mode(const char *input_filename, const char *output_filename) {
    Image original = load_bmp(input_filename);

    if (original.data == NULL) {
        printf("Failed to load BMP image\n");
        return 1;
    }

    printf("BMP image loaded successfully\n");
    print_image_info(&original);

    Matrix brightness = image_to_grayscale_matrix(&original);

    if (brightness.data == NULL) {
        printf("Failed to create brightness matrix\n");
        free_image(&original);
        return 1;
    }

    if (!is_valid_jpeg_size(&brightness)) {
        printf("Image size must be divisible by 8 for this simple JPEG implementation\n");
        free_matrix(&brightness);
        free_image(&original);
        return 1;
    }

    print_jpeg_blocks_info(&brightness);

    double first_block[JPEG_BLOCK_SIZE][JPEG_BLOCK_SIZE];
    double dct_block[JPEG_BLOCK_SIZE][JPEG_BLOCK_SIZE];
    double restored_block[JPEG_BLOCK_SIZE][JPEG_BLOCK_SIZE];

    extract_block(&brightness, 0, 0, first_block);
    print_block(first_block);

    forward_dct(first_block, dct_block);
    print_dct_block(dct_block);

    inverse_dct(dct_block, restored_block);
    print_restored_block(restored_block);

    Image processed = matrix_to_image(&brightness);

    if (processed.data == NULL) {
        printf("Failed to convert matrix to image\n");
        free_matrix(&brightness);
        free_image(&original);
        return 1;
    }

    if (!save_bmp(output_filename, &processed)) {
        printf("Failed to save BMP image\n");
        free_image(&processed);
        free_matrix(&brightness);
        free_image(&original);
        return 1;
    }

    printf("Grayscale BMP image saved successfully\n");

    free_image(&processed);
    free_matrix(&brightness);
    free_image(&original);

    return 0;
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

    if (strcmp(mode, "grayscale") == 0) {
        return run_grayscale_mode(input_filename, output_filename);
    }

    if (strcmp(mode, "blocks") == 0) {
        return run_blocks_mode(input_filename, output_filename);
    }

    printf("Unknown mode: %s\n", mode);
    print_usage(argv[0]);

    return 1;
}