// For getenv
#define _CRT_SECURE_NO_WARNINGS

#include "GaussianKernel.h"
#include "ImageEnhancer.h"
#include<opencv2/opencv.hpp>
#include <chrono>
#include <filesystem>

namespace fs = std::filesystem;

void downloadImage(const cv::Mat& image, const std::string& fileName);

void enhanceImage(const cv::Mat& image, const GaussianKernel& kernel, std::string& fileName, float detailAmplification, bool denoiseInputImage)
{

	std::cout << "\nImage size: " << image.cols << " x " << image.rows << std::endl;

	// Apply Gaussian Blur (Low Pass Filter)
	std::vector<unsigned char> lowPassImageArray(image.cols * image.rows * image.channels());

	if (denoiseInputImage) {
		// Apply Gaussian Blur first
		std::vector<unsigned char> denoisedImageArray{ kernel.convolve(image) };
		cv::Mat denoisedImage(image.rows, image.cols, image.type(), denoisedImageArray.data());

		lowPassImageArray = kernel.convolve(denoisedImage);

	}
	else {
		lowPassImageArray = kernel.convolve(image);

	}

	// High pass image
	std::vector<int> highPassImageArray(image.cols * image.rows * image.channels());

	for (int i{ 0 }; i < image.rows * image.cols * image.channels(); ++i) {
		highPassImageArray[i] = static_cast<int>(image.data[i]) - static_cast<int>(lowPassImageArray[i]);
	}

	// Enhanced image
	std::vector<unsigned char> enhancedImageArray(image.cols * image.rows * image.channels());

	for (int i{ 0 }; i < image.rows * image.cols * image.channels(); ++i) {
		float enhanced = static_cast<float>(image.data[i]) + highPassImageArray[i] * detailAmplification;
		enhancedImageArray[i] = static_cast<unsigned char>(std::clamp(enhanced, 0.0f, 255.0f));
	}

	cv::Mat enhancedImage(image.rows, image.cols, image.type(), enhancedImageArray.data());
	downloadImage(enhancedImage, fileName);

}

void downloadImage(const cv::Mat& image,const std::string& fileName) {

	fs::path downloadPath{ fs::path(getenv("USERPROFILE")) / "Downloads" };

	// Check if downlaod path exist
	if (!fs::exists(downloadPath)) {
		std::cout << "download folder was not found";
		return;
	}

	fs::path fullPath{ downloadPath / fileName };
	std::string fileStem{ fullPath.stem().string()};
	std::string fileExtenstion{ fullPath.extension().string() };
	
	int counter{ 0 };

	while (fs::is_regular_file(fullPath)) {
		++counter;
		std::string newFileName{ fileStem + "(" + std::to_string(counter) + ")" + fileExtenstion };
		fullPath = downloadPath / newFileName;

	}

	bool success{ cv::imwrite(fullPath.string(), image) };

	if (success) {
		std::cout << "Image downloaded into : " << fullPath.string() << "\n";
	}
	else {
		std::cerr << "Failed to download image" << "\n";
	}


}