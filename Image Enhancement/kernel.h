
#ifndef KERNEL_H
#define KERNEL_H

#include <vector>

std::vector<float> generateKernel(float sigma);
void displayKernel(const std::vector<float>& kernel);

#endif 
