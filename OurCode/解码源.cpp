#include <iostream>
#include "ImgDecode.h" // ���������ͷ�ļ�
#include "time.h"
#include<fstream>
#include <vector>
#include<opencv2/opencv.hpp>
#include<cstdio>
using namespace std;
int main() {
    clock_t start_time, end_time;
    start_time = clock();
    // ��ȡ��ά��ͼ��
    cv::Mat qrCodeImage = cv::imread("00001.png", cv::IMREAD_GRAYSCALE);
    threshold(qrCodeImage, qrCodeImage, 0, 255, cv::THRESH_BINARY);

    // ���ͼ���Ƿ�ɹ���ȡ
    if (qrCodeImage.empty()) {
        std::cerr << "Error: Unable to read QR code image." << std::endl;
        return 1;
    }

    // ����һ�� ImageInfo �ṹ���������������Ϣ
    ImageDecode::ImageInfo imageInfo;

    // ���ý��뺯�����н���
    if (!ImageDecode::Main(qrCodeImage, imageInfo)) {
        std::cout << "Error: Decoding failed. Check code mismatch." << std::endl;
        return 1;
    }

    // ��ӡ������
    std::cout << "Decoded information:" << std::endl;
    std::cout << "IsStart: " << imageInfo.IsStart << std::endl;
    std::cout << "IsEnd: " << imageInfo.IsEnd << std::endl;
    std::cout << "CheckCode: " << imageInfo.CheckCode << std::endl;
    std::cout << "FrameBase: " << imageInfo.FrameBase << std::endl;
    std::cout << "Info: ";
    for (const auto& data : imageInfo.Info) {
        std::cout << static_cast<unsigned int>(data) << " ";
    }
    //std::cout << std::endl;
    //std::ofstream outputFile("output1.txt");
    //for (const auto& data : imageInfo.Info) {
    //    outputFile << static_cast<unsigned int>(data) << " "; // ���ַ���ʮ����ֵд���ļ�������֮����ӿո�
    //}
    /*outputFile.close();*/
    end_time = clock();     //��ȡ����ʱ��
    double Times = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("%f seconds\n", Times);
    return 0;
}
