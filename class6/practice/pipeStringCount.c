/**
 * Implement a program in which a parent process writes a string of any length (up to a maximum) to a
 * pipe, and a child process reads that string from the pipe and prints out the length of the string. Test
 * this program out with the following strings:
 * (a) Hello World! The length should be 12.
 * (b) Linux Programming The length should be 17.
 *
*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("usage <>...");
        exit(EXIT_FAILURE);
    }
    int pipeDescriptor[2];
    int status, childStatus;
    char *message = argv[1];

    status = pipe(pipeDescriptor);

    if (status == -1) {
        printf("failed to create pipe");
        exit(EXIT_FAILURE);
    }

    int childId = fork();
    switch (childId) {
        case -1: {
            printf("failed to create child process\n");
            exit(EXIT_FAILURE);
        }
        case 0: {
            status = close(pipeDescriptor[1]);
            if (status == -1) {
                printf("parent failed to close write descriptor\n");
                _exit(EXIT_FAILURE);
            }

            char readChar;
            char string[1000];
            int numRead = read(pipeDescriptor[0], &readChar, sizeof(char));
            int index = 0;
            while (numRead > 0) {
                if (readChar == '\0') {
                    break;
                }
                string[index++] = readChar;
                numRead = read(pipeDescriptor[0], &readChar, sizeof(char));
            }

            if (numRead == -1) {
                printf("child failed to read from the pipe\n");
                _exit(EXIT_FAILURE);
            }
            printf("%s The length should be %d.\n", string, index);
            _exit(EXIT_SUCCESS);
        }

        default: {

            status = close(pipeDescriptor[0]);
            if (status == -1) {
                printf("parent failed to close read descriptor\n");
                exit(EXIT_FAILURE);
            }

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
