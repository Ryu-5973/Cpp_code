// tcp_client.c
// 面向连接的套接字:TCP套接字示例

// 创建套接字(成功时返回文件描述符,失败返回-1)
// int socket(int domain, int type, int protocol);
// 协议族(PROTOCOL FAMILY):
// PF_INET(IPv4互联网协议族),PF_INET6(IPv6互联网协议族),
// PF_LOCAL(本地通信的UNIX协议族),PF_PACKET(底层套接字的协议族)
// PF_IPX(IPX Vovell协议族)
// 套接字类型
// 面向连接的套接字(SOCK_STREAM)
// 面向消息的套接字(SOCK_DGRAM)
// 协议的最终选择
// 一般情况下传递0,除非遇到"同一协议族中存在多个数据传输方式相同的协议",
// 此时需要传递第三个参数具体指定协议信息.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

// 差错处理
void error_handling(char *message);

int main(int argc, char *argv[]) {
    int sock;
    struct sockaddr_in serv_addr;
    char message[30];
    int str_len = 0;
    int idx = 0, read_len = 0;

    if(argc != 3) {
        printf("Usage: %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0); // IPv4+面向连接=TCP
    if(sock == -1) {
        error_handling("socket() error");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        error_handling("connect() error!");
    }

    while(read_len = read(sock, &message[idx ++], 1)) {
        if(read_len == -1) {
            error_handling("read() error!");
        }
        str_len += read_len;
    }

    printf("Message from server: %s \n", message);
    printf("Function read call count: %d \n", str_len);
    close(sock);
    return 0;
}


void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
