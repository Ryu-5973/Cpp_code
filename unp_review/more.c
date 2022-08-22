#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

int main(int argc, char *argv[]) {
    if(argc != 2) {
        printf("Usage: %s command\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    FILE *fd;
    if((fd = popen(argv[1], "r")) == NULL) {
        printf("call popen failed\n");
        exit(EXIT_FAILURE);
    }else {
        char buf[500];
        while (getchar()) {
            if(fgets(buf, 500, fd) != NULL) {
                printf("%s\n", buf);
            }else exit(EXIT_SUCCESS);
        }
    }
    exit(EXIT_SUCCESS);
}