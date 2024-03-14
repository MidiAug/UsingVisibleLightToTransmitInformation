#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

int main() {
    // 打开视频文件
    std::string video_path = "Update Here";
    cv::VideoCapture cap(video_path);

    if (!cap.isOpened()) {
        std::cerr << "Error: Unable to open the video file." << std::endl;
        return -1;
    }

    // 定义帧计数器
    int frame_count = 0;

    while (true) {
        cv::Mat frame;

        // 读取视频帧
        cap >> frame;

        // 检查是否已经到达视频末尾
        if (frame.empty()) {
            std::cout << "End of video." << std::endl;
            break;
        }

        // 只处理每隔五帧的视频帧
        if (frame_count % 5 == 0) {
            // 在这里对帧进行处理，例如保存为图像文件
            std::string output_path = "frame_" + std::to_string(frame_count) + ".jpg";
            cv::imwrite(output_path, frame);
            std::cout << "Frame " << frame_count << " saved." << std::endl;
        }

        // 增加帧计数器
        frame_count++;

        // 检查按键以退出循环
        if (cv::waitKey(1) == 27) // ESC键的ASCII码为27
            break;
    }

    // 释放视频捕获对象
    cap.release();
    cv::destroyAllWindows();

    return 0;
}
