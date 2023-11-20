#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define FIFO_PATH "myfifo"


void removeFifo() {
    int status = unlink(FIFO_PATH);
    if (status == -1) {
        printf("ailed to remove request FIFO.\n");
    }
}
int main(int argc, char **argv) {
    pid_t childId;
    int status;

    if (argc < 2 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
        printf("Usage: ./fifo_example <n1> <n2> <n3> ...\n");
        return EXIT_FAILURE;
    }
    status = atexit(removeFifo);
    if (status != 0) {
        printf("unable to register as a handler");
        exit(EXIT_FAILURE);
    }
    // Create FIFO
    status = mkfifo(FIFO_PATH, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
    if (status == -1) {
        printf("Failed to create FIFO");
        return EXIT_FAILURE;
    }

    childId = fork();
    switch (childId) {
        case -1:
            printf("Failed to fork");
            return EXIT_FAILURE;

        case 0: { // Child process
            int fifoRead = open(FIFO_PATH, O_RDONLY);
            if (fifoRead == -1) {
                printf("Child failed to open FIFO for reading");
                _exit(EXIT_FAILURE);
            }
            /**
             * read and calculate
             */
            int number, sum = 0, numRead;
            numRead = read(fifoRead, &number, sizeof(number));
            while (numRead > 0) {
                sum += number;
                numRead = read(fifoRead, &number, sizeof(number));
            }

            printf("%d\n", sum);
            status = close(fifoRead);
            if (status != 0) {
                printf("failed to close read fifo");
                _exit(EXIT_FAILURE);
            }
            _exit(EXIT_SUCCESS);
        }

        default: { // Parent process
            int fifoWrite = open(FIFO_PATH, O_WRONLY);
            if (fifoWrite == -1) {
                printf("Parent failed to open FIFO for writing");
                return EXIT_FAILURE;
            }

            for (int i = 1; i < argc; i++) {
                int num = atoi(argv[i]);
                int writtenSize = write(fifoWrite, &num, sizeof(num));
                if (writtenSize != sizeof(num)) {
                    printf("Parent failed to write to FIFO");
                    exit(EXIT_FAILURE);
                }
            }
            status = close(fifoWrite);
            if (status != 0) {
                printf("failed to close read fifo");
                _exit(EXIT_FAILURE);
            }
            wait(NULL);

            exit(EXIT_SUCCESS);
        }
    }
}
