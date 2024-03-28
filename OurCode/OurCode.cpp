#include <iostream>
#include <opencv2/opencv.hpp>
#include "BinGenerator.h"
#include "Encode.h"
#include "ImgToVedio.h"
using namespace cv;
using namespace std;

int main()
{
    int mode = 0;
    cout << "选择模式" << endl;
    cout << "输入'0'生成含有二维码的视频" << endl;
    cout << "输入'1'导入视频并解码" << endl;
    //cin >> mode;
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

        int cnt;// 记录几张图片
        Mat img;
        cnt = Encode::fileToImg(datas, img);

        ImgToVedio::Image2Video("images", "output.avi", 5000);
        
    }
    else if (mode == 1)
    {

    }
}
