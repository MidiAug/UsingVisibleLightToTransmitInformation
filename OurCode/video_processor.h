// video_processor.h
#ifndef VIDEO_PROCESSOR_H
#define VIDEO_PROCESSOR_H

#include <opencv2/opencv.hpp>
#include <string>

namespace VideoProcessing {
    class VideoProcessor {
    private:
        static cv::VideoCapture cap;
        static cv::Mat frame;

    public:
        static bool convertVideoToFrames(const std::string& inputVideoPath, const std::string& outputFolderPath) {
            cap.open(inputVideoPath);

            if (!cap.isOpened()) {
                std::cerr << "Error opening video file" << std::endl;
                return false;
            }

            int frameCount = 0;

            while (true) {
                cap >> frame;
                if (frame.empty()) {
                    break;
                }

                std::string filename = outputFolderPath + "frame" + std::to_string(frameCount) + ".jpg";
                cv::imwrite(filename, frame);

                frameCount++;
            }

            cap.release();
            return true;
        }
    };

    cv::VideoCapture VideoProcessor::cap;
    cv::Mat VideoProcessor::frame;
}

//调用方式如下
//std::string inputVideoPath = "D:/computer network/project1/ff&opencx/ff&opencx/123.mp4";
//std::string outputFolderPath = "D:/computer network/project1/ff&opencx/vedio/";
//
//if (VideoProcessing::VideoProcessor::convertVideoToFrames(inputVideoPath, outputFolderPath)) {
//    std::cout << "Video frames conversion complete." << std::endl;
//}
//else {
//    std::cerr << "Error converting video frames." << std::endl;
//}

#endif // VIDEO_PROCESSOR_H