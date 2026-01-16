#include "GaussianKernel.h"
#include <cmath>
#include <iostream>
#include <algorithm>
#include <thread>

GaussianKernel::GaussianKernel(float sigma)
	: m_sigma{ sigma },
	m_radius{ static_cast<int>(std::ceil(3 * sigma)) },
	m_kernelDiameter{ 2 * m_radius + 1 },
	m_kernelSize{ m_kernelDiameter * m_kernelDiameter },
	m_kernel(m_kernelSize),
	m_kernel1D(m_kernelDiameter)
{
	if (sigma <= 0.0f) {
		throw std::invalid_argument("GaussianKernel: sigma must be > 0");
	}
	generate2DKernel();
	generate1DKernels();
}

void GaussianKernel::display2DKernel() const {
	for (int i{  }; i < m_kernelDiameter; ++i) {
		for (int j{  }; j < m_kernelDiameter; ++j) {
			std::cout << m_kernel[i * m_kernelDiameter + j] << " ";
		}
		std::cout << std::endl;
	}
}

void GaussianKernel::display1DKernel() const {
	for (int i{  }; i < m_kernelDiameter; ++i) {
		std::cout << m_kernel1D[i] << " ";
	}
}

void GaussianKernel::generate1DKernels() {
	float sum{ };

	// Generate 1d kernel
	for (int col{ -m_radius }; col <= m_radius; ++col) {
		float value = std::exp(-(col * col) / (2.0f * m_sigma * m_sigma));
		m_kernel1D[col + m_radius] = value;
		sum += value;
	}

	// Normalize 1d kernel
	for (int i{  }; i < m_kernelDiameter; ++i) {
		m_kernel1D[i] /= sum;
	}

}

float GaussianKernel::unnormalizedGaussian(int x, int y) {
	return std::exp(-((x * x + y * y) / (2.0f * m_sigma * m_sigma)));
}

void GaussianKernel::generate2DKernel() {
	float sum{};

	// Calculate unnormalized values and sum them
	for (int row{ -m_radius }; row <= m_radius; ++row) {
		for (int col = -m_radius; col <= m_radius; ++col) {
			float value{ unnormalizedGaussian(col, row) };
			sum += value;
			m_kernel[(row + m_radius) * m_kernelDiameter + (col + m_radius)] = value;
		}
	}

	// Normalize the kernel
	for (int i{  }; i < m_kernelSize; ++i) {
		m_kernel[i] /= sum;
	}
}

std::vector<unsigned char> GaussianKernel::convolve(const cv::Mat& image) const {
	int width{ image.cols };
	int height{ image.rows };
	if (image.channels() == 3) {
		// Colored image
		std::vector<unsigned char> inputImage(
			image.data,
			image.data + image.total() * image.channels()
		);
		return convolveColored(inputImage, width, height);
	}
	else if (image.channels() == 1) {
		// Grayscale image
		std::vector<unsigned char> inputImage(
			image.data,
			image.data + image.total()
		);
		return convolveGray(inputImage, width, height);
	}
	else {
		throw std::runtime_error("GaussianKernel::convolve: Unsupported number of channels");
	}
}


std::vector<unsigned char> GaussianKernel::convolveGray(const std::vector<unsigned char>& inputImage, int width, int height) const {

	// Temporary horizontal image
	std::vector<float> horizontalTemp(width * height);
	std::vector<unsigned char> outputImage(width * height);

	int totalThreads{ static_cast<int>(std::thread::hardware_concurrency()) };

	// Horizontal pass
	{
		std::vector<std::jthread> threads;

		auto processRows = [&](int startRow, int lastRow) {
			for (int row{ startRow }; row <= lastRow; ++row) {
				for (int col{  }; col < width; ++col) {
					float sum{ 0.0f };
					for (int k{ -m_radius }; k <= m_radius; ++k) {

						// The affected columns in horizontal pass
						int c{ col + k };
						if (c >= 0 && c < width) {
							sum += m_kernel1D[k + m_radius] * static_cast<float>(inputImage[c + row * width]);
						}
					}
					horizontalTemp[col + row * width] = sum;
				}
			}
			};
		int rowsPerThread{ static_cast<int>(height / totalThreads) };
		for (int threadId{  }; threadId < totalThreads; ++threadId) {
			int startRow{ threadId * rowsPerThread };
			int lastRow{ threadId == totalThreads - 1 ? height - 1 : startRow + rowsPerThread - 1 };
			threads.emplace_back(processRows, startRow, lastRow);
		}

	}

	// Vertical pass
	{
		std::vector<std::jthread> threads;
		auto processRows = [&](int startRow, int lastRow) {
			for (int row{ startRow }; row <= lastRow; ++row) {
				for (int col{ }; col < width; ++col) {
					float sum{ };
					for (int k{ -m_radius }; k <= m_radius; ++k) {
						int r{ row + k };
						if (r >= 0 && r < height) {
							sum += m_kernel1D[k + m_radius] * horizontalTemp[col + r * width];
						}
					}
					outputImage[col + row * width] =
						static_cast<unsigned char>(std::clamp(sum, 0.0f, 255.0f));
				}
			}
			};

		int rowsPerThread{ static_cast<int>(height / totalThreads) };
		for (int threadId{ }; threadId < totalThreads; ++threadId) {
			int startRow{ threadId * rowsPerThread };
			int lastRow{ threadId == totalThreads - 1 ? height - 1 : startRow + rowsPerThread - 1 };
			threads.emplace_back(processRows, startRow, lastRow);
		}

	}

	return outputImage;

}

std::vector<unsigned char> GaussianKernel::convolveColored(const std::vector<unsigned char>& inputImage, int width, int height) const {


	// Temporary horizontal image
	std::vector<float> horizontalTemp(width * height * 3);
	std::vector<unsigned char> outputImage(width * height * 3);

	int totalThreads{ static_cast<int>(std::thread::hardware_concurrency()) };


	// Each pixel in the image
	for (int spectrum{ }; spectrum < 3; ++spectrum) {
		// Horizontal pass
		{
			std::vector<std::jthread> threads;

			auto processRow = [&](int startRow, int lastRow) {
				for (int row{ startRow }; row <= lastRow; ++row) {
					for (int col{  }; col < width; ++col) {

						float sum{ };
						for (int k{ -m_radius }; k <= m_radius; ++k) {

							// The affected columns in horizontal pass
							int c{ col + k };
							if (c >= 0 && c < width) {
								sum += m_kernel1D[k + m_radius] * static_cast<float>(inputImage[(c + row * width) * 3 + spectrum]);
							}
						}
						horizontalTemp[(col + row * width) * 3 + spectrum] = sum;
					}
				}
				};

			int rowsPerThread{ static_cast<int>(height / totalThreads) };
			for (int threadId{ }; threadId < totalThreads; ++threadId) {
				int startRow{ threadId * rowsPerThread };
				int lastRow{ threadId == totalThreads - 1 ? height - 1 : startRow + rowsPerThread - 1 };
				threads.emplace_back(processRow, startRow, lastRow);
			}

		}

		// Vertical pass
		{
			std::vector<std::jthread> threads;

			auto processRow = [&](int startRow, int lastRow) {
				for (int row{ startRow }; row <= lastRow; ++row) {
					for (int col{ }; col < width; ++col) {
						float sum{ };
						for (int k{ -m_radius }; k <= m_radius; ++k) {
							int r{ row + k };
							if (r >= 0 && r < height) {
								sum += m_kernel1D[k + m_radius] * horizontalTemp[(col + r * width) * 3 + spectrum];
							}
						}
						outputImage[(col + row * width) * 3 + spectrum] =
							static_cast<unsigned char>(std::clamp(sum, 0.0f, 255.0f));
					}
				}
				};

			int rowsPerThread{ static_cast<int>(height / totalThreads) };
			for (int threadId{ }; threadId < totalThreads; ++threadId) {
				int startRow{ threadId * rowsPerThread };
				int lastRow{ threadId == totalThreads - 1 ? height - 1 : startRow + rowsPerThread - 1 };
				threads.emplace_back(processRow, startRow, lastRow);

			}

		}

	}


	return outputImage;

}
