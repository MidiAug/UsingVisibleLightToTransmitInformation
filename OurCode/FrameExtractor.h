#pragma once
#include <opencv2/opencv.hpp>
#include <iostream>
#include <filesystem> //这个需要C++17,右键OurCode，属性->常规->C++标准改为C++17及以上

bool isFrameWhite(const cv::Mat& frame, int threshold, double checkRatio);
bool FrameExtractor(const std::string& videoPath, const std::string& outputPath, double samplingRatio, int RGBThreshold);