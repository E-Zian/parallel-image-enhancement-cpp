#include <iostream>
#include <chrono>
#include<opencv2/opencv.hpp>
#include "GaussianKernel.h"
#include "ImageEnhancer.h"


int main() {
	
	// Kernel initialisation
	const GaussianKernel testingKernal{ 5.0f };

	// Reading image
	std::string image_path{ "C:/Users/User/Desktop/Egin/opencvTesting/lenna.png" };

	//cv::Mat image = cv::imread(image_path, cv::IMREAD_GRAYSCALE);
	//if (image.empty()) {
	//	std::cerr << "Error: Could not load image at " << image_path << std::endl;
	//	return -1;
	//}

	cv::Mat image{ cv::imread(image_path, cv::IMREAD_COLOR) };
	if (image.empty()) {
		std::cerr << "Error: Could not load colored image at " << image_path << std::endl;
		return -1;
	}

	
	enhanceImage(image, testingKernal);

}


