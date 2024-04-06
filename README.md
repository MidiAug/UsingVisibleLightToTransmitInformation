# 可见光通信系统
## 简介
本项目是一个基于可见光的通信系统，利用二维码作为信息的载体，通过编码和解码二维码来实现数据的传输。

该系统包括数据的打包、生成二维码图像、将二维码编入视频中，以及从视频中提取二维码并解码还原数据的完整流程。

## 部署本项目
本项目所需要的环境支持包括
- OpenCV 4.9.0
- C++17及以上版本

1. 安装OpenCV

本项目依赖于OpenCV 4.9.0，您可以从OpenCV官网下载相应版本并将其配置到环境变量中


2. 配置C++环境

确保您的系统中安装了支持C++17标准的编译器，我们建议您使用MSVC 2017及以上版本的编译器

对于Windows用户，推荐使用Visual Studio作为IDE，它提供了良好的C++支持和OpenCV集成方式。

## 使用说明
### 编码(encode)模式
进入编码模式后，可以通过如下的命令行接口运行程序
```shell
encode file_name.bin video_name.mp4 time
```
- ```file_name.bin```:待编码的二进制文件
- ```video_name.mp4```:编码后得到的视频文件名
- ```time```:视频长度(单位为毫秒)

这行命令将会根据所提供二进制文件生成一个指定时间的视频

我们的程序首先将二进制文件转换为二维码矩形，再将纯白矩形和二维码矩形交替写入mp4文件中得到视频

### 解码(decode)模式
用户可以用手机拍摄在编码模式下得到的视频文件，将拍摄后的视频导入本程序进行解码

启动程序后，选择decode模式。通过如下的命令行接口运行程序
```shell
decode video_name.mp4 file1_name.bin file2_name.bin
```
- ```video_name.mp4```：待解码的视频文件
- ```file1_name.bin```：解码得到的二进制文件
- ```file2_name.bin```：与编码前二进制文件file与解码后得到的二进制文件file1逐位对比得到的二进制文件，当file1中解码正确时，file2的对应位填入1；否则为0

## 功能模块
下面的功能模块以关键函数为例说明
- 基础配置
  
  Basic.h定义了程序所需要的基本参数
  ```c
  #define BIT 2 
  #define MARGIN 1
  #define HEIGHT 170
  #define WIDTH 265
  #define CAPACITY ((HEIGHT*WIDTH-(HEIGHT+WIDTH)*2*MARGIN+4*MARGIN*MARGIN-3*9*9)*BIT) //
  #define MULTIPLE 8 
  #define PICFORMAT ".png" // 图像格式
  #define THREADCNT 50
  ```
- 编码模块
  
  Encode.cpp与Encode.h包含了将二进制数据转换为二维码图像的核心算法。

  主要功能包括初始化图像空间、绘制二维码的基本图形（定位点、校正点）和将二进制数据编码到图像中。
  ```cpp
  // 寻像区域（3个）
  void block(int left, int top, Mat& img);

  // 画定位点（2条线）
  void findLacation(Mat& img);

  //画校正点 (6个）
  void correction(int x, int y, Mat& img);

  // 绘制具体像素点
  void drawPixel(int val, int x, int y, Mat& img);// 一位
  void drawPixel(int val, int lue, int x, int y, Mat& img); // 两位
  void drawPixel(int v1, int v2, int v3,int x, int y, Mat& img); // 三位
  ```
- 文件操作模块
  
  Files.cpp与Files.h实现了文件的读写操作，包括多线程保存图像、生成随机二进制文件、二进制文件的读取和CRC编码解码等功能。
  ```cpp
  //多线程保存图像
  void threadSave(Mat image, cv::String path);
  void threadSave(vector<Mat> &images, vector<cv::String> &paths);

  // 数据CRC编码解码，二进制文件以向量形式存储
  std::vector<int> CRCEncode(std::vector<int> data);
  std::vector<int> CRCDecode(std::vector<int> crcData);

  // 生成随机二进制文件
  void generateRandomBinaryFile(const std::string& filename, std::size_t size);

  // 读取二进制文件
  std::vector<int> readBinaryFile(const std::string& filename);

  // 将二进制数据输出
  void outBin(const std::vector<int>& bits, const std::string& filename);

  // 创建视频
  bool ImgToVideo(std::string imageFolderPath, std::string outputVideoPath, float time,int FPS, int whitefps, int imagefps);

  // 解码时的帧提取
  bool FrameExtractor(const std::string& videoPath, const std::string& outputPath, double samplingRatio = 0.1, int RGBThreshold = 200);
  ```
  该模块为数据的编解码提供了基础支持。
- 矩阵操作
  
  Matrix.h提供了一个通用的矩阵类和基本的数学操作，如加减乘除、幂运算等。

- 解码模块
  ```cpp
  // 解析二维码中的信息
  int readCode(std::string extCodePath, vector<int>& data, vector<int>& origianl,int debugMode = 0);
  ```
  decode.cpp与decode.h包含了从视频中提取二维码并解析为原始数据的功能。
  
  主要包括图像预处理、寻找和矫正二维码、解析二维码数据等步骤。
- 主程序
  
  OurCode.cpp为用户提供了选择编码或解码操作的接口，并集成了整个系统的工作流程。
