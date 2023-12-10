#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <string.h>
#include <unistd.h>

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
    struct mq_attr req_attr, resp_attr;
    Request request;
    Response response;
    unsigned priority;
    char client_mq_name[TEMPLATE_NAME_MAX_LENGTH];
    int status;

    req_attr.mq_flags = 0;
    req_attr.mq_maxmsg = 10;
    req_attr.mq_msgsize = sizeof(Request);
    req_attr.mq_curmsgs = 0;

    mqRequestDescriptor = mq_open(REQUEST_MQ_NAME,
                                  O_CREAT | O_RDONLY,
                                  S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH,
                                  &req_attr);
    if (mqRequestDescriptor == (mqd_t) -1) {
        perror("Error: Server: mq_open (request)");
        exit(EXIT_FAILURE);
    }

    resp_attr.mq_flags = 0;
    resp_attr.mq_maxmsg = 10;
    resp_attr.mq_msgsize = sizeof(Response);
    resp_attr.mq_curmsgs = 0;

    while (1) {
        if (mq_receive(mqRequestDescriptor,
                       (char *) &request,
                       sizeof(Request),
                       &priority) == -1) {
            printf("Error: Server: mq_receive\n");
            continue;
        }

        float sum = 0;
        for (int i = 0; i < 10; i++) {
            sum += request.nums[i];
        }
        response.result = sum / 10;

        snprintf(client_mq_name,
                 TEMPLATE_NAME_MAX_LENGTH,
                 CLIENT_MQ_TEMPLATE,
                 request.clientID);
        mqResponseDescriptor = mq_open(client_mq_name,
                                       O_WRONLY | O_CREAT,
                                       S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH,
                                       &resp_attr);
        if (mqResponseDescriptor == (mqd_t) -1) {
            printf("Error: Server: mq_open (response)\n");
            continue;
        }

        if (mq_send(mqResponseDescriptor,
                    (char *) &response,
                    sizeof(Response), 0) == -1) {
            printf("Error: Server: mq_send\n");
            mq_close(mqResponseDescriptor);
            continue;
        }

        mq_close(mqResponseDescriptor);
    }

    mq_close(mqRequestDescriptor);
    mq_unlink(REQUEST_MQ_NAME);
    exit(EXIT_SUCCESS);
}
