#pragma once
#include <iostream>
#include <fstream>
#include <random>

#include "Basic.h"
// 生成随机二进制文件的函数
void generateRandomBinaryFile(const std::string& filename, std::size_t size);

// 读取二进制文件
std::vector<int> readBinaryFile(const std::string& filename);

// 填补数据
void fillData(std::vector<int>& datas);