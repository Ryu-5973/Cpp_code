// wait.c
// 利用wait函数摧毁僵尸进程

/*
* #include <sys/wait.h>
* pid_t wait(int* staloc);
* WIFEXITED->子进程正常终止返回真
* WEXITSTATUS-返回子进程的返回值
*
* 调用wait函数事,如果没有已终止的子进程,那么程序将阻塞直到有子进程终止,因此需
* 谨慎调用该函数
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    int status;
    pid_t pid = fork();

    if(pid == 0) {
        return 3;
    }else {
        printf("Child PID: %d\n", pid);
        pid = fork();
        if(pid == 0) {
            exit(7);
        }else {
            printf("Child PID: %d \n", pid);
            wait(&status);
            if(WIFEXITED(status)) {
                printf("Child send one: %d\n", WEXITSTATUS(status));
            }
            wait(&status);
            if(WIFEXITED(status)) {
                printf("Child send two: %d\n", WEXITSTATUS(status));
            }
            sleep(30);
        }
    }
    return 0;
}