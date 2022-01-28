// set_buf.c
// 更改套接字时默认的I/O缓冲大小

// SO_RCVBUF是输入缓冲大小相关可选项
// SO_SNDBUF是输出缓冲大小相关可选项

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include "my_util.h"    // error_handling

int main(int argc, char* argv[]) {
    int sock;
    int snd_buf = 1024 * 3, rcv_buf = 1024 * 3, state;
    socklen_t len;

    sock = socket(PF_INET, SOCK_STREAM, 0); // tcp
    state = setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void*)&rcv_buf, sizeof(rcv_buf));
    if(state) {
        error_handling("setsockopt() error!");
    }

    state = setsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void*)&snd_buf, sizeof(snd_buf));
    if(state) {
        error_handling("setsockopt() error!");
    }

    len = sizeof(snd_buf);
    state = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void*)&snd_buf, &len);
    if(state) {
        error_handling("getsockopt() error");
    }

    len = sizeof(rcv_buf);
    state = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void*)&rcv_buf, &len);
    if(state) {
        error_handling("getsockopt() error");
    }
    
    printf("Input buffer size: %d \n", rcv_buf);
    printf("Output buffer size: %d \n", snd_buf);
    return 0;

}