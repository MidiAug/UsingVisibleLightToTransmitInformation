#pragma once
#include <opencv2/opencv.hpp>
#include <vector>
#include "Basic.h"

using namespace cv;
using namespace std;

namespace Encode {
    const int saveCorrection[6][2] =
    {
        WIDTH / 2, MARGIN + 7,// 上
        MARGIN + 7, HIGH / 2 ,// 中左
        WIDTH / 2, HIGH / 2 ,// 中中
        WIDTH - MARGIN - 7, HIGH / 2, //中右
        WIDTH / 2, HIGH - MARGIN - 8 ,//下中
        WIDTH - MARGIN - 7, HIGH - MARGIN - 8,// 下右
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
    void drawPixel(int val, int x, int y, Mat& img);
    void drawPixel(int val, int lue, int x, int y, Mat& img);

    void initImg(int high, int width, Mat& img);

    void fileToImg(std::vector<int>& datas, Mat& img,std::string outputPath);

    void saveImg(String filePath, Mat& img);
    void showImg(Mat& img);

}

