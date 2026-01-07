#include <iostream>
#include <chrono>
#include<opencv2/opencv.hpp>
#include "GaussianKernel.h"
void enhanceImageGrayScale(const cv::Mat& image, const GaussianKernel& kernel);
void enhacneImageColored(const cv::Mat& image, const GaussianKernel& kernel);
void enhanceImage(const cv::Mat& image, const GaussianKernel& kernel);
int main() {

	
	// Kernel initialisation
	const GaussianKernel testingKernal{ 5.0f };

	// Reading image
	std::string image_path = "C:/Users/User/Desktop/Egin/opencvTesting/lenna.png";

	//cv::Mat image = cv::imread(image_path, cv::IMREAD_GRAYSCALE);
	//if (image.empty()) {
	//	std::cerr << "Error: Could not load image at " << image_path << std::endl;
	//	return -1;
	//}

	cv::Mat image = cv::imread(image_path, cv::IMREAD_COLOR);

	if (image.empty()) {
		std::cerr << "Error: Could not load colored image at " << image_path << std::endl;
		return -1;
	}

	
	enhanceImage(image, testingKernal);

}
void enhanceImage(const cv::Mat& image, const GaussianKernel& kernel) {
	if (image.channels() == 3) {
		enhacneImageColored(image, kernel);

	}
	else {
		enhanceImageGrayScale(image, kernel);
	}
}

void enhacneImageColored(const cv::Mat& image, const GaussianKernel& kernel)
{
	using clock = std::chrono::high_resolution_clock;
	auto start = clock::now();
	std::cout << "Image size: " << image.cols << " x " << image.rows << std::endl;


	// Apply Gaussian Blur (Low Pass Filter)
	std::vector<unsigned char> denoisedImageArray = kernel.convolve(image, image.cols, image.rows);
	cv::Mat denoisedImage(image.rows, image.cols, CV_8UC3, denoisedImageArray.data());

	std::vector<unsigned char> lowPassImageArray = kernel.convolve(denoisedImage, image.cols, image.rows);
	cv::Mat lowPassImage(image.rows, image.cols, CV_8UC3, lowPassImageArray.data());


	// High pass image
	std::vector<int> highPassImageArray(image.cols * image.rows * image.channels());
	for (int i = 0; i < image.rows * image.cols * image.channels(); ++i) {
		highPassImageArray[i] = static_cast<int>(image.data[i]) - static_cast<int>(lowPassImageArray[i]);
	}
	std::vector<unsigned char> highPassVisualliserArray(image.cols * image.rows * image.channels());
	for (int i = 0; i < image.rows * image.cols * image.channels(); ++i) {
		highPassVisualliserArray[i] = static_cast<unsigned char>(std::clamp(highPassImageArray[i] + 127, 0, 255));
	}
	cv::Mat highPassImage(image.rows, image.cols, CV_8UC3, highPassVisualliserArray.data());

	float k = 1.0f;
	std::vector<unsigned char> enhancedImageArray(image.cols * image.rows * image.channels());
	for (int i = 0; i < image.rows * image.cols * image.channels(); ++i) {
		float enhanced = static_cast<float>(image.data[i]) + highPassImageArray[i] * k;
		enhancedImageArray[i] = static_cast<unsigned char>(std::clamp(enhanced, 0.0f, 255.0f));
	}
	cv::Mat enhancedImage(image.rows, image.cols, CV_8UC3, enhancedImageArray.data());

	// Display images

	cv::imshow("Original Image", image);
	cv::imshow("LowPass Image", lowPassImage);

	cv::imshow("Image Edges", highPassImage);
	cv::imshow("Enhanced Image", enhancedImage);


	auto end = clock::now();
	std::chrono::duration<double, std::milli> elapsed = end - start;

	std::cout << "Time: " << elapsed.count() << " ms\n";
	cv::waitKey(0);
}

void enhanceImageGrayScale(const cv::Mat& image,const GaussianKernel& kernel)
{
	using clock = std::chrono::high_resolution_clock;
	auto start = clock::now();
	std::cout << "Image size: " << image.cols << " x " << image.rows << std::endl;

	// Apply Gaussian Blur (Low Pass Filter)
	std::vector<unsigned char> denoisedImageArray = kernel.convolve(image, image.cols, image.rows);
	cv::Mat denoisedImage(image.rows, image.cols, CV_8UC1, denoisedImageArray.data());


	std::vector<unsigned char> lowPassImageArray = kernel.convolve(denoisedImage, image.cols, image.rows);

	//std::vector<unsigned char> lowPassImageArray = testingKernal.convolve(inputImage, image.cols, image.rows);
	cv::Mat lowPassImage(image.rows, image.cols, CV_8UC1, lowPassImageArray.data());


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


