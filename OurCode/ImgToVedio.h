#pragma once
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>

namespace ImgToVedio{
    /*
    * 功能：图像转视频
    * - imageFolderPath:图片文件夹路径
    * - outputVideoPath:输出视频路径
    * - time:时间(毫秒)
    */
    bool Image2Video(std::string imageFolderPath, std::string outputVideoPath, float time);
}