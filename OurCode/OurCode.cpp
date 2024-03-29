#include <iostream>
#include <opencv2/opencv.hpp>
#include "Basic.h"
#include "Encode.h"
#include "QR_location.h"
#include "Decode.h"
#include "CRC.h"
#include "Files.h"

using namespace cv;
using namespace std;

int main()
{
    int mode = 0;
    cout << "选择模式" << endl;
    cout << "输入'0'生成含有二维码的视频" << endl;
    cout << "输入'1'导入视频并解码" << endl;
    cin >> mode;
    if (mode == 0)
    {
        cv::Mat image = cv::imread("originalImages/image1.png", cv::IMREAD_UNCHANGED);
        // 生成二进制文件
        std::size_t fileSize = 10000; // 文件大小，单位为字节
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
        Encode::initImg(HIGH,WIDTH,img);
        Encode::fileToImg(filledDatas, img, imagePath,0);// 1\0表示 展示\不展示图片

        // 图片转视频
        Files::ImgToVideo(imagePath, "output.avi", 5000);
        
        cv::VideoCapture cap("output.avi");
        if (!cap.isOpened()) {
            std::cerr << "Error opening video file" << std::endl;
            return -1;
        }

        // 设置视频缩放比例
        cap.set(cv::CAP_PROP_FRAME_WIDTH, cap.get(cv::CAP_PROP_FRAME_WIDTH) * 2);
        cap.set(cv::CAP_PROP_FRAME_HEIGHT, cap.get(cv::CAP_PROP_FRAME_HEIGHT) * 2);

        // 读取视频帧
        cv::Mat frame;
        while (true) {
            // 读取下一帧
            cap >> frame;

            // 检查是否读取到帧
            if (frame.empty()) {
                break;
            }

            // 显示帧
            cv::namedWindow("Video", cv::WINDOW_NORMAL);

            cv::imshow("Video", frame);

            waitKey(1);
            // 等待按键
            if (cv::waitKey(30) == 27) {
                break;
            }
        }

        // 释放视频资源
        cap.release();

        return 0;

    }
    else if (mode == 1)
    {
        string videoName = "output.avi";
        string imgOutputPath = "exstractImages";
        string imgInputPath = "exstractImages";
        string codeOutputPath = "extractedCodes";
        //cout << "请输入你的视频路径" << endl;
        //cin >> vedioName;
        Files::VideoToImg(videoName, imgOutputPath);
        
        
        //QR_Location::Main(imgInputPath, codeOutputPath);
        vector<int> extractedDatas;
        Decode::Main("originalImages", extractedDatas);
        //cout << extractedDatas.size();
        //for (int i = 0; i < extractedDatas.size(); i++)
        //    cout << extractedDatas[i];
        Files::outBin(extractedDatas,"random_out.bin");
    }
}
