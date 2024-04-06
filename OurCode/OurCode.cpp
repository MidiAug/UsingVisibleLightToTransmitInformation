#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>
#include "Basic.h"
#include "Encode.h"
#include "Decode.h"
#include "Files.h"
#include <ctime>

using namespace cv;
using namespace std;

int getTime()
{
    auto cur = std::chrono::steady_clock::now();
    auto cur_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(cur);
    auto ms = cur_ms.time_since_epoch().count();
    return ms;
}
int main(int argc,char*argv[])
{
    cv::utils::logging::setLogLevel(utils::logging::LOG_LEVEL_SILENT);// 关闭控制台输出日志

    if (argc != 5)
    {
        std::cout << "请使用命令行且正确输入参数执行本程序" << endl;
        return -1;
    }

    string mode  = argv[1];
    //std::cout << "选择模式" << endl;
    //std::cout << "输入\"encode\"进行编码（生成随机bin文件、生成二维码、生成视频）" << endl;
    //std::cout << "输入\"decode\"进行解码（视频生成图片、扣出二维码、二维码转数据）" << endl;
    //cin >> mode;
    //while (true)
    //{
    if (mode == "encode")
    {
        //std::cout << endl;
        // std::cout << "当前编码状态：每" << BIT << "位二进制数据构成一位像素" << endl;

        // 生成二进制文件
        //Files::delete_files_with_format(".bin","");
        //Files::delete_files_with_format(".mp4", "");
        //string binFileName = Files::getFileName("生成的二进制文件",".bin");
            
        //std::cout << "请输入二进制文件大小（单位：字节）" << endl;
        // 文件大小，单位为字节
        //std::size_t fileSize;
        //cin >> fileSize;
        //while (fileSize < 1)
        //{
        //    std::cout << "请正确输入二进制文件大小" << endl;
        //    cin >> fileSize;
        //}
        //Files::generateRandomBinaryFile(binFileName, fileSize);
        //std::cout << "随机二进制文件生成成功" << std::endl;

        string binFileName = argv[2];
        // 读取、CRC、补齐二进制数据
        vector<int> originalDatas = Files::readBinaryFile(binFileName);
        vector<int> postCheckDatas = Files::CRCEncode(originalDatas);
        vector<int> filledDatas = Files::fillData(postCheckDatas);
            
        //std::cout << endl;
        std::cout << "二进制文件中共有" << originalDatas.size() << "个数据" << endl;
        std::cout << "CRC编码后共有" << postCheckDatas.size() << "个数据" << endl;
        std::cout << "填补数据后共有" << filledDatas.size() << "个数据" << endl;

        // 二进制转图片
        Mat img;
        int imgcnt;
        std::string imagePath = "originalImages";
        Files::create_or_clear_directory(imagePath);
        Encode::initImg(HEIGHT, WIDTH, img);
        imgcnt = Encode::fileToImg(filledDatas, img, imagePath, 0);// 1\0表示 展示\不展示图片

        // 图片转视频
        //string videoName = Files::getFileName("生成的视频文件（格式：mp4）", ".mp4");
        string videoName = argv[3];
        int length =  atoi(argv[4]);
        //std::cout << "请输入视频时长（单位：ms）" << endl;
        //cin >> length;
        //while (length < 1)
        //{
        //    std::cout << "请正确输入视频时长（单位：ms）" << endl;
        //    cin >> length;
        //}
        Files::ImgToVideo(imagePath, videoName, length, 90,5,5);

        return 0;
    }

    else if (mode == "decode")
    {
        vector<cv::String> tmp;
        cv::glob("*.bin"  , tmp);
        std::string binFilename =tmp[0]; // 文件名
        std::string outName[2];
            
        vector<int> originalDatas = Files::readBinaryFile(binFilename);
        vector<int> postCheckDatas = Files::CRCEncode(originalDatas);
        vector<int> filledDatas = Files::fillData(postCheckDatas);
        //std::std::cout << originalDatas.size() << " " << postCheckDatas.size() << " " << filledDatas.size() << "\n";
        //std::cout << endl;
        //string videoName = Files::getFileName("拍摄的视频文件（格式：mp4）", ".mp4");
        //std::cout << "输入输出文件名\n";
        //cin >> outName[0];
        //std::cout << "输入比对文件名\n";
        //cin >> outName[1];
        string videoName = argv[2];
        outName[0] = argv[3];
        outName[1] = argv[4];
        string originalCodePath = "originalCodes";
        string extFramePath = "extractedFrames";
        string extCodePath = "extractedCodes";

        // 截取帧
        std::cout << "---截取帧---" << endl;
        Files::FrameExtractor(videoName, extFramePath, 0.1, 100);

        // 提取二维码
        std::cout << "---提取二维码---" << endl;
        Decode::extractCode(extFramePath, extCodePath);

        // 解码二维码
        std::cout << "---解码二维码---" << endl;
        vector<int> extractedDatas;
        Decode::readCode(extCodePath, extractedDatas, filledDatas,1);

        //std::std::cout << extractedDatas.size() << '\n';
        //std::std::cout << filledDatas.size() << '\n';
        vector<int> unCheckDatas = Files::CRCDecode(extractedDatas);

        //计算误码率，这部分hw要求输出一个vout文件，每一位指示原文件和解码后是否比对正确
        vector<int> vout;
        int wrongBits = 0;
        for (int i = 0, j = 0; i < originalDatas.size(); ++i, ++j) {
            if (j >= unCheckDatas.size()) 
            vout.push_back(0);
            else {
            if (unCheckDatas[j] == originalDatas[i]) {
                vout.push_back(1);
                ++wrongBits;
            }
            else 
                vout.push_back(0);
                
            }
        }
        Files::outBin(vout, outName[1]);

        //std::std::cout << unCheckDatas.size() << '\n';
        Files::outBin(unCheckDatas, outName[0]);
        int n = 0;
        std::cout << "\n传输数据位数为：" << originalDatas.size() << "，即" << originalDatas.size() / 1024 << "Kb" << endl;
        std::cout <<"传输正确率为：" << static_cast<double>(wrongBits) / originalDatas.size()<<'\n';
            

        return 0;
    }
    //std::cout << "请勿输入\"encode\"或\"decode\"以外的文本。" << endl;
    //cin >> mode;

    //}

}
