
#ifndef KERNEL_H
#define KERNEL_H

#include <vector>

class GaussianKernel
{
public:
    GaussianKernel(float sigma = 1);
    void displayKernel();

private:
    float m_sigma;
    int m_radius;
    int m_kernalSize;
    std::vector<float> m_kernel;
    
    float unnormalizedGaussian(int x, int y);
    void generateKernel();
};



#endif 
