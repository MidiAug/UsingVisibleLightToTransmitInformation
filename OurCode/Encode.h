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
        WIDTH / 2, MARGIN + 7,// ��
        MARGIN + 7, HIGH / 2 ,// ����
        WIDTH / 2, HIGH / 2 ,// ����
        WIDTH - MARGIN - 7 - 1, HIGH / 2, //����
        WIDTH / 2, HIGH - MARGIN - 8 ,//����
        WIDTH - MARGIN -  7 - 1, HIGH - MARGIN - 7 -1,// ����
    };
    // ��Ҫ����������
    bool jump(int curR, int curC);

    // Ѱ������3����
    void block(int left, int top, Mat& img);
    // ����λ�㣨2���ߣ�
    void findLacation(Mat& img);
    //��У���� (6����
    void correction(int x, int y, Mat& img);

    // ������3�ֶ�������ͼ��
    void drawBasic(Mat& img);

    // ����������ֵ
    void drawPixel(int val, int x, int y, Mat& img);// һλ
    void drawPixel(int val, int lue, int x, int y, Mat& img); // ��λ
    void drawPixel(int v1, int v2, int v3,int x, int y, Mat& img); // ��λ

    void initImg(int high, int width, Mat& img);

    void fileToImg(std::vector<int>& datas, Mat& img,std::string outputPath, int wait = 0);

    void saveImg(String filePath, Mat& img);
    void showImg(Mat& img);

}

