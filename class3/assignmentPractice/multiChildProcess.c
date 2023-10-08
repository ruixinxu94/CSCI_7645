#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char ** argv) {
    int status, i;
    if (argc != 2 || strcmp(argv[1], "--help") == 0 || atoi(argv[1]) <= 0)  {
        printf("Usage: %s <n>\nwhere n must satisfy n > 0.\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    const int childProcessCount = atoi(argv[1]);

    for (i = 0; i < childProcessCount; i++) {
        int childPid = fork();
        switch (childPid) {
            case -1:
                printf("Failed to create child process\n");
                exit(EXIT_FAILURE);
            case 0:
                printf("\t|Parent process %d, created child process %d.\n", getppid(), getpid());
                sleep(1);
                printf("\t|Child process %d is complete.\n", getpid());
                printf("\t|----------------------------------------------------|\n");
                exit(EXIT_SUCCESS);
            default:
                wait(&status);
        }
    }

    printf("\t|Parent process %d is complete.\n", getpid());
    printf("\t|-------------------------END------------------------|\n");
    exit(EXIT_SUCCESS);
}
