
//����ļ������ά��ı���
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
	const int areapos[RectAreaCount][2][2] = //[2][2],��һά�ȴ���߿��ڶ�ά�ȴ������Ͻ�����
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

	// ���㺣�����У��λ��
	int calculateParityBits(int dataSize) {
		int m = 1;
		while ((1 << m) < (dataSize + m + 1)) {
			++m;
		}
		return m;
	}

	// ���ɺ�����
	std::vector<int> generateHammingCode(const std::vector<int>& data) {
		int dataSize = data.size();
		int m = calculateParityBits(dataSize);
		std::vector<int> hammingCode(dataSize + m, 0); // ��ʼ��Ϊ0

		// ��������λ����������
		int dataIndex = 0;
		for (int i = 0; i < hammingCode.size(); ++i) {
			// ��鵱ǰλ���Ƿ�ΪУ��λλ�ã�����ǣ�������
			if (((i + 1) & i) != 0) {
				continue;
			}
			// ��������λ
			hammingCode[i] = data[dataIndex++];
		}

		// ����ÿ��У��λ��ֵ
		for (int p = 0; p < m; ++p) {
			int parity = 0;
			int mask = 1 << p;
			for (int i = 0; i < hammingCode.size(); ++i) {
				if ((i & mask) != 0) {
					parity ^= hammingCode[i];
				}
			}
			// ���������żУ��λ���뵽��������
			hammingCode[mask - 1] = parity;
		}

		return hammingCode;
	}

	// ʹ�ú���������У��λ
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
		// ����У����
		for (int i = 0; i < 16; ++i)
		{
			mat.at<Vec3b>(QrPointSize + 1, SafeAreaWidth + i) = pixel[(checkcode & 1) ? 7 : 0]; // ���У����ĵ�ǰλΪ1����ʹ�ð�ɫ���ص㣬����ʹ�ú�ɫ���ص�
			checkcode >>= 1; // ÿ������һλ��׼��������һλУ����
		}

		// ����֡��
		for (int i = 0; i < 16; ++i)
		{
			mat.at<Vec3b>(QrPointSize + 2, SafeAreaWidth + i) = pixel[(FrameNo & 1) ? 7 : 0]; // ���֡�ŵĵ�ǰλΪ1����ʹ�ð�ɫ���ص㣬����ʹ�ú�ɫ���ص�
			FrameNo >>= 1; // ÿ������һλ��׼��������һλ֡��
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
		//���ƴ��ά��ʶ���
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
		//����С��ά��ʶ���
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
		const unsigned char* pos = (const unsigned char*)info; // ���������Ϣת��Ϊ�޷����ַ�ָ��
		const unsigned char* end = pos + len; // ������Ϣ��ĩβλ��

		// ���������������
		for (int i = 0; i < areapos[areaID][0][0]; ++i)
		{
			uint32_t outputCode = 0; // ���ڴ洢���������

			// ��������������У�ÿ�δ���8���ַ�
			for (int j = 0; j < areapos[areaID][0][1] / 8; ++j)
			{
				outputCode |= *pos++; // ����ǰ�ַ���ֵ����������

				// �������ص�
				for (int k = areapos[areaID][1][1]; k < areapos[areaID][1][1] + 8; ++k)
				{
					mat.at<Vec3b>(i + areapos[areaID][1][0], j * 8 + k) = pixel[(outputCode & 1) ? 7 : 0]; // �����ǰλΪ1����ʹ�ð�ɫ���ص㣬����ʹ�ú�ɫ���ص�
					outputCode >>= 1; // ����һλ��׼��������һλ�ַ�
				}

				if (pos == end) break; // ����Ѿ������������ַ������˳�ѭ��
			}

			if (pos == end) break; // ����Ѿ������������ַ������˳�ѭ��
		}
	}

	void BulidFrameFlag(Mat& mat, FrameType frameType, int tailLen)
	{
		// ������ʼ�ͽ�����־λ����ɫ
		const Vec3b startColor = (frameType == FrameType::Start || frameType == FrameType::StartAndEnd) ? pixel[White] : pixel[Black];
		const Vec3b endColor = (frameType == FrameType::End || frameType == FrameType::StartAndEnd) ? pixel[White] : pixel[Black];

		// ������ʼ��־λ
		mat.at<Vec3b>(QrPointSize, SafeAreaWidth) = startColor;
		mat.at<Vec3b>(QrPointSize, SafeAreaWidth + 1) = startColor;
		mat.at<Vec3b>(QrPointSize, SafeAreaWidth + 2) = (frameType == FrameType::Start || frameType == FrameType::StartAndEnd) ? pixel[Black] : pixel[White];
		mat.at<Vec3b>(QrPointSize, SafeAreaWidth + 3) = (frameType == FrameType::Start || frameType == FrameType::StartAndEnd) ? pixel[Black] : pixel[White];

		// ���ƽ�����־λ
		mat.at<Vec3b>(QrPointSize, SafeAreaWidth) = endColor;
		mat.at<Vec3b>(QrPointSize, SafeAreaWidth + 1) = endColor;
		mat.at<Vec3b>(QrPointSize, SafeAreaWidth + 2) = (frameType == FrameType::End || frameType == FrameType::StartAndEnd) ? pixel[White] : pixel[Black];
		mat.at<Vec3b>(QrPointSize, SafeAreaWidth + 3) = (frameType == FrameType::End || frameType == FrameType::StartAndEnd) ? pixel[White] : pixel[Black];

		// ����β����־λ����ɫ������β���������ñ�־λ����ɫ
		for (int i = 4; i < 16; ++i)
		{
			mat.at<Vec3b>(QrPointSize, SafeAreaWidth + i) = pixel[(tailLen & 1) ? 7 : 0]; // �����ǰλΪ1����ʹ�ð�ɫ���ص㣬����ʹ�ú�ɫ���ص�
			tailLen >>= 1; // ����һλ��׼��������һλ�ַ�
		}

	}

	Mat CodeFrame(FrameType frameType, const char* info, int tailLen, int FrameNo)
	{
		// ����һ����ɫ������ Mat ����
		Mat codeMat = Mat(FrameSize, FrameSize, CV_8UC3, Vec3d(255, 255, 255));

		// ���֡���Ͳ��ǽ���֡���Ҳ�����ʼ�ͽ���֡����β����������Ϊÿ֡�ֽ���
		if (frameType != FrameType::End && frameType != FrameType::StartAndEnd)
			tailLen = BytesPerFrame;

		// ���ư�ȫ����
		BulidSafeArea(codeMat);

		// ���ƶ�ά��ʶ���
		BulidQrPoint(codeMat);

		// ����У����
		int checkCode = CalCheckCode((const unsigned char*)info, tailLen,
			frameType == FrameType::Start || frameType == FrameType::StartAndEnd,
			frameType == FrameType::End || frameType == FrameType::StartAndEnd, FrameNo);

		// ����֡��־
		BulidFrameFlag(codeMat, frameType, tailLen);

		// ����У�����֡��
		BulidCheckCodeAndFrameNo(codeMat, checkCode, FrameNo % 65536);

		// ���β�����Ȳ���ÿ֡�ֽ�������β����������Ϊÿ֡�ֽ���
		if (tailLen != BytesPerFrame)
			tailLen = BytesPerFrame;

		// ������Ϣ��������
		for (int i = 0; i < RectAreaCount && tailLen > 0; ++i)
		{
			int lennow = std::min(tailLen, lenlim[i]);
			BulidInfoRect(codeMat, info, lennow, i);
			tailLen -= lennow;
			info += lennow;
		}

		return codeMat; // �������ɵĶ�ά��ͼ��
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
	// �򿪶������ļ�
	ifstream file("random.bin", ios::binary);
	if (!file) {
		cerr << "Error: Failed to open file 'random.bin'" << endl;
		return 1;
	}

	// ��ȡ�������ļ�����
	file.seekg(0, ios::end);
	int len = file.tellg();
	file.seekg(0, ios::beg);
	vector<char> buffer(len);
	file.read(buffer.data(), len);
	file.close();
	cout << buffer.data();
	// ����·��Ϊ��ǰ�ļ��У������ʽΪpng
	const char* savePath = "./";
	const char* outputFormat = "png";

	// ֡������
	int FrameCountLimit = 5;

	// �������������ɶ�ά��֡
	Code::Main(buffer.data(), len, savePath, outputFormat, FrameCountLimit);

	cout << "Generated QR code frames successfully!" << endl;

	return 0;
}