#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>


/*
 * copy the contents of a source file to a destination file
 * Compile as:
 * cc copy.c -o copy
 * Run as:
 * ./copy <source> destination>
 * OR
 * ./cpy --help
 * OR
 * ./cpy --h
 */

int main(int argc, char **argv) {
    int sourceFd, destinationFd;
    if (argc != 3 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
        printf("-usage...");
        exit(EXIT_FAILURE);
    }
    /**Open the source file for reading
     *
     * man open
     * man 2 open
     * to check the open manual
     * / to search in manual
     */
    sourceFd = open(argv[1], O_RDONLY);
    if (sourceFd == -1) {
        printf("%s :no such file, the reading failed\n", argv[1]);
        exit(EXIT_FAILURE);
    }
    /**
     *
        O_CREAT | O_WRONLY: These are flags that determine how to open the file:

        O_CREAT: If the file does not exist, it will be created.
        O_WRONLY: Open the file for writing only.
        S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH:
        These are file permission flags that are used if the file is being created (because of the O_CREAT flag).
        They determine the permissions of the newly created file:

        S_IRUSR: Read permission for the owner.
        S_IWUSR: Write permission for the owner.
        S_IRGRP: Read permission for the group.
        S_IROTH: Read permission for others.
     */
    destinationFd = open(argv[2],
                         O_CREAT | O_WRONLY,
                         S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

    if (destinationFd == -1) {
        printf("%s :no such file, the oepning failed\n", argv[2]);
        exit(EXIT_FAILURE);
    }
    /**TODO
     * write code for copying contents
     */
    int status = close(sourceFd);
    if (status == 1) {
        printf("failed to close te source file");
        exit(EXIT_FAILURE);
    }
    status = close(destinationFd);
    if (status == 1) {
        printf("failed to close te destination file");
        exit(EXIT_FAILURE);
    }


}