#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>


typedef struct {
    char lastname[20];
    char firstname[20];
    int count;
} Person;

int main(int argc, char *argv[]) {
    int status, index, numRead;
    mqd_t messageQueueDescriptor;
    struct mq_attr attributes;
    unsigned priority;
    Person person;
    char* messageQueueName = "/MyFirstMessageQueue2";

    messageQueueDescriptor = mq_open(messageQueueName, O_RDONLY | O_NONBLOCK);


    if (messageQueueDescriptor == (mqd_t) -1) {
        printf("Failed to open message queue.\n");
        exit(EXIT_FAILURE);
    }

    status = mq_getattr(messageQueueDescriptor, &attributes);
    printf("Maximum size of a message = %ld.\n", attributes.mq_msgsize);
    printf("Maximum number of messages = %ld.\n", attributes.mq_maxmsg);
    printf("Current number of messages = %ld.\n", attributes.mq_curmsgs);

    numRead = mq_receive(
            messageQueueDescriptor,
            (char *) &person,
            sizeof(Person),
            &priority
    );

    while (numRead > 0) {
        printf(
                "firstname = %s, lastname = %s, priority = %u.\n",
                person.firstname,
                person.lastname,
                priority
        );

        numRead = mq_receive(
                messageQueueDescriptor,
                (char *) &person,
                sizeof(Person),
                &priority
        );
    }

    if (numRead == -1) {
        printf("Failed to read from message queue.\n");
        exit(EXIT_FAILURE);
    }

    status = mq_close(messageQueueDescriptor);
    if (status == -1) {
        printf("Failed to close message queue.\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
