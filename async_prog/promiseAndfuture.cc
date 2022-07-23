/*
 * @Author: Ryu-59073
 * @Date: 2022-07-23 01:44:29
 * @LastEditors: Ryu-59073
 * @LastEditTime: 2022-07-23 02:23:49
 * @FilePath: /Cpp_code/async_prog/promiseAndfuture.cc
 * @Description: future 和 promise 使用, 以并行计算为例
 * 
 * Copyright (c) 2022 by Ryu-59073, All Rights Reserved. 
 */
#include <iostream>
#include <thread>
#include <mutex>
#include <future>
#include <vector>

#include <sys/time.h>

const int64_t maxn = 1<<30; 
const int64_t thread_num = 1 << 2; 

void accumulate(int64_t begin, int64_t end, 
        std::promise<int64_t> &accmu_promise) {
    int64_t ret = 0;
    for(int64_t i = begin; i < end; ++ i) {
        ret += i;
    }
    accmu_promise.set_value(ret);
}

int64_t serial() {
    int64_t ans = 0;

    for(int64_t i = 0; i < maxn; ++ i) {
        ans += i;
    }
    return ans;
}


int64_t parallel() {
    int64_t ans = 0;
    std::vector<std::promise<int64_t>*> promises;
    std::vector<std::future<int64_t>> futures;
    std::vector<std::thread> threads;
    int64_t seg_size = maxn / thread_num;
    for(int64_t i = 0; i < thread_num; ++ i) {
        std::promise<int64_t> *promise = new std::promise<int64_t>;
        std::future<int64_t> future = promise->get_future();
        // promises.push_back(promise);
        threads.push_back(std::thread(accumulate, i * seg_size, (i + 1)*seg_size, std::ref(*promise)));
        futures.push_back(std::move(future));
    }
    for(auto &f : futures) {
        ans += f.get();
    }
    for(auto &t : threads) {
        t.join();
    }
    return ans;
}

int main(int argc, char const *argv[]) {
    struct timeval t1, t2;
    double timeuse;
    gettimeofday(&t1, NULL);

    // serial();
    // int64_t ans = serial();
    // parallel()
    int64_t ans =parallel();

    gettimeofday(&t2,NULL);
    timeuse = (t2.tv_sec - t1.tv_sec) + (double)(t2.tv_usec - t1.tv_usec)/1000000.0;
    std::cout<<"time = "<<timeuse<< "s" << std::endl;
    
    std::cout << ans << std::endl;

    return 0;
}
