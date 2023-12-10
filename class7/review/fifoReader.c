#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <fcntl.h>
#include <signal.h>

char* FIFO_PATH = "myFifo";
// Cleanup function to remove FIFO
void cleanup() {
    printf("Cleaning up and removing FIFO.\n");
    unlink(FIFO_PATH);
}

// Signal handler for SIGINT
void signalHandler(int sig) {
    printf("Caught signal %d, cleaning up and exiting.\n", sig);
    exit(EXIT_SUCCESS);
}

int main(int argc, char **argv) {
    // Register cleanup function to be called at exit
    atexit(cleanup);

    // Set up signal handler for SIGINT
    signal(SIGINT, signalHandler);
    int values[5]; // Array to hold the read values
    int fifoDescriptor, numRead;
    int size = 5 * sizeof(int); // Size to read multiple integers
    fifoDescriptor = open(FIFO_PATH, O_RDONLY); // Open in blocking mode

    if (fifoDescriptor == -1) {
        printf("failed to open the FIFO\n");
        exit(EXIT_FAILURE);
    }

    numRead = read(fifoDescriptor, &values, size);
    if (numRead == -1) {
        printf("failed to read\n");
        exit(EXIT_FAILURE);
    }

    if (numRead > 0) {
        for (int i = 0; i < numRead / sizeof(int); i++) {
            printf("%d\n", values[i]);
        }
    }

    int status = close(fifoDescriptor);
    if (status != 0) {
        printf("failed to close the fifo\n");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}