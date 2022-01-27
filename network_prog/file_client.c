// file_client.c
// 基于半关闭的文件传输程序(客户端)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h> 
#include "my_util.h"

#define BUF_SIZE 30

int main(int argc, char *argv[]) {
    int sd;
    FILE *fp;

    char buf[BUF_SIZE];
    int read_cnt;
    struct sockaddr_in serv_addr;

    if(argc != 3) {
        printf("Usage : %s <IP> <port>", argv[0]);
        exit(1);
    }

    fp = fopen("receive.dat", "wb");
    sd = socket(PF_INET, SOCK_STREAM, 0);  // tcp

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    connect(sd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    while((read_cnt = read(sd, buf, BUF_SIZE)) != 0) {
        fwrite((void*)buf, 1, read_cnt, fp);
        // puts(buf);
    }
    puts("Received file data");
    write(sd, "Thank you", 10);
    fclose(fp);
    close(sd);
    return 0;
}
