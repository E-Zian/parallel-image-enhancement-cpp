
#ifndef GAUSSIAN_KERNEL_H
#define GAUSSIAN_KERNEL_H

#include <vector>
#include<opencv2/opencv.hpp>

class GaussianKernel
{
public:
    explicit GaussianKernel(float sigma = 1);
    void display2DKernel() const;
    void display1DKernel() const;

	// Apply convolution to an input image
    std::vector<unsigned char> convolve(const cv::Mat& image) const;

private:
    float m_sigma;
    int m_radius;
    int m_kernelDiameter;
    int m_kernelSize;

    std::vector<float> m_kernel;
    std::vector<float> m_kernel1D;

    std::vector<unsigned char> convolveGray(const std::vector<unsigned char>& inputImage, int width, int height) const;

    std::vector<unsigned char> convolveColored(const std::vector<unsigned char>& inputImage, int width, int height) const;

    float unnormalizedGaussian(int x, int y);
    void generate2DKernel();
	void generate1DKernels();
};



#endif 
