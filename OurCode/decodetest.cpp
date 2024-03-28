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
//�������ص�
Vec3b pixcel[2] = { (0,0,0),(255,255,255) };
//��ȡ�Զ����ά��ͼ���е�����
const int saveCorrection[6][2] =
{
    WIDTH / 2, MARGIN + 7,// ��
    MARGIN + 7, HIGH / 2 ,// ����
    WIDTH / 2, HIGH / 2 ,// ����
    WIDTH - MARGIN - 7, HIGH / 2, //����
    WIDTH / 2, HIGH - MARGIN - 8 ,//����
    WIDTH - MARGIN - 7, HIGH - MARGIN - 8,// ����
};
bool jump(int curR, int curC) {

    //Ѱ���
    if (curR <= MARGIN + 7 + 1) {
        if (curC <= MARGIN + 7 + 1 || curC >= WIDTH - (MARGIN + 7 + 1) - 1) return true;
    }
    else if (curR >= HIGH - (MARGIN + 7 + 1) - 1 && curC <= MARGIN + 7 + 1) return true;


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
//�жϸ����ص���1����0
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
            // ������λ������
            if (jump(curR, curC)) continue;
            judge(datas, curC, curR, img, index);
            index++;
        }
    }
}
int main() {
    //���ļ��е�һ��ͼ�񣬶�ȡͼ�񣬽�ͼ��ת��Ϊ�Ҷ�ͼ��
	Mat img = imread("image1.png");
    //�ж�ͼ���Ƿ�Ϊ��
    if (img.empty()) {
        		cout << "ͼ��Ϊ��" << endl;
		return -1;
    }
    ofstream pic("checkpic.png");
    pic << img;
    pic.close();
    //��ͼ��ת��Ϊ��ֵͼ��
    threshold(img, img, 100, 255, THRESH_BINARY);
    //����һ��vector����
    vector<int> datas(CAPACITY);
    //��ȡ��ά��ͼ���е�����
    readQRcode(img,datas);
    //����ȡ�������Զ�������ʽ������ļ���
    ofstream out("data.txt");
    for (int i = 0; i < datas.size(); i++) {
		out << datas[i];
	}
    out.close();
    return 0;
}
