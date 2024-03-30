#include"decode.h"

namespace Decode
{
    Vec3b pixel1[2] = { {0,0,0},{255,255,255} };

    // 黑 蓝 橙 白
    //Vec3b lowerThresholds[4] = {
    //    {0, 0, 0},// 00
    //    {170, 109, 0},//01
    //    {9, 86, 200},//10
    //    {156,156, 156}//11
    //};
    //Vec3b upperThresholds[4] = {
    //    {155, 155, 155},//00
    //    {255, 185, 100},//01
    //    {155, 160, 255},//10
    //    {255, 255, 255}//11
    //};

    Vec3b lowerThresholds[4] = {
        {0, 0, 0},// 00
        {160, 80, 0},//01
        {0, 60, 160},//10
        {156,156, 156}//11
    };
    Vec3b upperThresholds[4] = {
        {155, 155, 155},//00
        {255, 200, 130},//01
        {155, 160, 255},//10
        {255, 255, 255}//11
    };

    void test(vector<int>& data, Mat& image, vector<int>& origianl)
    {
        int arr[24];
        int error;
        float errorRate = 1;
        float newRate;
        int length = 256;
        for (int a = 0; a < length; a++)
        {
            for (int b = 0; b < length; b++)
            {
                for (int c = 0; c < length; c++)
                {
                    for (int d = 0; d < length; d++)
                    {
                        for (int e = 0; e < length; e++)
                        {
                            for (int f = 0; f < length; f++)
                            {
                                for (int g = 0; g < length; g++)
                                {
                                    for (int h = 0; h < length; h++)
                                    {
                                        for (int i = 0; i < length; i++)
                                        {
                                            for (int j = 140; j < length; j++)
                                            {
                                                for (int k = 140; k < length; k++)
                                                {
                                                    for (int l = 140; l < length; l++)
                                                    {
                                                        for (int m = 0; m < 171; m++)
                                                        {
                                                            for (int n = 0; n < 171; n++)
                                                            {
                                                                for (int o = 0; o < 171; o++)
                                                                {
                                                                    for (int p = 0; p < length; p++)
                                                                    {
                                                                        for (int q = 0; q < length; q++)
                                                                        {
                                                                            for (int r = 0; r < length; r++)
                                                                            {
                                                                                for (int s = 0; s < length; s++)
                                                                                {
                                                                                    for (int t = 0; t < length; t++)
                                                                                    {
                                                                                        for (int u = 0; u < length; u++)
                                                                                        {
                                                                                            for (int v = 0; v < length; v++)
                                                                                            {
                                                                                                for (int w = 0; w < length; w++)
                                                                                                {
                                                                                                    for (int x = 0; x < length; x++)
                                                                                                    {
                                                                                                        lowerThresholds[0][0] = a;
                                                                                                        lowerThresholds[0][1] = b;
                                                                                                        lowerThresholds[0][2] = c;
                                                                                                        lowerThresholds[1][0] = d;
                                                                                                        lowerThresholds[1][1] = e;
                                                                                                        lowerThresholds[1][2] = f;
                                                                                                        lowerThresholds[2][0] = g;
                                                                                                        lowerThresholds[2][1] = h;
                                                                                                        lowerThresholds[2][2] = i;
                                                                                                        lowerThresholds[3][0] = j;
                                                                                                        lowerThresholds[3][1] = k;
                                                                                                        lowerThresholds[3][2] = l;

                                                                                                        upperThresholds[0][0] = m;
                                                                                                        upperThresholds[0][1] = n;
                                                                                                        upperThresholds[0][2] = o;
                                                                                                        upperThresholds[1][0] = p;
                                                                                                        upperThresholds[1][1] = q;
                                                                                                        upperThresholds[1][2] = r;
                                                                                                        upperThresholds[2][0] = s;
                                                                                                        upperThresholds[2][1] = t;
                                                                                                        upperThresholds[2][2] = u;
                                                                                                        upperThresholds[3][0] = v;
                                                                                                        upperThresholds[3][1] = w;
                                                                                                        upperThresholds[3][2] = x;

                                                                                                        if (isColorConflict(upperThresholds, lowerThresholds)) continue;
                                                                                                        error = readCodeHelper(data, image, origianl);
                                                                                                        newRate = (float)error / CAPACITY * BIT;
                                                                                                        if (newRate < errorRate)
                                                                                                        {
                                                                                                            errorRate = newRate;
                                                                                                            arr[0] = a;
                                                                                                            arr[1] = b;
                                                                                                            arr[2] = c;
                                                                                                            arr[3] = d;
                                                                                                            arr[4] = e;
                                                                                                            arr[5] = f;
                                                                                                            arr[6] = g;
                                                                                                            arr[7] = h;
                                                                                                            arr[8] = i;
                                                                                                            arr[9] = j;
                                                                                                            arr[10] = k;
                                                                                                            arr[11] = l;
                                                                                                            arr[12] = m;
                                                                                                            arr[13] = n;
                                                                                                            arr[14] = o;
                                                                                                            arr[15] = p;
                                                                                                            arr[16] = q;
                                                                                                            arr[17] = r;
                                                                                                            arr[18] = s;
                                                                                                            arr[19] = t;
                                                                                                            arr[20] = u;
                                                                                                            arr[21] = v;
                                                                                                            arr[22] = w;
                                                                                                            arr[23] = x;
                                                                                                            cout << newRate << endl;
                                                                                                        }
                                                                                                    }
                                                                                                }
                                                                                            }
                                                                                        }
                                                                                    }
                                                                                }
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    bool isColorConflict(Vec3b* upperThresholds, Vec3b* lowerThresholds)
    {

        // 遍历每个颜色通道
        for (int i = 0; i < 4; i++) {
            // 检查每个颜色范围是否冲突
            for (int j = i + 1; j < 4; j++) {
                if (upperThresholds[i][0] >= lowerThresholds[j][0] &&
                    lowerThresholds[i][0] <= upperThresholds[j][0] &&
                    upperThresholds[i][1] >= lowerThresholds[j][1] &&
                    lowerThresholds[i][1] <= upperThresholds[j][1] &&
                    upperThresholds[i][2] >= lowerThresholds[j][2] &&
                    lowerThresholds[i][2] <= upperThresholds[j][2])
                {
                    cout << "颜色冲突" << endl;
                    for (int k = 0; k < 256; k++)
                    {
                        for (int l = 0; l < 256; l++)
                        {
                            for (int m = 0; m < 256; m++)
                            {
                                int cnt = 0;
                                if (lowerThresholds[0][0] <= k && k <= upperThresholds[0][0] &&
                                    lowerThresholds[0][1] <= l && l <= upperThresholds[0][1] &&
                                    lowerThresholds[0][2] <= m && m <= upperThresholds[0][2]) cnt++;
                                if (lowerThresholds[1][0] <= k && k <= upperThresholds[1][0] &&
                                    lowerThresholds[1][1] <= l && l <= upperThresholds[1][1] &&
                                    lowerThresholds[1][2] <= m && m <= upperThresholds[1][2]) cnt++;
                                if (lowerThresholds[2][0] <= k && k <= upperThresholds[2][0] &&
                                    lowerThresholds[2][1] <= l && l <= upperThresholds[2][1] &&
                                    lowerThresholds[2][2] <= m && m <= upperThresholds[2][2]) cnt++;
                                if (lowerThresholds[3][0] <= k && k <= upperThresholds[3][0] &&
                                    lowerThresholds[3][1] <= l && l <= upperThresholds[3][1] &&
                                    lowerThresholds[3][2] <= m && m <= upperThresholds[3][2]) cnt++;
                                if (cnt > 1)
                                {
                                    cout << "k: " << k << " l: " << l << " m: " << m;
                                    if (lowerThresholds[0][0] <= k && k <= upperThresholds[0][0] &&
                                        lowerThresholds[0][1] <= l && l <= upperThresholds[0][1] &&
                                        lowerThresholds[0][2] <= m && m <= upperThresholds[0][2]) cout << " 黑";
                                    if (lowerThresholds[1][0] <= k && k <= upperThresholds[1][0] &&
                                        lowerThresholds[1][1] <= l && l <= upperThresholds[1][1] &&
                                        lowerThresholds[1][2] <= m && m <= upperThresholds[1][2]) cout << " 蓝";
                                    if (lowerThresholds[2][0] <= k && k <= upperThresholds[2][0] &&
                                        lowerThresholds[2][1] <= l && l <= upperThresholds[2][1] &&
                                        lowerThresholds[2][2] <= m && m <= upperThresholds[2][2]) cout << " 橙";
                                    if (lowerThresholds[3][0] <= k && k <= upperThresholds[3][0] &&
                                        lowerThresholds[3][1] <= l && l <= upperThresholds[3][1] &&
                                        lowerThresholds[3][2] <= m && m <= upperThresholds[3][2]) cout << " 白";
                                    cout << endl;
                                    cout << lowerThresholds[1][0] << upperThresholds[1][0] << endl;
                                    return false;
                                }
                            }
                        }
                    }
                    return true;
                }
            }
        }

        // 没有冲突
        return false;

    }

    // 从帧中提取二维码
    Mat extractCodeHelper(Mat& src)
    {
        Mat image = src.clone();

        // 转换为灰度图像
        cvtColor(image, image, COLOR_BGR2GRAY);

        // 高斯
        GaussianBlur(image, image, Size(5, 5), 0); // 使用5x5的内核进行高斯模糊

        // 二值化阈值
        int thresholdValue = 128; // 阈值，范围0-255
        int maxValue = 255; // 最大值，通常为白色

        // 使用阈值进行二值化
        threshold(image, image, thresholdValue, maxValue, THRESH_BINARY);

        // 查找轮廓
        Canny(image, image, 50, 150);

        // 寻找轮廓
        vector<vector<Point>> contours;
        findContours(image, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

        for (size_t i = 0; i < contours.size(); ++i) {
            double perimeter = arcLength(contours[i], true);
            vector<Point> approxCurve;
            approxPolyDP(contours[i], approxCurve, 0.02 * perimeter, true);
            if (approxCurve.size() == 4) { // 如果是四边形
                // 绘制找到的矩形
                //for (int j = 0; j < 4; ++j) {
                //    line(image, approxCurve[j], approxCurve[(j + 1) % 4], Scalar(0, 255, 0), 2);
                //}

                int width = 1990;
                int height = 1110;

                Point2f srcPoints[4];
                Point2f dstPoints[4] = { Point2f(0, 0), Point2f(0, height), Point2f(width, height) ,Point2f(width, 0) };

                for (int j = 0; j < 4; ++j) {
                    srcPoints[j] = approxCurve[j];
                }
                Mat perspectiveMatrix = getPerspectiveTransform(srcPoints, dstPoints);
                Mat warpedImage;
                warpPerspective(src, warpedImage, perspectiveMatrix, Size(width, height));

                // 显示结果
                return warpedImage;
            }

        }
    }
    int extractCode(string& inFolderPath, string outFolderPath)
    {
        // 保证路径存在且为空
        Files::create_or_clear_directory(outFolderPath);

        // 获取图片文件列表
        std::vector<cv::String> imageFiles;
        cv::glob(inFolderPath + "/*" + PICFORMAT, imageFiles);  // 假设图片格式为png
        if (imageFiles.size() == 0)
        {
            cout << inFolderPath << "目录为空" << endl;
            return -1;
        }
        int cnt = 1;
        for (auto imgfile : imageFiles)
        {
            Mat src = imread(imgfile);
            Mat extCode = extractCodeHelper(src);

            imwrite(outFolderPath + "/extractedCode" + to_string(cnt++) + PICFORMAT, extCode);
        }
        return 0;
    }

    //判断是否需要跳过
    bool jump(int curR, int curC)
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

        if (curC <= WIDTH - MARGIN - 7 + 2 - 1 && curC >= WIDTH - MARGIN - 7 - 2 - 1) // 右2个（69）
        {
            if (curR <= HEIGHT - MARGIN - 7 + 2 - 1 && curR >= HEIGHT - MARGIN - 7 - 2 - 1) // 9
                return true;
            else if (curR <= HEIGHT / 2 + 2 && curR >= HEIGHT / 2 - 2) // 6
                return true;
        }


        if (curC == 7 + MARGIN || curR == 7 + MARGIN) return true;

        return false;
    }
    void showimg(Mat& img, float x, float y)
    {
        Size dsize = Size(round(x * img.cols), round(y * img.rows));
        resize(img, img, dsize, 0, 0);
        imshow("image", img);
    }

    //判断该像素点是1或者0
    inline void judge1(cv::Vec3b thisPixel, int& val)
    {
        val = (thisPixel == pixel1[0] ? 0 : 1);
    }
    inline void judge2(cv::Vec3b thisPixel, int& val1, int& val2)
    {
        bool is_black = (thisPixel[0] >= lowerThresholds[0][0] && thisPixel[0] <= upperThresholds[0][0]) &&
            (thisPixel[1] >= lowerThresholds[0][1] && thisPixel[1] <= upperThresholds[0][1]) &&
            (thisPixel[2] >= lowerThresholds[0][2] && thisPixel[2] <= upperThresholds[0][2]);

        bool is_blue = (thisPixel[0] >= lowerThresholds[1][0] && thisPixel[0] <= upperThresholds[1][0]) &&
            (thisPixel[1] >= lowerThresholds[1][1] && thisPixel[1] <= upperThresholds[1][1]) &&
            (thisPixel[2] >= lowerThresholds[1][2] && thisPixel[2] <= upperThresholds[1][2]);

        bool is_orange = (thisPixel[0] >= lowerThresholds[2][0] && thisPixel[0] <= upperThresholds[2][0]) &&
            (thisPixel[1] >= lowerThresholds[2][1] && thisPixel[1] <= upperThresholds[2][1]) &&
            (thisPixel[2] >= lowerThresholds[2][2] && thisPixel[2] <= upperThresholds[2][2]);

        bool is_white = (thisPixel[0] >= lowerThresholds[3][0] && thisPixel[0] <= upperThresholds[3][0]) &&
            (thisPixel[1] >= lowerThresholds[3][1] && thisPixel[1] <= upperThresholds[3][1]) &&
            (thisPixel[2] >= lowerThresholds[3][2] && thisPixel[2] <= upperThresholds[3][2]);

        if (is_black) val1 = 0, val2 = 0;
        else if (is_blue) val1 = 0, val2 = 1;
        else if (is_orange) val1 = 1, val2 = 0;
        else if (is_white) val1 = 1, val2 = 1;
    }


    //读取二维码
    int readCodeHelper(vector<int>& data, Mat& image, vector<int>& origianl, int debug)
    {
        //showimg(image,3,3);
        int count = 0;
        int error = 0;
        for (int curR = MARGIN; curR < HEIGHT - MARGIN; curR++)
        {
            for (int curC = MARGIN; curC < WIDTH - MARGIN; curC++)
            {
                if (jump(curR, curC)) continue;
                auto pixel = image.at<Vec3b>(curR * MULTIPLE + MULTIPLE / 2 - 1, curC * MULTIPLE + MULTIPLE / 2 - 1);
                if (BIT == 1)
                {
                    int val;
                    int oVal = origianl[count++];
                    judge1(pixel, val);
                    if (val != oVal)
                    {

                        cout << "curR: " << curR << " curC: " << curC;
                        cout << " Ori: " << oVal;
                        cout << " RGB: " << pixel << endl;
                        return -1;
                    }
                }

                else if (BIT == 2)
                {
                    int val1, val2, oVal1, oVal2;
                    oVal1 = origianl[count++], oVal2 = origianl[count++];
                    judge2(pixel, val1, val2);
                    if (val1 != oVal1 || val2 != oVal2)
                    {
                        error++;
                        if (debug == 1)
                        {
                            cout << "curR: " << curR << " curC: " << curC;
                            cout << " Ori: " << oVal1 << " " << oVal2;
                            cout << " cur:" << val1 << " " << val2;
                            cout << " RGB: " << pixel << endl;
                        
                        
                            rectangle(
                                image,
                                cv::Point(curC * MULTIPLE, curR * MULTIPLE),
                                cv::Point(curC * MULTIPLE + MULTIPLE, curR * MULTIPLE + MULTIPLE),
                                Scalar(0, 255, 255),
                                2
                            );
                            
                        }
                    }
                }
            }
        }
        //cout << error;
        return error;
    }
    //主函数
    int readCode(std::string extCodePath, vector<int>& data, vector<int>& origianl)
    {
        Mat image;
        // 获取图片文件列表
        std::vector<cv::String> imageFiles;
        if (isColorConflict(upperThresholds, lowerThresholds)) return -1;
        cv::glob(extCodePath + "/*" + PICFORMAT, imageFiles);  // 假设图片格式为png
        std::cout << extCodePath + "/*" + PICFORMAT << std::endl;
        for (int i = 0;i< imageFiles.size();i++)
        {
            image = imread(imageFiles[i]);
            int error = readCodeHelper(data, image, origianl,1);
            if (error == -1) return -1;
            
            cout << "图" << to_string(i + 1) <<"误码"<<error << " 误码率" << (float)error / CAPACITY * BIT;
            imwrite("error"+to_string(i)+PICFORMAT, image);
        }
        return 0;
    }


}