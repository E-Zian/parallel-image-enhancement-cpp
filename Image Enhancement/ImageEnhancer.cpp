#include <chrono>
#include<opencv2/opencv.hpp>
#include "GaussianKernel.h"
#include "ImageEnhancer.h"

void enhanceImage(const cv::Mat& image, const GaussianKernel& kernel, bool applyBlur, float detailAmplification)
{
	using clock = std::chrono::high_resolution_clock;
	auto start = clock::now();

	cv::imshow("Original Image", image);

	std::cout << "Image size: " << image.cols << " x " << image.rows << std::endl;

	// Apply Gaussian Blur (Low Pass Filter)
	std::vector<unsigned char> lowPassImageArray(image.cols * image.rows * image.channels());

	if (applyBlur) {
		// Apply Gaussian Blur first
		std::vector<unsigned char> denoisedImageArray = kernel.convolve(image);
		cv::Mat denoisedImage(image.rows, image.cols, image.type(), denoisedImageArray.data());

		lowPassImageArray = kernel.convolve(denoisedImage);
		cv::Mat lowPassImage(image.rows, image.cols, image.type(), lowPassImageArray.data());

		cv::imshow("LowPass Image", lowPassImage);

	}
	else {
		lowPassImageArray = kernel.convolve(image);
		cv::Mat lowPassImage(image.rows, image.cols, image.type(), lowPassImageArray.data());

		cv::imshow("LowPass Image", lowPassImage);
	}

	// High pass image
	std::vector<int> highPassImageArray(image.cols * image.rows * image.channels());

	for (int i = 0; i < image.rows * image.cols * image.channels(); ++i) {
		highPassImageArray[i] = static_cast<int>(image.data[i]) - static_cast<int>(lowPassImageArray[i]);
	}

	// For display purposes only
	std::vector<unsigned char> highPassVisualliserArray(image.cols * image.rows * image.channels());

	for (int i = 0; i < image.rows * image.cols * image.channels(); ++i) {
		highPassVisualliserArray[i] = static_cast<unsigned char>(std::clamp(highPassImageArray[i] + 127, 0, 255));
	}

	cv::Mat highPassImage(image.rows, image.cols, image.type(), highPassVisualliserArray.data());
	cv::imshow("Image Edges", highPassImage);

	// Enhanced image
	std::vector<unsigned char> enhancedImageArray(image.cols * image.rows * image.channels());

	for (int i = 0; i < image.rows * image.cols * image.channels(); ++i) {
		float enhanced = static_cast<float>(image.data[i]) + highPassImageArray[i] * detailAmplification;
		enhancedImageArray[i] = static_cast<unsigned char>(std::clamp(enhanced, 0.0f, 255.0f));
	}

	cv::Mat enhancedImage(image.rows, image.cols, image.type(), enhancedImageArray.data());
	cv::imshow("Enhanced Image", enhancedImage);

	// End time measurement
	auto end = clock::now();
	std::chrono::duration<double, std::milli> elapsed = end - start;

	std::cout << "Time: " << elapsed.count() << " ms\n";

	cv::waitKey(0);
}

