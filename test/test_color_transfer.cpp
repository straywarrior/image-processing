/*
 * test_color_transfer.cpp
 * @Author:     StrayWarrior
 * @Date:       2016-12-03
 * @Email:      i@straywarrior.com
 * Copyright (C) 2016 StrayWarrior
 *
 */


#include "color_transfer.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>

void testConvertColorRGB2LAlphaBeta(const std::string &_filename) {
#if HAVE_OPENCV
    cv::Mat source_image = cv::imread(_filename);
    cv::Mat lab_image, rgb_image;
    stray::convertColor(source_image, lab_image, stray::COLOR_BGR2LAlphaBeta);
    stray::convertColor(lab_image, rgb_image, stray::COLOR_LAlphaBeta2BGR);
    cv::imwrite(_filename + ".result.png", rgb_image);
#endif /* HAVE_OPENCV */
}

void testColorTransfer(const std::string &_ref_image, const std::string &_target_image) {
#if HAVE_OPENCV
    cv::Mat source_image = cv::imread(_ref_image);
    cv::Mat target_image = cv::imread(_target_image);
    stray::colorTransfer(source_image, target_image);
    cv::imwrite(_target_image + ".result.png", target_image);
#endif /* HAVE_OPENCV */
}

int main(int argc, char * argv[]) {
    testColorTransfer("test_color_transfer.ref.png", "test_color_transfer.target.png");
    return 0;
}


