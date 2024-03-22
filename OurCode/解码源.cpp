#include <iostream>
#include "ImgDecode.h" // 包含解码的头文件
#include "time.h"
#include<fstream>
#include <vector>
#include<opencv2/opencv.hpp>
#include<cstdio>
using namespace std;
int main() {
    clock_t start_time, end_time;
    start_time = clock();
    // 读取二维码图像
    cv::Mat qrCodeImage = cv::imread("00001.png", cv::IMREAD_GRAYSCALE);
    threshold(qrCodeImage, qrCodeImage, 0, 255, cv::THRESH_BINARY);

    // 检查图像是否成功读取
    if (qrCodeImage.empty()) {
        std::cerr << "Error: Unable to read QR code image." << std::endl;
        return 1;
    }

    // 创建一个 ImageInfo 结构体来保存解码后的信息
    ImageDecode::ImageInfo imageInfo;

    // 调用解码函数进行解码
    if (!ImageDecode::Main(qrCodeImage, imageInfo)) {
        std::cout << "Error: Decoding failed. Check code mismatch." << std::endl;
        return 1;
    }

    // 打印解码结果
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
    //    outputFile << static_cast<unsigned int>(data) << " "; // 将字符的十进制值写入文件，并在之间添加空格
    //}
    /*outputFile.close();*/
    end_time = clock();     //获取结束时间
    double Times = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("%f seconds\n", Times);
    return 0;
}
