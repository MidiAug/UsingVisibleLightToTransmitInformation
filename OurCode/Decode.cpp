#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    // 1. 读取图像
    cv::Mat src = cv::imread("qrcode.png");
    if (src.empty()) {
        std::cerr << "Error: Cannot read image!" << std::endl;
        return -1;
    }

    // 2. 创建QRCodeDetector对象
    cv::QRCodeDetector qrDecoder;

    // 3. 检测并解码二维码
    cv::Mat bbox, rectifiedImage;
    std::string data = qrDecoder.detectAndDecode(src, bbox, rectifiedImage);
    if (data.length() > 0) {
        std::cout << "Decoded Data : " << data << std::endl;

        // 4. 显示检测到的二维码边界
        int n = bbox.rows;
        for (int i = 0; i < n; i++) {
            cv::line(src, cv::Point(bbox.at<float>(i, 0), bbox.at<float>(i, 1)), cv::Point(bbox.at<float>((i + 1) % n, 0), bbox.at<float>((i + 1) % n, 1)), cv::Scalar(255, 0, 0), 3);
        }
        cv::imshow("Result", src);
        cv::waitKey(0);
    }
    else {
        std::cout << "QR Code not detected" << std::endl;
    }
    return 0;
}