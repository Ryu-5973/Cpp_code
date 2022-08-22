#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>

#include <limits.h>

#define FIFO_SERVER "./fifoserver"

int main(int argc, char *argv[]) {
    // PIPE_BUF
    int fd;
    char w_buf[4096 * 2];
    int real_wnum;
    memset(w_buf, 0, 4096 * 2);
    if((mkfifo(FIFO_SERVER, O_CREAT|O_EXCL) < 0) && (errno != EEXIST)) {
        printf("cannot create fifoserver\n");        
    }
    fd = open(FIFO_SERVER, O_WRONLY|O_NONBLOCK, 0); // 非阻塞
    // fd = open(FIFO_SERVER, O_WRONLY, 0); // 阻塞
    if(fd == -1 && errno == ENXIO) {
        printf("open error; no reading process\n");
    }
    real_wnum = write(fd, w_buf, 2048);
    if(real_wnum == -1) {
        if(errno == EAGAIN)printf("write to fifo error; try later\n");
    }else {
        printf("real write num is %d\n", real_wnum);

    }
    real_wnum = write(fd, w_buf, 8192);
    printf("real write num is %d\n", real_wnum);
    if(real_wnum == -1 && errno == EAGAIN) {
        printf("try later\n");
    }
} 