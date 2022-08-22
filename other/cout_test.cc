/*
 * @Author: Ryu-59073
 * @Date: 2022-08-07 08:52:58
 * @LastEditors: Ryu-59073
 * @LastEditTime: 2022-08-07 09:06:10
 * @FilePath: /Cpp_code/other/cout_test.cc
 * @Description: cout 是否线程安全测试
 * 
 * Copyright (c) 2022 by Ryu-59073, All Rights Reserved. 
 */
#include <iostream>
#include <thread>
#include <string>

int main(int argc, char const *argv[]) {
    auto print = []() {
        for(int i = 0; i < 1000; ++ i) {
            // std::cout << tmp << name << std::flush;
            // std::cout << "\033[34m" << 
            //     "111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111\n" << 
            //     "\033[0m" << std::flush;
            std::cout << "11111111111111111111111"
                      << "11111111111111111111111"
                      << "11111111111111111111111\n"
                      << std::flush;
        }
    };
    auto print1 = []() {
        for(int i = 0; i < 1000; ++ i) {
            // std::cout << tmp << name << std::flush;
            std::cout << "22222222222222222222222"
                      << "22222222222222222222222"
                      << "22222222222222222222222\n"
                      << std::flush;
        }
    };
    std::string name1 = "\033[34m111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111\033[0m\n";
    std::string name2 = "\033[31m222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222\033[0m\n";
    std::thread t1(print);
    std::thread t2(print1);
    t1.join();
    t2.join();
    return 0;
}

