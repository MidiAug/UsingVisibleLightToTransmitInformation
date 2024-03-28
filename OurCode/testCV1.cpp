#include <iostream>
#include <fstream>
#include <random>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;
using namespace cv;
#define MARGIN 1
#define HIGH 150
#define WIDTH 150
#define CAPACITY  HIGH * WIDTH - 2 * HIGH * MARGIN - 2 * WIDTH * MARGIN + 4 * MARGIN * MARGIN - 234// 234是二维码区域
// 生成二进制文件
void generateRandomBinaryFile(const std::string& filename, std::size_t size) {
    std::ofstream ofs(filename, std::ios::binary);
    if (!ofs.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    // 使用随机数生成器来生成随机二进制数据
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);

    for (std::size_t i = 0; i < size; ++i) {
        unsigned char byte = static_cast<unsigned char>(dis(gen));
        ofs.write(reinterpret_cast<const char*>(&byte), sizeof(byte));
    }

    ofs.close();
}
// 读取二进制文件
std::vector<int> readBinaryFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    std::vector<int> result;
    vector<int> tmp();
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return result;
    }

    char byte;

    // 读取文件内容
    while (file.read(&byte, sizeof(byte))) {
        // 将字符转换为整数并逐位提取
        for (int i = 7; i >= 0; --i) {
            int bit = (byte >> i) & 1;
            result.push_back(bit);
        }
    }

    file.close();

    return result;
}
// 添加crc
unsigned short crc16(const std::vector<int>& data) {
    unsigned short CRC16 = 0xFFFF;

    for (int i = 0; i < data.size(); i++) {
        CRC16 = data[i] ^ CRC16;
        for (int j = 0; j < 8; j++) {
            unsigned short tmp = CRC16 & 0x0001;
            CRC16 = CRC16 >> 1;
            if (tmp)
                CRC16 = CRC16 ^ 0xA001;
        }
    }

    return CRC16;
}
std::vector<int> generateChecksums(const std::vector<int>& data) {
    std::vector<int> checksums;

    for (int i = 0; i < data.size(); i += 16) {
        std::vector<int> chunk(data.begin() + i, data.begin() + i + 16);
        unsigned short crc = crc16(chunk);
        checksums.push_back(crc & 0xFF);
        checksums.push_back((crc >> 8) & 0xFF);
    }

    return checksums;
}

class FileToImg
{
public:
    // 画定位点
    void block(int left,int top, Mat img) {
        rectangle(
            img,
            Point(left, top),
            Point(left+7,top+7),
            Scalar(0,0,0)
            );
        rectangle(
            img,
            Point(left + 2, top + 2),
            Point(left + 5, top + 5),
            Scalar(0, 0, 0),
            FILLED
        );
    }
    // 画像素点
    void drawPixel(int val,int x, int y, Mat img) {
        if(val == 0)
            rectangle(
                img,
                Point(x, y),
                Point(x, y),
                Scalar(0, 0, 0)
            );
        else 
            rectangle(
                img,
                Point(x, y),
                Point(x, y),
                Scalar(255, 255, 255)
            );
    };
    void drawBlocks() {
        block(MARGIN, MARGIN, img);//左上
        block(WIDTH - MARGIN - 8, MARGIN, img);// 右上
        block(MARGIN, HIGH - MARGIN - 8, img); // 左下
    }

    void initImg(int high, int width) {
        img = Mat(cv::Size(high, width), CV_8UC3);
        img.setTo(Scalar(255, 255, 255));  
        
    }
    void fileToImg(vector<int> datas) {
        int numOfImg = datas.size() / (CAPACITY) + 1;
        int curNum = 0;
        int index = 0, curC = 0, curR = 0;
        string filename;
        cout <<"图片数量" << numOfImg << endl;
        for (int i = 0; i < numOfImg; i++)
        {
            string str1 = "image";
            string str2 = to_string(i+1);
            string str3 = ".png";
            filename = str1 + str2 + str3;
            initImg(HIGH, WIDTH);

            drawBlocks();
            int data;
            curC = curR = 0;
            int flag = 1;
            for (int curR = MARGIN; curR < HIGH - MARGIN && flag; curR++)
            {
                for (int curC = MARGIN; curC < WIDTH - MARGIN && flag; curC++)
                {
                    if (datas.empty() || index >= datas.size()) {
                        flag = 0;
                        continue;
                    }
                    // 跳过定位码区域
                    if (curR <= MARGIN + 7 + 1) {
                        if (curC <= MARGIN + 7 + 1 || curC >= WIDTH - (MARGIN + 7 + 1) - 1)continue;
                    }
                    else if (curR >= HIGH - (MARGIN + 7 + 1) && curC <= MARGIN + 7 + 1) continue;
                    data = datas[index++];
                    drawPixel(data, curC, curR, img);
                }
            }
            curNum++;
            bool result = cv::imwrite(filename, img);
            if (i==0 && index != CAPACITY) cout << "容量计算有误，重新计算" << endl;
        }
        
    }
    void saveImgs(String filePath,Mat img) {
        imwrite(filePath, img);
    }
    void showImg() {
        cv::namedWindow("image", cv::WINDOW_NORMAL);
        cv::imshow("image", img);
    }
private:
    Mat img;

   
};

int main()
{
    std::size_t fileSize = 5000; // 文件大小，单位为字节
    std::string filename = "random.bin"; // 文件名
    generateRandomBinaryFile(filename, fileSize);
    std::cout << "Random binary file generated successfully." << std::endl;

    vector<int> datas = readBinaryFile(filename);
    vector<int>check = generateChecksums(datas);
    cout << datas.size();
    cout << check.size();
    FileToImg f2i;
    f2i.fileToImg(datas);
    f2i.showImg();
    waitKey(0);
    return 0;
}
