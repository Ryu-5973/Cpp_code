/*
 * @Author: Ryu-59073
 * @Date: 2022-07-23 01:05:05
 * @LastEditors: Ryu-59073
 * @LastEditTime: 2022-07-23 01:14:27
 * @FilePath: /Cpp_code/async_prog/false_awake.cc
 * @Description: 测试notify_one 是否会产生虚假唤醒
 * 
 * @Conclution: notify_one 会产生虚假唤醒，但是为啥呢？
 * 
 * Copyright (c) 2022 by Ryu-59073, All Rights Reserved. 
 */
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>

int main(int argc, char const *argv[]) {
    std::mutex mtx;
    std::condition_variable cv, cv1;
    int var = 0;
    bool ready = false;
    std::vector<std::thread> threads;
    auto func = [&](){
        while(1) {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [&](){
                std::cout << std::this_thread::get_id() << " get var = " << var << std::endl;
                return ready;
            });
            ready = false;
            lock.unlock();
            cv1.notify_one();
        }

    };
    for(int i = 0; i < 200; ++ i) {
        threads.push_back(std::thread(func));
    }
    for(int i = 0; i < 500; ++ i) {
        std::unique_lock<std::mutex> lock(mtx);
        cv1.wait(lock, [&](){
            return ready == false;
        });
        ready = true;
        var ++;
        cv.notify_one();
    }
    for(auto &t : threads) {
        t.join();
    }
    return 0;
}
