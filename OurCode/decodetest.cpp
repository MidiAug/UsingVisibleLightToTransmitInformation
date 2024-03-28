#include <iostream>
#include<fstream>
#include <vector>
#include<opencv2/opencv.hpp>
#include<cstdio>
#define MARGIN 1
#define HIGH 97
#define WIDTH 97
#define CAPACITY 8483
using namespace std;
using namespace cv;
//定义像素点
Vec3b pixcel[2] = { (0,0,0),(255,255,255) };
//读取自定义二维码图像中的内容
const int saveCorrection[6][2] =
{
    WIDTH / 2, MARGIN + 7,// 上
    MARGIN + 7, HIGH / 2 ,// 中左
    WIDTH / 2, HIGH / 2 ,// 中中
    WIDTH - MARGIN - 7, HIGH / 2, //中右
    WIDTH / 2, HIGH - MARGIN - 8 ,//下中
    WIDTH - MARGIN - 7, HIGH - MARGIN - 8,// 下右
};
bool jump(int curR, int curC) {

    //寻像点
    if (curR <= MARGIN + 7 + 1) {
        if (curC <= MARGIN + 7 + 1 || curC >= WIDTH - (MARGIN + 7 + 1) - 1) return true;
    }
    else if (curR >= HIGH - (MARGIN + 7 + 1) - 1 && curC <= MARGIN + 7 + 1) return true;


    // 矫正点
    if (curC <= WIDTH / 2 + 2 && curC >= WIDTH / 2 - 2) // 中间三个（258）
    {
        if (curR <= MARGIN + 7 + 2 && curR >= MARGIN + 7 - 2) return true;
        else if (curR <= WIDTH / 2 + 2 && curR >= WIDTH / 2 - 2) return true;
        else if (curR <= WIDTH - MARGIN - 7 + 2 && curR >= WIDTH - MARGIN - 7 - 2 - 1) return true;
    }

    if (curC <= MARGIN + 7 + 2 && curC >= MARGIN + 7 - 2) // 左中 4
        if (curR >= HIGH / 2 - 2 && curR <= HIGH / 2 + 2)
            return true;

    if (curC <= WIDTH - MARGIN - 7 + 2 && curC >= WIDTH - MARGIN - 7 - 2) // 右2个（69）
    {
        if (curR <= HIGH - MARGIN - 7 + 2 - 1 && curR >= HIGH - MARGIN - 7 - 2 - 1) // 9
            return true;
        else if (curR <= HIGH / 2 + 2 && curR >= HIGH / 2 - 2) // 6
            return true;
    }


    if (curC == 7 + MARGIN || curR == 7 + MARGIN) return true;

    return false;
}
//判断该像素点是1或者0
void judge(vector<int>& data,int curC,int curR,Mat image,int count)
{
    auto temp = image.at<Vec3b>(curR, curC);
    int t = 0;
    t = (temp == pixcel[0] ? 0 : 1);
    data.push_back(t);
}
void readQRcode(Mat img,vector<int> &datas) 
{
    int curC, curR;
    int flag = 1;
    int index = 0;
    for (int curR = MARGIN; curR < HIGH - MARGIN && flag; curR++)
    {
        for (int curC = MARGIN; curC < WIDTH - MARGIN && flag; curC++)
        {
            if (index >= CAPACITY) {
                flag = 0;
                continue;
            }
            // 跳过定位码区域
            if (jump(curR, curC)) continue;
            judge(datas, curC, curR, img, index);
            index++;
        }
    }
}
int main() {
    //打开文件中的一个图像，读取图像，将图像转换为灰度图像
	Mat img = imread("image1.png");
    //判断图像是否为空
    if (img.empty()) {
        		cout << "图像为空" << endl;
		return -1;
    }
    ofstream pic("checkpic.png");
    pic << img;
    pic.close();
    //将图像转换为二值图像
    threshold(img, img, 100, 255, THRESH_BINARY);
    //定义一个vector数组
    vector<int> datas(CAPACITY);
    //读取二维码图像中的内容
    readQRcode(img,datas);
    //将读取的内容以二进制形式输出到文件中
    ofstream out("data.txt");
    for (int i = 0; i < datas.size(); i++) {
		out << datas[i];
	}
    out.close();
    return 0;
}
