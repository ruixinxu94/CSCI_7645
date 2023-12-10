//#include <stdlib.h>
//#include <string.h>
//#include <stdio.h>
//#include <fcntl.h>
//#include <unistd.h>
//
///*
// * chapter 4 or 5
// */
///*
// * copy the contents of a source file to a destination file
// * Compile as:
// * cc copy.c -o copy
// * Run as:
// * ./copy <source> destination>
// * OR
// * ./cpy --help
// * OR
// * ./cpy --h
// */
//
///*
// * 'ln' file linking
// * ln textoutput.txt myFolder/textoutput.txt
// * this is called hard link, when the reference become 0, the created file gone
// *
// * symbolic link
// * ln -s textoutput.txt myFolder/
// *
//
// */
//
//
//#define CHUNK_SIZE 1024
//
//int main(int argc, char **argv) {
//    int sourceFd, destinationFd;
//    char buffer[CHUNK_SIZE];    // the c use ASCII while java using  the unicode, in c just a byte array
//    int numRead, numWritten;
//
//
//    if (argc != 3 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
//        printf("-usage...");
//        exit(EXIT_FAILURE);
//    }
//    /**Open the source file for reading */
//    sourceFd = open(argv[1], O_RDONLY);
//    if (sourceFd == -1) {
//        printf("%s :no such file, the reading failed\n", argv[1]);
//        exit(EXIT_FAILURE);
//    }
//    /**
//     *
//        O_CREAT | O_WRONLY: These are flags that determine how to open the file:
//
//        O_CREAT: If the file does not exist, it will be created.
//        O_WRONLY: Open the file for writing only.
//        S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH:
//        These are file permission flags that are used if the file is being created (because of the O_CREAT flag).
//        They determine the permissions of the newly created file:
//
//        S_IRUSR: Read permission for the owner.
//        S_IWUSR: Write permission for the owner.
//        S_IRGRP: Read permission for the group.
//        S_IROTH: Read permission for others.
//     */
//     /**
//      * rw- r-- r--
//      * 110  100  100
//      * 6   4     4
//      * and the command is chmod 644
//      * 111 111 101
//      * 7 7 5
//      *
//      */
//      // copy
//    destinationFd = open(argv[2],
//                         O_CREAT | O_WRONLY,
//                         S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
//
//    /*
//     * append something other than override
//     */
////    destinationFd = open(argv[2],
////                         O_CREAT | O_WRONLY | O_APPEND,
////                         S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
//    // exclued create
////    destinationFd = open(argv[2],
////                         O_CREAT | O_WRONLY | O_EXCL,
////                         S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
//
//    if (destinationFd == -1) {
//        printf("%s :no such file, the openning failed\n", argv[2]);
//        exit(EXIT_FAILURE);
//    }
//    /**
//     * write code for copying contents
//     */
//    /**
//     * Read the first chunk
//     */
//    numRead = read(sourceFd,  buffer, CHUNK_SIZE);
//    while (numRead > 0) {
//        /**
//         * write that chunk of data
//         * numRead = read(sourceFd, buffer, CHUNK_SIZE); attempts to read up to CHUNK_SIZE
//         * bytes from sourceFd into buffer.
//T         he actual number of bytes read is stored in numRead. This number can be less than CHUNK_SIZE
//         for various reasons,
//         such as reaching the end of the file.
//         */
//         numWritten = write(destinationFd, buffer, numRead); //  should only write the byte,
//         // not the chunck size
//         /**
//          * check fo partial write error
//          */
//          if (numWritten < numRead) {
//              printf("Partial write error .\n");
//              exit(EXIT_FAILURE);
//          }
//          numRead = read(sourceFd, buffer, CHUNK_SIZE);
//          /**
//           * check for failed read
//           */
//           if (numRead == -1) {
//               printf("Read error .\n");
//               exit(EXIT_FAILURE);
//           }
//    }
//
//    int status = close(sourceFd);
//    if (status == 1) {
//        printf("failed to close te source file");
//        exit(EXIT_FAILURE);
//    }
//    status = close(destinationFd);
//    if (status == 1) {
//        printf("failed to close te destination file");
//        exit(EXIT_FAILURE);
//    }
//
//
//}