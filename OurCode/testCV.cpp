#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <vector>
using namespace std;
using namespace cv;
#define MARGIN 1
#define HIGH 150
#define WIDTH 150
class FileToImg
{
public:
    // 画定位点
    void block(int left,int top, Mat img) {
        rectangle(
            img,
            Point(left, top),
            Point(left+7,top+7),
            Scalar(0,0,0)
            );
        rectangle(
            img,
            Point(left + 2, top + 2),
            Point(left + 5, top + 5),
            Scalar(0, 0, 0),
            FILLED
        );
    }
    // 画像素点
    void drawPixel(int val,int x, int y, Mat img) {
        if(val == 0)
            rectangle(
                img,
                Point(x, y),
                Point(x, y),
                Scalar(0, 0, 0)
            );
        else 
            rectangle(
                img,
                Point(x, y),
                Point(x, y),
                Scalar(255, 255, 255)
            );
    };
    void drawBlocks() {
        block(MARGIN, MARGIN, img);//左上
        block(WIDTH - MARGIN - 8, MARGIN, img);// 右上
        block(MARGIN, HIGH - MARGIN - 8, img); // 左下
    }

    void initImg(int high, int width) {
        img = Mat(cv::Size(high, width), CV_8UC3);
        img.setTo(Scalar(255, 255, 255));  
        
    }
    void fileToImg(vector<char> ch) {
        drawBlocks();
        int num; // 第几张图
        char data;
        int index = 0;
        int curC = 0, curR = 0;
        int flag = 1;
        for (int curR = MARGIN; curR < HIGH - MARGIN && flag; curR++)
        {
            for (int curC = MARGIN; curC < WIDTH - MARGIN && flag; curC++)
            {
                if (ch.empty() || index >= ch.size()) {
                    flag = 0;
                    continue;
                }
                // 跳过定位码区域
                if (curR <= MARGIN + 7 + 1) {
                    if (curC <= MARGIN + 7 + 1 || curC >= WIDTH - (MARGIN + 7 + 1)-1)continue;
                }
                else if (curR >= HIGH - (MARGIN + 7 + 1) && curC <= MARGIN + 7 + 1) continue;
                data = ch[index++];
                drawPixel(data - '0', curC, curR, img);
            }
        }
        capacity = HIGH * WIDTH - 2 * HIGH * MARGIN - 2 * WIDTH * MARGIN + 4 * MARGIN * MARGIN - 234;// 234是二维码区域
        cout << index << endl;
        cout << capacity << endl;
    }
    void saveImgs(String filePath,Mat img) {
        imwrite(filePath, img);
    }
    void showImg() {
        cv::namedWindow("image", cv::WINDOW_NORMAL);
        cv::imshow("image", img);
    }
private:
    Mat img;
    int capacity;

   
};

int main()
{
    char tmp;
    vector<char> datas;
    fstream bins("tmp.txt");
    while (bins >> tmp) {
        datas.push_back(tmp);
    }
    FileToImg file;
    file.initImg(HIGH, WIDTH);
    file.fileToImg(datas);
    file.showImg();
    waitKey(0);
    destroyAllWindows();
    return 0;
}
