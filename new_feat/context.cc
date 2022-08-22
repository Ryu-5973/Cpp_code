/*
 * @Author: Ryu-59073
 * @Date: 2022-08-18 08:50:22
 * @LastEditors: Ryu-59073
 * @LastEditTime: 2022-08-18 09:10:58
 * @FilePath: /Cpp_code/new_feat/context.cc
 * @Description: 注意：这个不是新特性
 * 
 * Copyright (c) 2022 by Ryu-59073, All Rights Reserved. 
 */
#include <iostream>

#include <ucontext.h>

void Hello(ucontext_t& ctx) {
    std::cout << "Hello ";
    setcontext(&ctx);
}

void World() {
    std::cout << "World" << std::endl;
}

int main() {
    ucontext_t ctx;
    getcontext(&ctx);
    ctx.uc_link = nullptr;
    ctx.uc_stack.ss_sp = malloc(1000);
    ctx.uc_stack.ss_size = 1000;
    makecontext(&ctx, World, 0);
    Hello(ctx);
    return 0;
}
