// echo_server.c
// 服务器段在同一时刻只与一个客户端相连,并提供回声服务
// 服务器段依次向5个客户端提供服务并退出
// 客户端接收用户输入字符串并发送到服务器端
// 服务器端将接收的字符串数据传回客户端,即"回声"
// 服务器端与客户端之间的字符串回声一直执行到客户端输入Q为止

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "my_util.h"

#define BUF_SIZE 1024
#define OPSZ 4

int calculate(int opnum, int opnds[], char oprator);

int main(int argc, char *argv[]) {
    int serv_sock, clnt_sock;
    char opinfo[BUF_SIZE];
    int result, opnd_cnt, i;
    int recv_cnt, recv_len;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_sz;

    if(argc != 2) {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1) {
        error_handling("socket() error!");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        error_handling("bind() error!");
    }

    if(listen(serv_sock, 5) == -1) {
        error_handling("listen() error");
    }

    clnt_addr_sz = sizeof(clnt_addr);
    for(i = 0; i < 5; ++ i) {
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_sz);
        if(clnt_sock == -1) {
            error_handling("accept() error!");
        }else {
            printf("Connect client %d \n", i + 1);
        }

        read(clnt_sock, &opnd_cnt, 1);

        recv_len = 0;
        while((opnd_cnt * OPSZ + 1) > recv_len) {
            recv_cnt = read(clnt_sock, &opinfo[recv_len], BUF_SIZE - 1);
            recv_len += recv_cnt;
        }
        result = calculate(opnd_cnt, (int*)opinfo, opinfo[recv_len - 1]);
        printf("result: %d\n", result);
        write(clnt_sock, (char*)&result, sizeof(result));
        close(clnt_sock);
    }
    close(serv_sock);
    return 0;
}

int calculate(int opnum, int opnds[], char op) {
    int result = opnds[0], i;
    printf("opnum = %d\n", opnum);
    printf("%d ", opnds[0]);
    switch (op)
    {
    case '+':
        for(i = 1; i < opnum; ++ i) {
            printf("%d ", opnds[i]);
            result += opnds[i];
        }
        break;
    case '-':
        for(i = 1; i < opnum; ++ i) {
            printf("%d ", opnds[i]);
            result -= opnds[i];
        }
        break;
    case '*':
        for(i = 1; i < opnum; ++ i) {
            printf("%d ", opnds[i]);
            result *= opnds[i];
        }
        break;
    default:
        break;
    }
    printf("\n");
    return result;
}