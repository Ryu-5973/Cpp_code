// inet_aton.c
// 网络地址初始化与匹配

// #include<arpa/inet.h>
// int inet_aton(const char *string, struct int_addr *addr);
// 成功时返回1,失败返回0

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include "my_util.h"

int main(int argc, char *argv[]) {
    char *addr = "127.232.124.79";
    struct sockaddr_in addr_inet;

    if(!inet_aton(addr, &addr_inet.sin_addr)) {
        error_handling("Conversion error");
    }else {
        printf("Network ordered integer addr: %#x \n",
            addr_inet.sin_addr.s_addr);
    }
    return 0;
}