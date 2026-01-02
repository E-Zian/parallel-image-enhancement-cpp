#include "kernel.h"
#include <cmath>
#include <iostream>
#include <algorithm>
#include <chrono>

GaussianKernel::GaussianKernel(float sigma)
	: m_sigma{ sigma },
	m_radius{ static_cast<int>(std::ceil(3 * sigma)) },
	m_kernalSize{ 2 * m_radius + 1 },
	m_kernel(m_kernalSize* m_kernalSize)
{
	generateKernel();
}

void GaussianKernel::displayKernel() const {
	for (int i = 0; i < m_kernalSize; ++i) {
		for (int j = 0; j < m_kernalSize; ++j) {
			std::cout << m_kernel[i * m_kernalSize + j] << " ";
		}
		std::cout << std::endl;
	}
}

float GaussianKernel::unnormalizedGaussian(int x, int y) {
	return std::exp(-((x * x + y * y) / (2.0f * m_sigma * m_sigma)));
}

void GaussianKernel::generateKernel() {
	float sum = 0.0f;

	// Calculate unnormalized values and sum them
	for (int row = -m_radius; row <= m_radius; ++row) {
		for (int col = -m_radius; col <= m_radius; ++col) {
			float value = unnormalizedGaussian(col, row);
			sum += value;
			m_kernel[(row + m_radius) * m_kernalSize + (col + m_radius)] = value;
		}
	}

	// Normalize the kernel
	for (int i = 0; i < m_kernalSize * m_kernalSize; ++i) {
		m_kernel[i] /= sum;
	}
}

void GaussianKernel::convolve(const unsigned char* inputImage, unsigned char* outputImage, int width, int height) const {
	using clock = std::chrono::high_resolution_clock;
	auto start = clock::now();

	for (int row = 0; row < height; ++row) {
		for (int col = 0; col < width; ++col) {

			float sum = 0.0f;

			for (int kRow = -m_radius; kRow <= m_radius; ++kRow) {
				for (int kCol = -m_radius; kCol <= m_radius; ++kCol) {

					// Skip out-of-bounds pixels
					if (col + kCol < 0 || row + kRow < 0 || col + kCol >=width || row + kRow >=height) {
						continue;
					}
					sum += m_kernel[(kCol + m_radius) + ((kRow + m_radius) * m_kernalSize)] * inputImage[(col + kCol) + (row + kRow * width)];
				}
			}


			outputImage[col + row * width] = static_cast<unsigned char>(std::clamp(sum, 0.0f, 255.0f));


		}
	}
	auto end = clock::now();

	std::chrono::duration<double, std::milli> elapsed = end - start;
	std::cout << "Time: " << elapsed.count() << " ms\n";
}