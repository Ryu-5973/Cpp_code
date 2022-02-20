#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define KEY     0x231f
#define SIZE    4096

int main(int argc, char *argv[]) {
    int shmid = shmget(KEY, SIZE, IPC_CREAT|0666);
    if(shmid == -1) { 
        perror("shmget");
        exit(1);
    }
    // key_t key = ftok("../", 123);
    char *shmaddr;

    pid_t pid = fork();
    if(pid == 0) {// 子进程
        shmaddr = (char*)shmat(shmid, NULL, 0);
        system("ipcs -m"); //查看共享内存
        if(shmaddr < 0){
            perror("shmat");
            exit(1);
        }

        printf("data = [%s]\n", shmaddr);
        if(shmdt(shmaddr) < 0){
            perror("shmdt");
            exit(1);
        }else {
            printf("deleted shared-memory\n");
        }
    
        //删除共享内存
        shmctl(shmid, IPC_RMID, NULL);
    
        system("ipcs -m"); //查看共享内存

    }else {// 父进程
        shmaddr = (char*)shmat(shmid, NULL, 0);
        if(shmaddr < 0){
            perror("shmat");
            exit(1);
        }
    
        //拷贝数据至共享内存区
        printf("copy data to shared-memory\n");
        bzero(shmaddr, SIZE); // 共享内存清空
        strcpy(shmaddr, "hello world");
        wait(0);
    }
    return 0;
}