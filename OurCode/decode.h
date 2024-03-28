#pragma once
#include<opencv2/opencv.hpp>

namespace Decode
{
	using namespace cv;
	using namespace std;
	//判断是否需要跳过
	bool jump(int curR, int curC);
	void judge(vector<int>& data, int curC, int curR, Mat image, int count);
	void readQrCode(vector<int>& data, Mat image);
	void Main(Mat& image, vector<int>& data);
}
