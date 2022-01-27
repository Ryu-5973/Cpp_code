// uecho_con_client.c
// 创建已连接的UDP套接字

/*
* UDP通过sendto函数传输数据的大致过程可分为以下3个阶段
** 1. 向UDP套接字注册目标IP和端口号
** 2. 传输数据
** 3. 删除UDP套接字中注册的目标地址信息
* 这种未注册目标地址信息的套接字称为未连接套接字
* 使用connect连接的套接字称为连接套接字
* 与同一主机进行长时间通信时,将套接字变成连接套接字会提高整体性能
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "my_util.h"    // error_handling

#define BUF_SIZE 30

int main(int argc, char *argv[]) {
    int sock;
    char message[BUF_SIZE];
    int str_len;
    socklen_t addr_sz;

    struct sockaddr_in serv_addr, from_addr;
    if(argc != 3) {
        printf("Usage : %s <IP> <port>", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(sock == -1) {
        error_handling("socket() error");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    while (1) {
        fputs("Insert message(q to quit): ", stdout);
        fgets(message, sizeof(message), stdin);
        if(!strcmp(message, "q\n") || !strcmp(message, "Q\n")) {
            break;
        }
        /*
        sendto(sock, message, strlen(message), 0,
                (struct sockaddr*)&serv_addr, sizeof(serv_addr));
        */
       write(sock, message, strlen(message));
       /*
        addr_sz = sizeof(from_addr);
        strlen = recvfrom(sock, message, BUF_SIZE, 0,
                (struct sockaddr*)&from_addr, &addr_sz);
       */ 
        str_len = read(sock, message, sizeof(message) - 1);

        message[str_len] = 0;
        printf("Message from server: %s", message);
    }
    close(sock);
    
    return 0;
}