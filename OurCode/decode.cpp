#include"decode.h"

namespace Decode
{
    Vec3b pixel1[2] = { {0,0,0},{255,255,255} };

    // 黑 蓝 橙 白
    //Vec3b lowerThresholds[4] = {
    //    {0, 0, 0},// 00
    //    {150, 80, 0},//01
    //    {0, 40, 150},//10
    //    {156,156, 156}//11
    //};
    //Vec3b upperThresholds[4] = {
    //    {149, 149, 149},//00
    //    {255, 211, 135},//01
    //    {155, 160, 255},//10
    //    {255, 255, 255}//11
    //};

    int COLORMODE = 0;
    // 0暗,1亮
    Vec3b lowerThresholds[2][4] = {
        {
            {0, 0, 0},// 00
            {150, 80, 0},//01
            {0, 40, 150},//10
            {156,156, 156}//11
         },
        {
            {0, 0, 0},// 00
            {150, 60, 0},//01
            {0, 40, 150},//10
            {190,190, 190}//11
         }

    };
    Vec3b upperThresholds[2][4] = {
        {
            {149, 149, 149},//00
            {255, 211, 135},//01
            {155, 160, 255},//10
            {255, 255, 255}//11
         },
        {
            {149, 149, 149},//00
            {255, 211, 135},//01
            {180, 180, 255},//10
            {255, 255, 255}//11
         }
    };


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
    void threadExtract(vector<Mat>& images,vector<String> paths)
    {
        std::vector<std::thread> threads;
        for (int i = 0; i < images.size(); i++) {
            threads.push_back(std::thread([i, &images,&paths] {
                extractCodeHelper(images[i],paths[i]);
                }));
        }

        for (auto& thread : threads) {
            thread.join();
        }
    }
    // 直接保存
    void extractCodeHelper(Mat& src,String& path)
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

        // 查找最大轮廓的四边形
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
            width = WIDTH * MULTIPLE;
            height = HEIGHT * MULTIPLE;
            dstPoints[0] = Point2f(width, 0);
            dstPoints[1] = Point2f(0, 0);
            dstPoints[2] = Point2f(0, height);
            dstPoints[3] = Point2f(width, height);

        }
        else
        {
            //cout << "width:" << width << " < height: " << height << "." << endl;
            width = WIDTH * MULTIPLE;
            height = HEIGHT * MULTIPLE;
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

        // 显示结果
        imwrite(path, warpedImage);

    }
    // 返回单张图
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

        // 查找最大轮廓的四边形
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
            width = WIDTH * MULTIPLE;
            height = HEIGHT * MULTIPLE;
            dstPoints[0] = Point2f(width, 0);
            dstPoints[1] = Point2f(0, 0);
            dstPoints[2] = Point2f(0, height);
            dstPoints[3] = Point2f(width, height);

        }
        else
        {
            //cout << "width:" << width << " < height: " << height << "." << endl;
            width = WIDTH * MULTIPLE;
            height = HEIGHT * MULTIPLE;
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

        // 显示结果
        return warpedImage;
    }

    int extractCode(string& inFolderPath, string outFolderPath)
    {
        // 保证路径存在且为空
        Files::create_or_clear_directory(outFolderPath);

        // 获取图片文件列表
        std::vector<cv::String> imageFiles;
        std::vector<cv::Mat> images;
        std::vector<cv::Mat> waitImages;// 待处理图像
        cv::Mat src;
        std::vector<cv::String> paths;
        cv::String path;
        cv::glob(inFolderPath + "/*" + PICFORMAT, imageFiles);  // 假设图片格式为png
        if (imageFiles.size() == 0)
        {
            cout << "错误：帧图片路径" << inFolderPath << "为空" << endl;
            return -1;
        }
        String fileName = imageFiles[0].substr(0, imageFiles[0].size() - 5);
        for (int i = 1; i <= imageFiles.size(); i++)
        {
            src = imread(fileName+to_string(i)+PICFORMAT);
            path = outFolderPath + "/extractedCode" + to_string(i) + PICFORMAT;
            paths.push_back(path);
            cout << "提取二维码：" << path << endl;
            //namedWindow("src", WINDOW_NORMAL);
            //imshow("src",src);
            //waitKey(0);


            //images.push_back(extractCodeHelper(src));
            //if(i!=0&&i%THREADCNT==0)
            //{
            //    Files::threadSave(images, paths);
            //    images.clear();
            //    paths.clear();
            //}

            
            images.push_back(imread(fileName + to_string(i) + PICFORMAT));
            if (i != 0 && i % THREADCNT == 0)
            {
                threadExtract(images, paths);
                images.clear();
                paths.clear();
            }

        }
        threadExtract(images, paths);
        images.clear();
        paths.clear();

        //Files::threadSave(images, paths);
        //images.clear();
        //paths.clear();
        //return 0;
    }

    //判断是否需要跳过
    bool jump(int curR, int curC)
    {

        //寻像点
        if (curR <= MARGIN + 7 + 1) {
            if (curC <= MARGIN + 7 + 1 || curC >= WIDTH - (MARGIN + 7 + 1) - 1) return true;
        }
        else if (curR >= HEIGHT - (MARGIN + 7 + 1) - 1 && curC <= MARGIN + 7 + 1) return true;


        //// 矫正点
        //if (curC <= WIDTH / 2 + 2 && curC >= WIDTH / 2 - 2) // 中间三个（258）
        //{
        //    if (curR <= MARGIN + 7 + 2 && curR >= MARGIN + 7 - 2) return true;
        //    else if (curR <= HEIGHT / 2 + 2 && curR >= HEIGHT / 2 - 2) return true;
        //    else if (curR <= HEIGHT - MARGIN - 7 + 2 - 1 && curR >= HEIGHT - MARGIN - 7 - 2 - 1) return true;
        //}

        //if (curC <= MARGIN + 7 + 2 && curC >= MARGIN + 7 - 2) // 左中 4
        //    if (curR >= HEIGHT / 2 - 2 && curR <= HEIGHT / 2 + 2)
        //        return true;

        //if (curC <= WIDTH - MARGIN - 7 + 2 - 1 && curC >= WIDTH - MARGIN - 7 - 2 - 1) // 右2个（69）
        //{
        //    if (curR <= HEIGHT - MARGIN - 7 + 2 - 1 && curR >= HEIGHT - MARGIN - 7 - 2 - 1) // 9
        //        return true;
        //    else if (curR <= HEIGHT / 2 + 2 && curR >= HEIGHT / 2 - 2) // 6
        //        return true;
        //}


        //if (curC == 7 + MARGIN || curR == 7 + MARGIN) return true;

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
        bool is_black = (thisPixel[0] >= lowerThresholds[COLORMODE][0][0] && thisPixel[0] <= upperThresholds[COLORMODE][0][0]) &&
            (thisPixel[1] >= lowerThresholds[COLORMODE][0][1] && thisPixel[1] <= upperThresholds[COLORMODE][0][1]) &&
            (thisPixel[2] >= lowerThresholds[COLORMODE][0][2] && thisPixel[2] <= upperThresholds[COLORMODE][0][2]);

        bool is_blue = (thisPixel[0] >= lowerThresholds[COLORMODE][1][0] && thisPixel[0] <= upperThresholds[COLORMODE][1][0]) &&
            (thisPixel[1] >= lowerThresholds[COLORMODE][1][1] && thisPixel[1] <= upperThresholds[COLORMODE][1][1]) &&
            (thisPixel[2] >= lowerThresholds[COLORMODE][1][2] && thisPixel[2] <= upperThresholds[COLORMODE][1][2]);

        bool is_orange = (thisPixel[0] >= lowerThresholds[COLORMODE][2][0] && thisPixel[0] <= upperThresholds[COLORMODE][2][0]) &&
            (thisPixel[1] >= lowerThresholds[COLORMODE][2][1] && thisPixel[1] <= upperThresholds[COLORMODE][2][1]) &&
            (thisPixel[2] >= lowerThresholds[COLORMODE][2][2] && thisPixel[2] <= upperThresholds[COLORMODE][2][2]);

        bool is_white = (thisPixel[0] >= lowerThresholds[COLORMODE][3][0] && thisPixel[0] <= upperThresholds[COLORMODE][3][0]) &&
            (thisPixel[1] >= lowerThresholds[COLORMODE][3][1] && thisPixel[1] <= upperThresholds[COLORMODE][3][1]) &&
            (thisPixel[2] >= lowerThresholds[COLORMODE][3][2] && thisPixel[2] <= upperThresholds[COLORMODE][3][2]);

        if (is_black) val1 = 0, val2 = 0;
        else if (is_blue) val1 = 0, val2 = 1;
        else if (is_orange) val1 = 1, val2 = 0;
        else if (is_white) val1 = 1, val2 = 1;
    }

    int setColorMode(Mat& img,float ratio)
    {
        Vec3b pixel;
        int totalRGB = 0;
        int rowBegin = (0.5 - ratio / 2) * HEIGHT;
        int colBegin = (0.5 - ratio / 2) * WIDTH;
        for(int row = rowBegin; row<rowBegin+HEIGHT*ratio;row+=MULTIPLE)
        {
            for (int col = colBegin; col < colBegin+WIDTH*ratio; col+= MULTIPLE)
            {
                pixel = img.at<Vec3b>(row, col);
                totalRGB += pixel[0];
                totalRGB += pixel[1];
                totalRGB += pixel[2];
            }
        }
        //cout << totalRGB << endl;
        return totalRGB;
    }

    //读取二维码
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
                auto pixel = image.at<Vec3b>(curR * MULTIPLE + MULTIPLE / 2  , curC * MULTIPLE + MULTIPLE / 2 );
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
                        if (debug == 2)
                        {
                            cout << "curR: " << curR << " curC: " << curC;
                            cout << " Ori: " << oVal1 << " " << oVal2;
                            cout << " cur:" << val1 << " " << val2;
                            cout << " RGB: " << pixel << endl;

                        }
                        if(debug==1){
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
    int readCode(std::string extCodePath, vector<int>& data, vector<int>& origianl,int debugMode)
    {
        int totalRGB = 0;
        Files::create_or_clear_directory("errors");
        Mat image;
        // 获取图片文件列表
        std::vector<cv::String> imageFiles;
        cv::glob(extCodePath + "/*" + PICFORMAT, imageFiles);  // 假设图片格式为png
        String fileName = imageFiles[0].substr(0, imageFiles[0].size() - 5);
        if (imageFiles.size() == 0)
        {
            cout << "错误：提取二维码路径"<<extCodePath<<"为空" << endl;
        }
        //int index;
        //for ( index = 1; index <= 5; index++)
        //{
        //    image = imread(fileName + to_string(index) + PICFORMAT);
        //    totalRGB += setColorMode(image, 0.3);
        //    if (index == imageFiles.size()||index == 5) break;
        //}
        //cout << "TRGB" << totalRGB << endl;
        //if ((float)totalRGB / index > 29000) COLORMODE = 1;
        //else COLORMODE = 0;
        COLORMODE = 1;
        if (isColorConflict(upperThresholds[COLORMODE], lowerThresholds[COLORMODE])) return -1;

        for (int i = 1;i<= imageFiles.size();i++)
        {

            //cout << fileName + to_string(i) + PICFORMAT << endl;
            image = imread(fileName + to_string(i) + PICFORMAT);
            int error = readCodeHelper(i-1,data, image, origianl, debugMode);
            if (error == -1) return -1;
            float errorRatio = (float)error / CAPACITY * BIT;
            //if (errorRatio > 0.7)
            //{
            //    cout << "图" << to_string(i) << "误码数过大" << endl;
            //    cout << "图片数量或顺序出错" << endl;
            //    return -1;
            //}
            cout << "图" << to_string(i) << " 误码数" << error << " 误码率" << errorRatio << endl;
            if(debugMode==1||debugMode == 2)imwrite("errors/"+to_string(i)+PICFORMAT, image);
        }
        return 0;
    }


}