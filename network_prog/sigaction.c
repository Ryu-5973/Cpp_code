// sigaction.c
// sigaction函数示例


/*
* #include <signal.h>
* int sigaction(int signo, const struct sigaction* act, struct sigaction* old_action);
* 成功时返回0, 失败时返回-1
* struct sigaction {
*   void (*sa_handler)(int);    // 信号处理函数
*   sigset_t sa_mask;   // 信号相关的选项(0)
*   int sa_flags;       // 信号相关的特性(0)
* }
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

int main(int argc, char* argv[]) {
    int i;
    struct sigaction act;
    act.sa_handler = timeout;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGALRM, &act, 0);
    alarm(2);

    for(i = 0; i < 3; ++ i) {
        puts("wait...");
        // 发生信号时将唤醒由于调用sleep函数而进入阻塞状态的进程
        // 而且一旦唤醒,就不会再进入睡眠状态
        sleep(100);
    }
    return 0;
}
