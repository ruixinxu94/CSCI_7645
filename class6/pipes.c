/**
* parent writes 100, 150, 200, 250 to a pipe in that order
 * Child reads them from the pipe and prnts them in taht order
 *
*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    int pipeDescriptor[2];
    int status, childStatus;
    int values[] = {100, 150, 200, 250};
    int value;

    int readValues[100]; // test if reader doesn't know how much size , items are there

    status = pipe(pipeDescriptor);

    if (status == -1) {
        printf("failed to create pipe");
        exit(EXIT_FAILURE);
    }

    /** At this point, pipeDescriptor[0]
        is the read descriptor, pipeDescriptor[1] is the write descriptor
     **/

    int childId = fork();
    switch (childId) {
        case -1: {
            printf("failed to create child process\n");
            exit(EXIT_FAILURE);
        }
        case 0: {
            /** read from the pipe until there is nothing left to read **/
            status = close(pipeDescriptor[1]);
            if (status == -1) {
                printf("parent failed to close write descriptor\n");
                _exit(EXIT_FAILURE);
            }

            int numRead = read(pipeDescriptor[0], &value, sizeof(int));
            while (numRead > 0) {
                printf("child read %d\n", value);
                numRead = read(pipeDescriptor[0], &value, sizeof(int));
            }
//            int numRead = read(pipeDescriptor[0], readValues, sizeof(int));
//            while (numRead > 0) {
//                // how many integres did i actually read?
//                int numValues = numRead / sizeof(int);
//                for (int i = 0; i < numValues; i++) {
//                    value = readValues[i];
//                    printf("child read %d\n", value);
//                }
//                numRead = read(pipeDescriptor[0], readValues, sizeof(int));
//            }
            if (numRead == -1) {
                printf("child failed to read from the pipe\n");
                _exit(EXIT_FAILURE);
            }
            _exit(EXIT_SUCCESS);
        }

        default: {
            /**
             * write the values to the pipe
             */

            /**
             * close the read descriptor
             */
            status = close(pipeDescriptor[0]);
            if (status == -1) {
                printf("parent failed to close read descriptor\n");
                exit(EXIT_FAILURE);
            }

            /**
             * write all the values at one time
             * if the reader doesn't know the size, we should parse with fixed size
             */
//            int numWritten = write(pipeDescriptor[1], &values, sizeof(values));
//            if (numWritten != sizeof(values)) {
//                printf("partial write happens");
//                exit(EXIT_FAILURE);
//            }

            for (int i = 0; i < 4; i++) {
                int numWritten = write(pipeDescriptor[1], &values[i], sizeof(int));
                if (numWritten != sizeof(int)) {
                    printf("partial write happens");
                    exit(EXIT_FAILURE);
                }
            }

            status = close(pipeDescriptor[1]);
            if (status == -1) {
                printf("parent failed to close write descriptor\n");
                exit(EXIT_FAILURE);
            }
            status = wait(&childStatus);
            if (status == -1) {
                printf("failed to wait the child to exit\n");
                exit(EXIT_FAILURE);
            }
            exit(EXIT_SUCCESS);
        }

    }
}
