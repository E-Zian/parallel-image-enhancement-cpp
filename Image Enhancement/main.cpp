#include <iostream>
#include <chrono>
#include<opencv2/opencv.hpp>
#include "GaussianKernel.h"

int main() {
	using clock = std::chrono::high_resolution_clock;
	auto start = clock::now();
	
	// Kernel initialisation
	const GaussianKernel testingKernal{ 3.0f };
	std::cout << "Generated Gaussian Kernel\n";
	testingKernal.display2DKernel();
	std::cout << "Generated Gaussian 1D Kernel\n";

	testingKernal.display1DKernel();
	// Reading image
	std::string image_path = "C:/Users/User/Desktop/Egin/opencvTesting/lenna.png";

	cv::Mat image = cv::imread(image_path, cv::IMREAD_GRAYSCALE);

	if (image.empty()) {
		std::cerr << "Error: Could not load image at " << image_path << std::endl;
		return -1;
	}

	std::cout << "Image size: " << image.cols << " x " << image.rows << std::endl;

	// Convert to vector
	std::vector<unsigned char> inputImage(
		image.data,                       // pointer to first pixel
		image.data + image.total()          // pointer past last pixel
	);

	// Apply Gaussian Blur (Low Pass Filter)
	std::vector<unsigned char> denoisedImageArray = testingKernal.convolve(inputImage, image.cols, image.rows);
	std::vector<unsigned char> lowPassImageArray = testingKernal.convolve(denoisedImageArray, image.cols, image.rows);
	cv::Mat denoisedImage(image.rows, image.cols, CV_8UC1, denoisedImageArray.data());
	cv::Mat lowPassImage(image.rows, image.cols, CV_8UC1, lowPassImageArray.data());
	for (int i = 0; i < 10; ++i)
		std::cout << (int)denoisedImageArray[i] << " ";
	std::cout << "\n";

	// High pass image
	std::vector<int> highPassImageArray(image.cols * image.rows);
	for (int i = 0; i < image.rows * image.cols; ++i) {
		highPassImageArray[i] = static_cast<int>(image.data[i]) - static_cast<int>(lowPassImageArray[i]);
	}
	std::vector<unsigned char> highPassVisualliserArray(image.cols * image.rows);
	for (int i = 0; i < image.rows * image.cols; ++i) {
		highPassVisualliserArray[i] = static_cast<unsigned char>(std::clamp(highPassImageArray[i] + 127, 0, 255));
	}
	cv::Mat highPassImage(image.rows, image.cols, CV_8UC1, highPassVisualliserArray.data());

	float k = 1.0f;
	std::vector<unsigned char> enhancedImageArray(image.cols * image.rows);
	for (int i = 0; i < image.rows * image.cols; ++i) {
		float enhanced = static_cast<float>(image.data[i]) + highPassImageArray[i] * k;
		enhancedImageArray[i] = static_cast<unsigned char>(std::clamp(enhanced, 0.0f, 255.0f));
	}
	cv::Mat enhancedImage(image.rows, image.cols, CV_8UC1, enhancedImageArray.data());

	// Display images
	cv::namedWindow("Original Image", cv::WINDOW_NORMAL);
	cv::namedWindow("Blurred Image", cv::WINDOW_NORMAL);
	cv::namedWindow("LowPass Image", cv::WINDOW_NORMAL);

	cv::namedWindow("Image Edges", cv::WINDOW_NORMAL);
	cv::namedWindow("Enhanced Image", cv::WINDOW_NORMAL);
	cv::imshow("Original Image", image);
	cv::imshow("Blurred Image", denoisedImage);
	cv::imshow("LowPass Image", lowPassImage);

	cv::imshow("Image Edges", highPassImage);
	cv::imshow("Enhanced Image", enhancedImage);


	auto end = clock::now();
	std::chrono::duration<double, std::milli> elapsed = end - start;

	std::cout << "Time: " << elapsed.count() << " ms\n";
	cv::waitKey(0);


}


