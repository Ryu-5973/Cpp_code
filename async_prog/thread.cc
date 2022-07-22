/*
 * @Author: Ryu-59073
 * @Date: 2022-07-22 07:49:43
 * @LastEditors: Ryu-59073
 * @LastEditTime: 2022-07-22 07:58:54
 * @FilePath: /Cpp_code/async_prog/thread.cc
 * @Description: C++11 std::thread 使用
 * 
 * Copyright (c) 2022 by Ryu-59073, All Rights Reserved. 
 */
#include <iostream>
#include <thread>
#include <mutex>
#include <string_view>

int main(int argc, char const *argv[]) {
    std::mutex mtx;
    auto fun = [&](std::string_view a) {
        std::unique_lock<std::mutex> lock(mtx);
        std::cout << "thread id = " << std::this_thread::get_id() << "\t " << std::endl; 
    };
    std::thread t(fun, "hello");
    fun("world");
    t.join();
    return 0;
}

