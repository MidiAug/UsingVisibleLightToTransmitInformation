#include "BinGenerator.h"

// 生成随机二进制文件的函数
void generateRandomBinaryFile(const std::string& filename, std::size_t size)
{
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
    std::vector<int> tmp();
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

// 填补数据
void fillData(std::vector<int>& datas)
{
    std::vector<int> fillArr(16);
    fillArr = { 1,1,1,0,1,1,0,0,0,0,0,1,0,0,0,1 };
    int left = CAPACITY - (datas.size() % CAPACITY);
    int cnt = 0;
    for (int i = 0; i < left / 16; i++)
    {
        datas.insert(datas.end(), fillArr.begin(), fillArr.end());
    }
    left %= 16;
    for (int i = 0; i < left; i++)
    {
        datas.push_back(fillArr[i]);
    }
}

