#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "bmp.h"
#include "processing.h"
#include "metrics.h"
#include "matrix.h"
#include "jpeg.h"

void print_usage(const char *program_name) {
    printf("Usage:\n");
    printf("  %s grayscale <input.bmp> <output.bmp>\n", program_name);
    printf("  %s blocks <input.bmp> <output.bmp>\n", program_name);
    printf("  %s jpeg <input.bmp> <output.bmp> <quality>\n", program_name);
    printf("\n");
    printf("Examples:\n");
    printf("  %s grayscale images/test_beck.bmp output.bmp\n", program_name);
    printf("  %s blocks images/test_beck.bmp output.bmp\n", program_name);
    printf("  %s jpeg images/test_beck.bmp output.bmp 1.0\n", program_name);
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
    double ssim = calculate_ssim(&original, &processed);

    printf("PSNR: %.2f dB\n", psnr);
    printf("SSIM: %.4f\n", ssim);

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
    int quantized_block[JPEG_BLOCK_SIZE][JPEG_BLOCK_SIZE];
    double dequantized_block[JPEG_BLOCK_SIZE][JPEG_BLOCK_SIZE];
    double restored_block[JPEG_BLOCK_SIZE][JPEG_BLOCK_SIZE];

    extract_block(&brightness, 0, 0, first_block);
    print_block(first_block);

    forward_dct(first_block, dct_block);
    print_dct_block(dct_block);

    quantize_block(dct_block, quantized_block, 1.0);
    print_quantized_block(quantized_block);

    dequantize_block(quantized_block, dequantized_block, 1.0);

    inverse_dct(dequantized_block, restored_block);
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

int run_jpeg_mode(const char *input_filename, const char *output_filename, double quality) {
    Image original = load_bmp(input_filename);

    if (original.data == NULL) {
        printf("Failed to load BMP image\n");
        return 1;
    }

    printf("BMP image loaded successfully\n");
    print_image_info(&original);
    printf("JPEG quality factor: %.2f\n", quality);

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

    JpegStats stats;

    Matrix jpeg_matrix = apply_jpeg_compression(&brightness, quality, &stats);

    if (jpeg_matrix.data == NULL) {
        printf("Failed to apply JPEG compression\n");
        free_matrix(&brightness);
        free_image(&original);
        return 1;
    }

    Image original_gray = matrix_to_image(&brightness);
    Image processed = matrix_to_image(&jpeg_matrix);

    if (original_gray.data == NULL || processed.data == NULL) {
        printf("Failed to convert matrix to image\n");
        free_image(&processed);
        free_image(&original_gray);
        free_matrix(&jpeg_matrix);
        free_matrix(&brightness);
        free_image(&original);
        return 1;
    }

    double psnr = calculate_psnr(&original_gray, &processed);
    double ssim = calculate_ssim(&original_gray, &processed);

    printf("JPEG-like processing completed\n");
    printf("PSNR: %.2f dB\n", psnr);
    printf("SSIM: %.4f\n", ssim);
    
    int non_zero_coefficients = stats.total_coefficients - stats.zero_coefficients;

    double zero_percent = 0.0;
    double non_zero_percent = 0.0;
    double compression_ratio = 0.0;

    if (stats.total_coefficients > 0) {
        zero_percent = 100.0 * stats.zero_coefficients / stats.total_coefficients;
        non_zero_percent = 100.0 * non_zero_coefficients / stats.total_coefficients;
    }

    if (non_zero_coefficients > 0) {
        compression_ratio = (double)stats.total_coefficients / non_zero_coefficients;
    }

    printf("Total DCT coefficients: %d\n", stats.total_coefficients);
    printf("Zero coefficients after quantization: %d\n", stats.zero_coefficients);
    printf("Zero coefficients percent: %.2f%%\n", zero_percent);
    printf("Non-zero coefficients after quantization: %d\n", non_zero_coefficients);
    printf("Estimated useful coefficients percent: %.2f%%\n", non_zero_percent);
    printf("Estimated compression ratio: %.2f:1\n", compression_ratio);

    if (!save_bmp(output_filename, &processed)) {
        printf("Failed to save BMP image\n");
        free_image(&processed);
        free_image(&original_gray);
        free_matrix(&jpeg_matrix);
        free_matrix(&brightness);
        free_image(&original);
        return 1;
    }

    printf("BMP image saved successfully\n");

    free_image(&processed);
    free_image(&original_gray);
    free_matrix(&jpeg_matrix);
    free_matrix(&brightness);
    free_image(&original);

    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
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

    if (strcmp(mode, "jpeg") == 0) {
        if (argc != 5) {
            printf("JPEG mode requires quality parameter\n");
            print_usage(argv[0]);
            return 1;
        }

        double quality = atof(argv[4]);

        if (quality <= 0.0) {
            printf("Quality must be greater than 0\n");
            return 1;
        }

        return run_jpeg_mode(input_filename, output_filename, quality);
    }

    printf("Unknown mode: %s\n", mode);
    print_usage(argv[0]);

    return 1;
}