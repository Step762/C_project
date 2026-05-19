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

double calculate_ssim(const Image *original, const Image *processed) {
    if (original == NULL || processed == NULL) {
        return -1.0;
    }

    if (original->data == NULL || processed->data == NULL) {
        return -1.0;
    }

    if (original->width != processed->width || original->height != processed->height) {
        return -1.0;
    }

    long long count = original->width * original->height * 3;

    double mean_original = 0.0;
    double mean_processed = 0.0;

    for (long long i = 0; i < count; i++) {
        mean_original += original->data[i];
        mean_processed += processed->data[i];
    }

    mean_original /= count;
    mean_processed /= count;

    double variance_original = 0.0;
    double variance_processed = 0.0;
    double covariance = 0.0;

    for (long long i = 0; i < count; i++) {
        double original_diff = original->data[i] - mean_original;
        double processed_diff = processed->data[i] - mean_processed;

        variance_original += original_diff * original_diff;
        variance_processed += processed_diff * processed_diff;
        covariance += original_diff * processed_diff;
    }

    variance_original /= count - 1;
    variance_processed /= count - 1;
    covariance /= count - 1;

    double c1 = 6.5025;
    double c2 = 58.5225;

    double numerator = (2.0 * mean_original * mean_processed + c1) *
                       (2.0 * covariance + c2);

    double denominator = (mean_original * mean_original +
                          mean_processed * mean_processed + c1) *
                         (variance_original + variance_processed + c2);

    if (denominator == 0.0) {
        return 1.0;
    }

    return numerator / denominator;
}