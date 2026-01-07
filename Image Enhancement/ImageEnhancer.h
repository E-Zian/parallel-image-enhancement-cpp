#ifndef IMAGE_ENHANCER
#define IMAGE_ENHANCER

void enhanceImage(const cv::Mat& image, const GaussianKernel& kernel, bool applyBlur = false, float detailAmplification = 1.0);

#endif IMAGE_ENHANCER
