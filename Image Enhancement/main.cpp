#include <iostream>
#include "kernel.h"

int main() {
	std::cout << "Generating Gaussian Kernel with sigma = 1.0\n";
	displayKernel(generateKernel(1.0f));
}