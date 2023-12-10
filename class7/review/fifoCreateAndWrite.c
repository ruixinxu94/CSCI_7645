#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char **argv) {
    int status;

    char* fifoName = "myFifo";
    status = mkfifo(fifoName, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);

    if (status == -1) {
        printf("Failed to create the FIFO \n");
        exit(EXIT_FAILURE);
    }

    // Values to write to FIFO
    int valuesToWrite[5] = {1, 3, 5, 7, 9};

    // Open the FIFO for writing
    int fifoDescriptor = open(fifoName, O_WRONLY);

    if (fifoDescriptor == -1) {
        printf("Failed to open the FIFO \n");
        exit(EXIT_FAILURE);
    }

    // Write to FIFO
    int size = sizeof(valuesToWrite);
    int numWritten = write(fifoDescriptor, valuesToWrite, size);

    if (numWritten < size) {
        printf("Partial write error \n");
        close(fifoDescriptor);
        exit(EXIT_FAILURE);
    }

    // Close the FIFO descriptor
    status = close(fifoDescriptor);
    if (status != 0) {
        printf("Failed to close the FIFO \n");
        exit(EXIT_FAILURE);
    }

    printf("Data written to FIFO successfully.\n");
    exit(EXIT_SUCCESS);
}
