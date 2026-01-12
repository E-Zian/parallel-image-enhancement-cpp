## Gaussian filtering image enhancement using standard thread library
This is a C++ implementation of spatial-domain image enhancement using Gaussian filtering, with multithreading via the standard thread library. Multithreading is used to reduce the execution time of the image convolution by parallelizing the computation across multiple threads.

## Dependencies

This project requires [OpenCV](https://opencv.org/).  

- Make sure OpenCV is installed on your system.
- Add OpenCV `include` and `lib` directories to your Visual Studio project.
- Add the OpenCV `bin` directory (DLLs) to your system PATH if running the program outside VS.

## How to Run

**Parameters:**
- `<imagePath>` : Path to the input image (e.g., `C:\Images\lenna.png`)
- `<sigma>` : Value used for calculation of the kernel (float), e.g., `1.0`
- `<grayScale> *optional` : Reads the image as grayscale (bool), e.g., `1`
- `<detailAmp> *optional` : Enhancement factor, used as the k value for high pass (float), e.g., `1.0` 
- `<denoise> *optional` : Apply initial blur for denoising before getting low pass (boolean), e.g., `1` 

**Example:**

When the build is included into the enviromental path variables
- `C:\> image_enhancement "C:\{path}\{image-name}.{extenstion-name}" 5.0`
- `C:\> image_enhancement "C:\{path}\{image-name}.{extenstion-name}" 3.0 1 1.5 `

## Result

Results may be adjusted by changing the sigma and detailAmp values
<table>
<tr>
<td>Original</td>
<td>Enhanced</td>
<td>Enhanced Grayscale</td>
</tr>
<tr>
<td><img src="https://github.com/user-attachments/assets/0d28d8cc-c375-46ad-8268-240f083b71de" width="200"/></td>
<td><img src="https://github.com/user-attachments/assets/a06f8bbc-b7e2-4f45-8db1-ffe6587256ed" width="200"/></td>
<td><img src="https://github.com/user-attachments/assets/a77d6081-9f14-49aa-ae1e-8b12964f7ce9" width="200"/></td>
</tr>
</table>

