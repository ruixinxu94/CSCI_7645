#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <mqueue.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int status;
    if (argc != 2 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
        printf("Usage: ./mqreader <message queue name>\n");
        return EXIT_FAILURE;
    }

    mqd_t mqd;
    struct mq_attr attr;
    int message;

    mqd = mq_open(argv[1], O_RDONLY);
    if (mqd == (mqd_t)-1) {
        printf("Failed to open message queue\n");
        return EXIT_FAILURE;
    }

    if (mq_getattr(mqd, &attr) == -1) {
        printf("Failed to get message queue attributes");
        mq_close(mqd);
        mq_unlink(argv[1]);
        return EXIT_FAILURE;
    }

    while (attr.mq_curmsgs > 0) {
        if (mq_receive(mqd, (char*)&message, attr.mq_msgsize, NULL) == -1) {
            printf("Failed to read from message queue\n");
            status = mq_close(mqd);
            if (status == -1) {
                printf("Failed to close message queue.\n");
                exit(EXIT_FAILURE);
            }
            mq_unlink(argv[1]);
            return EXIT_FAILURE;
        }

        printf("%d\n", message);
        attr.mq_curmsgs--;
    }

    status = mq_close(mqd);
    if (status == -1) {
        printf("Failed to close message queue.\n");
        exit(EXIT_FAILURE);
    }
    mq_unlink(argv[1]); // Remove the message queue

    return EXIT_SUCCESS;
}
