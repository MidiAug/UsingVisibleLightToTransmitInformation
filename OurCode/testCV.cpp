#include <iostream>
#include <fstream>
#include <random>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;
using namespace cv;
#define MARGIN 1
<<<<<<< HEAD
#define HIGH 97
#define WIDTH 97

=======
#define HIGH 150
#define WIDTH 150
#define CAPACITY  HIGH * WIDTH - 2 * HIGH * MARGIN - 2 * WIDTH * MARGIN + 4 * MARGIN * MARGIN - 234// 234是二维码区域

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

std::vector<int> readBinaryFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    std::vector<int> result;

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
>>>>>>> eb0a1d84b9d0b2afed1c64c4422561ca4c5c0695
class FileToImg
{
public:
    const int capacity = HIGH * WIDTH - 2 * HIGH * MARGIN - 2 * WIDTH * MARGIN + 4 * MARGIN * MARGIN - 234;// 234是二维码区域
    const int saveCorrection[6][2] =
    {
        WIDTH / 2, MARGIN + 7,
        MARGIN + 7, WIDTH / 2 ,
        WIDTH / 2, WIDTH / 2 ,
        WIDTH / 2, WIDTH - MARGIN - 7 ,
        WIDTH - MARGIN - 7, WIDTH - MARGIN - 7,
        WIDTH - MARGIN - 7, WIDTH / 2
    };
    // 画寻像
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

    //画定位点
    void findLacation(Mat img)
    {
        for (int i = MARGIN+7; i < WIDTH - 8; i++)
        {
            if (i % 2 == 0)
            {
                rectangle(
                    img,
                    Point(i, MARGIN + 7),
                    Point(i, MARGIN + 7),
                    Scalar(0, 0, 0)
                );
                rectangle(
                    img,
                    Point(MARGIN + 7, i),
                    Point(MARGIN + 7, i),
                    Scalar(0, 0, 0)
                );
            }
            else
            {
                rectangle(
                    img,
                    Point(i, MARGIN + 7),
                    Point(i, MARGIN + 7),
                    Scalar(255, 255, 255)
                );
                rectangle(
                    img,
                    Point(MARGIN + 7, i),
                    Point(MARGIN + 7, i),
                    Scalar(255, 255, 255)
                );
            }
        }
    }
    //画校正点
    void correction(int x,int y,Mat img)
    {
        rectangle(
            img,
            Point(x, y),
            Point(x, y),
            Scalar(0, 0, 0)
        );
        rectangle(
            img,
            Point(x - 2, y - 2),
            Point(x + 2, y + 2),
            Scalar(0, 0, 0)
        );
    }
    void drawBlocks() {
        block(MARGIN, MARGIN, img);//左上
        block(WIDTH - MARGIN - 8, MARGIN, img);// 右上
        block(MARGIN, HIGH - MARGIN - 8, img); // 左下
    }
    void drawCorrection()
    {
        correction(WIDTH / 2, MARGIN + 7,img);
        correction(MARGIN + 7, WIDTH / 2, img);
        correction(WIDTH / 2, WIDTH / 2, img);
        correction(WIDTH / 2, WIDTH - MARGIN - 7, img);
        correction(WIDTH - MARGIN - 7, WIDTH - MARGIN - 7, img);
        correction(WIDTH - MARGIN - 7, WIDTH / 2, img);
    }
    void initImg(int high, int width) {
        img = Mat(cv::Size(high, width), CV_8UC3);
        img.setTo(Scalar(255, 255, 255));  
    }
<<<<<<< HEAD

    //补齐数据
    void fillData(vector<char> ch)
    {
        vector<char> fillarr(16);
        fillarr = { 1,1,1,0,1,1,0,0,0,0,0,1,0,0,0,1 };
        while (sizeof(ch) / sizeof(ch[0]) < capacity)
        {
            ch.insert(ch.end(), fillarr.begin(), fillarr.end());
        }
    }

    void fileToImg(vector<char> ch) {
        drawBlocks();
        drawCorrection();
        findLacation(img);
        int num; // 第几张图
        char data;
        int index = 0;
        int curC = 0, curR = 0;
        int flag = 1;
        for (int curR = MARGIN; curR < HIGH - MARGIN && flag; curR++)
=======
    void fileToImg(vector<int> datas) {
        int numOfImg = datas.size() / (CAPACITY) + 1;
        int curNum = 0;
        int index = 0, curC = 0, curR = 0;
        cout << numOfImg;
        for (int i = 0; i < numOfImg; i++)
>>>>>>> eb0a1d84b9d0b2afed1c64c4422561ca4c5c0695
        {
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
<<<<<<< HEAD
                for (int i = 0; i < 6; i++)
                {
                    if (curR == saveCorrection[i][0] && curC == saveCorrection[i][1])
                        continue;
                }
                // 跳过定位码区域
                if (curR <= MARGIN + 7 + 1) {
                    if (curC <= MARGIN + 7 + 1 || curC >= WIDTH - (MARGIN + 7 + 1) - 1)continue;
                }
                else if (curR >= HIGH - (MARGIN + 7 + 1) && curC <= MARGIN + 7 + 1) continue;
                data = ch[index++];
                drawPixel(data - '0', curC, curR, img);
=======
>>>>>>> eb0a1d84b9d0b2afed1c64c4422561ca4c5c0695
            }
            curNum++;
            std::string filename = "output_image" + curNum;
            filename += ".png";
            bool result = cv::imwrite(filename, img);
            cout << filename;
            if (index != CAPACITY) cout << "容量计算有误，重新计算" << endl;
        }
        
<<<<<<< HEAD
        cout << index << endl;
        cout << capacity << endl;
=======
>>>>>>> eb0a1d84b9d0b2afed1c64c4422561ca4c5c0695
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

<<<<<<< HEAD
int main() {

    char tmp;
    vector<char> datas;
    fstream bins("random.bin");
    while (bins >> tmp) {
        datas.push_back(tmp);
    }
    FileToImg file;
    file.initImg(HIGH, WIDTH);
    file.fileToImg(datas);
    file.showImg();
=======
int main()
{
    std::size_t fileSize = 5000; // 文件大小，单位为字节
    std::string filename = "random.bin"; // 文件名
    generateRandomBinaryFile(filename, fileSize);
    std::cout << "Random binary file generated successfully." << std::endl;

    vector<int> datas = readBinaryFile(filename);

    FileToImg f2i;
    f2i.fileToImg(datas);
    f2i.showImg();
>>>>>>> eb0a1d84b9d0b2afed1c64c4422561ca4c5c0695
    waitKey(0);
    destroyAllWindows();
    return 0;
}