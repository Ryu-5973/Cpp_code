// hello_server.c 
// 构建服务器端接收请求后向请求者返回"Hello World!"答复

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h> // socker头文件

// 差错处理
void error_handling(char *message);

// 常用API
// int socket(int domain, int type, int protocol);
// int bind(int sockfd, struct sockaddr *myaddr, socklen_t addrlen);
// int listen(int sockfd, int backlog);
// int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

// 网络编程接受连接请求的套接字创建过程如下：
// 第一步，调用socket函数创建套接字
// 第二步，调用bind函数分配ip地址和端口号
// 第三步，调用listen函数转为可接受请求状态
// 第四步，调用accept函数受理连接请求

int main(int argc, char *argv[]) {
    int serv_sock;  // 服务器端socket
    int clnt_sock;  // 客户端socket

    struct sockaddr_in serv_addr;   // 服务器端地址
    struct sockaddr_in clnt_addr;   // 客户机端地址
    socklen_t clnt_addr_size;

    char message[] = "Hello World!";

    if(argc != 2) {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    // 创建服务器套接字
    serv_sock = socket(PF_INET, SOCK_STREAM, 0); // TCP
    if(serv_sock == -1) {
        error_handling("socket() error");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    // 分配IP地址和端口号
    if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        error_handling("bind() error");
    }

    // 将套接字转换为可接收连接状态
    if(listen(serv_sock, 5) == -1) {
        error_handling("listen() error");
    }

    clnt_addr_size = sizeof(clnt_addr);
    // 受理连接请求,如果在没有连接请求的情况下调用该函数,则不会返回,直到有连接请求为止
    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_sock, &clnt_addr_size);
    if(clnt_sock == -1) {
        error_handling("accept() error");
    }

    // 传输数据
    write(clnt_sock, message, sizeof(message));
    close(clnt_sock);
    close(serv_sock);
    return 0;
}

void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}


