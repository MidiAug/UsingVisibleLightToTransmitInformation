#pragma once
#include<opencv2/opencv.hpp>
#include"Basic.h"
#include"Files.h"
using namespace cv;
using namespace std;

namespace Decode
{

	int extractCode(string& inFolderPath, string outFolderPath);
	Mat extractCodeHelper(Mat& src);
	bool isColorConflict(Vec3b* upperThresholds, Vec3b* lowerThresholds);
	//判断是否需要跳过
	bool jump(int curR, int curC);
	inline void judge1( cv::Vec3b thisPixel,int &val);
	inline void judge2( cv::Vec3b thisPixel, int &val1, int &val2);
	int readCodeHelper(int curCodeNo, vector<int>& data, Mat& image, vector<int>& origianl,int debugMode);
	int readCode(std::string extCodePath, vector<int>& data, vector<int>& origianl,int debugMode = 0);
	void showimg(Mat& img, float x, float y);
}