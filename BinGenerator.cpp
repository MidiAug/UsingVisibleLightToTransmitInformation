#include <iostream>
#include <fstream>
#include <random>

// ��������������ļ��ĺ���
void generateRandomBinaryFile(const std::string& filename, std::size_t size) {
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

int main() {
  std::size_t fileSize = 1000; // �ļ���С����λΪ�ֽ�
  std::string filename = "random.bin"; // �ļ���

  generateRandomBinaryFile(filename, fileSize);

  std::cout << "Random binary file generated successfully." << std::endl;

  return 0;
}
