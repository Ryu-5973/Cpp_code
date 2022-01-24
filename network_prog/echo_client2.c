// echo_client2.c
// ehco客户端程序2,提前确定接收数据的大小

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "my_util.h"

#define BUF_SIZE 1024

int main(int argc, char *argv[]) {
    int sock;
    char message[BUF_SIZE];
    int str_len, resv_len, resv_cnt;
    struct sockaddr_in serv_addr;

    if(argc != 3) {
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1) {
        error_handling("socket() error!");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        error_handling("connect() error!");
    }else {
        puts("Connect.........");
    }

    while(1) {
        fputs("Input Message(Q to quit): ", stdout);
        fgets(message, BUF_SIZE, stdin);

        if(!strcmp(message, "q\n") || !strcmp(message, "Q\n")){
            break;
        }

        str_len = write(sock, message, strlen(message));
        resv_len = 0;
        while(resv_len < str_len) { // 解决read无法读完所有服务器write的数据
            resv_cnt = read(sock, &message[resv_len], BUF_SIZE - 1);
            if(resv_cnt == -1) {
                error_handling("read() error!");
            }
            resv_len += resv_cnt;
        }
        message[resv_len] = 0;
        printf("Message from server: %s", message);
    }
    close(sock);
    return 0;
}