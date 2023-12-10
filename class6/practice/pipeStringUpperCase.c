
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>

void closePipes(int pipe1, int pipe2) {
    if (close(pipe1) == -1 || close(pipe2) == -1) {
        perror("Failed to close pipe descriptors");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: %s <string>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int requestPipe[2];
    int responsePipe[2];
    int childStatus;
    char *message = argv[1];

    if (pipe(responsePipe) == -1) {
        printf("Failed to create response pipe");
        exit(EXIT_FAILURE);
    }

    if (pipe(requestPipe) == -1) {
        printf("Failed to create request pipe");
        exit(EXIT_FAILURE);
    }

    int childId = fork();
    switch (childId) {
        case -1: {
            printf("Failed to create child process");
            exit(EXIT_FAILURE);
        }
        case 0: {  // Child Process
            closePipes(requestPipe[1], responsePipe[0]);

            char readChar;
            char string[1000];
            int numRead = read(requestPipe[0], &readChar, sizeof(char));
            int index = 0;

            while (numRead > 0) {
                if (readChar == '\0') {
                    break;
                }
                string[index++] = toupper(readChar);
                numRead = read(requestPipe[0], &readChar, sizeof(char));
            }

            if (numRead == -1) {
                perror("Child failed to read from request pipe");
                _exit(EXIT_FAILURE);
            }
            string[index] = '\0';  // Null-terminate the string

            write(responsePipe[1], string, index + 1);
            closePipes(requestPipe[0], responsePipe[1]);
            _exit(EXIT_SUCCESS);
        }

        default: {  // Parent Process
            closePipes(requestPipe[0], responsePipe[1]);

            int messageLength = strlen(message) + 1;
            write(requestPipe[1], message, messageLength);

            char buffer[1000];
            int index = 0;
            int numRead = read(responsePipe[0], &buffer[index], 1);
            while (numRead > 0) {
                if (buffer[index] == '\0') {
                    break;
                }
                index++;
                numRead = read(responsePipe[0], &buffer[index], 1);
            }

            if (numRead == -1) {
                printf("Parent failed to read from response pipe");
                exit(EXIT_FAILURE);
            }

            buffer[index] = '\0';
            printf("%s\n", buffer);

            closePipes(requestPipe[1], responsePipe[0]);

            if (wait(&childStatus) == -1) {
                printf("Failed to wait for child to exit");
                exit(EXIT_FAILURE);
            }
            exit(EXIT_SUCCESS);
        }
    }
}