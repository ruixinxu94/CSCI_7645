#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char **argv) {
    int values[5]; // Array to hold the read values
    int fifoDescriptor, numRead;
    int size = 5 * sizeof(int); // Size to read multiple integers
    fifoDescriptor = open("mysecondFifo", O_RDONLY); // Open in blocking mode

    if (fifoDescriptor == -1) {
        perror("failed to open the FIFO");
        exit(EXIT_FAILURE);
    }

    numRead = read(fifoDescriptor, &values, size);
    if (numRead == -1) {
        perror("failed to read");
        exit(EXIT_FAILURE);
    }

    if (numRead > 0) {
        for (int i = 0; i < numRead / sizeof(int); i++) {
            printf("%d\n", values[i]);
        }
    }

    int status = close(fifoDescriptor);
    if (status != 0) {
        perror("failed to close the fifo");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}