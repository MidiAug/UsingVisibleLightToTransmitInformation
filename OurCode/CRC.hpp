#pragma once
#include <iostream>
#include <bitset>
#include <vector>
#include <unordered_map>
#include <cmath>

using Code = std::vector<std::bitset<16>>;
using CRCCode = std::vector<std::bitset<21>>;
const int table[17] = {0, 0xc, 0x6, 0x3, 0x13, 0x1b, 0x1f, 0x1d, 0x1c, 0xe, 0x7, 0x11, 0x1a, 0xd, 0x14, 0xa, 0x5};

CRCCode CRCEncodeHelper(Code data);
std::vector<int> CRCEncode(std::vector<int> data);
Code CRCDecodeHelper(CRCCode crcData);
std::vector<int> CRCDecode(std::vector<int> crcData);