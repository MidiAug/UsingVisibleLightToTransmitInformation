#include "BinGenerator.h"

// ��������������ļ��ĺ���
void generateRandomBinaryFile(const std::string& filename, std::size_t size)
{
  std::ofstream ofs(filename, std::ios::binary);
  if (!ofs.is_open()) {
    std::cerr << "Failed to open file: " << filename << std::endl;
    return;
  }

  // ʹ��������������������������������
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, 255);

  for (std::size_t i = 0; i < size; ++i) {
    unsigned char byte = static_cast<unsigned char>(dis(gen));
    ofs.write(reinterpret_cast<const char*>(&byte), sizeof(byte));
  }

  ofs.close();
}

// ��ȡ�������ļ�
std::vector<int> readBinaryFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    std::vector<int> result;
    std::vector<int> tmp();
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return result;
    }

    char byte;

    // ��ȡ�ļ�����
    while (file.read(&byte, sizeof(byte))) {
        // ���ַ�ת��Ϊ��������λ��ȡ
        for (int i = 7; i >= 0; --i) {
            int bit = (byte >> i) & 1;
            result.push_back(bit);
        }
    }

    file.close();

    return result;
}

// �����
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

