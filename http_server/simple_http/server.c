/*
 * @Author: Ryu-59073
 * @Date: 2022-07-23 18:34:37
 * @LastEditors: Ryu-59073
 * @LastEditTime: 2022-07-23 23:28:59
 * @FilePath: /Cpp_code/http_server/simple_http/server.c
 * @Description: 一个极其简单的HTTP服务器，C语言实现
 * 
 * Copyright (c) 2022 by Ryu-59073, All Rights Reserved. 
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include <fcntl.h>

#define CONNMAX 1000
#define BYTES   1024

char *ROOT;
int listenfd, clients[CONNMAX];
void error(char*);
void startServer(char*);
void respond(int);

int main(int argc, char *argv[]) {
    struct sockaddr_in clientaddr;
    socklen_t addrlen;
    char c;

    // Default values PATH=~/ and PORT=10000
    char PORT[6];
    ROOT = getenv("PWD");
    strcpy(PORT, "10000");

    int slot = 0;

    // Parsing the command line arguments
    while((c = getopt(argc, argv, "p:r:")) != -1) {
        switch (c) {
        case 'r':
            ROOT = malloc(strlen(optarg));
            strcpy(ROOT, optarg);
            break;
        case 'p':
            strcpy(ROOT, optarg);
            break;
        case '?':
            fprintf(stderr, "Wrong arguments given!!!\n");
            exit(1);
        default:
            exit(1);
        }
    }

    printf("Server started at port no. %s%s%s with root directory as %s%s%s\n", "\033[92m", PORT, "\033[0m", "\033[92m", ROOT, "\033[0m");

    // Setting all elements to -1, sognofies there is no client connected
    for(int i = 0; i < CONNMAX; ++ i) {
        clients[i] = -1;
    }
    startServer(PORT);


    // ACCEPT connection
    while(1) {
        addrlen = sizeof(clientaddr);
        clients[slot] = accept(listenfd, (struct sockaddr*) &clientaddr, &addrlen);

        if(clients[slot] < 0) {
            error("accept() error");
        }else {
            if(fork() == 0) {
                respond(slot);
                exit(0);
            }
        }
        while(clients[slot] != -1) slot = (slot + 1) % CONNMAX;
    }
    return 0;
}

// start Server
void startServer(char *port) {
    struct addrinfo hints, *res, *p;

    // getaddrinfo for host;
    bzero(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if(getaddrinfo(NULL, port, &hints, &res) != 0) {
        perror("getaddrinfo() error");
        exit(1);
    }
    // socket and bind
    for(p = res; p != NULL; p = p->ai_next) {
        listenfd = socket(p->ai_family, p->ai_socktype, 0);
        if(listenfd == -1) continue;
        if(bind(listenfd, p->ai_addr, p->ai_addrlen) == 0) break;
    }
    if(p == NULL) {
        perror("socket() or bind()");
        exit(1);
    }

    freeaddrinfo(res);

    // listen for incoming connection
    if(listen(listenfd, 1000000) != 0) {
        perror("listen() error");
        exit(1);
    }
}

// client connection
void respond(int n) {
    char mesg[1<<16], *reqline[3], data_to_send[BYTES], path[1<<16];
    int rcvd, fd, bytes_read;

    bzero(&mesg, 1<<16);

    rcvd = recv(clients[n], mesg, 1<<16, 0);

    if(rcvd < 0) {
        fprintf(stderr, "recv() error\n");
    }else if(rcvd == 0) {
        fprintf(stderr, "Client disconnected unexpectedly.\n");
    }else {
        printf("%s", mesg);
        reqline[0] = strtok(mesg, " ");
        printf("%s\n", reqline[0]);
        if(strncmp(reqline[0], "GET\0", 4) == 0) {
            printf("GET!\n");
            reqline[1] = strtok(NULL, " ");
            reqline[2] = strtok(NULL, " \t\n");
            printf("%s %s\n", reqline[1], reqline[2]);
            if(strncmp(reqline[2], "HTTP/1.0", 8) != 0 && strncmp(reqline[2], "HTTP/1.1", 8) != 0) {
                write(clients[n], "HTTP/1.0 400 Bad Request\n", 25);
            }else {
                if(strncmp(reqline[1], "/\0", 2) == 0) {
                    reqline[1] = "/index.html";
                }
                strcpy(path, ROOT);
                strcpy(&path[strlen(ROOT)], reqline[1]);
                printf("file: %s\n", path);

                if((fd = open(path, O_RDONLY)) != -1) {
                    send(clients[n], "HTTP/1.0 200 OK\n\n", 17, 0);
                    while((bytes_read = read(fd, data_to_send, BYTES)) > 0) {
                        write(clients[n], data_to_send, bytes_read);
                    }
                }else write(clients[n], "HTTP/1.0 404 Not Found\n", 23);
            }
        }else {
            char *s = "GET\0";
            printf("test\n");
            for(int i = 0; i < 4; ++ i) {
                printf("%d %d\n", reqline[0][i], s[i]);
            }
            printf("%s\n", reqline[0]);
            printf("%s\n", s);
        }
    }

    // Closing SOCKET
    shutdown(clients[n], SHUT_RDWR);
    close(clients[n]);
    clients[n] = -1;
}
