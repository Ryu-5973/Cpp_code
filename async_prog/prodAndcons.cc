/*
 * @Author: Ryu-59073
 * @Date: 2022-07-22 08:55:44
 * @LastEditors: Ryu-59073
 * @LastEditTime: 2022-07-23 00:55:47
 * @FilePath: /Cpp_code/async_prog/prodAndcons.cc
 * @Description: 
 * 
 * Copyright (c) 2022 by Ryu-59073, All Rights Reserved. 
 */
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <random>
#include <vector>
#include <queue>
#include <unistd.h>

int main(int argc, char const *argv[]) {
    std::mutex write_mtx, mtx;
    std::condition_variable not_empty, not_full;
    int prod_num = 10;
    int cons_num = 5;
    int capacity = 10;

    struct node{
        uint64_t prod_id;
        char data;
        node(uint64_t id): prod_id(id), data(rand() % 26 + 'a') {}
    };
    std::queue<node*> q;

    auto producer = [&](uint64_t id){
        {
            std::lock_guard<std::mutex> lock(write_mtx);
            std::cout << "producer(" << id << ") come in\n";
        }
        while(1) {
            std::unique_lock<std::mutex> lock(mtx);
            not_full.wait(lock, [&q, &capacity]{
                return q.size() < capacity;
            });

            auto data = new node(id);
            q.push(data);
            std::cout <<  "producer(" << id << ") produce data " << data->data  << " queue length = " << q.size() << "\n";
            lock.unlock();
            not_empty.notify_all();
            sleep(4);
        }
    };

    auto consumer = [&](uint64_t id){
        {
            std::lock_guard<std::mutex> lock(write_mtx);
            std::cout << "consumer(" << id << ") come in\n";
        }
        while(1) {
            std::unique_lock<std::mutex> lock(mtx);
            not_empty.wait(lock, [&q]{
                return q.empty() == false;
            });

            auto data = q.front();
            q.pop();
            std::cout <<  "consumer(" << id << ") get data " << data->data << " from producer(" << data->prod_id <<")\n";
            lock.unlock();
            not_full.notify_all();
            sleep(2);
        }
    };

    std::vector<std::thread> threads;
    for(int i = 0; i < cons_num; ++ i) {
        threads.push_back(std::thread(consumer, i));
    }
    for(int i = 0; i < prod_num; ++ i) {
        threads.push_back(std::thread(producer, i));
    }
    for(auto &thread : threads) {
        thread.join();
    }
    return 0;
}
