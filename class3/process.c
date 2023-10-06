#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
/** exitv
 * atexit
 *
 */

void printBye() {
    printf("Process %d says bye bye\n", getpid());
};
int main(int agc, char **argv) {
    int childId, status;
    /**
     *
     */
    atexit(printBye);
    /**
     * create the child
     */

    childId = fork();

    switch (childId) {
        case -1:
            printf("failed to create a child process\n");
            exit(EXIT_FAILURE);
        case 0:
            printf("I am the child process with id %d, pccparent id is %d\n", getpid(), getppid());
            sleep(3);
            printf("child is awake again\n");
            _exit(EXIT_SUCCESS);
        default:
            printf("I am the parent process with id %d, child id is %d\n", getpid(), childId);
            /**
             *
             */
            wait(&status);
            printf("child exited with status %d\n", status);
            exit(EXIT_SUCCESS);
    }

}