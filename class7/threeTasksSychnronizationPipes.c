#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUF_SIZE 1

void doTask(int childIndex, char *taskName) {
    printf("Child %d completed task: %s\n", childIndex, taskName);
}

int main(int argc, char *argv[]) {
    int pd[2]; // One pipe for all tasks
    pid_t child;
    int status, childStatus, index;
    char buffer[BUF_SIZE];
    int numRead;

    // Counters for signals received for each task
    int countT1 = 0, countT2 = 0, countT3 = 0;

    status = pipe(pd);
    if (status == -1) {
        printf("Could not open pipe.\n");
        exit(1);
    }

    for (index = 0; index < 2; index++) {
        child = fork();
        switch (child) {
            case -1:
                printf("Could not create child.\n");
                exit(1);

            case 0: // Child process
                close(pd[0]); // Close read end of the pipe

                doTask(index, "T1");
                write(pd[1], "1", BUF_SIZE); // Signal completion of T1

                doTask(index, "T2");
                write(pd[1], "2", BUF_SIZE); // Signal completion of T2

                doTask(index, "T3");
                write(pd[1], "3", BUF_SIZE); // Signal completion of T3

                close(pd[1]); // Close write end of the pipe
                exit(0);

            default:
                break;
        }
    }

    // Parent process
    close(pd[1]); // Close write end of the pipe

    // Read signals from the pipe and synchronize tasks
    while ((countT1 < 2) || (countT2 < 2) || (countT3 < 2)) {
        numRead = read(pd[0], buffer, BUF_SIZE);
        if (numRead > 0) {
            if (buffer[0] == '1') countT1++;
            else if (buffer[0] == '2') countT2++;
            else if (buffer[0] == '3') countT3++;
        }

        if (countT1 == 2) {
            printf("All children have completed their first task.\n");
            countT1++; // To prevent printing this message multiple times
        }

        if (countT2 == 2) {
            printf("All children have completed their second task.\n");
            countT2++;
        }

        if (countT3 == 2) {
            printf("All children have completed their third task.\n");
            countT3++;
        }
    }

    close(pd[0]); // Close read end of the pipe

    // Wait for child processes to exit
    for (index = 0; index < 2; index++) {
        wait(&childStatus);
    }

    printf("All children have completed their tasks and exited.\n");
    exit(EXIT_SUCCESS);
}
