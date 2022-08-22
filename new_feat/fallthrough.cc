/*
 * @Author: Ryu-59073
 * @Date: 2022-08-09 09:25:00
 * @LastEditors: Ryu-59073
 * @LastEditTime: 2022-08-09 09:25:14
 * @FilePath: /Cpp_code/new_feat/fallthrough.cc
 * @Description: C++17 [[fallthrough]]
 * 
 * Copyright (c) 2022 by Ryu-59073, All Rights Reserved. 
 */
#include<iostream>

using namespace std;

int main(int argc, char const *argv[]) {
    const int64_t val = 1;
    switch (val) {
    case 1:
        cout << 1 << endl;
    case 2:
        cout << 2 << endl;
    default:
        break;
    }
    return 0;
}
