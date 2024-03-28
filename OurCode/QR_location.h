#pragma once
#include <opencv2/opencv.hpp>
#include <iostream>    
#include <opencv2\core\core.hpp>
#include <iostream>  
#include <stdio.h>
#include <string>
#include <sstream>

using namespace cv;
using namespace std;

namespace QR_Location
{
	Mat transformCorner(Mat src, RotatedRect rect);
	Mat transformQRcode(Mat src, RotatedRect rect, double angle);
	// ”√”⁄≈–∂œΩ«µ„
	bool IsQrPoint(vector<Point>& contour, Mat& img);
	bool isCorner(Mat& image);
	double Rate(Mat& count);
	int leftTopPoint(vector<Point> centerPoint);
	vector<int> otherTwoPoint(vector<Point> centerPoint, int leftTopPointIndex);
	double rotateAngle(Point leftTopPoint, Point rightTopPoint, Point leftBottomPoint);
	int Main(string inputFolderPath, string outputFolderPath);

}