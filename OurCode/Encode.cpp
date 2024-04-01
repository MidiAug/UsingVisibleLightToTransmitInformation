#include "Encode.h"

void Encode::fileToImg(std::vector<int>& datas, Mat& img, std::string outputPath,int wait) {
    initImg(HEIGHT, WIDTH, img);
    int numOfImg = datas.size() / (CAPACITY);
    int curNum = 0;
    int index = 0, curC = 0, curR = 0;
    string filename;
    cout << "生成二维码图片数量：" << numOfImg << endl;
    Mat white = ScaleToDisSize(img, MULTIPLE);
    cv::imwrite(outputPath + "/image0"+PICFORMAT, white);
    drawBasic(img);
    //cv::namedWindow("Window with adjustable size", cv::WINDOW_NORMAL);
    //imshow("Window with adjustable size", img);
    //waitKey(0);

    for (int i = 0; i < numOfImg; i++)
    {
        string str1 = "image";
        string str2 = to_string(i + 1);
        string str3 = ".png";
        filename = str1 + str2 + str3;

        
        int data1;
        int data2;
        int data3;
        curC = curR = 0;
        int flag = 1;
        for (int curR = MARGIN; curR < HEIGHT - MARGIN && flag; curR++)
        {
            for (int curC = MARGIN; curC < WIDTH - MARGIN && flag; curC++)
            {
                if (datas.empty() || index >= datas.size()) {
                    flag = 0;
                    continue;
                }
                if (jump(curR, curC)) continue;
                // 跳过定位码区域
                
                if (BIT == 1)
                {
                    data1 = datas[index++];
                    drawPixel(data1, curC, curR, img);
                }
                else if (BIT == 2)
                {
                    data1 = datas[index++];
                    data2 = datas[index++];
                    drawPixel(data1, data2, curC, curR, img);
                }
                else if (BIT == 3)
                {
                    data1 = datas[index++];
                    data2 = datas[index++];
                    data3 = datas[index++];
                    drawPixel(data1, data2, data3, curC, curR, img);
                }
            }
        }
        curNum++;
        if (i == 0 && index != CAPACITY) cout << "容量计算有误，重新计算，index为" << index <<"" << endl;
        
        Mat adjust = ScaleToDisSize(img, MULTIPLE);
        if (wait == 1) {
            showImg(adjust);
            waitKey(0);
        }
        saveImg(outputPath + '/' + filename, adjust);
    }
}

// 需要跳过的区域
bool Encode::jump(int curR, int curC)
{

    //寻像点
    if (curR <= MARGIN + 7 + 1) {
        if (curC <= MARGIN + 7 + 1 || curC >= WIDTH - (MARGIN + 7 + 1) - 1) return true;
    }
    else if (curR >= HEIGHT - (MARGIN + 7 + 1) - 1 && curC <= MARGIN + 7 + 1) return true;


    // 矫正点
    if (curC <= WIDTH / 2 + 2 && curC >= WIDTH / 2 - 2) // 中间三个（258）
    {
        if (curR <= MARGIN + 7 + 2 && curR >= MARGIN + 7 - 2) return true;
        else if (curR <= HEIGHT / 2 + 2 && curR >= HEIGHT / 2 - 2) return true;
        else if (curR <= HEIGHT - MARGIN - 7 + 2 - 1 && curR >= HEIGHT - MARGIN - 7 - 2 - 1) return true;
    }

    if (curC <= MARGIN + 7 + 2 && curC >= MARGIN + 7 - 2) // 左中 4
        if (curR >= HEIGHT / 2 - 2 && curR <= HEIGHT / 2 + 2)
            return true;

    if (curC <= WIDTH - MARGIN - 7 + 2 -1&& curC >= WIDTH - MARGIN - 7 - 2 -1 ) // 右2个（69）
    {
        if (curR <= HEIGHT - MARGIN - 7 + 2 - 1 && curR >= HEIGHT - MARGIN - 7 - 2 - 1) // 9
            return true;
        else if (curR <= HEIGHT / 2 + 2 && curR >= HEIGHT / 2 - 2) // 6
            return true;
    }


    if (curC == 7 + MARGIN || curR == 7 + MARGIN) return true;

    return false;
}

// 寻像区域（3个）
void Encode::block(int left, int top, Mat& img) {
    rectangle(
        img,
        Point(left, top),
        Point(left + 7, top + 7),
        Scalar(0, 0, 0)
    );
    rectangle(
        img,
        Point(left + 2, top + 2),
        Point(left + 5, top + 5),
        Scalar(0, 0, 0),
        FILLED
    );
}
// 画定位点（2条线）
void Encode::findLacation(Mat& img)
{
    int row, col;
    col = MARGIN + 7;
    for (int row = MARGIN +7 + 2; row < HEIGHT - MARGIN - 7 - 2; row++)
    {
        if (row % 2 == 1) drawPixel(0, col, row, img);
        else drawPixel(1, col, row, img);
    }

    row = MARGIN + 7;
    for (int col = MARGIN + 7 + 2; col < WIDTH - MARGIN - 7 - 2; col++)
    {
        if (col % 2 == 1) drawPixel(0, col, row, img);
        else drawPixel(1, col, row, img);
    }
}
//画校正点 (6个）
void Encode::correction(int x, int y, Mat& img)
{
    rectangle(
        img,
        Point(x, y),
        Point(x, y),
        Scalar(0, 0, 0)
    );
    rectangle(
        img,
        Point(x-1, y-1),
        Point(x+1, y+1),
        Scalar(255, 255, 255)
    );
    rectangle(
        img,
        Point(x - 2, y - 2),
        Point(x + 2, y + 2),
        Scalar(0, 0, 0)
    );
}
// 将上述3种东西画到图里
void Encode::drawBasic(Mat& img) {
    findLacation(img);
    // 寻像区域
    block(MARGIN, MARGIN, img);//左上
    block(WIDTH - MARGIN - 8, MARGIN, img);// 右上
    block(MARGIN, HEIGHT - MARGIN - 8, img); // 左下

    for (int i = 0; i < 6; i++)
        correction(saveCorrection[i][0], saveCorrection[i][1], img);

}

// 画具体像素值
void Encode::drawPixel(int val, int x, int y, Mat& img) {
    Scalar color;
    if (val == 0)color = cv::Scalar(0, 0, 0);
    else color = cv::Scalar(255, 255, 255);

    rectangle(
        img,
        Point(x, y),
        Point(x, y),
        color
    );
};

void Encode::drawPixel(int val, int lue, int x, int y, Mat& img) {
    Scalar color;
    if (val == 0 && lue == 0) color = cv::Scalar(0, 0, 0);
    else if (val == 0 && lue == 1) color = cv::Scalar(232, 162, 0);
    //else if (val == 1 && lue == 0) color = cv::Scalar(0, 125, 255);
    else if (val == 1 && lue == 0) color = cv::Scalar(23, 93, 255);
    else  color = cv::Scalar(255, 255, 255);

    rectangle(
        img,
        Point(x, y),
        Point(x, y),
        color
    );
};

void Encode::drawPixel(int v1, int v2, int v3, int x, int y, Mat& img)
{
    Scalar color;
    /*if (v1 == 0 && v2 == 0 && v3 == 0) color = cv::Scalar(0, 0, 255);
    else if (v1 == 0 && v2 == 0 && v3 == 1) color = cv::Scalar(255, 0, 0);
    else if (v1 == 0 && v2 == 1 && v3 == 0) color = cv::Scalar(0, 255, 255);
    else if (v1 == 0 && v2 == 1 && v3 == 1) color = cv::Scalar(128, 0, 128);
    else if (v1 == 1 && v2 == 0 && v3 == 0) color = cv::Scalar(0, 255, 0);
    else if (v1 == 1 && v2 == 0 && v3 == 1) color = cv::Scalar(0, 165, 255);
    else if (v1 == 1 && v2 == 1 && v3 == 0) color = cv::Scalar(203, 192, 255);
    else  color = cv::Scalar(255, 255, 255);*/

    //if (v1 == 0 && v2 == 0 && v3 == 0) color = cv::Scalar(0, 0, 0);
    //else if (v1 == 0 && v2 == 0 && v3 == 1) color = cv::Scalar(0, 0, 255);
    //else if (v1 == 0 && v2 == 1 && v3 == 0) color = cv::Scalar(0, 255, 0);
    //else if (v1 == 0 && v2 == 1 && v3 == 1) color = cv::Scalar(0, 255, 255);
    //else if (v1 == 1 && v2 == 0 && v3 == 0) color = cv::Scalar(255, 0, 0);
    //else if (v1 == 1 && v2 == 0 && v3 == 1) color = cv::Scalar(255, 0, 255);
    //else if (v1 == 1 && v2 == 1 && v3 == 0) color = cv::Scalar(255, 255, 0);
    //else  color = cv::Scalar(255, 255, 255);

    if (v1 == 0 && v2 == 0 && v3 == 0) color = cv::Scalar(10, 10, 10);
    else if (v1 == 0 && v2 == 0 && v3 == 1) color = cv::Scalar(0, 0, 255);
    else if (v1 == 0 && v2 == 1 && v3 == 0) color = cv::Scalar(0, 255, 0);
    else if (v1 == 0 && v2 == 1 && v3 == 1) color = cv::Scalar(0, 255, 255);
    else if (v1 == 1 && v2 == 0 && v3 == 0) color = cv::Scalar(255, 0, 0);
    else if (v1 == 1 && v2 == 0 && v3 == 1) color = cv::Scalar(255, 0, 255);
    else if (v1 == 1 && v2 == 1 && v3 == 0) color = cv::Scalar(255, 255, 0);
    else  color = cv::Scalar(245, 245, 245);

    rectangle(
        img,
        Point(x, y),
        Point(x, y),
        color
    );
}

Mat Encode::ScaleToDisSize(const Mat& src,int outputRate)
{
    Mat dis;
    dis = Mat( HEIGHT * outputRate, WIDTH * outputRate, CV_8UC3);
    for (int i = 0; i < HEIGHT * outputRate; ++i)
    {
        for (int j = 0; j < WIDTH  * outputRate; ++j)
        {
            dis.at<Vec3b>(i, j) = src.at<Vec3b>(i / outputRate, j / outputRate);
            //cout << "i:" << i << " j:" << j << " i/out:" << i / outputRate << " j/out:" << j / outputRate << endl;
        }
    }
    return dis;
}

void Encode::initImg(int height, int width, Mat& img) {
    img = Mat(cv::Size(width, HEIGHT), CV_8UC3);
    img.setTo(Scalar(255, 255, 255));
}

void Encode::saveImg(String filePath, Mat& img) {

    cv::imwrite(filePath, img);
}

void Encode::showImg(Mat& img) {
    cv::namedWindow("image", cv::WINDOW_NORMAL);
    cv::imshow("image", img);
}
