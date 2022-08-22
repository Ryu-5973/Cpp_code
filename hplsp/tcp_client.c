/*
 * @Author: Ryu-59073
 * @Date: 2022-07-13 00:34:08
 * @LastEditors: Ryu-59073
 * @LastEditTime: 2022-07-13 05:07:26
 * @FilePath: /Cpp_code/hplsp/tcp_client.c
 * @Description: 用于测试服务端程序的TCP客户端
 * 
 * Copyright (c) 2022 by Ryu-59073, All Rights Reserved. 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "../util.h"

#define BUF_SIZE 1024

void Usage() {
    printf("Usage : ./tcp_clent [-i <IP>] -p <port> [-o]\n");
    exit(-1);
}


int main(int argc, char *argv[]) {
    int sock;
    char message[BUF_SIZE];
    int str_len;
    struct sockaddr_in serv_addr;

    char ip[20] = "127.0.0.1";
    int port = -1;
    int isOOB = 0;

    // Usage : ./tcp_clent [-i <IP>] -p <port> [-o]
    char c;
    while ((c = getopt(argc, argv, "i:p:o::")) != EOF) {
        switch (c) {
        case 'i':
            stpcpy(ip, optarg);
            break;
        case 'p':
            port = atoi(optarg);
            break;
        case 'o':
            isOOB = MSG_OOB;
            break;
        default:
            Usage();
            break;
        }
    }
    
    if(port == -1) {
        Usage();
    }
    
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1) {
        error_handling("socket() error!");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(ip);
    serv_addr.sin_port = htons(port);

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
        if(strlen(message) == 1) continue;
        send(sock, message, strlen(message) - 1, isOOB);
        str_len = recv(sock, message, BUF_SIZE - 1, 0);
        message[str_len] = 0;
        printf("Message from server: %s\n", message);
    }
    close(sock);
    return 0;
}
