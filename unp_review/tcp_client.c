#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>



int main(int argc, char *argv[]) {
    if(argc != 3) {
        printf("Usage: %s [IP] [port]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // socket 
    int fd = socket(PF_INET, SOCK_STREAM, 0);
    if(fd == -1) {
        printf("socket failure\n");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof addr);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    addr.sin_addr.s_addr = inet_addr(argv[1]);

    // connect
    if(connect(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        printf("connect error\n");
        exit(EXIT_FAILURE);
    }else {
        puts("Connecting.........\n");
    }


    char buf[BUFSIZ];
    int str_len;
    while (1) {
        fputs("Input Message(Q to quit): ", stdout);
        fgets(buf, BUFSIZ, stdin);

        if(!strcmp(buf, "q\n") || !strcmp(buf, "Q\n")){
            break;
        }

        write(fd, buf, strlen(buf));
        str_len = read(fd, buf, BUFSIZ - 1);
        buf[str_len] = 0;
        printf("Message from server: %s", buf);
    }
    // close
    close(fd);
    exit(EXIT_SUCCESS);
}