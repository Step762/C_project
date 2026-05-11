#include <stdio.h>

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

    return 0;
}