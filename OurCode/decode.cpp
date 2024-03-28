#include"decode.h"

namespace Decode
{
	//�������ص�
    Vec3b pixel[2] = { (0,0,0),(255,255,255) };
    //�ж��Ƿ���Ҫ����
    bool jump(int curR, int curC)
    {

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
            else if (curR <= WIDTH - MARGIN - 7 + 2 - 1 && curR >= WIDTH - MARGIN - 7 - 2 - 1) return true;
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

    void showimg(Mat &img, float x, float y)
    {
        Size dsize = Size(round(x * img.cols), round(y * img.rows));
        resize(img, img, dsize, 0, 0);
        imshow("image", img);
    }
    

    //�жϸ����ص���1����0
    void judge(vector<int>& data, int curR, int curC, Mat image)
    {
        auto temp = image.at<Vec3b>(curR, curC);
        int t = 0;
        t = (temp == pixel[0] ? 0 : 1);
        data.push_back(t);
    }
	//��ȡ��ά��
    void readQrCode(vector<int>& data, Mat &image)
    {
        //showimg(image,3,3);
		int count = 0;
        for (int i = MARGIN; i < WIDTH-MARGIN; i++)
        {
            for (int j = MARGIN; j < HIGH - MARGIN; j++)
            {
                if (jump(j, i)) continue;
                judge(data, i, j, image);
            }
		}
	}
	//������
    void Main(std::string codeFolderPath, vector<int>& data)
    {
        Mat image;
        // ��ȡͼƬ�ļ��б�
        std::vector<cv::String> imageFiles;
        cv::glob(codeFolderPath + "/*.png", imageFiles);  // ����ͼƬ��ʽΪpng
        std::cout << codeFolderPath + "/*.png" << std::endl;
        for (auto imageFile : imageFiles)
        {
            image = imread(imageFile);
            readQrCode(data, image);
        }
	}
}