#include <cmath>  
#include <iostream>
#include "kernel.h"

float unnormalizedGaussian(int x, int y, float sigma);

std::vector<float> generateKernel(float sigma) {
	int radius = static_cast<int>(std::ceil(3 * sigma));
	int kernel_size = 2 * radius + 1;

	std::vector<float> kernel(kernel_size* kernel_size);

	// For normalization
	float sum = 0.0f;

	for (int row = -radius; row <= radius; ++row) {
		for (int col = -radius; col <= radius; ++col) {

			float value = unnormalizedGaussian(col, row, sigma);

			sum += value;
			kernel[(row+radius) * kernel_size + (col + radius)] = value;
		}
	}

	// Normalize the kernel
	for (int i = 0; i < kernel_size * kernel_size; ++i) {
		kernel[i] /= sum;
	}

	return kernel;
}

// Function to compute the unnormalized Gaussian value at (x, y)
float unnormalizedGaussian(int x, int y, float sigma) {
	return std::exp(-((x * x + y * y) / (2 * sigma * sigma)));
}

void displayKernel(const std::vector<float>& kernel) {
	int size = static_cast<int>(std::sqrt(kernel.size()));
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			std::cout << kernel[i * size + j] << " ";
		}
		std::cout << std::endl;
	}
}