﻿#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>

cv::Mat getImageOfHistogram(const cv::Mat& image);
void colorReduce(const cv::Mat& inputImage, cv::Mat& outputImage, int div = 64);
void colorReverse(const cv::Mat& inputImage, cv::Mat& outputImage);
void morphologyOption(const cv::Mat& input, cv::Mat& output, const int morphOperator, const int morphElem = 0, const int mrophSize = 1);

#endif // MAINWINDOW_H

