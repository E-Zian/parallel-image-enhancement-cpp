
#ifndef GAUSSIAN_KERNEL_H
#define GAUSSIAN_KERNEL_H

#include <vector>

class GaussianKernel
{
public:
    explicit GaussianKernel(float sigma = 1);

    void display2DKernel() const;
    void display1DKernel() const;

	// Apply convolution to an input image
    std::vector<unsigned char> convolve(const std::vector<unsigned char>& inputImage, int width, int height) const;

private:
    float m_sigma;
    int m_radius;
    int m_kernelDiameter;
    int m_kernelSize;

    std::vector<float> m_kernel;
    std::vector<float> m_kernel1D;

    
    float unnormalizedGaussian(int x, int y);
    void generate2DKernel();
	void generate1DKernels();
};



#endif 
