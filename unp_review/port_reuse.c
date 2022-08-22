#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>

#define PORT 9000

int main() {
    pid_t pid = fork();
    if(pid == -1) exit(EXIT_FAILURE);
    if(pid == 0) {
        int fd = socket(PF_INET, SOCK_STREAM, 0);
        int opt =1;
        setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const void *)&opt, sizeof(opt));
        struct sockaddr_in addr;
        memset(&addr, 0, sizeof addr);
        addr.sin_family = AF_INET;
        addr.sin_port = htons(PORT);
        addr.sin_addr.s_addr = INADDR_ANY;
        if(bind(fd, (struct sockaddr*)&addr, sizeof addr) == -1) {
            printf ("parent bind failure\n");
            exit(EXIT_FAILURE);
        }else printf("parent bind\n");
        if(listen(fd, 5) == -1) {
            printf ("parent listen failure\n");
            exit(EXIT_FAILURE);
        }else printf("parent listen\n");
        while (1){}
        wait(NULL);
    }else { // child
        int fd = socket(PF_INET, SOCK_DGRAM, 0);
        int opt = 1;
        setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const void *)&opt, sizeof(opt));
        struct sockaddr_in addr;
        memset(&addr, 0, sizeof addr);
        addr.sin_family = AF_INET;
        addr.sin_port = htons(PORT);
        addr.sin_addr.s_addr = INADDR_ANY;
        if(bind(fd, (struct sockaddr*)&addr, sizeof addr) == -1) {
            printf ("child bind failure\n");
            exit(EXIT_FAILURE);
        }else printf("child bind\n");
        while (1){}
    }
    return 0;
}