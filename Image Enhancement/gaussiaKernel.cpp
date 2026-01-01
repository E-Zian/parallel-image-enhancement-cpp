#include "kernel.h"
#include <cmath>
#include <iostream>

GaussianKernel::GaussianKernel(float sigma)
    : m_sigma{ sigma },
    m_radius{ static_cast<int>(std::ceil(3 * sigma)) },
    m_kernalSize{ 2 * m_radius + 1 },
    m_kernel(m_kernalSize* m_kernalSize)
{
    generateKernel();
}

void GaussianKernel::displayKernel() {
    for (int i = 0; i < m_kernalSize; ++i) {
        for (int j = 0; j < m_kernalSize; ++j) {
            std::cout << m_kernel[i * m_kernalSize + j] << " ";
        }
        std::cout << std::endl;
    }
}

float GaussianKernel::unnormalizedGaussian(int x, int y) {
    return std::exp(-((x * x + y * y) / (2.0f * m_sigma * m_sigma)));
}

void GaussianKernel::generateKernel() {
    float sum = 0.0f;
    for (int row = -m_radius; row <= m_radius; ++row) {
        for (int col = -m_radius; col <= m_radius; ++col) {
            float value = unnormalizedGaussian(col, row);
            sum += value;
            m_kernel[(row + m_radius) * m_kernalSize + (col + m_radius)] = value;
        }
    }

    for (int i = 0; i < m_kernalSize * m_kernalSize; ++i) {
        m_kernel[i] /= sum;
    }
}