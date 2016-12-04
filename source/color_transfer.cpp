/*
 * color_transfer.cpp
 * @Author:     StrayWarrior
 * @Date:       2016-12-03
 * @Email:      i@straywarrior.com
 * Copyright (C) 2016 StrayWarrior
 *
 * Distributed under terms of the MIT license.
 */

#include "color_transfer.hpp"
#include <cmath>

namespace stray {

#if HAVE_OPENCV

namespace {

template <typename T>
T log10_wrapper(T x) {
    if (x - 0.0 < 1e-6)
        return 0;
    else
        return log10(x);
}

template <typename T>
uint8_t uint8_wrapper(T x) {
    if (x < 0)
        return 0;
    else if (x > 255)
        return 255;
    else
        return x;
}

void BGR2LAlphaBeta(cv::InputArray _src, cv::OutputArray _dst) {
    cv::Mat src = _src.getMat();
    cv::Mat dst = _dst.getMat();

    assert(src.channels() == 3);

    if (dst.size() != src.size() || dst.type() != CV_32FC3) {
        _dst.create(src.size(), CV_32FC3);
        dst = _dst.getMat();
    }

    typedef cv::Point3_<uint8_t> Pixel;
    typedef cv::Point3_<float> PixelF;
    uint8_t R, G, B;
    float L, M, S;
    for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
            Pixel & p1 = src.at<Pixel>(i, j);
            B = p1.x;
            G = p1.y;
            R = p1.z;
            L = 0.3811 * R + 0.5783 * G + 0.0402 * B;
            M = 0.1967 * R + 0.7244 * G + 0.0782 * B;
            S = 0.0241 * R + 0.1288 * G + 0.8444 * B;
            L = log10_wrapper<float>(L);
            M = log10_wrapper<float>(M);
            S = log10_wrapper<float>(S);

            PixelF & p2 = dst.at<PixelF>(i, j);
            p2.x = 0.577350269189626 * L + 0.577350269189626 * M + 0.577350269189626 * S;
            p2.y = 0.408248290463863 * L + 0.408248290463863 * M - 0.816496580927726 * S;
            p2.z = 0.707106781186548 * L - 0.707106781186548 * M;
        }
    }
}
    
void LAlphaBeta2BGR(cv::InputArray _src, cv::OutputArray _dst) {
    cv::Mat src = _src.getMat();
    cv::Mat dst = _dst.getMat();

    assert(src.channels() == 3);

    if (dst.size() != src.size() || dst.type() != CV_8UC3) {
        _dst.create(src.size(), CV_8UC3);
        dst = _dst.getMat();
    }

    typedef cv::Point3_<uint8_t> Pixel;
    typedef cv::Point3_<float> PixelF;
    float l, a, b;
    float L, M, S;
    for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
            PixelF & p1 = src.at<PixelF>(i, j);
            l = p1.x;
            a = p1.y;
            b = p1.z;
            L = 0.577350269189626 * l + 0.408248290463863 * a + 0.707106781186548 * b;
            M = 0.577350269189626 * l + 0.408248290463863 * a - 0.707106781186548 * b;
            S = 0.577350269189626 * l - 0.816496580927726 * a;
            L = pow(10, L);
            M = pow(10, M);
            S = pow(10, S);

            Pixel & p2 = dst.at<Pixel>(i, j);
            p2.z = uint8_wrapper(4.4679 * L - 3.5873 * M + 0.1193 * S);   // R
            p2.y = uint8_wrapper(-1.2186 * L + 2.3809 * M - 0.1624 * S);  // G
            p2.x = uint8_wrapper(0.0497 * L - 0.2439 * M + 1.2045 * S);   // B
        }
    }
}

} /* anonymous namespace */

void convertColor(cv::InputArray _src, cv::OutputArray _dst, int _mode) {
    typedef void (*func_t)(cv::InputArray /* src */, cv::OutputArray /* dst */);
    static const func_t funcs[] = {
        BGR2LAlphaBeta,
        LAlphaBeta2BGR
    };

    assert(0 <= _mode && _mode < 2);

    func_t func = funcs[_mode];
    
    func(_src, _dst);
}

void colorTransfer(cv::InputArray _ref, cv::InputOutputArray _target) {
    cv::Mat ref = _ref.getMat();
    cv::Mat target = _target.getMat();

    CV_Assert(ref.type() == target.type());
    CV_Assert(ref.channels() == 3 && ref.depth() == CV_8U);

    // Transfer the color space to lab space.
    cv::Mat ref_lab, target_lab;
    convertColor(ref, ref_lab, COLOR_BGR2LAlphaBeta);
    convertColor(target, target_lab, COLOR_BGR2LAlphaBeta);

    // Compute the mean and standard deviations.
    cv::Scalar ref_means, target_means;
    cv::Scalar ref_stddevs, target_stddevs;
    cv::meanStdDev(ref_lab, ref_means, ref_stddevs);
    cv::meanStdDev(target_lab, target_means, target_stddevs);

    // Match the mean and standard deviations
    typedef cv::Point3_<float> PixelF;
    for (int i = 0; i < target_lab.rows; ++i) {
        for (int j = 0; j < target_lab.cols; ++j) {
            PixelF & p1 = target_lab.at<PixelF>(i, j);
            p1.x = target_stddevs[0] / ref_stddevs[0] * (p1.x - target_means[0]) + ref_means[0];
            p1.y = target_stddevs[1] / ref_stddevs[1] * (p1.y - target_means[1]) + ref_means[1];
            p1.z = target_stddevs[2] / ref_stddevs[2] * (p1.z - target_means[2]) + ref_means[2];
        }
    }
    
    // Transfer the color space to lab space
    convertColor(target_lab, target, COLOR_LAlphaBeta2BGR);
}

#endif /* HAVE_OPENCV */

}
