/*
 * @Author: Ryu-59073
 * @Date: 2022-07-13 00:56:54
 * @LastEditors: Ryu-59073
 * @LastEditTime: 2022-07-25 01:19:37
 * @FilePath: /Cpp_code/util.h
 * @Description: util
 * 
 * Copyright (c) 2022 by Ryu-59073, All Rights Reserved. 
 */
#ifndef CPP_CODE_UTIL_H
#define CPP_CODE_UTIL_H

#include <stdio.h>
#include <stdlib.h>

// 报错输出
void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}



#endif  // CPP_CODE_UTIL_H