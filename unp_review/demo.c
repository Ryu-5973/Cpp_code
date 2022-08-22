#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <arpa/inet.h>

int main() {
    // system("ps -a");
    // execl("./pipe", "pipe");
    // wait(0);
    // INADDR_NONE
    uint32_t res = inet_addr("255.255.255.255");
    if(res == INADDR_NONE) printf("false\n");
    // uint32_t res = inet_addr("0.0.0.1");
    printf("%x\n", res);
    // system("ps -a");
    return 0;
}