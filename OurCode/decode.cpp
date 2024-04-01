#include"decode.h"

namespace Decode
{
    Vec3b pixel1[2] = { {0,0,0},{255,255,255} };


    // �� �� �� ��
    //Vec3b lowerThresholds[4] = {
    //    {0, 0, 0},// 00
    //    {150, 80, 0},//01
    //    {0, 40, 150},//10
    //    {181,156, 156}//11
    //};
    //Vec3b upperThresholds[4] = {
    //    {149, 149, 149},//00
    //    {255, 200, 135},//01
    //    {180, 160, 255},//10
    //    {255, 255, 255}//11
    //};


    // �� �� �� ��
    Vec3b lowerThresholds[4] = {
        {0, 0, 0},// 00
        {150, 80, 0},//01
        {0, 40, 150},//10
        {156,156, 156}//11
    };
    Vec3b upperThresholds[4] = {
        {149, 149, 149},//00
        {255, 200, 135},//01
        {155, 160, 255},//10
        {255, 255, 255}//11
    };

    bool isColorConflict(Vec3b* upperThresholds, Vec3b* lowerThresholds)
    {

        // ����ÿ����ɫͨ��
        for (int i = 0; i < 4; i++) {
            // ���ÿ����ɫ��Χ�Ƿ��ͻ
            for (int j = i + 1; j < 4; j++) {
                if (upperThresholds[i][0] >= lowerThresholds[j][0] &&
                    lowerThresholds[i][0] <= upperThresholds[j][0] &&
                    upperThresholds[i][1] >= lowerThresholds[j][1] &&
                    lowerThresholds[i][1] <= upperThresholds[j][1] &&
                    upperThresholds[i][2] >= lowerThresholds[j][2] &&
                    lowerThresholds[i][2] <= upperThresholds[j][2])
                {
                    cout << "��ɫ��ͻ" << endl;
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
                                        lowerThresholds[0][2] <= m && m <= upperThresholds[0][2]) cout << " ��";
                                    if (lowerThresholds[1][0] <= k && k <= upperThresholds[1][0] &&
                                        lowerThresholds[1][1] <= l && l <= upperThresholds[1][1] &&
                                        lowerThresholds[1][2] <= m && m <= upperThresholds[1][2]) cout << " ��";
                                    if (lowerThresholds[2][0] <= k && k <= upperThresholds[2][0] &&
                                        lowerThresholds[2][1] <= l && l <= upperThresholds[2][1] &&
                                        lowerThresholds[2][2] <= m && m <= upperThresholds[2][2]) cout << " ��";
                                    if (lowerThresholds[3][0] <= k && k <= upperThresholds[3][0] &&
                                        lowerThresholds[3][1] <= l && l <= upperThresholds[3][1] &&
                                        lowerThresholds[3][2] <= m && m <= upperThresholds[3][2]) cout << " ��";
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

        // û�г�ͻ
        return false;

    }

    // ��֡����ȡ��ά��
    Mat extractCodeHelper(Mat& src)
    {
        Mat image = src.clone();

        // ת��Ϊ�Ҷ�ͼ��
        cvtColor(image, image, COLOR_BGR2GRAY);

        // ��˹
        GaussianBlur(image, image, Size(5, 5), 0); // ʹ��5x5���ں˽��и�˹ģ��

        // ��ֵ����ֵ
        int thresholdValue = 128; // ��ֵ����Χ0-255
        int maxValue = 255; // ���ֵ��ͨ��Ϊ��ɫ

        // ʹ����ֵ���ж�ֵ��
        threshold(image, image, thresholdValue, maxValue, THRESH_BINARY);

        // ��������
        Canny(image, image, 50, 150);

        // Ѱ������
        vector<vector<Point>> contours;
        findContours(image, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

        // ��������������ı���
        int largestContourIndex = -1;
        double largestArea = 0;
        vector<Point> approxCurve;
        for (size_t i = 0; i < contours.size(); i++) {
            double perimeter = arcLength(contours[i], true);
            approxPolyDP(contours[i], approxCurve, 0.02 * perimeter, true);
            if (approxCurve.size() != 4) continue;
            double area = contourArea(contours[i]);
            if (area > largestArea) {
                largestArea = area;
                largestContourIndex = i;
            }
        }

        double perimeter = arcLength(contours[largestContourIndex], true);
        approxPolyDP(contours[largestContourIndex], approxCurve, 0.02 * perimeter, true);

        double width = norm(approxCurve[0] - approxCurve[1]);
        double height = norm(approxCurve[1] - approxCurve[2]);
        Point2f dstPoints[4];
        if (width > height)
        {
            //cout << "width:" <<width<<" > height: "<<height<<"." << endl;
            width = 1990;
            height = 1110;
            dstPoints[0] = Point2f(width, 0);
            dstPoints[1] = Point2f(0, 0);
            dstPoints[2] = Point2f(0, height);
            dstPoints[3] = Point2f(width, height);

        }
        else
        {
            //cout << "width:" << width << " < height: " << height << "." << endl;
            width = 1990;
            height = 1110;
            dstPoints[0] = Point2f(0, 0);
            dstPoints[1] = Point2f(0, height);
            dstPoints[2] = Point2f(width, height);
            dstPoints[3] = Point2f(width, 0);
        }
                
        Point2f srcPoints[4];

        for (int j = 0; j < 4; ++j) {
            srcPoints[j] = approxCurve[j];
        }
        Mat perspectiveMatrix = getPerspectiveTransform(srcPoints, dstPoints);
        Mat warpedImage;
        warpPerspective(src, warpedImage, perspectiveMatrix, Size(width, height));

        // ��ʾ���
        return warpedImage;
    }
    int extractCode(string& inFolderPath, string outFolderPath)
    {
        // ��֤·��������Ϊ��
        Files::create_or_clear_directory(outFolderPath);

        // ��ȡͼƬ�ļ��б�
        std::vector<cv::String> imageFiles;
        cv::glob(inFolderPath + "/*" + PICFORMAT, imageFiles);  // ����ͼƬ��ʽΪpng
        if (imageFiles.size() == 0)
        {
            cout << "����֡ͼƬ·��" << inFolderPath << "Ϊ��" << endl;
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

    //�ж��Ƿ���Ҫ����
    bool jump(int curR, int curC)
    {

        //Ѱ���
        if (curR <= MARGIN + 7 + 1) {
            if (curC <= MARGIN + 7 + 1 || curC >= WIDTH - (MARGIN + 7 + 1) - 1) return true;
        }
        else if (curR >= HEIGHT - (MARGIN + 7 + 1) - 1 && curC <= MARGIN + 7 + 1) return true;


        // ������
        if (curC <= WIDTH / 2 + 2 && curC >= WIDTH / 2 - 2) // �м�������258��
        {
            if (curR <= MARGIN + 7 + 2 && curR >= MARGIN + 7 - 2) return true;
            else if (curR <= HEIGHT / 2 + 2 && curR >= HEIGHT / 2 - 2) return true;
            else if (curR <= HEIGHT - MARGIN - 7 + 2 - 1 && curR >= HEIGHT - MARGIN - 7 - 2 - 1) return true;
        }

        if (curC <= MARGIN + 7 + 2 && curC >= MARGIN + 7 - 2) // ���� 4
            if (curR >= HEIGHT / 2 - 2 && curR <= HEIGHT / 2 + 2)
                return true;

        if (curC <= WIDTH - MARGIN - 7 + 2 - 1 && curC >= WIDTH - MARGIN - 7 - 2 - 1) // ��2����69��
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

    //�жϸ����ص���1����0
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


    //��ȡ��ά��
    int readCodeHelper(int curCodeNo,vector<int>& data, Mat& image, vector<int>& origianl, int debug)
    {
        //showimg(image,3,3);
        int count = curCodeNo*CAPACITY;
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
                    data.push_back(val);
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
                    data.push_back(val1);
                    data.push_back(val2);
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
    //������
    int readCode(std::string extCodePath, vector<int>& data, vector<int>& origianl,int debugMode)
    {
        Mat image;
        // ��ȡͼƬ�ļ��б�
        std::vector<cv::String> imageFiles;
        if (isColorConflict(upperThresholds, lowerThresholds)) return -1;
        cv::glob(extCodePath + "/*" + PICFORMAT, imageFiles);  // ����ͼƬ��ʽΪpng
        if (imageFiles.size() == 0)
        {
            cout << "������ȡ��ά��·��"<<extCodePath<<"Ϊ��" << endl;
        }
        for (int i = 0;i< imageFiles.size();i++)
        {
            image = imread(imageFiles[i]);
            int error = readCodeHelper(i,data, image, origianl, debugMode);
            if (error == -1) return -1;
            
            cout << "ͼ" << to_string(i + 1) << " ������" << error << " ������" << (float)error / CAPACITY * BIT << endl;
            if(debugMode==1)imwrite("error"+to_string(i)+PICFORMAT, image);
        }
        return 0;
    }


}