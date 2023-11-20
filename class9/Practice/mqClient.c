#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <unistd.h>
#include <string.h>

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
    Request request;
    Response response;
    char client_mq_name[MAX_LENGTH];
    int status;

    // Set up client request
    request.clientID = getpid();
    for (int i = 0; i < 10; i++) {
        request.nums[i] = (float)i;  // Example data
    }

    // Create client's response queue
    snprintf(client_mq_name, MAX_LENGTH, CLIENT_MQ_TEMPLATE, getpid());
    response_mqd = mq_open(client_mq_name, O_CREAT | O_RDONLY, 0644, NULL);
    if (response_mqd == (mqd_t)-1) {
        perror("Client: mq_open (response)");
        exit(EXIT_FAILURE);
    }

    // Open server's request queue
    request_mqd = mq_open(REQUEST_MQ, O_WRONLY);
    if (request_mqd == (mqd_t)-1) {
        perror("Client: mq_open (request)");
        mq_close(response_mqd);
        mq_unlink(client_mq_name);
        exit(EXIT_FAILURE);
    }

    // Send request to server
    status = mq_send(request_mqd, (char*)&request, sizeof(Request), 0);
    if (status == -1) {
        perror("Client: mq_send");
        mq_close(request_mqd);
        mq_close(response_mqd);
        mq_unlink(client_mq_name);
        exit(EXIT_FAILURE);
    }

    // Read response from server
    status = mq_receive(response_mqd, (char*)&response, sizeof(Response), NULL);
    if (status == -1) {
        perror("Client: mq_receive");
        mq_close(request_mqd);
        mq_close(response_mqd);
        mq_unlink(client_mq_name);
        exit(EXIT_FAILURE);
    }

    printf("Received response: %f\n", response.result);

    // Cleanup
    mq_close(request_mqd);
    mq_close(response_mqd);
    mq_unlink(client_mq_name);

    return 0;
}
