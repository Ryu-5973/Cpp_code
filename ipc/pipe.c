#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    int fd[2];
    if(pipe(fd) == -1) {
        printf("create pipe error");
        return 1;
    }
    pid_t pid = fork();
    if(pid == 0) {// 子进程
        pid = fork();
        if(pid == 0) {  // 孙进程
            close(fd[0]);
            write(fd[1], "Hello World", 12);
        }else {
            close(fd[0]);
            close(fd[1]);
            wait(0);
        }
    }else { // 父进程
        close(fd[1]);
        char buf[20];
        read(fd[0], buf, 12);
        printf("%s\n", buf);
        wait(0);
    }
    return 0;
}