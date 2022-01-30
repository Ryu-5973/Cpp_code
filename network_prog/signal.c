// signal.c
// 信号处理实例

/*
* #include <signal.h>
* void (*signal(int signo, void (*func)(int)))(int);
* 为了在产生信号时调用,返回之前注册的函数指针
* 函数名: signal
* 参数: int signo, void(*func)(int)
* 返回类型:参数为int,返回void型函数指针
*
* 
* SIGALRM: 已到通过调用alarm函数注册的时间
* SIGINT: 输入CTRL+C
* SIGCHLD: 子进程终止
*
* #include<unistd.h>
* unsigned int alarm(unsigned int seconds);
* 返回0或以秒为单位的距SIGALRM信号发生的时间
* second传0取消SIGALRM信号的预约
*/

#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void timeout(int sig) {
    if(sig == SIGALRM) {
        puts("Time out!");
    }
    alarm(2);
}
void keycontrol(int sig) {
    if(sig == SIGINT) {
        puts("CTRL+C pressed");
    }
}

int main(int argc, char* argv[]) {
    int i;
    signal(SIGALRM, timeout);
    signal(SIGINT, keycontrol);
    alarm(2);

    for(i = 0; i < 3; ++ i) {
        puts("wait...");
        // 发生信号时将唤醒由于调用sleep函数而进入阻塞状态的进程
        // 而且一旦唤醒,就不会再进入睡眠状态
        sleep(100);
    }
    return 0;
}
