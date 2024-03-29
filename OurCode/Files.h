#pragma once
#include <io.h>
#include <direct.h>
#include <iostream>
#include <fstream>
#include <random>
#include <bitset>
#include "Basic.h"
#include <bitset>
#include <vector>
#include <unordered_map>
#include <cmath>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

namespace Files
{

	// ����CRC�������
	using Code = std::vector<std::bitset<16>>;
	using CRCCode = std::vector<std::bitset<21>>;
	const int table[17] = { 0, 0xc, 0x6, 0x3, 0x13, 0x1b, 0x1f, 0x1d, 0x1c, 0xe, 0x7, 0x11, 0x1a, 0xd, 0x14, 0xa, 0x5 };

	CRCCode CRCEncodeHelper(Code data);
	std::vector<int> CRCEncode(std::vector<int> data);
	Code CRCDecodeHelper(CRCCode crcData);
	std::vector<int> CRCDecode(std::vector<int> crcData);


	// ��������������ļ��ĺ���
	void generateRandomBinaryFile(const std::string& filename, std::size_t size);

	// ��ȡ�������ļ�
	std::vector<int> readBinaryFile(const std::string& filename);

	// �����
	std::vector<int> fillData(std::vector<int>& datas);

	// ���������������
	void outBin(const std::vector<int>& bits, const std::string& filename);

	// �ļ��в���
	void create_or_clear_directory(const std::string& dir);
	void create_or_clear_directory(const std::string& dir, cv::Mat& img);

	// ͼƬ����Ƶ���ת��
	bool ImgToVideo(std::string imageFolderPath, std::string outputVideoPath, float time);

	bool VideoToImg(std::string vedioPath, std::string outputPath);



}


