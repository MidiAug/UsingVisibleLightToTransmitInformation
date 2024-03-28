#pragma once
#include<opencv2/opencv.hpp>
#include"Basic.h"
using namespace cv;
using namespace std;

namespace Decode
{
	//判断是否需要跳过
	bool jump(int curR, int curC);
	void judge(vector<int>& data, int curC, int curR, Mat image, int count);
	void readQrCode(vector<int>& data, Mat image);
	void Main(std::string codeFolderPath, vector<int>& data)
}
