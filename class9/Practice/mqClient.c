#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

typedef struct {
    pid_t clientID;
    float nums[10];
} Request;

typedef struct {
    float result;
} Response;

#define REQUEST_MQ_NAME "/request_mq"
#define CLIENT_MQ_TEMPLATE "/response_mq_%d"
#define TEMPLATE_NAME_MAX_LENGTH 100

int main() {
    mqd_t mqRequestDescriptor, mqResponseDescriptor;
    Request request;
    Response response;
    char client_mq_name[TEMPLATE_NAME_MAX_LENGTH];
    struct mq_attr resp_attr;
    int status;

    srand(time(NULL) ^ getpid());  // Seed the random number generator

    request.clientID = getpid();
    for (int i = 0; i < 10; i++) {
        request.nums[i] = rand() % 2;  // Generate either 0 or 1
    }

    snprintf(client_mq_name, TEMPLATE_NAME_MAX_LENGTH, CLIENT_MQ_TEMPLATE, getpid());
    resp_attr.mq_flags = 0;
    resp_attr.mq_maxmsg = 10;
    resp_attr.mq_msgsize = sizeof(Response);
    resp_attr.mq_curmsgs = 0;
    mqResponseDescriptor = mq_open(client_mq_name,
                                   O_CREAT | O_RDONLY,
                                   S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH,
                                   &resp_attr);
    if (mqResponseDescriptor == (mqd_t) -1) {
        printf("Error: Client: mq_open (response)\n");
        exit(EXIT_FAILURE);
    }

    mqRequestDescriptor = mq_open(REQUEST_MQ_NAME, O_WRONLY);
    if (mqRequestDescriptor == (mqd_t) -1) {
        printf("Error: Client: mq_open (request)\n");
        mq_close(mqResponseDescriptor);
        mq_unlink(client_mq_name);
        exit(EXIT_FAILURE);
    }

    status = mq_send(mqRequestDescriptor, (char *) &request, sizeof(Request), 0);
    if (status == -1) {
        printf("Error: Client: mq_send\n");
        mq_close(mqRequestDescriptor);
        mq_close(mqResponseDescriptor);
        mq_unlink(client_mq_name);
        exit(EXIT_FAILURE);
    }

    status = mq_receive(mqResponseDescriptor, (char *) &response, sizeof(Response), NULL);
    if (status == -1) {
        printf("Error: Client: mq_receive\n");
        mq_close(mqRequestDescriptor);
        mq_close(mqResponseDescriptor);
        mq_unlink(client_mq_name);
        exit(EXIT_FAILURE);
    }

    printf("Received response: %f\n", response.result);

    mq_close(mqRequestDescriptor);
    mq_close(mqResponseDescriptor);
    mq_unlink(client_mq_name);

    return 0;
}
