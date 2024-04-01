#include "FrameExtractor.h"

bool isFrameWhite(const cv::Mat& frame, int threshold, double checkRatio) {
  int rowsToCheck = frame.rows * checkRatio; // 检查中心的行数
  int colsToCheck = frame.cols * checkRatio; // 检查中心的列数
  int rowStart = (frame.rows - rowsToCheck) / 2;
  int colStart = (frame.cols - colsToCheck) / 2;

  cv::Mat centerRegion = frame(cv::Range(rowStart, rowStart + rowsToCheck), cv::Range(colStart, colStart + colsToCheck));

  int whitePixels = 0;
  int totalPixels = centerRegion.rows * centerRegion.cols;

  for (int row = 0; row < centerRegion.rows; ++row) {
    for (int col = 0; col < centerRegion.cols; ++col) {
      cv::Vec3b pixel = centerRegion.at<cv::Vec3b>(row, col);
      if (pixel[0] > threshold && pixel[1] > threshold && pixel[2] > threshold) {
        ++whitePixels;
      }
    }
  }
  std::cout << whitePixels << " " << totalPixels << '\n';
  return (static_cast<double>(whitePixels) / totalPixels) < 0.01;
}

bool FrameExtractor(const std::string& videoPath,const std::string& outputPath,double samplingRatio=0.1,int RGBThreshold=200) {
  std::string outputDirectory = outputPath;
  // 检查文件夹是否存在，如果不存在，则创建它
  if (!std::filesystem::exists(outputDirectory)) {
    std::filesystem::create_directory(outputDirectory);
  }

  cv::VideoCapture cap(videoPath);
  if (!cap.isOpened()) {
    std::cout << "Video opening failed,please retry\n";
    return false;
  }

  cv::Mat frame;
  bool doSaveFrame = false;
  int frameNumber = 0;
  int threshold = RGBThreshold;
  double checkRatio = samplingRatio;

  while (true) {
    if (!cap.read(frame)) break;

    if (doSaveFrame && !isFrameWhite(frame, threshold, checkRatio)) {
      std::string filename = outputDirectory + "/frame_" + std::to_string(frameNumber) + ".jpg";
      cv::imwrite(filename, frame);
      std::cout << "Saved: " << filename << std::endl;
      doSaveFrame = false;
      ++frameNumber;
    }
    else {
      doSaveFrame = isFrameWhite(frame, threshold, checkRatio);
    }
  }

  std::cout << "Processing Successfully\n";
  return true;
}