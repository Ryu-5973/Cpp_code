// file_server.c
// 基于半关闭的文件传输程序(服务器端)

/*
* 单方面断开连接带来的问题:另一方可能存在为发送消息时接收方就已经断开连接.
* 解决方法是"只关闭一部分数据交换中使用的流"
* 一旦两台主机间建立了套接字连接,每个主机就会拥有单独的输入流和输出流
* "优雅地断开连接方式"是指非同时断开两个流,close()函数同时断开两个流
*/

/*
* 用于半关闭的函数: shutdown函数
* #include<sys/socket.h>
* int shutdown(int sock, int howto);
* howto(传递断开方式的信息):
* * SHUT_RD: 断开输入流
* * SHUT_WR: 断开输出流
* * SHUT_RDWA: 同时断开I/O流
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h> 
#include "my_util.h"

#define BUF_SIZE 30

int main(int argc, char *argv[]) {
    int serv_sd, clnt_sd;
    FILE *fp;
    char buf[BUF_SIZE];
    int read_cnt;

    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_sz;

    if(argc != 2) {
        printf("Usage : %s <port>", argv[0]);
        exit(1);
    }

    fp = fopen("file_server.c", "rb");
    serv_sd = socket(PF_INET, SOCK_STREAM, 0);  // tcp

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    bind(serv_sd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    listen(serv_sd, 5);

    clnt_addr_sz = sizeof(clnt_addr);
    clnt_sd = accept(serv_sd, (struct sockaddr*)&serv_addr, &clnt_addr_sz);

    while(1) {
        read_cnt = fread((void*)buf, 1, BUF_SIZE, fp);
        // puts(buf);
        // printf("%s", buf);
        if(read_cnt < BUF_SIZE) {
            write(clnt_sd, buf, read_cnt);
            break;
        }
        write(clnt_sd, buf, BUF_SIZE);
    }

    shutdown(clnt_sd, SHUT_WR); // 关写通道
    read(clnt_sd, buf, BUF_SIZE);
    printf("Message from client: %s \n", buf);

    fclose(fp);
    close(clnt_sd);
    close(serv_sd);
    return 0;
}
