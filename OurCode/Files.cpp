#include "Files.h"

namespace Files
{
    void create_or_clear_directory(const std::string& dir) {
        // ����ļ����Ƿ����
        if (_access(dir.c_str(), 0) == -1) {
            // �������򴴽��ļ���
            _mkdir(dir.c_str());
        }
        else {
            // ������ɾ���ļ����ڲ��������ļ�
            // ��Ŀ¼�������"\\*.*"���е�һ������
            std::string newDir = dir + "\\*.*";
            // ���ڲ��ҵľ��
            intptr_t handle;
            struct _finddata_t fileinfo;
            // ��һ�β���
            handle = _findfirst(newDir.c_str(), &fileinfo);

            if (handle == -1) {
                return;
            }
            do
            {
                if (fileinfo.attrib & _A_SUBDIR)
                {
                    // ���Ϊ�ļ��У������ļ���·�����ٴα���
                    if (strcmp(fileinfo.name, ".") == 0 || strcmp(fileinfo.name, "..") == 0)
                    {
                        continue;
                    }
                    // ��Ŀ¼�������"\\"����������Ŀ¼��������һ������
                    newDir = dir + "\\" + fileinfo.name;
                    //�ȱ���ɾ���ļ����µ��ļ�����ɾ���յ��ļ���
                    create_or_clear_directory(newDir.c_str());
                    //ɾ���ļ���
                    _rmdir(newDir.c_str());
                }
                else
                {
                    string file_path = dir + "\\" + fileinfo.name;
                    //ɾ���ļ�
                    remove(file_path.c_str());
                }
            } while (!_findnext(handle, &fileinfo));
            _findclose(handle);
        }
    }

    void create_or_clear_directory(const std::string& dir,cv::Mat &img) {
        // ����ļ����Ƿ����
        if (_access(dir.c_str(), 0) == -1) {
            // �������򴴽��ļ���
            _mkdir(dir.c_str());
        }
        else {
            // ������ɾ���ļ����ڲ��������ļ�
            // ��Ŀ¼�������"\\*.*"���е�һ������
            std::string newDir = dir + "\\*.*";
            // ���ڲ��ҵľ��
            intptr_t handle;
            struct _finddata_t fileinfo;
            // ��һ�β���
            handle = _findfirst(newDir.c_str(), &fileinfo);

            if (handle == -1) {
                return;
            }
            do
            {
                if (fileinfo.attrib & _A_SUBDIR)
                {
                    // ���Ϊ�ļ��У������ļ���·�����ٴα���
                    if (strcmp(fileinfo.name, ".") == 0 || strcmp(fileinfo.name, "..") == 0)
                    {
                        continue;
                    }
                    // ��Ŀ¼�������"\\"����������Ŀ¼��������һ������
                    newDir = dir + "\\" + fileinfo.name;
                    //�ȱ���ɾ���ļ����µ��ļ�����ɾ���յ��ļ���
                    create_or_clear_directory(newDir.c_str());
                    //ɾ���ļ���
                    _rmdir(newDir.c_str());
                }
                else
                {
                    string file_path = dir + "\\" + fileinfo.name;
                    //ɾ���ļ�
                    remove(file_path.c_str());
                }
            } while (!_findnext(handle, &fileinfo));
            _findclose(handle);
        }
        cv::imwrite(dir + "/image0.png", img);
    }


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
    std::vector<int> fillData(std::vector<int>& originalDatas)
    {
        std::vector<int> filledDatas;
        filledDatas.insert(filledDatas.begin(), originalDatas.begin(), originalDatas.end());
        std::vector<int> fillArr(16);
        fillArr = { 1,1,1,0,1,1,0,0,0,0,0,1,0,0,0,1 };
        int left = CAPACITY - (originalDatas.size() % CAPACITY);
        int cnt = 0;
        for (int i = 0; i < left / 16; i++)
        {
            filledDatas.insert(filledDatas.end(), fillArr.begin(), fillArr.end());
        }
        left %= 16;
        for (int i = 0; i < left; i++)
        {
            filledDatas.push_back(fillArr[i]);
        }
        return filledDatas;
    }

    // ���������
    void outBin(const std::vector<int>& bits, const std::string& filename) {
        std::ofstream outFile(filename, std::ios::binary);
        if (!outFile) {
            std::cerr << "Cannot open file: " << filename << std::endl;
            return;
        }

        // ������Ч���ȣ�ȷ����8�ı���
        size_t effectiveLength = (bits.size() / 8) * 8;

        for (size_t i = 0; i < effectiveLength; i += 8) {
            char byte = 0;
            for (int bit = 0; bit < 8; ++bit) {
                byte |= bits[i + bit] << (7 - bit); // ��vector�е�Ԫ����ϳ�һ���ֽ�
            }
            outFile.write(&byte, sizeof(byte)); // д���ļ�
        }

        outFile.close();
    }

    // ����CRC��
    CRCCode CRCEncodeHelper(Code data) {
        CRCCode crcCode;
        for (auto x : data) {
            uint8_t crc = 0;
            std::string bString = x.to_string();
            for (int i = 0; i < 16; ++i) {
                if (bString[i] == '1')
                    crc ^= table[i + 1];
            }
            uint64_t crcData = (x.to_ulong() << 5) | crc;
            crcCode.push_back(std::bitset<21>(crcData));
        }
        return crcCode;
    }

    std::vector<int> CRCEncode(std::vector<int> data) {
        // ÿһλ¼���ַ������ַ���תbitset������helper����ת��vector
        std::string temp = "";
        Code code;
        int cnt = 1;
        for (int i = 0; i < data.size(); ++i) {
            if (i % 16 == 0)
                temp = "";
            if (data[i] == 1)
                temp += "1";
            else
                temp += "0";
            if (temp.length() == 16)
                code.push_back(std::bitset<16>(temp));
        }
        CRCCode crcCode = CRCEncodeHelper(code);
        temp = "";
        for (auto x : crcCode)
            temp += x.to_string();
        std::vector<int> ans;
        for (auto x : temp) {
            if (x == '1')
                ans.push_back(1);
            else if (x == '0')
                ans.push_back(0);
        }
        return ans;
    }

    // CRC����
    Code CRCDecodeHelper(CRCCode crcData) {
        Code decodeData;
        for (auto x : crcData) {
            // ��ȡ����λ
            std::bitset<16> data = std::bitset<16>(x.to_ullong() >> 5);
            // ��ȡУ��λ
            uint8_t crcExtracted = x.to_ulong() & 0x1F; // ��ȡ��5λ��ΪCRC
            uint8_t crc = 0;
            std::string bString = x.to_string();
            for (int i = 0; i < 16; ++i) {
                if (bString[i] == '1')
                    crc ^= table[i + 1];
            }
            // ���CRC�Ƿ�ƥ�䣬��ȷ���Ƿ��д���
            if (crc == crcExtracted) {
                decodeData.push_back(data); // ����޴���ֱ���������
            }
            else {
                // ����д��󣬲��Ҳ���������λ
                int wrongBit = -1;
                for (int i = 1; i <= 16; ++i) {
                    if ((crc ^ crcExtracted) == table[i]) {
                        wrongBit = i; // ���������1��ʼ��bitset��������0��ʼ������������Ҫ-1
                        break;
                    }
                }
                std::cout << wrongBit << '\n';
                if (wrongBit != -1) {
                    // ��ת����λ
                    if (bString[wrongBit - 1] == '1')
                        bString[wrongBit - 1] = '0';
                    if (bString[wrongBit - 1] == '0')
                        bString[wrongBit - 1] = '1';
                    data = std::bitset<16>(bString);
                    decodeData.push_back(data);
                }
            }
        }
        return decodeData;
    }

    std::vector<int> CRCDecode(std::vector<int> crcData) {
        std::string temp = "";
        CRCCode crcCode;
        int cnt = 1;
        for (int i = 0; i < crcData.size(); ++i) {
            if (i % 21 == 0)
                temp = "";
            if (crcData[i] == 1)
                temp += "1";
            else
                temp += "0";
            if (temp.length() == 21)
                crcCode.push_back(std::bitset<21>(temp));
        }
        Code code = CRCDecodeHelper(crcCode);
        temp = "";
        for (auto x : code)
            temp += x.to_string();
        std::vector<int> ans;
        for (auto x : temp) {
            if (x == '1')
                ans.push_back(1);
            else if (x == '0')
                ans.push_back(0);
        }
        return ans;
    }

    /*
* ���ܣ�ͼ��ת��Ƶ
* - imageFolderPath:ͼƬ�ļ���·��
* - outputVideoPath:�����Ƶ·��
* - time:ʱ��(����)
*/
    bool ImgToVideo(std::string imageFolderPath, std::string outputVideoPath, float time) {

        // ��ȡͼƬ�ļ��б�
        std::vector<cv::String> imageFiles;
        cv::glob(imageFolderPath + "/*.png", imageFiles);  // ����ͼƬ��ʽΪpng
        std::cout << imageFolderPath + "/*.png" << std::endl;

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

        cv::Mat frame0 = cv::imread(imageFiles[0]);// ���Ű�ͼ
        for (int i = 0; i < imageFiles.size(); i++)
        {
            cv::Mat frame = cv::imread(imageFiles[i]);
            videoWriter.write(frame0);
            for (int i = 1; i < times-1; i++) { // ÿ��ͼƬ50֡
                videoWriter.write(frame);
            }
        }

        videoWriter.release();

        std::cout << "video created in " << outputVideoPath << std::endl;
        return true;
    }

    bool VideoToImg(std::string vedioPath, std::string outputPath)
    {
        // ����Ƶ�ļ�
        cv::VideoCapture cap(vedioPath);

        Files::create_or_clear_directory(outputPath);

        if (!cap.isOpened()) {
            std::cerr << "Error: Unable to open the video file." << std::endl;
            return -1;
        }

        // ����֡������
        int frame_count = 0;

        while (true) {
            cv::Mat frame;

            // ��ȡ��Ƶ֡
            cap >> frame;

            // ����Ƿ��Ѿ�������Ƶĩβ
            if (frame.empty()) {
                std::cout << "End of video." << std::endl;
                break;
            }

            // ֻ����ÿ����֡����Ƶ֡
            if (frame_count % 5 == 0) {
                // �������֡���д������籣��Ϊͼ���ļ�
                std::string output_path = outputPath + "/frame_" + std::to_string(frame_count) + ".png";
                cv::imwrite(output_path, frame);
                std::cout << "Frame " << frame_count << " saved." << std::endl;
            }

            // ����֡������
            frame_count++;

            // ��鰴�����˳�ѭ��
            if (cv::waitKey(1) == 27) // ESC����ASCII��Ϊ27
                break;
        }

        // �ͷ���Ƶ�������
        cap.release();
        cv::destroyAllWindows();


    }

}