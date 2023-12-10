/**
* parent writes 100, 150, 200, 250 to a pipe in that order
 * Child reads them from the pipe and prnts them in taht order
 *
*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<sys/wait.h>

int main(int argc, char **argv) {
    int pipeDescriptor[2];
    int status, childStatus;
    char value;
    char message[100];
    strcpy(message, "hello world");

    status = pipe(pipeDescriptor);

    if (status == -1) {
        printf("failed to create pipe");
        exit(EXIT_FAILURE);
    }

    /** At this point, pipeDescriptor[0]
        is the read descriptor, pipeDescriptor[1] is the write descriptor
     **/

    int childId = fork();
    switch (childId) {
        case -1: {
            printf("failed to create child process\n");
            exit(EXIT_FAILURE);
        }
        case 0: {
            /** read from the pipe until there is nothing left to read **/
            status = close(pipeDescriptor[1]);
            if (status == -1) {
                printf("parent failed to close write descriptor\n");
                _exit(EXIT_FAILURE);
            }

            char readChar;
            int numRead = read(pipeDescriptor[0], &readChar, sizeof(char));
            while (numRead > 0) {
                if (readChar == '\0') {
                    printf("\n");
                    break;
                }
                printf("%c", readChar);
                numRead = read(pipeDescriptor[0], &readChar, sizeof(char));
            }

            if (numRead == -1) {
                printf("child failed to read from the pipe\n");
                _exit(EXIT_FAILURE);
            }
            _exit(EXIT_SUCCESS);
        }

        default: {
            /**
             * write the values to the pipe
             */

            /**
             * close the read descriptor
             */
            status = close(pipeDescriptor[0]);
            if (status == -1) {
                printf("parent failed to close read descriptor\n");
                exit(EXIT_FAILURE);
            }

            /**
             * write all the values at one time
             * if the reader doesn't know the size, we should parse with fixed size
             * need the \n
             */
            int messageLength = strlen(message) + 1;
            int numWritten = write(pipeDescriptor[1], message, messageLength);
            if (numWritten != messageLength) {
                printf("partial write happens");
                exit(EXIT_FAILURE);
            }


            status = close(pipeDescriptor[1]);
            if (status == -1) {
                printf("parent failed to close write descriptor\n");
                exit(EXIT_FAILURE);
            }
            status = wait(&childStatus);
            if (status == -1) {
                printf("failed to wait the child to exit\n");
                exit(EXIT_FAILURE);
            }
            exit(EXIT_SUCCESS);
        }

    }
}
