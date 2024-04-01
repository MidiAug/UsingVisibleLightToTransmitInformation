#pragma once
#include <opencv2/opencv.hpp>
#include <iostream>
#include <filesystem> //�����ҪC++17,�Ҽ�OurCode������->����->C++��׼��ΪC++17������

bool isFrameWhite(const cv::Mat& frame, int threshold, double checkRatio);
bool FrameExtractor(const std::string& videoPath, const std::string& outputPath, double samplingRatio, int RGBThreshold);