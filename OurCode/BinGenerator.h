#pragma once
#include <iostream>
#include <fstream>
#include <random>

#include "Basic.h"
// ��������������ļ��ĺ���
void generateRandomBinaryFile(const std::string& filename, std::size_t size);

// ��ȡ�������ļ�
std::vector<int> readBinaryFile(const std::string& filename);

// �����
void fillData(std::vector<int>& datas);