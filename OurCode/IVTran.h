#pragma once
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include "Basic.h"

/*
* 功能：图像转视频
* - imageFolderPath:图片文件夹路径
* - outputVideoPath:输出视频路径
* - time:时间(毫秒)
*/
bool ImgToVideo(std::string imageFolderPath, std::string outputVideoPath, float time);

bool VideoToImg(std::string vedioPath, std::string outputPath);
