// zombie.c
// 创建僵尸进程

/*
* 僵尸进程:进程完成工作后(执行完main函数中的程序后)应被销毁,但有时这些进程将
* 变成僵尸进程,占用系统中的重要资源.向exit函数传递的参数值和main函数的return
* 语句返回值都会传递给操作系统.而操作系统不会销毁子进程,直到把这些值传递给产生
* 该子进程的父进程.处在这种状态下的进程就是僵尸进程
*/

#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    pid_t pid = fork();
    if(pid == 0) {  // id child process
        puts("Hi, I am a child process");
    }else {
        printf("Child Process ID: %d \n", pid);
        sleep(30);
    }
    if(pid == 0) {
        puts("End child process");
    }else {
        puts("End parent process");
    }
    return 0;
}