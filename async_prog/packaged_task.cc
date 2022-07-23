/*
 * @Author: Ryu-59073
 * @Date: 2022-07-23 02:59:15
 * @LastEditors: Ryu-59073
 * @LastEditTime: 2022-07-23 03:02:42
 * @FilePath: /Cpp_code/async_prog/packaged_task.cc
 * @Description: std::packaged_task使用
 * 
 * Copyright (c) 2022 by Ryu-59073, All Rights Reserved. 
 */
#include <iostream>
#include <thread>
#include <future>
#include <cmath>

int main(int argc, char const *argv[]) {
    std::packaged_task<int(int, int)> task([](int a, int b){
        return std::pow(a, b);
    });
    std::future<int> f = task.get_future();

    std::thread task_td(std::move(task), 2, 10);
    task_td.join();

    std::cout << "task_thread:\t" << f.get() << '\n';
    return 0;
}
