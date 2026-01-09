#include "GaussianKernel.h"
#include "ImageEnhancer.h"
#include<opencv2/opencv.hpp>
#include <iostream>
#include <filesystem>

int main(int argc, char** argv) {
	if (argc <= 0) {
		std::cout << "parameter : imagePath(string), sigmaValue(float), detailAmplification(float)*, denoiseInputImage(bool)*";
	}

	//std::string image_path{ argv[0]};
	//float sigma{ std::stof(argv[1]) };	
	//float detailAmplification{ std::stof(argv[2]) };
	//bool denoiseInputImage{ std::stoi(argv[3]) };


	// Kernel initialisation
	const GaussianKernel testingKernal{ 5.0f };

	// Reading image
	std::string image_path{ "C:/Users/User/Desktop/Egin/opencvTesting/lenna.png" };

	std::filesystem::path pathObj(image_path);

	// Get just the filename
	std::string filename = pathObj.filename().string();

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

	
	enhanceImage(image, testingKernal, filename);

}


