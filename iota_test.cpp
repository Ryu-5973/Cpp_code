/*
 * @Author: Ryu-59073
 * @Date: 2022-06-12 08:00:19
 * @LastEditors: Ryu-59073
 * @LastEditTime: 2022-07-23 09:14:28
 * @FilePath: /Cpp_code/iota_test.cpp
 * @Description: 
 * 
 * Copyright (c) 2022 by Ryu-59073, All Rights Reserved. 
 */
#include <iostream>
#include <numeric>
#include <vector>

using namespace std;

int main(){
    vector<int> ve(5);
    cout << "Size = " << ve.size() << endl;
    iota(ve.begin(), ve.end(), 0);
    cout << "Size = " << ve.size() << endl;
    return 0;
}