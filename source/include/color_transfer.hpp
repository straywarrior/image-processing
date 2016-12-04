/*
 * color_transfer.hpp
 * @Author:     StrayWarrior
 * @Date:       2016-12-03
 * @Email:      i@straywarrior.com
 * Copyright (C) 2016 StrayWarrior
 *
 */

#ifndef _COLOR_TRANSFER_HPP_
#define _COLOR_TRANSFER_HPP_

#if HAVE_OPENCV
#include <opencv2/opencv.hpp>
#endif /* HAVE_OPENCV */

/*!
@defgroup color_transfer Color Transfer functionality
*/

//! @addtogroup color_transfer
//! @{

//! @brief main namespace for my project
namespace stray {

//! @brief The codes of color convertion type.
enum ColorConvertionCodes { 
    COLOR_BGR2LAlphaBeta = 0,
    COLOR_LAlphaBeta2BGR = 1
};
#if HAVE_OPENCV

/*! @brief Convert the color space of a image.
 *
 * It's similar to cv::cvtColor()
 * @param _src input array of the image.
 * @param _out output array of the image.
 * @param _mode color convertion code.
 * @see ColorConvertionCodes
*/
void convertColor(cv::InputArray _src, cv::OutputArray _out, int _mode);

/*! @brief Transfer color of an image to another image (Color Match).
 *
 * @param _ref input array of the image that will be referref to.
 * @param _target input-output array of the image that will be transfered.
*/
void colorTransfer(cv::InputArray _ref, cv::InputOutputArray _target);

#endif /* HAVE_OPENCV */

}
//! @}
#endif /* _COLOR_TRANSFER_HPP_ */
