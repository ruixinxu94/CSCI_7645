#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int createChildProcesses(int processSize) {
    int status, childPid;
    for (int i = 0; i < processSize; i++) {
        childPid = fork();
        switch (childPid) {
            case 0:
                printf("Child process is complete.\n");
                exit(EXIT_SUCCESS);
            case -1:
                printf("error happens when execute process %d\n", i + 1);
                exit(EXIT_FAILURE);
            default:
                wait(&status);
        }
    }
    printf("Parent process is complete.\n");
    exit(EXIT_SUCCESS);
}

int main(int argc, char **argv) {
    if (argc < 2 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
        printf("Usage: ./processes <n>\n");
        exit(EXIT_FAILURE);
    }
    int processSize = atoi(argv[1]);
    if (processSize <= 0) {
        printf("Usage: ./processes: process size must be larger than zero\n");
        exit(EXIT_FAILURE);
    }
    return createChildProcesses(processSize);
}