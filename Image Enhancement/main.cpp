#include "GaussianKernel.h"
#include "ImageEnhancer.h"
#include<opencv2/opencv.hpp>
#include <iostream>
#include <filesystem>

struct ProgramArguments
{
	std::string image_path;
	float sigma;
	float detailAmplification;
	bool denoiseInputImage;
};

std::optional< ProgramArguments> parseArgs(int argc, char** argv);

int main(int argc, char** argv) {

	auto args{ parseArgs(argc,argv) };
	if (!args) {
		return 1;
	}

	std::cout <<"image path : " << args->image_path;

	// Kernel initialisation
	const GaussianKernel testingKernal{ args->sigma };

	std::filesystem::path pathObj(args->image_path);

	// Get just the filename
	std::string filename = pathObj.filename().string();

	cv::Mat image{ cv::imread(args->image_path, cv::IMREAD_COLOR) };
	if (image.empty()) {
		std::cerr << "Error: Could not load colored image at " << args->image_path << std::endl;
		return -1;
	}


	enhanceImage(image, testingKernal, filename);

}

std::optional< ProgramArguments> parseArgs(int argc, char** argv) {
	if (argc < 3 || argc > 5) {
		std::cerr << "Usage: " << argv[0]
			<< " \n<imagePath>(string) \n<sigma>(float) \n[detailAmp](float) \n[denoise](bool)\n";
		return std::nullopt;
	}

	ProgramArguments args{};

	try {
		args.image_path = argv[1];
		args.sigma = std::stof(argv[2]);

		if (argc == 4) {
			args.detailAmplification = std::stof(argv[3]);
		}

		if (argc == 5) {
			args.denoiseInputImage = (std::stoi(argv[4]) != 0);
		}

		if (args.sigma <= 0) {
			return std::nullopt;

		}
		return args;
	}
	catch (const std::exception& e) {
		std::cerr << "Error parsing arguments: " << e.what() << "\n";
	}
	
}

