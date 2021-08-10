#include <iostream>
#include <chrono>
#include <string>

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>

extern "C" {
    #include <box_filter.h>
}

using box_func = std::function<void(unsigned char*, unsigned char*, int, int)>;

void benchmark(box_func&& func, cv::Mat& image, const char* name, bool is_write = false) {
    using namespace std::chrono;

    cv::Mat filtered_image = cv::Mat::zeros(image.size[0], image.size[1], CV_8UC1);
    const int iter_count = 100;
    std::vector<int> time_values;
    time_values.reserve(iter_count);

    for (int i = 0; i < iter_count; i++) {
        high_resolution_clock::time_point _t0{high_resolution_clock::now()};
        func(image.data, filtered_image.data, image.size[1], image.size[0]);
        high_resolution_clock::time_point _t1{high_resolution_clock::now()};
        int dt = duration_cast<microseconds>(_t1 - _t0).count();
        time_values.push_back(dt);
    }
    std::sort(time_values.begin(), time_values.end());
    std::cout << name << ", 50%:     " << time_values[iter_count / 2] << std::endl;
    std::cout << name << ", 10%:     " << time_values[iter_count / 10] << std::endl;
    std::cout << name << ", 90%:     " << time_values[iter_count * 9 / 10] << std::endl;
    std::cout << "========================================" << std::endl;
    
    if (is_write) {
        cv::imwrite("res.pgm", filtered_image);
    }
}

int main(int argc, char** argv) {
    std::string filename;
    if (argc != 2) {
        filename = "lena.pgm";
    } else {
        filename = argv[1];
    }

    cv::Mat image = cv::imread(filename, CV_LOAD_IMAGE_ANYDEPTH);
    if (image.data == nullptr) {
        std::cout << "image wasn't found" << std::endl;
        return 0;
    }

    benchmark(box_filter_5x5, image, "initial version");
    benchmark(box_filter_optimized_5x5, image, "optimized version", true);

    return 0;
}
