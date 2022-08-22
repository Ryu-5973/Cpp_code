#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <arpa/inet.h>

int main(int argc, char *argv[]) {
    if(argc != 2) {
        printf("Usage: %s [port]\n", argv[0]);
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
    addr.sin_port = htons(atoi(argv[1]));
    addr.sin_addr.s_addr = INADDR_ANY; 

    // bind
    if(bind(fd, (struct sockaddr*)&addr, sizeof addr) == -1) {
        printf("bind failure\n");
        exit(EXIT_FAILURE);
    }

    // listen
    if(listen(fd, 5) == -1) {
        printf("listen failure");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in clnt_addr;
    int clnt_fd;
    // accept
    // echo server(read & write) 
    char buf[BUFSIZ];
    int cnt = 0;
    socklen_t sock_len = sizeof clnt_addr;
    while (1){
        // accept
        clnt_fd = accept(fd, (struct sockaddr*)&clnt_addr, &sock_len);
        if(clnt_fd == -1) {
            printf("accept failure\n");
            exit(EXIT_FAILURE);
        }
        printf("Connect client %d\n", cnt ++);

        // read
        int str_len;
        while ((str_len = read(clnt_fd, buf, BUFSIZ)) != 0){
            // write
            write(clnt_fd, buf, str_len);
        }
        
        // close
        close(clnt_fd);
    }
    // close
    close(fd);
    exit(EXIT_SUCCESS);
}