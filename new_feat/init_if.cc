/*
 * @Author: Ryu-59073
 * @Date: 2022-08-09 09:15:36
 * @LastEditors: Ryu-59073
 * @LastEditTime: 2022-08-09 09:16:54
 * @FilePath: /Cpp_code/new_feat/init_if.cc
 * @Description: C++17 init if语句
 * 
 * Copyright (c) 2022 by Ryu-59073, All Rights Reserved. 
 */
#include<iostream>

using namespace std;

int64_t getVal() {
    return rand();
}

int main(int argc, char const *argv[]) {
    if(int val = getVal(); val > 100) {
        cout << val << endl;
        cout << "bigger" << endl;
    }else {
        cout << val << endl;
        cout << "smaller" << endl;
    }
    return 0;
}

