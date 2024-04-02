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
#include <filesystem> //�����ҪC++17,�Ҽ�OurCode������->����->C++��׼��ΪC++17������


using namespace std;
using namespace cv;

namespace Files
{
	// ʹ�ÿ���̨��ȡ��ʽΪformat���ļ���
	string getFileName(string fileInfo, string format);

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

	// ����������ļ���
	void create_or_clear_directory(const std::string& dir);

	// ɾ��ָ��Ŀ¼�µ�format�ļ�
	void delete_files_with_format(string format, string path);

	// ͼƬ����Ƶ���ת��
	bool ImgToVideo(std::string imageFolderPath, std::string outputVideoPath, float time,int FPS, int whitefps);



	bool isFrameWhite(const cv::Mat& frame, int threshold, double checkRatio);

	/// <summary>
	///   ʵ�ֶ�ά����ȡ
	/// </summary>
	/// <param name="videoPath">
	///   ָ������Ƶ·��
	/// </param>
	/// <param name="outputPath">
	///   ָ�����ļ���·�������򴴽�
	/// </param>
	/// <param name="samplingRatio">
	///   ֡�����������ı���������isFrameWhite�У�Ĭ��Ϊ0.1
	/// </param>
	/// <param name="RGBThreshold">
	///   �ж��Ƿ�Ϊ��ɫ�ķ�����ֵ������isFrameWhite�У�Ĭ��Ϊ200
	/// </param>
	/// <returns>
	///   �Ƿ�ɹ���ȡ��ά�룬һ���߼�ֵ
	/// </returns>
	bool FrameExtractor(const std::string& videoPath, const std::string& outputPath, double samplingRatio = 0.1, int RGBThreshold = 200);

}


