// waitpid.c
// 使用waitpid函数销毁僵尸进程

/*
* #include<sys/wait.h>
* pid_t waitpid(pid_t pid, int* statloc, int options);
* pid 子进程id,传-1则和wait一样
* staloc 和wait具有相同意义
* 传递头文件sys/wait.h声明的常量WNOHANG,即使没有终止的子进程也不会进入阻塞状态,
* 而是返回0并退出函数
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    int status;
    pid_t pid = fork();

    if(pid == 0) {
        sleep(15);
        return 24;
    }else {
        while (!waitpid(-1, &status, WNOHANG)){
            sleep(3);
            puts("sleep 3sec.");
        }
        if(WIFEXITED(status)) {
            printf("Child send %d \n", WEXITSTATUS(status));
        }
    }
    return 0;
}