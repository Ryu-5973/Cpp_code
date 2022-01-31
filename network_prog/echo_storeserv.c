// echo_storeserv.c
// 保存消息的回声服务器端

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "my_util.h"

#define BUF_SIZE 100
void read_childproc(int sig);

int main(int argc, char* argv[]) {
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    int fds[2];

    pid_t pid;
    struct sigaction act;
    socklen_t addr_sz;
    int str_len, state;
    char buf[BUF_SIZE];
    if(argc != 2) {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    // 信号处理函数(子进程销毁处理函数初始化)
    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    state = sigaction(SIGCHLD, &act, 0);

    // socket初始化
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);    // tcp
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    // 端口绑定
    if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        error_handling("bind() error");
    }
    // 开始监听
    if(listen(serv_sock, 5) == -1) {
        error_handling("listen() error");
    }

    pipe(fds);
    pid = fork();
    if(pid == 0) {  // 子进程
        FILE* fp = fopen("echomsg.txt", "wt");
        char msgbuf[BUF_SIZE];
        int i, len;

        for(i = 0; i < 10; ++ i) {
            len = read(fds[0], msgbuf, BUF_SIZE);
            fwrite((void*)msgbuf, 1, len, fp);
        }
        fclose(fp);
        return 0;
    }

    while(1) {
        addr_sz = sizeof(clnt_addr);
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &addr_sz);
        if(clnt_sock == -1) {
            // puts("accept nothing....");
            continue;
        }else {
            puts("new client connected...");
        }
        pid = fork();
        if(pid == -1) {
            close(clnt_sock);
            continue;
        }
        if(pid == 0) { // 子进程运行区域 
            close(serv_sock);
            pid_t my_pid = getpid();
            while((str_len = read(clnt_sock, buf, BUF_SIZE)) != 0) {
                printf("Childproc %d received: %s\n", my_pid, buf);
                write(fds[1], buf, str_len);
            }

            close(clnt_sock);
            puts("client disconnected...");
            return 0;
        }else { // 父进程断开clnt_sock引用
            close(clnt_sock);
        }
    }
    close(serv_sock);
    return 0;
} 

void read_childproc(int sig) {
    int status;
    pid_t pid = waitpid(-1, &status, WNOHANG);
    printf("Removed proc id: %d \n", pid);
}