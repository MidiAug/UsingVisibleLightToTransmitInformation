#include "IVTran.h"

    /*
    * 功能：图像转视频
    * - imageFolderPath:图片文件夹路径
    * - outputVideoPath:输出视频路径
    * - time:时间(毫秒)
    */
bool ImgToVideo(std::string imageFolderPath, std::string outputVideoPath, float time) {

    // 获取图片文件列表
    std::vector<cv::String> imageFiles;
    cv::glob(imageFolderPath + "/*.png", imageFiles);  // 假设图片格式为png
    std::cout << imageFolderPath + "/*.png" << std::endl;

    // 每张图片录入几次()
    constexpr int FPS = 5;
    int times = time / 1000 * FPS / imageFiles.size();

    // 检查是否有图片
    if (imageFiles.empty()) {
        std::cerr << "images not found" << std::endl;
        return false;
    }

    // 获取图像尺寸(这里我们图像的尺寸应该都是一样的)
    cv::Mat firstImage = cv::imread(imageFiles[0]);

    // 创建视频编写器
    cv::VideoWriter videoWriter(outputVideoPath, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), FPS, firstImage.size()); // 设置帧率为 5

    // 检查视频编写器是否成功打开
    if (!videoWriter.isOpened()) {
        std::cerr << "failed to open the video writer." << std::endl;
        return false;
    }

    cv::Mat frame0 = cv::imread(imageFiles[0]);// 首张白图
    for (int i = 0; i < imageFiles.size(); i++)
    {
        cv::Mat frame = cv::imread(imageFiles[i]);
        //videoWriter.write(frame0);
        for (int i = 0; i < times; i++) { // 每张图片50帧
            videoWriter.write(frame);
        }
    }

    videoWriter.release();

    std::cout << "video created in " << outputVideoPath << std::endl;
    return true;
}

bool VideoToImg(std::string vedioPath,std::string outputPath)
{
    // 打开视频文件
    cv::VideoCapture cap(vedioPath);

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
            std::string output_path = outputPath+"/frame_" + std::to_string(frame_count) + ".png";
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


}
