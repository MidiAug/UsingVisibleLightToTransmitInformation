#include <iostream>
#include <opencv2/opencv.hpp>
#include "Basic.h"
#include "Encode.h"
#include "QR_location.h"
#include "Decode.h"
#include "Files.h"

using namespace cv;
using namespace std;

int main()
{
    int mode = 0;
    cout << "当前编码状态：每" << BIT << "位二进制构成一位像素" << endl;
    cout << "选择模式" << endl;
    cout << "输入'0'生成含有二维码的视频" << endl;
    cout << "输入'1'导入视频并解码" << endl;
    cin >> mode;
    if (mode == 0)
    {
        //cv::Mat image = cv::imread("originalImages/image1.png", cv::IMREAD_UNCHANGED);
        // 生成二进制文件
        std::size_t fileSize = 30000; // 文件大小，单位为字节
        std::string binFilename = "random.bin"; // 文件名
        //Files::generateRandomBinaryFile(binFilename, fileSize);
        //std::cout << "Random binary file generated successfully." << std::endl;


        // 读取、CRC、补齐二进制数据
        vector<int> originalDatas = Files::readBinaryFile(binFilename);
        vector<int> postCheckDatas = Files::CRCEncode(originalDatas);
        vector<int> filledDatas = Files::fillData(postCheckDatas);

        cout << "二进制文件中共有" << originalDatas.size() << "个数据" << endl;
        cout << "CRC编码后共有" << postCheckDatas.size() << "个数据" << endl;
        cout << "填补数据后共有" << filledDatas.size() << "个数据" << endl;


        // 二进制转图片
        Mat img;
        std::string imagePath = "originalImages";
        Encode::initImg(HEIGHT, WIDTH, img);
        Encode::fileToImg(filledDatas, img, imagePath, 0);// 1\0表示 展示\不展示图片

        // 图片转视频
        Files::ImgToVideo(imagePath, "output.mp4", 3000,90);
        // 30000KB,3000ms,90fps
    }
    
    else if (mode == 1)
    {
        std::string binFilename = "random.bin"; // 文件名
        vector<int> originalDatas = Files::readBinaryFile(binFilename);
        vector<int> postCheckDatas = Files::CRCEncode(originalDatas);
        vector<int> filledDatas = Files::fillData(postCheckDatas);

        int flag = 0;
        string videoName = "output.avi";
        string imgOutputPath = "exstractImages";
        string imgInputPath = "exstractImages";
        string originalCodePath = "originalCodes";
        //cout << "请输入你的视频路径" << endl;
        //cin >> vedioName;
        //Files::VideoToImg(videoName, imgOutputPath);
        string extFramePath = "extractedFrames";
        string extCodePath = "extractedCodes";

        //flag = Decode::extractCode(extFramePath, extCodePath);
        if (flag == -1) return -1;

        vector<int> extractedDatas;
        Decode::readCode("test", extractedDatas,filledDatas);
        //for (int i = 0; i < extractedDatas.size(); i++)
        //    cout << extractedDatas[i];
        //cout << extractedDatas.size();
        Files::outBin(extractedDatas,"random_out.bin");

        return 0;
    }
}
