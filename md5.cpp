#ifndef MD5_C
#define MD5_C
#include <cstdio>
#include <cstdlib>
#include <getopt.h>
#include <string>
#include <vector>
#include <ctime>
#include <iostream>
#include <cstring>

#include "md5.h"

using namespace std;

my_md5::my_md5() {
    init();
}

my_md5::~my_md5() {

}


unsigned int my_md5::left_shift(unsigned int x, int pos) {
    return (x << pos) | (x >> (32 - pos));
}

void my_md5::swap(unsigned int &a, unsigned int &b, unsigned int &c, unsigned int &d) {
    unsigned int tmp = d;
    d = c;
    c = b;
    b = a;
    a = tmp;
}

unsigned int my_md5::F(unsigned int x, unsigned int y, unsigned int z) {
    return ((x & y) | ((~x) & z));
}

unsigned int my_md5::G(unsigned int x, unsigned int y, unsigned int z) {
    return ((x & z) | (y & (~z)));
}

unsigned int my_md5::H(unsigned int x, unsigned int y, unsigned int z) {
    return (x ^ y ^ z);
}

unsigned int my_md5::I(unsigned int x, unsigned int y, unsigned int z) {
    return (y ^ (x | (~z)));
}



void my_md5::FF(unsigned int &a, unsigned int b, unsigned int c, unsigned int d, unsigned int Mi, unsigned int s, unsigned int Ti) {
    a = left_shift(a + F(b, c, d) + Mi + Ti, s) + b;
}

void my_md5::GG(unsigned int &a, unsigned int b, unsigned int c, unsigned int d, unsigned int Mi, unsigned int s, unsigned int Ti) {
    a = left_shift(a + G(b, c, d) + Mi + Ti, s) + b;
}

void my_md5::HH(unsigned int &a, unsigned int b, unsigned int c, unsigned int d, unsigned int Mi, unsigned int s, unsigned int Ti) {
    a = left_shift(a + H(b, c, d) + Mi + Ti, s) + b;
}

void my_md5::II(unsigned int &a, unsigned int b, unsigned int c, unsigned int d, unsigned int Mi, unsigned int s, unsigned int Ti) {
    a = left_shift(a + I(b, c, d) + Mi + Ti, s) + b;
}


void my_md5::transform(unsigned char block[64]) {
    // 获取CV数据
    unsigned int a = state[0], b = state[1], c = state[2], d = state[3];
    unsigned int x[16];

    //将输入的char转为int
    char2int(x, block, 64);

    for (int i = 0; i < 64; i++) {
        if (i < 16) {
            FF(a, b, c, d, x[i], s[i], T[i]);
        }
        if (16 <= i && i < 32) {
            GG(a, b, c, d, x[(1 + 5 * i) % 16], s[i], T[i]);
        }
        if (32 <= i && i < 48) {
            HH(a, b, c, d, x[(5 + 3 * i) % 16], s[i], T[i]);
        }
        if (48 <= i) {
            II(a, b, c, d, x[(7 * i) % 16], s[i], T[i]);
        }
        swap(a, b, c, d);
    }

    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
}


void my_md5::init() {
    state[0] = A;
    state[1] = B;
    state[2] = C;
    state[3] = D;
    is_padding = false;
    memset(count, 0, sizeof(count));
    memset(output, 0, sizeof(output));
}


void my_md5::calc() {
    string str = "";
    last_str(str);
}


void my_md5::last_str() {
    string str = "";
    last_str(str);
}

void my_md5::last_str(string& str) {
    update((unsigned char*)(str.c_str()), str.size());
    padding();

    int2char(output, state, 16);
}


void my_md5::add_str(string& str) {
    update((unsigned char*)(str.c_str()), str.size());
}

void my_md5::update(const unsigned char * input_str, int str_len) {
    int index = (count[0] >> 3) & 0x3f; // 0x3f = 63 = 六个1，即计算 512 / 8 % 64，为已经填进去、不完整的块长
    count[0] += str_len << 3;
    if (count[0] < (str_len << 3)) {
        count[1] += str_len >> 29;
    }

    int padding_len = 64 - index;
    if (str_len >= padding_len) {
        memcpy(buffer + index, input_str, padding_len);
        transform(buffer);
        for (int i = padding_len; i + 64 < str_len; i += 64) {
            transform((unsigned char*)input_str + i);
        }
        index = 0;
    }
    memcpy(buffer + index, input_str, padding_len); // 如果整个填进去都不能填满512bit就填，但是不transform
}

void my_md5::padding() {
    unsigned char pad[64] = {0};
    pad[0] = 0x80;

    if (!is_padding) {
        unsigned char lens[8];// 最后64bit放长度数据
        int2char(lens, count, 8);
        int index = (count[0] >> 3) & 0x3f;
        int pad_len = index < 56 ? (56 - index) : (64 + 56 - index);
        update(pad, pad_len);
        update(lens, 8);
        is_padding = true;
    }
}

void my_md5::char2int(unsigned int *output, const unsigned char *input, int length) {
    unsigned int i = 0, j = 0;
    while (j < length) {
        output[i] = (input[j]) | (input[j + 1] << 8) | (input[j + 2] << 16) | (input[j + 3] << 24);
        i++;
        j += 4;
    }
}

void my_md5::int2char(unsigned char *output, const unsigned int *input, int length) {
    unsigned int i = 0, j = 0;
    while (j < length) {
        output[j] = input[i] & 0xff;
        output[j + 1] = (input[i] >> 8) & 0xff;
        output[j + 2] = (input[i] >> 16) & 0xff;
        output[j + 3] = (input[i] >> 24) & 0xff;
        i++;
        j += 4;
    }
}

void my_md5::show_result() {
    cout << "MD5: " << result() << endl;
}

char* my_md5::result() {
    _result[32] = '\0';
    for (int i = 0; i < 16; ++i) {
        sprintf(_result + i * 2, "%02x", output[i]);
    }
    return _result;
}

#endif