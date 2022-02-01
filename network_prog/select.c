// select.c
// select函数使用示例

// 使用select函数可以将多个文件描述符集中到一起统一监视,项目如下:
// 是否存在套接字接收数据?
// 无需阻塞传输数据的套接字有哪些?
// 哪些套接字发生了异常

// slect函数的调用方法和顺序
// 步骤一: 设置文件描述符,指定监视范围,设置超时
// 步骤二: 调用select函数
// 步骤三: 查看调用结果

// fd_set变量的注册和修改
// FD_ZERO(fd_set* fdset);
// FD_SET(int fd, fd_set* fdset);
// FD_CLR(int fd, fd_set* fdset);
// FD_ISSET(int fd, fd_set* fdset);

// select 函数
// #include <sys/select.h>
// #include <sys/time.h>
// int select(int maxfd, fd_set* readset, fd_set* writeset, 
//      fd_set* exceptset, const struct timeval* timeout);
// 成功时返回大于0的值,失败返回-1
// maxfd: 监视对象文件描述符数量
// readset 将所有关注"是否存在待读取数据"的文件描述符注册到fd_set型变量,并传递其地址值
// writeset 将所有关注"是否传输无阻塞数据"的文件描述符注册到fd_set型变量,并传递其地址值
// exceptset 将所有关注"是否发生异常"的文件描述符注册到fd_set型变量,并传递其地址值 
// timeout: 调用select函数后,为防止陷入无限阻塞状态,传递潮湿(time-out)信息.
// 返回值 返回大于0的值,该值i是发生事件的文件描述符数


#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 30

int main(int argc, char* argv[]) {
    fd_set reads, temps;
    int result, str_len;
    char buf[BUF_SIZE];
    struct timeval timeout;
    FD_ZERO(&reads);
    FD_SET(0, &reads);  // 文件描述符0是标准输入(终端)

    /* 不能在这里设置timeout,因为调用select函数后.结构体timeval
        的值会替换为超时前的剩余时间.因此,调用select后要重新初始化
    timeout.tv_sec = 5;
    timeout.tv_usec = 5000;
    */

    while(1) {
        temps = reads;
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;
        result = select(1, &temps, 0, 0, &timeout);
        if(result == -1) {
            puts("select() error!");
            break;
        }else if(result == 0) {
            puts("Time-out!");
        }else {
            if(FD_ISSET(0, &temps)) {
                str_len = read(0, buf, BUF_SIZE);
                buf[str_len] = 0;
                printf("message from console: %s", buf);
            }
        }
    }
    return 0;
}




