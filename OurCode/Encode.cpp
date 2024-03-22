
//这个文件负责二维码的编码
#include"Encode.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <opencv2/opencv.hpp>
using namespace std;

namespace Code
{
	const int BytesPerFrame = 1242;
	const int FrameSize = 108;
	const int FrameOutputRate = 10;
	const int SafeAreaWidth = 2;
	const int QrPointSize = 18;
	const int SmallQrPointbias = 6;
	const int RectAreaCount = 7;
	const Vec3b pixel[8] = 
	{ 
		Vec3b(0,0,0),Vec3b(0,0,255),Vec3b(0,255,0),Vec3b(0,255,255),
		Vec3b(255,0,0),Vec3b(255,0,255),Vec3b(255,255,0), Vec3b(255,255,255)
	};
	//const int lenlim[RectAreaCount] = { 426,432,1944,432,432,48,24 };
	const int lenlim[RectAreaCount] = { 138,144,648,144,144,16,8 };
	const int areapos[RectAreaCount][2][2] = //[2][2],第一维度代表高宽，第二维度代表左上角坐标
	{
		{{69,16},{QrPointSize + 3,SafeAreaWidth}},
		{{16,72},{SafeAreaWidth,QrPointSize}},
		{{72,72},{QrPointSize,QrPointSize}},
		{{72,16},{QrPointSize,FrameSize - QrPointSize}},
		{{16,72},{FrameSize - QrPointSize,QrPointSize}},
		{{8,16},{FrameSize  - QrPointSize,FrameSize - QrPointSize}},
		{{8,8},{FrameSize - QrPointSize + 8,FrameSize - QrPointSize}}
	};
	enum color
	{
		Black = 0,
		White = 7
	};
	enum class FrameType
	{
		Start = 0,
		End = 1,
		StartAndEnd = 2,
		Normal = 3
	};
	Mat ScaleToDisSize(const Mat& src)
	{
		Mat dis;
		constexpr int FrameOutputSize = FrameSize * FrameOutputRate;
		dis = Mat(FrameOutputSize, FrameOutputSize, CV_8UC3);
		for (int i = 0; i < FrameOutputSize; ++i)
		{
			for (int j = 0; j < FrameOutputSize; ++j)
			{
				dis.at<Vec3b>(i,j) = src.at<Vec3b>(i/10, j/10);
			}
		}
		return dis;
	}

	// 计算海明码的校验位数
	int calculateParityBits(int dataSize) {
		int m = 1;
		while ((1 << m) < (dataSize + m + 1)) {
			++m;
		}
		return m;
	}

	// 生成海明码
	std::vector<int> generateHammingCode(const std::vector<int>& data) {
		int dataSize = data.size();
		int m = calculateParityBits(dataSize);
		std::vector<int> hammingCode(dataSize + m, 0); // 初始化为0

		// 复制数据位到海明码中
		int dataIndex = 0;
		for (int i = 0; i < hammingCode.size(); ++i) {
			// 检查当前位置是否为校验位位置，如果是，则跳过
			if (((i + 1) & i) != 0) {
				continue;
			}
			// 复制数据位
			hammingCode[i] = data[dataIndex++];
		}

		// 计算每个校验位的值
		for (int p = 0; p < m; ++p) {
			int parity = 0;
			int mask = 1 << p;
			for (int i = 0; i < hammingCode.size(); ++i) {
				if ((i & mask) != 0) {
					parity ^= hammingCode[i];
				}
			}
			// 将计算的奇偶校验位插入到海明码中
			hammingCode[mask - 1] = parity;
		}

		return hammingCode;
	}

	// 使用海明码生成校验位
	uint16_t CalCheckCode(const unsigned char* info, int len, bool isStart, bool isEnd, uint16_t frameBase)
	{
		uint16_t ans = 0;
		int cutlen = (len / 2) * 2;
		for (int i = 0; i < cutlen; i += 2)
			ans ^= ((uint16_t)info[i] << 8) | info[i + 1];
		if (len & 1)
			ans ^= (uint16_t)info[cutlen] << 8;
		ans ^= len;
		ans ^= frameBase;
		uint16_t temp = (isStart << 1) + isEnd;
		ans ^= temp;
		return ans;
	}

	void BulidCheckCodeAndFrameNo(Mat& mat, uint16_t checkcode, uint16_t FrameNo)
	{
		// 绘制校验码
		for (int i = 0; i < 16; ++i)
		{
			mat.at<Vec3b>(QrPointSize + 1, SafeAreaWidth + i) = pixel[(checkcode & 1) ? 7 : 0]; // 如果校验码的当前位为1，则使用白色像素点，否则使用黑色像素点
			checkcode >>= 1; // 每次右移一位，准备处理下一位校验码
		}

		// 绘制帧号
		for (int i = 0; i < 16; ++i)
		{
			mat.at<Vec3b>(QrPointSize + 2, SafeAreaWidth + i) = pixel[(FrameNo & 1) ? 7 : 0]; // 如果帧号的当前位为1，则使用白色像素点，否则使用黑色像素点
			FrameNo >>= 1; // 每次右移一位，准备处理下一位帧号
		}
	}

	void BulidSafeArea(Mat& mat)
	{
		constexpr int pos[4][2][2] =
		{
			{{0,FrameSize},{0,SafeAreaWidth}},
			{{0,FrameSize},{FrameSize - SafeAreaWidth,FrameSize}},
			{{0, SafeAreaWidth },{0,FrameSize}},
			{{FrameSize - SafeAreaWidth,FrameSize},{0,FrameSize}}
		};
		for (int k=0;k<4;++k)
			for (int i = pos[k][0][0]; i < pos[k][0][1]; ++i)
				for (int j = pos[k][1][0]; j < pos[k][1][1]; ++j)
					mat.at<Vec3b>(i,j)=pixel[White];

		return;
	}
	void BulidQrPoint(Mat& mat)
	{
		//绘制大二维码识别点
		constexpr int pointPos[4][2] = 
		{ 
			{0,0},
		    {0,FrameSize- QrPointSize},
		    {FrameSize - QrPointSize,0}
		};
		const Vec3b vec3bBig[9] =
		{
			pixel[Black],
			pixel[Black],
			pixel[Black],
			pixel[White],
			pixel[White],
			pixel[Black],
			pixel[Black],
			pixel[White],
			pixel[White]
		};
		for (int i = 0; i < 3; ++i)
			for (int j = 0; j < QrPointSize; ++j)
				for (int k = 0; k < QrPointSize; ++k)
					mat.at<Vec3b>(pointPos[i][0] + j, pointPos[i][1] + k) =
						vec3bBig[(int)max(fabs(j-8.5), fabs(k-8.5))];
		//绘制小二维码识别点
		constexpr int posCenter[2] = { FrameSize - SmallQrPointbias,FrameSize - SmallQrPointbias };
		const Vec3b vec3bsmall[5] =
		{ 
			pixel[Black],
			pixel[Black],
			pixel[White],
			pixel[Black],
			pixel[White],
		};
		for (int i = -4; i <= 4; ++i)
			for (int j = -4; j <= 4; ++j)
				mat.at<Vec3b>(posCenter[0] + i, posCenter[1] + j) = 
							  vec3bsmall[max(abs(i),abs(j))];

	}
	
	void BulidInfoRect(Mat& mat, const char* info, int len, int areaID)
	{
		const unsigned char* pos = (const unsigned char*)info; // 将输入的信息转换为无符号字符指针
		const unsigned char* end = pos + len; // 计算信息的末尾位置

		// 遍历矩形区域的行
		for (int i = 0; i < areapos[areaID][0][0]; ++i)
		{
			uint32_t outputCode = 0; // 用于存储输出的码字

			// 遍历矩形区域的列，每次处理8个字符
			for (int j = 0; j < areapos[areaID][0][1] / 8; ++j)
			{
				outputCode |= *pos++; // 将当前字符的值存入码字中

				// 绘制像素点
				for (int k = areapos[areaID][1][1]; k < areapos[areaID][1][1] + 8; ++k)
				{
					mat.at<Vec3b>(i + areapos[areaID][1][0], j * 8 + k) = pixel[(outputCode & 1) ? 7 : 0]; // 如果当前位为1，则使用白色像素点，否则使用黑色像素点
					outputCode >>= 1; // 右移一位，准备处理下一位字符
				}

				if (pos == end) break; // 如果已经处理完所有字符，则退出循环
			}

			if (pos == end) break; // 如果已经处理完所有字符，则退出循环
		}
	}

	void BulidFrameFlag(Mat& mat, FrameType frameType, int tailLen)
	{
		// 设置起始和结束标志位的颜色
		const Vec3b startColor = (frameType == FrameType::Start || frameType == FrameType::StartAndEnd) ? pixel[White] : pixel[Black];
		const Vec3b endColor = (frameType == FrameType::End || frameType == FrameType::StartAndEnd) ? pixel[White] : pixel[Black];

		// 绘制起始标志位
		mat.at<Vec3b>(QrPointSize, SafeAreaWidth) = startColor;
		mat.at<Vec3b>(QrPointSize, SafeAreaWidth + 1) = startColor;
		mat.at<Vec3b>(QrPointSize, SafeAreaWidth + 2) = (frameType == FrameType::Start || frameType == FrameType::StartAndEnd) ? pixel[Black] : pixel[White];
		mat.at<Vec3b>(QrPointSize, SafeAreaWidth + 3) = (frameType == FrameType::Start || frameType == FrameType::StartAndEnd) ? pixel[Black] : pixel[White];

		// 绘制结束标志位
		mat.at<Vec3b>(QrPointSize, SafeAreaWidth) = endColor;
		mat.at<Vec3b>(QrPointSize, SafeAreaWidth + 1) = endColor;
		mat.at<Vec3b>(QrPointSize, SafeAreaWidth + 2) = (frameType == FrameType::End || frameType == FrameType::StartAndEnd) ? pixel[White] : pixel[Black];
		mat.at<Vec3b>(QrPointSize, SafeAreaWidth + 3) = (frameType == FrameType::End || frameType == FrameType::StartAndEnd) ? pixel[White] : pixel[Black];

		// 设置尾部标志位的颜色，根据尾部长度设置标志位的颜色
		for (int i = 4; i < 16; ++i)
		{
			mat.at<Vec3b>(QrPointSize, SafeAreaWidth + i) = pixel[(tailLen & 1) ? 7 : 0]; // 如果当前位为1，则使用白色像素点，否则使用黑色像素点
			tailLen >>= 1; // 右移一位，准备处理下一位字符
		}

	}

	Mat CodeFrame(FrameType frameType, const char* info, int tailLen, int FrameNo)
	{
		// 创建一个白色背景的 Mat 对象
		Mat codeMat = Mat(FrameSize, FrameSize, CV_8UC3, Vec3d(255, 255, 255));

		// 如果帧类型不是结束帧并且不是起始和结束帧，则将尾部长度设置为每帧字节数
		if (frameType != FrameType::End && frameType != FrameType::StartAndEnd)
			tailLen = BytesPerFrame;

		// 绘制安全区域
		BulidSafeArea(codeMat);

		// 绘制二维码识别点
		BulidQrPoint(codeMat);

		// 计算校验码
		int checkCode = CalCheckCode((const unsigned char*)info, tailLen,
			frameType == FrameType::Start || frameType == FrameType::StartAndEnd,
			frameType == FrameType::End || frameType == FrameType::StartAndEnd, FrameNo);

		// 绘制帧标志
		BulidFrameFlag(codeMat, frameType, tailLen);

		// 绘制校验码和帧号
		BulidCheckCodeAndFrameNo(codeMat, checkCode, FrameNo % 65536);

		// 如果尾部长度不是每帧字节数，则将尾部长度设置为每帧字节数
		if (tailLen != BytesPerFrame)
			tailLen = BytesPerFrame;

		// 绘制信息矩形区域
		for (int i = 0; i < RectAreaCount && tailLen > 0; ++i)
		{
			int lennow = std::min(tailLen, lenlim[i]);
			BulidInfoRect(codeMat, info, lennow, i);
			tailLen -= lennow;
			info += lennow;
		}

		return codeMat; // 返回生成的二维码图像
	}

	void Main(const char* info, int len,const char * savePath,const char * outputFormat,int FrameCountLimit)
	{
		Mat output;
		char fileName[128];
		int counter = 0;
		if (FrameCountLimit == 0) return;
		if (len <= 0)
			;
		else if (len <= BytesPerFrame)
		{
			unsigned char BUF[BytesPerFrame + 5];
			memcpy(BUF, info, sizeof(unsigned char) * len);
			for (int i = len; i <= BytesPerFrame; ++i)
				BUF[i] = rand() % 256;
			output = ScaleToDisSize(CodeFrame(FrameType::StartAndEnd, (char*)BUF, len, 0));
			sprintf_s(fileName, "%s\\%05d.%s", savePath, counter++, outputFormat);
			imwrite(fileName, output);
		}
		else
		{
			int i = 0;
			len -= BytesPerFrame;
			output= ScaleToDisSize(CodeFrame(FrameType::Start, info, len, 0));
			--FrameCountLimit;
			
			sprintf_s(fileName, "%s\\%05d.%s", savePath, counter++, outputFormat);
			imwrite(fileName, output);

			while (len > 0 && FrameCountLimit > 0)
			{
				info += BytesPerFrame;
				--FrameCountLimit;
				if (len - BytesPerFrame > 0)
				{
					if (FrameCountLimit>0)
						output = ScaleToDisSize(CodeFrame(FrameType::Normal, info, BytesPerFrame, ++i));
					else output = ScaleToDisSize(CodeFrame(FrameType::End, info, BytesPerFrame, ++i));
				}
				else
				{
					unsigned char BUF[BytesPerFrame + 5];
					memcpy(BUF, info, sizeof(unsigned char) * len);
					for (int i = len; i <= BytesPerFrame; ++i)
						BUF[i] = rand() % 256;
					output = ScaleToDisSize(CodeFrame(FrameType::End, (char*)BUF, len, ++i));
				}
				len -= BytesPerFrame;
				sprintf_s(fileName, "%s\\%05d.%s", savePath, counter++, outputFormat);
				imwrite(fileName, output);
			};
		}
		return;
	}
}
int main() {
	// 打开二进制文件
	ifstream file("random.bin", ios::binary);
	if (!file) {
		cerr << "Error: Failed to open file 'random.bin'" << endl;
		return 1;
	}

	// 读取二进制文件内容
	file.seekg(0, ios::end);
	int len = file.tellg();
	file.seekg(0, ios::beg);
	vector<char> buffer(len);
	file.read(buffer.data(), len);
	file.close();
	cout << buffer.data();
	// 保存路径为当前文件夹，输出格式为png
	const char* savePath = "./";
	const char* outputFormat = "png";

	// 帧数限制
	int FrameCountLimit = 5;

	// 调用主函数生成二维码帧
	Code::Main(buffer.data(), len, savePath, outputFormat, FrameCountLimit);

	cout << "Generated QR code frames successfully!" << endl;

	return 0;
}