#include "Encode.h"

void Encode::fileToImg(std::vector<int> &datas, Mat &img,std::string outputPath) {
    initImg(HIGH, WIDTH, img);
    int numOfImg = datas.size() / (CAPACITY);
    int curNum = 0;
    int index = 0, curC = 0, curR = 0;
    string filename;
    cout << "ͼƬ����" << numOfImg << endl;
    for (int i = 0; i < numOfImg; i++)
    {
        string str1 = "image";
        string str2 = to_string(i + 1);
        string str3 = ".png";
        filename = str1 + str2 + str3;

        drawBasic(img);
        int data;
        curC = curR = 0;
        int flag = 1;
        for (int curR = MARGIN; curR < HIGH - MARGIN && flag; curR++)
        {
            for (int curC = MARGIN; curC < WIDTH - MARGIN && flag; curC++)
            {
                if (datas.empty() || index >= datas.size()) {
                    flag = 0;
                    continue;
                }
                if (jump(curR, curC)) continue;
                // ������λ������

                data = datas[index++];
                drawPixel(data, curC, curR, img);
            }
        }
        curNum++;
        if (i == 0 && index != CAPACITY) cout << "���������������¼���" << endl;
        showImg(img);
        waitKey(0);
        saveImg(outputPath + '/' + filename, img);
    }
}

// ��Ҫ����������
bool Encode::jump(int curR, int curC)
{

    //Ѱ���
    if (curR <= MARGIN + 7 + 1) {
        if (curC <= MARGIN + 7 + 1 || curC >= WIDTH - (MARGIN + 7 + 1) - 1) return true;
    }
    else if (curR >= HIGH - (MARGIN + 7 + 1) -1 && curC <= MARGIN + 7 + 1) return true;


    // ������
    if (curC <= WIDTH / 2 + 2 && curC >= WIDTH / 2 - 2) // �м�������258��
    {
        if (curR <= MARGIN + 7 + 2 && curR >= MARGIN + 7 - 2) return true;
        else if (curR <= WIDTH / 2 + 2 && curR >= WIDTH / 2 - 2) return true;
        else if (curR <= WIDTH - MARGIN - 7 + 2 && curR >= WIDTH - MARGIN - 7 - 2 - 1) return true;
    }

    if (curC <= MARGIN + 7 + 2 && curC >= MARGIN + 7 - 2) // ���� 4
        if (curR >= HIGH / 2 - 2 && curR <= HIGH / 2 + 2)
            return true;

    if (curC <= WIDTH - MARGIN - 7 + 2 && curC >= WIDTH - MARGIN - 7 - 2) // ��2����69��
    {
        if (curR <= HIGH - MARGIN - 7 + 2 - 1 && curR >= HIGH - MARGIN - 7 - 2 - 1) // 9
            return true;
        else if (curR <= HIGH / 2 + 2 && curR >= HIGH / 2 - 2) // 6
            return true;
    }


    if (curC == 7 + MARGIN || curR == 7 + MARGIN) return true;

    return false;
}

// Ѱ������3����
void Encode::block(int left, int top, Mat &img) {
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
// ����λ�㣨2���ߣ�
void Encode::findLacation(Mat &img)
{
    for (int i = MARGIN + 7; i < WIDTH - 8; i++)
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
//��У���� (6����
void Encode::correction(int x, int y, Mat &img)
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
// ������3�ֶ�������ͼ��
void Encode::drawBasic(Mat &img) {

    // Ѱ������
    block(MARGIN, MARGIN, img);//����
    block(WIDTH - MARGIN - 8, MARGIN, img);// ����
    block(MARGIN, HIGH - MARGIN - 8, img); // ����

    findLacation(img);

    for (int i = 0; i < 6; i++)
    {
        correction(saveCorrection[i][0], saveCorrection[i][1], img);
    }
}

// ����������ֵ
void Encode::drawPixel(int val, int x, int y, Mat &img) {
    if (val == 0)
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

void Encode::initImg(int high, int width, Mat &img) {
    img = Mat(cv::Size(high, width), CV_8UC3);
    img.setTo(Scalar(255, 255, 255));
}

void Encode::saveImg(String filePath, Mat &img) {
    cv::imwrite(filePath, img);
}

void Encode::showImg(Mat &img) {
    cv::namedWindow("image", cv::WINDOW_NORMAL);
    cv::imshow("image", img);
}
