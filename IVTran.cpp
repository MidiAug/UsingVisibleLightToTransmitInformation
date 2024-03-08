#include"C:\\opencv\\build\\include\\opencv2\\opencv.hpp"
#include <iostream>

namespace IVTran {
  /*
  * 功能：图像转视频
  * - imageFolderPath:图片文件夹路径
  * - outputVideoPath:输出视频路径
  * - time:时间(毫秒)
  */
  bool Image2Video(std::string imageFolderPath, std::string outputVideoPath, float time) {

    // 获取图片文件列表
    std::vector<cv::String> imageFiles;
    cv::glob(imageFolderPath + "/*.png", imageFiles);  // 假设图片格式为png

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

    // 逐一将图片写入视频
    for (const auto& imageFile : imageFiles) {
      cv::Mat frame = cv::imread(imageFile);
      for (int i = 0; i < times; i++) { // 每张图片50帧
        videoWriter.write(frame);
      }
    }
    videoWriter.release();

    std::cout << "video created in " << outputVideoPath << std::endl;
    return true;
  }
}

int main() {
  IVTran::Image2Video("C:\\Users\\yuy788\\Desktop\\image", "outPut.avi", 10000);
  return 0;
}