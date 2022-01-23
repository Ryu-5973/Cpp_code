// endian_conv.c
// 字节序转换

// short: 2byte long: 4byte
// unsigned short htons(unsigned short);
// unsigned short ntohs(unsigned short);
// unsigned long htonl(unsigned long);
// unsigned long ntohl(unsigned long);

// 主机存储可能有大端存储,小端存储
// 网络中统一编码为大端序

// 数据在传输过程中会自动进行转换,除了向sockaddr_in结构体变量填充数据外
// 其他情况无需考虑字节序问题

#include <stdio.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
    unsigned short host_port = 0x1234;
    unsigned short net_port;
    unsigned long host_addr = 0x12345678;
    unsigned long net_addr;

    net_port = htons(host_port);
    net_addr = htonl(host_addr);

    printf("Host ordered port: %#x \n", host_port);
    printf("Network ordered port: %#x \n", net_port);
    printf("Host ordered address: %#lx \n", host_addr);
    printf("Network ordered address: %#lx \n", net_addr);
    return 0;
}
