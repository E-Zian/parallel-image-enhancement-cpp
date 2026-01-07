#include "GaussianKernel.h"
#include <cmath>
#include <iostream>
#include <algorithm>


GaussianKernel::GaussianKernel(float sigma)
	: m_sigma{ sigma },
	m_radius{ static_cast<int>(std::ceil(3 * sigma)) },
	m_kernelDiameter{ 2 * m_radius + 1 },
	m_kernelSize{ m_kernelDiameter * m_kernelDiameter },
	m_kernel(m_kernelSize),
	m_kernel1D(m_kernelDiameter)
{
	generate2DKernel();
	generate1DKernels();
}

void GaussianKernel::display2DKernel() const {
	for (int i = 0; i < m_kernelDiameter; ++i) {
		for (int j = 0; j < m_kernelDiameter; ++j) {
			std::cout << m_kernel[i * m_kernelDiameter + j] << " ";
		}
		std::cout << std::endl;
	}
}

void GaussianKernel::display1DKernel() const {
	for (int i = 0; i < m_kernelDiameter; ++i) {
		std::cout << m_kernel1D[i] << " ";
	}
}

void GaussianKernel::generate1DKernels() {
	float sum = 0.0f;

	// Generate 1d kernel
	for (int col = -m_radius; col <= m_radius; ++col) {
		float value = std::exp(-(col * col) / (2.0f * m_sigma * m_sigma));
		m_kernel1D[col + m_radius] = value;
		sum += value;
	}

	// Normalize 1d kernel
	for (int i = 0; i < m_kernelDiameter; ++i) {
		m_kernel1D[i] /= sum;
	}

}

float GaussianKernel::unnormalizedGaussian(int x, int y) {
	return std::exp(-((x * x + y * y) / (2.0f * m_sigma * m_sigma)));
}

void GaussianKernel::generate2DKernel() {
	float sum = 0.0f;

	// Calculate unnormalized values and sum them
	for (int row = -m_radius; row <= m_radius; ++row) {
		for (int col = -m_radius; col <= m_radius; ++col) {
			float value = unnormalizedGaussian(col, row);
			sum += value;
			m_kernel[(row + m_radius) * m_kernelDiameter + (col + m_radius)] = value;
		}
	}

	// Normalize the kernel
	for (int i = 0; i < m_kernelSize; ++i) {
		m_kernel[i] /= sum;
	}
}

std::vector<unsigned char> GaussianKernel::convolve(const std::vector<unsigned char>& inputImage, int width, int height) const {

	// Temporary horizontal image
	std::vector<float> horizontalTemp(width * height);
	std::vector<unsigned char> outputImage(width * height);

	// Each pixel in the image
	for (int row = 0; row < height; ++row) {
		for (int col = 0; col < width; ++col) {
			float sum = 0.0f;
			for (int k = -m_radius; k <= m_radius; ++k) {
				int c = col + k;
				if (c >= 0 && c < width) {
					sum += m_kernel1D[k + m_radius] * inputImage[c + row * width];
				}
			}
			horizontalTemp[col + row * width] = sum;
		}
	}

	// Vertical pass
	for (int row = 0; row < height; ++row) {
		for (int col = 0; col < width; ++col) {
			float sum = 0.0f;
			for (int k = -m_radius; k <= m_radius; ++k) {
				int r = row + k;
				if (r >= 0 && r < height) {
					sum += m_kernel1D[k + m_radius] * horizontalTemp[col + r * width];
				}
			}
			outputImage[col + row * width] =
				static_cast<unsigned char>(std::clamp(sum, 0.0f, 255.0f));
		}
	}
	
	return outputImage;


}

