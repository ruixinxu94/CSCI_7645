#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int status;
    pid_t p = getpid();  // Get the parent process ID
    int child_id = 0;
//    int child_pids[3];

    for (int i = 0; i < 3; ++i) {
        pid_t child_pid = fork();

//        if (child_pid == 0) {  // Child pr
//        socess

        switch (child_pid) {
            case (0) : {
                child_id = i + 1;
                switch (child_id) {
                    case 1:  // Child process x
                        printf("Child process x going to sleep for 10 seconds\n");
                        sleep(10);
                        printf("Exiting child process x\n");
                        exit(EXIT_SUCCESS);
                    case 2:  // Child process y
                        printf("Child process y going to sleep for 5 seconds\n");
                        sleep(5);
                        printf("Exiting child process y\n");
                        exit(EXIT_SUCCESS);
                    case 3:  // Child process z
                        printf("Child process z going to sleep for 2 seconds\n");
                        sleep(2);
                        printf("Exiting child process z\n");
                        exit(EXIT_SUCCESS);
                    default:
                        exit(EXIT_FAILURE);
                }
            }
                // Should never reach here
            case (-1): {
                perror("fork");
                exit(EXIT_FAILURE);
            }
            default: {  // Parent process
                char child_label = 'x' + i;
                waitpid(child_pid, &status, 0);
                printf("Parent process %d created child process %c\n", p, child_label);
            }
        }
    }
    exit(EXIT_SUCCESS);
}