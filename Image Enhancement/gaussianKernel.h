
#ifndef GAUSSIAN_KERNEL_H
#define GAUSSIAN_KERNEL_H

#include <vector>

class GaussianKernel
{
public:
    explicit GaussianKernel(float sigma = 1);

    void displayKernel() const;

	// Apply convolution to an input image
	void convolve(const unsigned char* inputImage, unsigned char* outputImage, int width, int height) const;

private:
    float m_sigma;
    int m_radius;
    int m_kernelSize;
    std::vector<float> m_kernel;
    
    float unnormalizedGaussian(int x, int y);
    void generateKernel();
};



#endif 
