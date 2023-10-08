
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

const char *FILE_OUTPUT_PATH = "dest.txt";
#define CHUNK_SIZE 1024

int catFiles(int argc, char **argv) {
    int status;
    int sourceFd, destinationFd;
    char buffer[CHUNK_SIZE];  // C uses ASCII while Java uses Unicode; in C, just a byte array

    int fileSize = argc - 1;

    // Open the destination file once, before entering the loop
    destinationFd = open(FILE_OUTPUT_PATH, O_WRONLY | O_APPEND | O_CREAT,
                         S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (destinationFd == -1) {
        printf("failed to open or create file %s\n", FILE_OUTPUT_PATH);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < fileSize; i++) {
        int numRead, numWritten;
        sourceFd = open(argv[i + 1], O_RDONLY);
        if (sourceFd == -1) {
            printf("failed to open file %s\n", argv[i + 1]);
            // Consider closing destinationFd before exiting
            close(destinationFd);
            exit(EXIT_FAILURE);
        }
        numRead = read(sourceFd, buffer, CHUNK_SIZE);
        while (numRead > 0) {
            numWritten = write(destinationFd, buffer, numRead);
            if (numWritten == -1) {
                perror("write");
                close(sourceFd);
                close(destinationFd);
                exit(EXIT_FAILURE);
            } else if (numWritten < numRead) {
                printf("partial write happens\n");
                close(sourceFd);
                close(destinationFd);
                exit(EXIT_FAILURE);
            }
            numRead = read(sourceFd, buffer, CHUNK_SIZE);
        }

        if (numRead == -1) {
            printf("failed to read file %s\n", argv[i + 1]);
            // Consider closing sourceFd and destinationFd before exiting
            close(sourceFd);
            close(destinationFd);
            exit(EXIT_FAILURE);
        }

        status = close(sourceFd);
        if (status == -1) {
            printf("failed to close file\n");
            close(destinationFd);
            exit(EXIT_FAILURE);
        }

    }

    status = close(destinationFd);
    if (status == -1) {
        printf("failed to close file\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}

int main(int argc, char **argv) {
    if (argc < 2 || strcmp(argv[1], "--help") == 0) {  // Changed argc < 1 to argc < 2
        printf("<usage>...\n");
        exit(EXIT_FAILURE);
    }
    return catFiles(argc, argv);
}