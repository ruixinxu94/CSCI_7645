#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char **argv) {
    int pipeDescriptor[2];
    pid_t childId;
    int status;

    if (argc < 2 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
        printf("Usage: ./pipe <n1> <n2> <n3> ...\n");
        return EXIT_FAILURE;
    }
    status = pipe(pipeDescriptor);
    if (status == -1) {
        printf("Failed to create pipe\n");
        return EXIT_FAILURE;
    }

    childId = fork();
    switch (childId) {
        case -1:
            printf("Failed to fork\n");
            return EXIT_FAILURE;

        case 0:
            status = close(pipeDescriptor[1]);
            if (status== -1) {
                printf("Child failed to close write-end of the pipe\n");
                _exit(EXIT_FAILURE);
            }

            int number;
            int sum = 0;
            int numRead= read(pipeDescriptor[0], &number, sizeof(number));
            while (numRead > 0) {
                sum += number;
                numRead= read(pipeDescriptor[0], &number, sizeof(number));
            }

            printf("%d\n", sum);

            status = close(pipeDescriptor[0]);
            if (status == -1) {
                printf("Child failed to close read pipe\n");
                _exit(EXIT_FAILURE);
            }

            _exit(EXIT_SUCCESS);

        default:
            status = close(pipeDescriptor[0]);
            if (status == -1) {
                printf("Parent failed to close read pipe\n");
                return EXIT_FAILURE;
            }

            for (int i = 1; i < argc; i++) {
                int num = atoi(argv[i]);
                status = write(pipeDescriptor[1], &num, sizeof(num));
                if (status != sizeof(num)) {
                    printf("Parent failed to write to pipe\n");
                    exit(EXIT_FAILURE);
                }
            }

            if (close(pipeDescriptor[1]) == -1) {
                printf("Parent failed to close writepipe\n");
                exit(EXIT_FAILURE);
            }
            wait(NULL);

            exit(EXIT_SUCCESS);
    }
}
