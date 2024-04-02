#pragma once
#include <filesystem>
#include <opencv2/opencv.hpp>
#include <vector>
#include "Basic.h"
#include "Files.h"

using namespace cv;
using namespace std;

namespace Encode {
    const int saveCorrection[6][2] =
    {
        WIDTH / 2, MARGIN + 7,// 上
        MARGIN + 7, HEIGHT / 2 ,// 中左
        WIDTH / 2, HEIGHT / 2 ,// 中中
        WIDTH - MARGIN - 7 - 1, HEIGHT / 2, //中右
        WIDTH / 2, HEIGHT - MARGIN - 8 ,//下中
        WIDTH - MARGIN -  7 - 1, HEIGHT - MARGIN - 7 -1,// 下右
    };
    // 需要跳过的区域
    bool jump(int curR, int curC);

    // 寻像区域（3个）
    void block(int left, int top, Mat& img);
    // 画定位点（2条线）
    void findLacation(Mat& img);
    //画校正点 (6个）
    void correction(int x, int y, Mat& img);

    // 将上述3种东西画到图里
    void drawBasic(Mat& img);

    // 画具体像素值
    void drawPixel(int val, int x, int y, Mat& img);// 一位
    void drawPixel(int val, int lue, int x, int y, Mat& img); // 两位
    void drawPixel(int v1, int v2, int v3,int x, int y, Mat& img); // 三位

    Mat ScaleToDisSize(const Mat& src, int outputRate);

    void initImg(int height, int width, Mat& img);

    int fileToImg(std::vector<int>& datas, Mat& img,std::string outputPath, int wait = 0);

    void saveImg(String filePath, Mat& img);
    void showImg(Mat& img);

}

