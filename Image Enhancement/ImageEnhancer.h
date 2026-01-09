#ifndef IMAGE_ENHANCER
#define IMAGE_ENHANCER

void enhanceImage(const cv::Mat& image, const GaussianKernel& kernel, std::string& fileName, float detailAmplification = 1.0, bool denoiseInputImage = false);

#endif 
