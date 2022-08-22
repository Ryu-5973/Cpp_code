#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include <unistd.h>

int main(int argc, char **argv) {
    int data_processed;
    int file_pipes[2];
    const char some_data[] = "123";
    char buffer[BUFSIZ + 1];
    int fork_result;
    memset(buffer, 0, sizeof(buffer));
    if(pipe(file_pipes) == 0) {
        fork_result = fork();
        if(fork_result == -1) {
            fprintf(stderr, "Fork failure");
            exit(EXIT_FAILURE);
        }
    }
    if(fork_result == 0) {
        data_processed = read(file_pipes[0], buffer, BUFSIZ);
        printf("Read %d bytes: %s\n", data_processed, buffer);
        exit(EXIT_SUCCESS);
    }else {
        data_processed = write(file_pipes[1], some_data, strlen(some_data));
        printf("Write %d bytes\n", data_processed);
        wait(0);
    }
    exit(EXIT_SUCCESS);
}