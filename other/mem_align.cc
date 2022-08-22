/*
 * @Author: Ryu-59073
 * @Date: 2022-07-25 09:02:05
 * @LastEditors: Ryu-59073
 * @LastEditTime: 2022-07-28 19:53:12
 * @FilePath: /Cpp_code/other/mem_align.cc
 * @Description: 
 * 
 * Copyright (c) 2022 by Ryu-59073, All Rights Reserved. 
 */
#include <iostream>
#pragma pack(4)

using namespace std;

int main(int argc, char const *argv[]) {
    struct A{
        int *d;
        __int128_t a;
        int* b;
        int *c;
    };
    struct B{
        int *b;
        __int128_t a;
    };
    struct C{
        bool a;
        bool b;
        bool c;
        bool d;
        char e;
    };
    A a;
    B b;
    C c;
    cout << sizeof(a) << endl;
    cout << sizeof(b) << endl;
    cout << sizeof(c) << endl;
    return 0;
}
