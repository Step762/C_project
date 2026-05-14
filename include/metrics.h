#ifndef METRICS_H
#define METRICS_H

#include "bmp.h"

double calculate_mse(const Image *original, const Image *processed);
double calculate_psnr(const Image *original, const Image *processed);

#endif