#pragma once
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include "Basic.h"

/*
* ���ܣ�ͼ��ת��Ƶ
* - imageFolderPath:ͼƬ�ļ���·��
* - outputVideoPath:�����Ƶ·��
* - time:ʱ��(����)
*/
bool ImgToVideo(std::string imageFolderPath, std::string outputVideoPath, float time);

bool VideoToImg(std::string vedioPath, std::string outputPath);
