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
#include <filesystem> //这个需要C++17,右键OurCode，属性->常规->C++标准改为C++17及以上


using namespace std;
using namespace cv;

namespace Files
{
	// 使用控制台获取格式为format的文件名
	string getFileName(string fileInfo, string format);

	// 数据CRC编码解码
	using Code = std::vector<std::bitset<16>>;
	using CRCCode = std::vector<std::bitset<21>>;
	const int table[17] = { 0, 0xc, 0x6, 0x3, 0x13, 0x1b, 0x1f, 0x1d, 0x1c, 0xe, 0x7, 0x11, 0x1a, 0xd, 0x14, 0xa, 0x5 };

	CRCCode CRCEncodeHelper(Code data);
	std::vector<int> CRCEncode(std::vector<int> data);
	Code CRCDecodeHelper(CRCCode crcData);
	std::vector<int> CRCDecode(std::vector<int> crcData);


	// 生成随机二进制文件的函数
	void generateRandomBinaryFile(const std::string& filename, std::size_t size);

	// 读取二进制文件
	std::vector<int> readBinaryFile(const std::string& filename);

	// 填补数据
	std::vector<int> fillData(std::vector<int>& datas);

	// 将二进制数据输出
	void outBin(const std::vector<int>& bits, const std::string& filename);

	// 创建或清空文件夹
	void create_or_clear_directory(const std::string& dir);

	// 删除指定目录下的format文件
	void delete_files_with_format(string format, string path);

	// 图片与视频间的转换
	bool ImgToVideo(std::string imageFolderPath, std::string outputVideoPath, float time,int FPS, int whitefps);



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
	bool FrameExtractor(const std::string& videoPath, const std::string& outputPath, double samplingRatio = 0.1, int RGBThreshold = 200);

}


