// inet_addr.c
// 网络地址的初始化与匹配


// #include <arpa/inet.h>
// in_addr_t inet_addr(const char *string);
// 成功时返回32位大端序整数型值,失败返回INADDR_NONE

#include <stdio.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
    char *addr1 = "1.2.3.4";
    char *addr2 = "1.2.3.256";

    unsigned long conv_addr = inet_addr(addr1);
    if(conv_addr == INADDR_NONE) {
        printf("Error occured! \n");
    }else {
        printf("Network ordered integer addr: %#lx \n", conv_addr);
    }

    conv_addr = inet_addr(addr2);
    if(conv_addr == INADDR_NONE) {
        printf("Error occured! \n");
    }else {
        printf("Network ordered integer addr: %#lx \n", conv_addr);
    }
    return 0;
}
