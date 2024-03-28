#include "CRC.hpp"

// 生成CRC码
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

std::vector<int> CRCEncode(std::vector<int> data){
  // 每一位录入字符串，字符串转bitset，再用helper，再转回vector
  std::string temp="";
  Code code;
  int cnt=1;
  for(int i=0;i<data.size();++i){
    if(i%16==0)
      temp="";
    if(data[i]==1)
      temp+="1";
    else
      temp+="0";
    if(temp.length()==16)
      code.push_back(std::bitset<16>(temp));
  }
  CRCCode crcCode=CRCEncodeHelper(code);
  temp="";
  for(auto x:crcCode)
    temp+=x.to_string();
  std::vector<int> ans;
  for(auto x:temp){
    if(x=='1')
      ans.push_back(1);
    else if(x=='0')
      ans.push_back(0);
  }
  return ans;
}


// CRC解码
Code CRCDecodeHelper(CRCCode crcData) {
  Code decodeData;
  for (auto x : crcData) {
    // 提取数据位
    std::bitset<16> data = std::bitset<16>(x.to_ullong() >> 5);
    // 提取校验位
    uint8_t crcExtracted = x.to_ulong() & 0x1F; // 提取低5位作为CRC
    uint8_t crc = 0;
    std::string bString = x.to_string();
    for (int i = 0; i < 16; ++i) {
      if (bString[i]=='1')
        crc ^= table[i + 1];
    }
    // 检查CRC是否匹配，以确定是否有错误
    if (crc == crcExtracted) {
      decodeData.push_back(data); // 如果无错误，直接添加数据
    } else {
      // 如果有错误，查找并修正错误位
      int wrongBit = -1;
      for (int i = 1; i <= 16; ++i) {
        if ((crc ^ crcExtracted) == table[i]) {
          wrongBit = i; // 表的索引从1开始，bitset的索引从0开始，所以这里需要-1
          break;
        }
      }
      std::cout<<wrongBit<<'\n';
      if (wrongBit != -1) {
        // 翻转错误位
        if(bString[wrongBit-1]=='1')
          bString[wrongBit-1]='0';
        if(bString[wrongBit-1]=='0')
          bString[wrongBit-1]='1';
        data=std::bitset<16>(bString);
        decodeData.push_back(data);
      }
    }
  }
  return decodeData;
}

std::vector<int> CRCDecode(std::vector<int> crcData){
  std::string temp="";
  CRCCode crcCode;
  int cnt=1;
  for(int i=0;i<crcData.size();++i){
    if(i%21==0)
      temp="";
    if(crcData[i]==1)
      temp+="1";
    else
      temp+="0";
    if(temp.length()==21)
      crcCode.push_back(std::bitset<21>(temp));
  }
  Code code=CRCDecodeHelper(crcCode);
  temp="";
  for(auto x:code)
    temp+=x.to_string();
  std::vector<int> ans;
  for(auto x:temp){
    if(x=='1')
      ans.push_back(1);
    else if(x=='0')
      ans.push_back(0);
  }
  return ans;
}


int main() {
  // 示例：编码然后解码
  std::vector<int> oData={1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0};
  auto encodedData = CRCEncode(oData);
  auto decodedData = CRCDecode(encodedData);

  for (const auto& data : decodedData) {
    std::cout << data;
  }

  return 0;
}


