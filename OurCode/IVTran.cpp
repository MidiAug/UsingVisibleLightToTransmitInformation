#include "IVTran.h"

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
        //videoWriter.write(frame0);
        for (int i = 0; i < times; i++) { // ÿ��ͼƬ50֡
            videoWriter.write(frame);
        }
    }

    videoWriter.release();

    std::cout << "video created in " << outputVideoPath << std::endl;
    return true;
}

bool VideoToImg(std::string vedioPath,std::string outputPath)
{
    // ����Ƶ�ļ�
    cv::VideoCapture cap(vedioPath);

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
            std::string output_path = outputPath+"/frame_" + std::to_string(frame_count) + ".png";
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
