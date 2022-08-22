/*
 * @Author: Ryu-59073
 * @Date: 2022-08-03 07:10:35
 * @LastEditors: Ryu-59073
 * @LastEditTime: 2022-08-03 07:15:47
 * @FilePath: /Cpp_code/c_demo.c
 * @Description: 
 * 
 * 
 * Copyright (c) 2022 by Ryu-59073, All Rights Reserved. 
 */
#include <stdio.h>

int main(int argc, char const *argv[]) {
    // char *s = "\0\x1\x2\x3\x4\x5\x6\x7";
    char *s = "\x7\x6\x5\x4\x3\x2\x1\0";
    for(int i = 0; i < 8; ++ i) {
        printf("%d\n", (int)(s[i]));
        printf("%d\n", *(int*)(s+i));
    }
    printf("%d\n", (*(int *)s) & 255);
    return 0;
}

