#include"decode.h"

namespace Decode
{
	//定义像素点
	Vec3b pixcel[2] = { (0,0,0),(255,255,255) };
    //判断是否需要跳过
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
    void judge(vector<int>& data, int curC, int curR, Mat image, int count)
    {
        auto temp = image.at<Vec3b>(curR, curC);
        int t = 0;
        t = (temp == pixcel[0] ? 0 : 1);
        data[count] = t;
    }
	//读取二维码
    void readQrCode(vector<int>& data, Mat image)
    {
		int count = 0;
        for (int i = 0; i < WIDTH; i++)
        {
            for (int j = 0; j < HIGH; j++)
            {
				if (jump(j, i)) continue;
				judge(data, i, j, image, count);
				count++;
			}
		}
	}
	//主函数
    void Main(std::string codeFolderPath, vector<int>& data)
    {
        Mat image;
        // 获取图片文件列表
        std::vector<cv::String> imageFiles;
        cv::glob(codeFolderPath + "/*.png", imageFiles);  // 假设图片格式为png
        std::cout << codeFolderPath + "/*.png" << std::endl;
        for (auto imageFile : imageFiles)
        {
            image = imread(imageFile);
            readQrCode(data, image);
        }
	}
}