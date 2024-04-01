﻿#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>
#include "Basic.h"
#include "Encode.h"
#include "QR_location.h"
#include "Decode.h"
#include "Files.h"

using namespace cv;
using namespace std;

int main()
{
    cv::utils::logging::setLogLevel(utils::logging::LOG_LEVEL_SILENT);// 关闭控制台输出日志

    string mode ;
    cout << "选择模式" << endl;
    cout << "输入\"encode\"进行编码（生成随机bin文件、生成二维码、生成视频）" << endl;
    cout << "输入\"dncode\"进行解码（视频生成图片、扣出二维码、二维码转数据）" << endl;
    cin >> mode;
    while (true)
    {
        if (mode == "encode")
        {
            cout << endl;
            // cout << "当前编码状态：每" << BIT << "位二进制数据构成一位像素" << endl;

            // 生成二进制文件
            Files::delete_files_with_format(".bin","");
            string binFileName = Files::getFileName("生成的二进制文件",".bin");
            cout << "请输入二进制文件大小（单位：字节）" << endl;
            // 文件大小，单位为字节
            std::size_t fileSize;
            cin >> fileSize;
            while (fileSize < 1)
            {
                cout << "请正确输入二进制文件大小" << endl;
                cin >> fileSize;
            }
            Files::generateRandomBinaryFile(binFileName, fileSize);
            std::cout << "Random binary file generated successfully." << std::endl;

            // 读取、CRC、补齐二进制数据
            vector<int> originalDatas = Files::readBinaryFile(binFileName);
            vector<int> postCheckDatas = Files::CRCEncode(originalDatas);
            vector<int> filledDatas = Files::fillData(postCheckDatas);
            
            cout << endl;
            cout << "二进制文件中共有" << originalDatas.size() << "个数据" << endl;
            cout << "CRC编码后共有" << postCheckDatas.size() << "个数据" << endl;
            cout << "填补数据后共有" << filledDatas.size() << "个数据" << endl;
            cout << endl;

            // 二进制转图片
            Mat img;
            std::string imagePath = "originalImages";
            Files::create_or_clear_directory(imagePath);
            Encode::initImg(HEIGHT, WIDTH, img);
            Encode::fileToImg(filledDatas, img, imagePath, 0);// 1\0表示 展示\不展示图片

            // 图片转视频
            string videoName = Files::getFileName("生成的视频文件（格式：mp4）", ".mp4");
            int length = 0;
            cout << "请输入视频时长（单位：ms）" << endl;
            cin >> length;
            while (length < 1)
            {
                cout << "请正确输入视频时长（单位：ms）" << endl;
                cin >> length;
            }
            Files::ImgToVideo(imagePath, videoName, length, 90,10);

            return 0;
        }

        else if (mode == "decode")
        {
            vector<cv::String> tmp;
            cv::glob("*.bin"  , tmp);

            std::string binFilename = tmp[0]; // 文件名
            vector<int> originalDatas = Files::readBinaryFile(binFilename);
            vector<int> postCheckDatas = Files::CRCEncode(originalDatas);
            vector<int> filledDatas = Files::fillData(postCheckDatas);

            string videoName = Files::getFileName("拍摄的视频文件（格式：mp4）", ".mp4");
            string originalCodePath = "originalCodes";
            string extFramePath = "extractedFrames";
            string extCodePath = "extractedCodes";

            Files::FrameExtractor(videoName, extFramePath,0.1,250);
            return 0;

            Decode::extractCode(extFramePath, extCodePath);

            vector<int> extractedDatas;
            Decode::readCode(extCodePath, extractedDatas, filledDatas);
            //vector<int> unCheckDatas = Files::CRCDecode(extractedDatas);
            //int n = 0;
            //for (int i = 0; i < unCheckDatas.size(); i++)
            //{
            //    if (unCheckDatas[i] != originalDatas[i])
            //        n++;
            //}
            //cout << n;
            //Files::outBin(unCheckDatas,"random_out.bin");

            return 0;
        }

        cout << "请勿输入'0'或'1'以外的文本。" << endl;
        cin >> mode;
    }
}
