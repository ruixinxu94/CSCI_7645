#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <mqueue.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int status;
    if (argc != 2 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
        printf("Usage: ./mqwriter <message queue name>\n");
        return EXIT_FAILURE;
    }

    mqd_t mqd;
    struct mq_attr attr;
    int message;

    // Set default attributes
    struct mq_attr default_attr;
    default_attr.mq_flags = 0; // Blocking read/write
    default_attr.mq_maxmsg = 10; // Maximum number of messages
    default_attr.mq_msgsize = sizeof(int); // Size of each message
    default_attr.mq_curmsgs = 0; // Number of messages currently in the queue

    mqd = mq_open(argv[1], O_RDONLY | O_CREAT, S_IRUSR | S_IWUSR, &default_attr);
    if (mqd == (mqd_t)-1) {
        printf("Failed to open or create message queue\n");
        return EXIT_FAILURE;
    }

    if (mq_getattr(mqd, &attr) == -1) {
        printf("Failed to get message queue attributes\n");
        mq_close(mqd);
        return EXIT_FAILURE;
    }

    while (attr.mq_curmsgs > 0) {
        if (mq_receive(mqd, (char*)&message, attr.mq_msgsize, NULL) == -1) {
            printf("Failed to read from message queue\n");
            status = mq_close(mqd);
            if (status == -1) {
                printf("Failed to close message queue\n");
                exit(EXIT_FAILURE);
            }
            return EXIT_FAILURE;
        }

        printf("%d\n", message);
        attr.mq_curmsgs--;
    }

    status = mq_close(mqd);
    if (status == -1) {
        printf("Failed to close message queue\n");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
