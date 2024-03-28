#pragma once
#include <opencv2/opencv.hpp>
#include <vector>
using namespace cv;
using namespace std;

#define MARGIN 1
#define HIGH 97
#define WIDTH 97
#define CAPACITY 8483

namespace Encode {
    const int saveCorrection[6][2] =
    {
        WIDTH / 2, MARGIN + 7,// ��
        MARGIN + 7, HIGH / 2 ,// ����
        WIDTH / 2, HIGH / 2 ,// ����
        WIDTH - MARGIN - 7, HIGH / 2, //����
        WIDTH / 2, HIGH - MARGIN - 8 ,//����
        WIDTH - MARGIN - 7, HIGH - MARGIN - 8,// ����
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
    void drawPixel(int val, int x, int y, Mat& img);

    void initImg(int high, int width, Mat& img);

    int fileToImg(std::vector<int>& datas, Mat& img);

    void saveImg(String filePath, Mat& img);
    void showImg(Mat& img);

}

