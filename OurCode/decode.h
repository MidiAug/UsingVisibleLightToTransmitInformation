#pragma once
#include<opencv2/opencv.hpp>
#include"Basic.h"
using namespace cv;
using namespace std;

namespace Decode
{
	//�ж��Ƿ���Ҫ����
	bool jump(int curR, int curC);
	void judge(vector<int>& data, int curC, int curR, Mat imaget);
	void readQrCode(vector<int>& data, Mat& image);
	void Main(std::string codeFolderPath, vector<int>& data);
	void showimg(Mat& img, float x, float y);
}