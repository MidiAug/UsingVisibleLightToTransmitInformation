#pragma once

#define BIT 2 // ��λ����һ�����أ�Ŀǰ֧��123
//#define CAPACITY (8116*BIT) //margin=2 HEIGHT = WIDTH = 97
//#define CAPACITY (8483*BIT) //margin=1 HEIGHT = WIDTH = 97
#define MARGIN 1
#define HEIGHT 160
#define WIDTH 260
#define CAPACITY ((HEIGHT*WIDTH-(HEIGHT+WIDTH)*2*MARGIN+4*MARGIN*MARGIN-3*9*9)*BIT) //margin= 2 HEIGHT = 111, WIDTH = 199 

#define MULTIPLE 9 //ͼ��Ŵ���
#define PICFORMAT ".png" // ͼƬ��ʽΪpng
#define THREADCNT 50
