#include"C:\\opencv\\build\\include\\opencv2\\opencv.hpp"
#include <iostream>

namespace IVTran {
  /*
  * ���ܣ�ͼ��ת��Ƶ
  * - imageFolderPath:ͼƬ�ļ���·��
  * - outputVideoPath:�����Ƶ·��
  * - time:ʱ��(����)
  */
  bool Image2Video(std::string imageFolderPath, std::string outputVideoPath, float time) {

    // ��ȡͼƬ�ļ��б�
    std::vector<cv::String> imageFiles;
    cv::glob(imageFolderPath + "/*.png", imageFiles);  // ����ͼƬ��ʽΪpng

    // ÿ��ͼƬ¼�뼸��()
    constexpr int FPS = 5;
    int times = time / 1000 * FPS / imageFiles.size();

    // ����Ƿ���ͼƬ
    if (imageFiles.empty()) {
      std::cerr << "images not found" << std::endl;
      return false;
    }

    // ��ȡͼ��ߴ�(��������ͼ��ĳߴ�Ӧ�ö���һ����)
    cv::Mat firstImage = cv::imread(imageFiles[0]);

    // ������Ƶ��д��
    cv::VideoWriter videoWriter(outputVideoPath, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), FPS, firstImage.size()); // ����֡��Ϊ 5

    // �����Ƶ��д���Ƿ�ɹ���
    if (!videoWriter.isOpened()) {
      std::cerr << "failed to open the video writer." << std::endl;
      return false;
    }

    // ��һ��ͼƬд����Ƶ
    for (const auto& imageFile : imageFiles) {
      cv::Mat frame = cv::imread(imageFile);
      for (int i = 0; i < times; i++) { // ÿ��ͼƬ50֡
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