#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

typedef struct {
    char lastname[20];
    char firstname[20];
} Person;

int main() {
    mqd_t messageQueueDescriptor;
    struct mq_attr attributes;
    int status;
    int index;
    char* messageQueueName = "/MyFirstMessageQueue2";

    Person persons[5] = {
            {"Alice",  "A"},
            {"Bob",    "B"},
            {"Cathy",  "C"},
            {"Daniel", "D"},
            {"Ethan",  "E"}
    };

    // Define the attributes of the message queue
    attributes.mq_msgsize = sizeof(Person);
    attributes.mq_maxmsg = 5;

    // Create and open the message queue
    messageQueueDescriptor = mq_open(messageQueueName,
                                     O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR, &attributes);
    if (messageQueueDescriptor == (mqd_t) -1) {
        printf("Failed to create message queue");
        exit(EXIT_FAILURE);
    }

    // Write messages to the queue
    for (index = 0; index < 5; index++) {
        // fifo
//        status = mq_send(messageQueueDescriptor,
//                         (char *) &persons[index], sizeof(Person), 0);
        // lsfo
        status = mq_send(messageQueueDescriptor, (char *) &persons[index], sizeof(Person), index);
        if (status == -1) {
            printf("Failed to write to message queue");
            mq_close(messageQueueDescriptor);
            mq_unlink(messageQueueName);
            exit(EXIT_FAILURE);
        }
    }

    // Close the message queue
    status = mq_close(messageQueueDescriptor);
    if (status == -1) {
        printf("Failed to close message queue");
        mq_unlink(messageQueueName);
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
