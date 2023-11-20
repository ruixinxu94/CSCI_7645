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

#define REQUEST_MQ "/request_mq"
#define CLIENT_MQ_TEMPLATE "/response_mq_%d"
#define MAX_LENGTH 100

int main() {
    mqd_t request_mqd, response_mqd;
    struct mq_attr attr;
    Request request;
    Response response;
    unsigned priority;
    char client_mq_name[MAX_LENGTH];
    int status;

    // Set up attributes for the message queue
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(Request);
    attr.mq_curmsgs = 0;

    // Create the request message queue
    request_mqd = mq_open(REQUEST_MQ, O_CREAT | O_RDONLY, 0644, &attr);
    if (request_mqd == (mqd_t)-1) {
        perror("Server: mq_open (request)");
        exit(EXIT_FAILURE);
    }

    while (1) {
        // Read request from the queue
        status = mq_receive(request_mqd, (char*)&request, sizeof(Request), &priority);
        if (status == -1) {
            perror("Server: mq_receive");
            continue;  // Continue on error
        }

        // Process the request (calculate average as an example)
        float sum = 0;
        for (int i = 0; i < 10; i++) {
            sum += request.nums[i];
        }
        response.result = sum / 10;

        // Prepare client MQ name
        snprintf(client_mq_name, MAX_LENGTH, CLIENT_MQ_TEMPLATE, request.clientID);

        // Open client's response queue
        response_mqd = mq_open(client_mq_name, O_WRONLY);
        if (response_mqd == (mqd_t)-1) {
            perror("Server: mq_open (response)");
            continue;  // Continue on error
        }

        // Send response
        status = mq_send(response_mqd, (char*)&response, sizeof(Response), 0);
        if (status == -1) {
            perror("Server: mq_send");
            mq_close(response_mqd);
            continue;  // Continue on error
        }

        // Close client's response queue
        mq_close(response_mqd);
    }

    // Cleanup (not reached in this example)
    mq_close(request_mqd);
    mq_unlink(REQUEST_MQ);
    return 0;
}
