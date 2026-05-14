#include <math.h>
#include <stddef.h>

#include "metrics.h"

double calculate_mse(const Image *original, const Image *processed) {
    if (original == NULL || processed == NULL) {
        return -1.0;
    }

    if (original->data == NULL || processed->data == NULL) {
        return -1.0;
    }

    if (original->width != processed->width || original->height != processed->height) {
        return -1.0;
    }

    long long pixels_count = original->width * original->height * 3;
    double sum = 0.0;

    for (long long i = 0; i < pixels_count; i++) {
        double difference = original->data[i] - processed->data[i];
        sum += difference * difference;
    }

    return sum / pixels_count;
}

double calculate_psnr(const Image *original, const Image *processed) {
    double mse = calculate_mse(original, processed);

    if (mse <= 0.0) {
        return 99.0;
    }

    return 10.0 * log10((255.0 * 255.0) / mse);
}