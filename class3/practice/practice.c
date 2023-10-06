#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char **argv) {
    int childProcess, status;
    if (argc < 2 || strcmp(argv[1], "--help") == 0) {
        printf("Usage: %s <n>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    const int PROCESS_COUNT = atoi(argv[1]);
    if (PROCESS_COUNT <= 0) {
        printf("count must larger than zero\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < PROCESS_COUNT; i++) {
        childProcess = fork();
        switch (childProcess) {
            case 0:
                printf("parent started process %d\n", i + 1);
                sleep(1);
                printf("child %d ends with pid %d\n", i + 1, getpid());
                exit(EXIT_SUCCESS);
            case -1:
                printf("error happens when %d\n", i + 1);
                exit(EXIT_FAILURE);
            default:
                wait(&status);
        }
    }
    printf("parent process %d ends", getpid());
    exit(EXIT_SUCCESS);

}