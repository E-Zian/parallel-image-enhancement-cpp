#include <iostream>
#include <chrono>
#include<opencv2/opencv.hpp>
#include "gaussianKernel.h"

int main() {
	using clock = std::chrono::high_resolution_clock;
	auto start = clock::now();
	
	// Kernel initialisation
	const GaussianKernel testingKernal{ 2.0f };
	testingKernal.displayKernel();
	
	// Reading image
	std::string image_path = "C:/Users/User/Desktop/Egin/opencvTesting/trees.jpg";

	cv::Mat image = cv::imread(image_path, cv::IMREAD_GRAYSCALE);

	if (image.empty()) {
		std::cerr << "Error: Could not load image at " << image_path << std::endl;
		return -1;
	}
	uchar* inputImage = image.data;

	std::cout << "Image size: " << image.cols << " x " << image.rows << std::endl;

	// Low pass image
	std::vector<unsigned char> lowPassImageArray(image.cols * image.rows);

	testingKernal.convolve(inputImage, lowPassImageArray.data(), image.cols, image.rows);

	cv::Mat lowPassImage(image.rows, image.cols, CV_8UC1, lowPassImageArray.data());

	// High pass image
	std::vector<int> highPassImageArray(image.cols * image.rows);
	for (int i = 0; i < image.rows* image.cols; ++i) {
		highPassImageArray[i] = static_cast<int>(image.data[i]) - static_cast<int>(lowPassImageArray[i]);
	}
	std::vector<unsigned char> highPassVisualliserArray(image.cols * image.rows);
	for (int i = 0; i < image.rows * image.cols; ++i) {
		highPassVisualliserArray[i] = static_cast<unsigned char>(std::clamp(highPassImageArray[i] + 127, 0, 255));
	}
	cv::Mat highPassImage(image.rows, image.cols, CV_8UC1, highPassVisualliserArray.data());


	// Enhanced image (original + high pass*k)
	float k = 2.0f;
	std::vector<unsigned char> enhancedImageArray(image.cols * image.rows);
	for (int i = 0; i < image.rows * image.cols; ++i) {
		float enhanced = static_cast<float>(image.data[i]) + highPassImageArray[i] * k;
		enhancedImageArray[i] = static_cast<unsigned char>(std::clamp(enhanced, 0.0f, 255.0f));
	}
	cv::Mat enhancedImage(image.rows, image.cols, CV_8UC1, enhancedImageArray.data());

	// Display images
	cv::namedWindow("Original Image", cv::WINDOW_NORMAL);
	cv::namedWindow("Blurred Image", cv::WINDOW_NORMAL);
	cv::namedWindow("Image Edges", cv::WINDOW_NORMAL);
	cv::namedWindow("Enhanced Image", cv::WINDOW_NORMAL);
	cv::imshow("Original Image", image);
	cv::imshow("Blurred Image", lowPassImage);
	cv::imshow("Image Edges", highPassImage);
	cv::imshow("Enhanced Image", enhancedImage);


	auto end = clock::now();
	std::chrono::duration<double, std::milli> elapsed = end - start;

	std::cout << "Time: " << elapsed.count() << " ms\n";
	cv::waitKey(0);


}
