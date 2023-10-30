/*
 * create a new fifo
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


int main(int argc, char **argv) {
    int status;

    // S_IXUSR, S_IXGRP, S_IXOTHER should never be here for security
    status = mkfifo("mysecondFifo", S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);

    if (status == -1) {
        printf("failed to create the FIFO. \n");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);

}