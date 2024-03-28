#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <vector>
using namespace std;
using namespace cv;
#define MARGIN 1
#define HIGH 97
#define WIDTH 97

class FileToImg
{
public:
    const int capacity = HIGH * WIDTH - 2 * HIGH * MARGIN - 2 * WIDTH * MARGIN + 4 * MARGIN * MARGIN - 234;// 234是二维码区域
    const int saveCorrection[6][2] =
    {
        WIDTH / 2, MARGIN + 7,
        MARGIN + 7, WIDTH / 2 ,
        WIDTH / 2, WIDTH / 2 ,
        WIDTH / 2, WIDTH - MARGIN - 7 ,
        WIDTH - MARGIN - 7, WIDTH - MARGIN - 7,
        WIDTH - MARGIN - 7, WIDTH / 2
    };
    // 画寻像
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

    //画定位点
    void findLacation(Mat img)
    {
        for (int i = MARGIN+7; i < WIDTH - 8; i++)
        {
            if (i % 2 == 0)
            {
                rectangle(
                    img,
                    Point(i, MARGIN + 7),
                    Point(i, MARGIN + 7),
                    Scalar(0, 0, 0)
                );
                rectangle(
                    img,
                    Point(MARGIN + 7, i),
                    Point(MARGIN + 7, i),
                    Scalar(0, 0, 0)
                );
            }
            else
            {
                rectangle(
                    img,
                    Point(i, MARGIN + 7),
                    Point(i, MARGIN + 7),
                    Scalar(255, 255, 255)
                );
                rectangle(
                    img,
                    Point(MARGIN + 7, i),
                    Point(MARGIN + 7, i),
                    Scalar(255, 255, 255)
                );
            }
        }
    }
    //画校正点
    void correction(int x,int y,Mat img)
    {
        rectangle(
            img,
            Point(x, y),
            Point(x, y),
            Scalar(0, 0, 0)
        );
        rectangle(
            img,
            Point(x - 2, y - 2),
            Point(x + 2, y + 2),
            Scalar(0, 0, 0)
        );
    }
    void drawBlocks() {
        block(MARGIN, MARGIN, img);//左上
        block(WIDTH - MARGIN - 8, MARGIN, img);// 右上
        block(MARGIN, HIGH - MARGIN - 8, img); // 左下
    }
    void drawCorrection()
    {
        correction(WIDTH / 2, MARGIN + 7,img);
        correction(MARGIN + 7, WIDTH / 2, img);
        correction(WIDTH / 2, WIDTH / 2, img);
        correction(WIDTH / 2, WIDTH - MARGIN - 7, img);
        correction(WIDTH - MARGIN - 7, WIDTH - MARGIN - 7, img);
        correction(WIDTH - MARGIN - 7, WIDTH / 2, img);
    }
    void initImg(int high, int width) {
        img = Mat(cv::Size(high, width), CV_8UC3);
        img.setTo(Scalar(255, 255, 255));  
    }

    //补齐数据
    void fillData(vector<char> ch)
    {
        vector<char> fillarr(16);
        fillarr = { 1,1,1,0,1,1,0,0,0,0,0,1,0,0,0,1 };
        while (sizeof(ch) / sizeof(ch[0]) < capacity)
        {
            ch.insert(ch.end(), fillarr.begin(), fillarr.end());
        }
    }

    void fileToImg(vector<char> ch) {
        drawBlocks();
        drawCorrection();
        findLacation(img);
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
                for (int i = 0; i < 6; i++)
                {
                    if (curR == saveCorrection[i][0] && curC == saveCorrection[i][1])
                        continue;
                }
                // 跳过定位码区域
                if (curR <= MARGIN + 7 + 1) {
                    if (curC <= MARGIN + 7 + 1 || curC >= WIDTH - (MARGIN + 7 + 1) - 1)continue;
                }
                else if (curR >= HIGH - (MARGIN + 7 + 1) && curC <= MARGIN + 7 + 1) continue;
                data = ch[index++];
                drawPixel(data - '0', curC, curR, img);
            }
        }
        
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

int main() {

    char tmp;
    vector<char> datas;
    fstream bins("random.bin");
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