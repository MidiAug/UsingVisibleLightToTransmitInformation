#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    // 1. ��ȡͼ��
    cv::Mat src = cv::imread("qrcode.png");
    if (src.empty()) {
        std::cerr << "Error: Cannot read image!" << std::endl;
        return -1;
    }

    // 2. ����QRCodeDetector����
    cv::QRCodeDetector qrDecoder;

    // 3. ��Ⲣ�����ά��
    cv::Mat bbox, rectifiedImage;
    std::string data = qrDecoder.detectAndDecode(src, bbox, rectifiedImage);
    if (data.length() > 0) {
        std::cout << "Decoded Data : " << data << std::endl;

        // 4. ��ʾ��⵽�Ķ�ά��߽�
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