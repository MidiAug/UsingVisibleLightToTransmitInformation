#pragma once
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>

namespace ImgToVedio{
    /*
    * ���ܣ�ͼ��ת��Ƶ
    * - imageFolderPath:ͼƬ�ļ���·��
    * - outputVideoPath:�����Ƶ·��
    * - time:ʱ��(����)
    */
    bool Image2Video(std::string imageFolderPath, std::string outputVideoPath, float time);
}