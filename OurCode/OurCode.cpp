#include <iostream>
#include <opencv2/opencv.hpp>
#include "Basic.h"
#include "Bin.h"
#include "Encode.h"
#include "IVTran.h"
#include "QR_location.h"
#include "Decode.h"

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
        std::size_t fileSize = 5000; // 文件大小，单位为字节
        std::string binFilename = "random.bin"; // 文件名
        //generateRandomBinaryFile(binFilename, fileSize);
        //std::cout << "Random binary file generated successfully." << std::endl;

        vector<int> datas = readBinaryFile(binFilename);
        cout << "共有" << datas.size() << "个数据" << endl;
        fillData(datas);
        cout << "填补数据后共有" << datas.size() << "个数据" << endl;

        std::string imagePath = "originalImages";

        Mat img;
        Encode::initImg(HIGH,WIDTH,img);
        //Encode::saveImg(imagePath + "/image0.png",img);
        Encode::fileToImg(datas, img, imagePath);

        ImgToVideo(imagePath, "output.avi", 5000);
        
    }
    else if (mode == 1)
    {
        string videoName = "output.avi";
        string imgOutputPath = "exstractImages";
        string imgInputPath = "exstractImages";
        string codeOutputPath = "extractedCodes";
        //cout << "请输入你的视频路径" << endl;
        //cin >> vedioName;
        //VideoToImg(videoName, imgOutputPath);
        
        
        //QR_Location::Main(imgInputPath, codeOutputPath);
        vector<int> extractedDatas;
        Decode::Main("originalImages", extractedDatas);
        //cout << extractedDatas.size();
        //for (int i = 0; i < extractedDatas.size(); i++)
        //    cout << extractedDatas[i];
        outBin(extractedDatas,"random_out.bin");
    }
}
