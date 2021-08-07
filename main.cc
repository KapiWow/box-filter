#include <iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>

extern "C" {
    #include <box_filter.h>
}

int main() {
    cv::Mat image = cv::imread("lena.pgm");
    cv::Mat filtered_image = cv::Mat::zeros(image.size[1], image.size[0], CV_8UC1);
    box_filter_5x5(image.data, filtered_image.data, image.size[1], image.size[0]);
    return 0;
}
