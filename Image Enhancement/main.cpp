#include <iostream>
#include<opencv2/opencv.hpp>
#include "kernel.h"

int main() {
	std::cout << "Generating Gaussian Kernel with sigma = 1.0\n";
	
	// Kernel initialisation
	const GaussianKernel testingKernal{ 1.0f };
	testingKernal.displayKernel();
	
	// Reading image
	std::string image_path = "C:/Users/User/Desktop/Egin/opencvTesting/testingIMG.jpeg";

	cv::Mat image = cv::imread(image_path, cv::IMREAD_GRAYSCALE);

	if (image.empty()) {
		std::cerr << "Error: Could not load image at " << image_path << std::endl;
		return -1;
	}
	uchar* inputImage = image.data;

	// Output image
	std::vector<unsigned char> outputImage(image.cols * image.rows);

	std::cout << "Image size: " << image.cols << " x " << image.rows << std::endl;

	// Convolving
	testingKernal.convolve(inputImage, outputImage.data(), image.cols, image.rows);

	cv::Mat outputMat(image.rows, image.cols, CV_8UC1, outputImage.data());

	// Display images
	cv::imshow("Original Image", image);
	cv::imshow("Blurred Image", outputMat);
	cv::waitKey(0);


}