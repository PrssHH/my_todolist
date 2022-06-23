#ifndef MD5_H
#define MD5_H
#include <cstdio>
#include <cstdlib>
#include <getopt.h>
#include <string>
#include <vector>
#include <ctime>
#include <iostream>
#include <cstring>
using namespace std;

class my_md5 {
public:
    my_md5();

    void init();
    
    // 这个函数可以不断的往里塞str，相当于分段加入md5的源数据
    void add_str(string& str); 

    void last_str();
    void last_str(string& str); 
    void calc(); 
    // 上面三个作用相同，加入最后一个str，并且计算md5
    

    char* result();
    void show_result();
    
    ~my_md5();

private:
    unsigned int state[4]; // 四个寄存器
    unsigned int count[2]; // 长度统计
    unsigned char buffer[512]; // 输入
    unsigned char output[128]; // 输出
    char _result[33];
    bool is_padding;

    const unsigned int A = 0x67452301;
    const unsigned int B = 0xefcdab89;
    const unsigned int C = 0x98badcfe;
    const unsigned int D = 0x10325476;

    const unsigned int T[64] = { 
        0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
        0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501, 
        0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
        0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821, 
        0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa, 
        0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8, 
        0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 
        0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a, 
        0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c, 
        0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70, 
        0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05, 
        0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665, 
        0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, 
        0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1, 
        0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1, 
        0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
    };

    const unsigned int s[64] = {
        7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
        5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
        4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
        6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
    };


    // 往hash的源数据里塞东西
    void update(const unsigned char * input_str, int str_len);

    // 填充函数
    void padding();


    // 变换一个区块
    void transform(unsigned char block[64]);

    void int2char(unsigned char *output, const unsigned int *input, int length);
    void char2int(unsigned int *output, const unsigned char *input, int length);

    // 循环左移函数
    unsigned int left_shift(unsigned int x, int pos);

    // 轮换abcd
    void swap(unsigned int &a, unsigned int &b, unsigned int &c, unsigned int &d);

    // 轮函数
    unsigned int F(unsigned int x, unsigned int y, unsigned int z);
    unsigned int G(unsigned int x, unsigned int y, unsigned int z);
    unsigned int H(unsigned int x, unsigned int y, unsigned int z);
    unsigned int I(unsigned int x, unsigned int y, unsigned int z);

    // 压缩函数
    void FF(unsigned int &a, unsigned int b, unsigned int c, unsigned int d, unsigned int Mi, unsigned int s, unsigned int Ti);
    void GG(unsigned int &a, unsigned int b, unsigned int c, unsigned int d, unsigned int Mi, unsigned int s, unsigned int Ti);
    void HH(unsigned int &a, unsigned int b, unsigned int c, unsigned int d, unsigned int Mi, unsigned int s, unsigned int Ti);
    void II(unsigned int &a, unsigned int b, unsigned int c, unsigned int d, unsigned int Mi, unsigned int s, unsigned int Ti);
};

#endif