/*
 * @Author: Ryu-59073
 * @Date: 2022-07-23 09:13:02
 * @LastEditors: Ryu-59073
 * @LastEditTime: 2022-07-23 09:23:38
 * @FilePath: /Cpp_code/async_prog/async.cc
 * @Description: std::async 使用
 * 
 * Copyright (c) 2022 by Ryu-59073, All Rights Reserved. 
 */
#include <iostream>
#include <vector>
#include <algorithm>
#include <future>
#include <mutex>
#include <numeric>
#include <string>

std::mutex mtx;
struct X{
    void foo(int i, const std::string& str) {
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << str << " " << i << "\n";
    }
    void bar(const std::string& str) {
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << str << '\n';
    }
    int operator()(int i) {
        std::lock_guard<std::mutex> lockk(mtx);
        std::cout << i << '\n';
        return i + 10;
    }
};


template <typename RandomIt>
int64_t parallel_sum(RandomIt beg, RandomIt end) {
    auto len = end - beg;
    if (len < 1000) {
        return std::accumulate(beg, end, 0);
    }
 
    RandomIt mid = beg + len/2;
    auto handle = std::async(std::launch::async,
        parallel_sum<RandomIt>, mid, end);
    int64_t sum = parallel_sum(beg, mid);
    return sum + handle.get();
}


int main(int argc, char const *argv[]) {
    std::vector<int64_t> ve(1 << 20);
    iota(ve.begin(), ve.end(), 0);
    std::cout << "The sum is " << parallel_sum(ve.begin(), ve.end()) << '\n';

    X x;
    auto a1 = std::async(&X::foo, &x, 42, "Hello");
    auto a2 = std::async(std::launch::deferred, &X::bar, x, "world!");
    auto a3 = std::async(std::launch::async, X(), 43);
    a2.wait();
    std::cout << a3.get() << "\n";
    return 0;
}
