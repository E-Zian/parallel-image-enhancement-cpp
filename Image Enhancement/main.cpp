#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    std::cout << "OpenCV version: " << CV_VERSION << std::endl;

    // Create a 400x400 black image
    cv::Mat img = cv::Mat::zeros(400, 400, CV_8UC3);

    // Draw a red circle in the center
    cv::circle(img, cv::Point(200, 200), 100, cv::Scalar(0, 0, 255), -1);

    // Display the image
    cv::imshow("Test Image", img);

    std::cout << "Press any key in the window to exit..." << std::endl;
    cv::waitKey(0); // Wait for a key press

    return 0;
}