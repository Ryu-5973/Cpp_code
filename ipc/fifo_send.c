#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int myfifo = mkfifo("myfifo", __S_IFIFO|0666);
    if(myfifo == -1) {
        perror("mkfifo error");
        exit(1);
    }
    int fd = open("./myfifo", O_RDWR);
    if(fd == -1) {
        perror("open error");
        exit(1);
    }
    char buf[1024];
    while(1) {
        printf("fifo_send# ");
        fflush(stdout);
        ssize_t s = read(0, buf, sizeof(buf) - 1);
        if(s > 0) {
            buf[s - 1] = 0;
            if(write(fd, buf, s) == -1) {
                perror("write error");
                exit(1);
            }
        }
    }
    close(fd);
    return 0;
}