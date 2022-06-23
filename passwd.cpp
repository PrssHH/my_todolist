// 对密码进行多重hash的处理
#ifndef PSWD_C
#define PSWD_C
#include "passwd.h"
#include "md5.h"

// 密码打算先采用几个朴素的可逆加密，然后再hash
// 先进行一个md5的算然后把原密码按顺序间隔插入md5然后再md5再md5

string passwd(string s) {
    my_md5 m;
    string result1 = "";
    char *ptr1, *ptr2, str[] = "iuwasdgftaweszdfgawerwerqzxceqxicuo", *ptr3;
    

    m.last_str(s);
    ptr1 = m.result();
    //ptr2 = s.c_str();
    ptr2 = &s[0];
    ptr3 = str;

    while (*ptr1 && *ptr2) {
        result1 = result1 + *ptr1 + *ptr2;
        ptr1++; ptr2++;
    }
    while (*ptr1) {
        result1 = result1 + *ptr1 + *ptr3;
        ptr1++; ptr3++;
    }
    while (*ptr2) {
        result1 = result1 + *ptr2 + *ptr3;
        ptr2++; ptr3++;
    }


    m.init();
    m.last_str(result1);
    ptr1 = m.result();
    result1 = "";

    while(*ptr1) {
        result1 = result1 + *ptr1;
        ptr1++;
    }


    m.init();
    m.last_str(result1);
    ptr1 = m.result();
    result1 = "";

    while(*ptr1) {
        result1 = result1 + *ptr1;
        ptr1++;
    }
    return result1;
}

#endif