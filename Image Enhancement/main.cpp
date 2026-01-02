#include <iostream>
#include<opencv2/opencv.hpp>
#include "kernel.h"

int main() {
	std::cout << "Generating Gaussian Kernel with sigma = 1.0\n";
	
	const GaussianKernel testingKernal{ 1.0f };
	testingKernal.displayKernel();

	std::string image_path = "C:/Users/User/Desktop/Egin/opencvTesting/testingIMG.jpeg";

	cv::Mat image = cv::imread(image_path, cv::IMREAD_GRAYSCALE);

	if (image.empty()) {
		std::cerr << "Error: Could not load image at " << image_path << std::endl;
		return -1;
	}

	//cv::imshow("Original Image", image);
	//cv::waitKey(0);

	uchar* imageByteArray = image.data;         
	size_t numBytes = image.total() * image.elemSize(); 
	std::cout << "Image size: " << image.cols << " x " << image.rows << std::endl;
	std::cout << "Total bytes: " << numBytes << std::endl;

	 //Example: print first 10 pixel values
	for (int i = 0; i < 10; i++) {
		std::cout << (int)imageByteArray[i] << " ";
	}
	std::cout << std::endl;
}