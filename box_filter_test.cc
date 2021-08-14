#include <gtest/gtest.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>

#include <box_filter_simd.hh>

extern "C" {
    #include <box_filter.h>
}

const char* filename = "lena.pgm";

TEST(box_filter, optimal_check) 
{
    cv::Mat image = cv::imread(filename, CV_LOAD_IMAGE_ANYDEPTH);
    cv::Mat filtered_image_ref = cv::Mat::zeros(image.size[0], image.size[1], CV_8UC1);
    cv::Mat filtered_image_tested = cv::Mat::zeros(image.size[0], image.size[1], CV_8UC1);
    box_filter_5x5(image.data, filtered_image_ref.data, image.size[1], image.size[0]);
    box_filter_optimized_5x5(image.data, filtered_image_tested.data, image.size[1], image.size[0]);

    for(int i = 0; i < image.size[0] * image.size[1]; i++) {
        ASSERT_EQ(filtered_image_tested.data[i], filtered_image_ref.data[i]);
    }
}

TEST(box_filter, simd_check) 
{
    cv::Mat image = cv::imread(filename, CV_LOAD_IMAGE_ANYDEPTH);
    cv::Mat filtered_image_ref = cv::Mat::zeros(image.size[0], image.size[1], CV_8UC1);
    cv::Mat filtered_image_tested = cv::Mat::zeros(image.size[0], image.size[1], CV_8UC1);
    box_filter_5x5(image.data, filtered_image_ref.data, image.size[1], image.size[0]);
    box_filter_simd_5x5(image.data, filtered_image_tested.data, image.size[1], image.size[0]);

    for(int i = 0; i < image.size[0] * image.size[1]; i++) {
        ASSERT_EQ(filtered_image_tested.data[i], filtered_image_ref.data[i]);
    }
}

