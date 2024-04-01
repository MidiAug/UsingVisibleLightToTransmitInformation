#pragma once
#include <opencv2/opencv.hpp>
#include <iostream>
#include <filesystem> //�����ҪC++17,�Ҽ�OurCode������->����->C++��׼��ΪC++17������

/// <summary>
///   �ж��Ƿ�Ϊ��֡��ͨ�������������İ�ɫ�����Ƿ񵽴�һ������ʵ��
/// </summary>
/// <param name="frame">
///   ��ǰ֡
/// </param>
/// <param name="threshold">
///   �ж��Ƿ�Ϊ��ɫ���ص���ֵ���統����Ϊ250ʱ��ʾ��������������250�����ر��϶�Ϊ��ɫ����
/// </param>
/// <param name="checkRatio"></param>
///   ��ǰ֡�����������ı�����������Ϊ0.1ʱ����ʾ��ȡ��ǰ֡����0.1*0.1�������������Ƿ�Ϊ��ɫ����
/// <returns>
///   �Ƿ�Ϊ��֡��һ������ֵ
/// </returns>
bool isFrameWhite(const cv::Mat& frame, int threshold, double checkRatio);

/// <summary>
///   ʵ�ֶ�ά����ȡ
/// </summary>
/// <param name="videoPath">
///   ָ������Ƶ·��
/// </param>
/// <param name="outputPath">
///   ָ�����ļ���·�������򴴽�
/// </param>
/// <param name="samplingRatio">
///   ֡�����������ı���������isFrameWhite�У�Ĭ��Ϊ0.1
/// </param>
/// <param name="RGBThreshold">
///   �ж��Ƿ�Ϊ��ɫ�ķ�����ֵ������isFrameWhite�У�Ĭ��Ϊ200
/// </param>
/// <returns>
///   �Ƿ�ɹ���ȡ��ά�룬һ���߼�ֵ
/// </returns>
bool FrameExtractor(const std::string& videoPath, const std::string& outputPath, double samplingRatio, int RGBThreshold);