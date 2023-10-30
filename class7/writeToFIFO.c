/*
 * write to fifo
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>



int main(int argc, char **argv) {
    int valuesToWrite[5] = {1 ,3, 5, 7,9};

    int fifoDescriptor, numWritten;
    int size = 5*sizeof(int);
    fifoDescriptor = open("mysecondFifo", O_RDWR);

    if (fifoDescriptor == -1) {
        printf("failed to open the FIFO. \n");
        exit(EXIT_FAILURE);
    }

    numWritten = write(fifoDescriptor, valuesToWrite, size);
    if (numWritten < size) {
        printf("partial write error. \n");
        exit(EXIT_FAILURE);
    }

    int status = close(fifoDescriptor);
    if (status != 0) {
        printf("failed to close the fifo. \n");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);

}