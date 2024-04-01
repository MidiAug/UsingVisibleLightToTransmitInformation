#pragma once
#include <opencv2/opencv.hpp>
#include <iostream>
#include <filesystem> //这个需要C++17,右键OurCode，属性->常规->C++标准改为C++17及以上

/// <summary>
///   判断是否为白帧，通过检查中心区域的白色像素是否到达一定比例实现
/// </summary>
/// <param name="frame">
///   当前帧
/// </param>
/// <param name="threshold">
///   判断是否为白色像素的阈值，如当设置为250时表示三个分量均超过250的像素被认定为白色像素
/// </param>
/// <param name="checkRatio"></param>
///   当前帧抽样检查区域的比例，当设置为0.1时，表示抽取当前帧中心0.1*0.1比例的区域检查是否为白色像素
/// <returns>
///   是否为白帧，一个布尔值
/// </returns>
bool isFrameWhite(const cv::Mat& frame, int threshold, double checkRatio);

/// <summary>
///   实现二维码提取
/// </summary>
/// <param name="videoPath">
///   指定的视频路径
/// </param>
/// <param name="outputPath">
///   指定的文件夹路径，无则创建
/// </param>
/// <param name="samplingRatio">
///   帧抽样检查区域的比例，传入isFrameWhite中，默认为0.1
/// </param>
/// <param name="RGBThreshold">
///   判断是否为白色的分量阈值，传入isFrameWhite中，默认为200
/// </param>
/// <returns>
///   是否成功抽取二维码，一个逻辑值
/// </returns>
bool FrameExtractor(const std::string& videoPath, const std::string& outputPath, double samplingRatio, int RGBThreshold);