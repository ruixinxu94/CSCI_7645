#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>

#define TRUE 1
#define FALSE 0
#define SERVER_FIFO_NAME "RequestFifo"
#define CLIENT_FIFO_TEMPLATE "ResponseFifo.%d"
#define MAX_LENGTH 100

typedef struct {
    pid_t clientId;
    unsigned numTickets;
} Request;

typedef struct {
    unsigned firstTicket;
    unsigned lastTicket;
} Response;

void signalHandler(int dummyInput) {
    exit(EXIT_SUCCESS);
}

volatile unsigned nextTicket = 1;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *giveOutTickets(void *requestPointer) {
    Request *request = (Request *) requestPointer;
    int status, responseFifoDescriptor, numWritten;
    Response response;
    char responseFifoName[MAX_LENGTH];

    printf("Server received a request from client %d for %d tickets\n", request->clientId, request->numTickets);

    status = pthread_mutex_lock(&mutex);
    if (status != 0) {
        printf("Failed to lock\n");
        exit(EXIT_FAILURE);
    }

    response.firstTicket = nextTicket;
    response.lastTicket = response.firstTicket + request->numTickets - 1;
    nextTicket = response.lastTicket + 1;

    status = pthread_mutex_unlock(&mutex);
    if (status != 0) {
        printf("Failed to unlock\n");
        exit(EXIT_FAILURE);
    }

    snprintf(responseFifoName, MAX_LENGTH, CLIENT_FIFO_TEMPLATE, request->clientId);
    responseFifoDescriptor = open(responseFifoName, O_WRONLY);
    if (responseFifoDescriptor == -1) {
        printf("Server failed to open response FIFO.\n");
        exit(EXIT_FAILURE);
    }

    numWritten = write(responseFifoDescriptor, &response, sizeof(Response));
    if (numWritten < sizeof(Response)) {
        printf("Server encountered partial write error.\n");
        exit(EXIT_FAILURE);
    }

    status = close(responseFifoDescriptor);
    if (status == -1) {
        printf("Server failed to close response FIFO.\n");
        exit(EXIT_FAILURE);
    }

    free(requestPointer);
    pthread_exit(NULL);
}

void removeFIFO() {
    int status = unlink(SERVER_FIFO_NAME);
    if (status == -1) {
        printf("Server failed to remove request FIFO.\n");
    }
}

int main(int argc, char **argv) {
    int status, requestFifoDescriptor, dummyDescriptor;
    pthread_t requestHandlerThread;

    status = atexit(removeFIFO);
    if (status != 0) {
        printf("Server was unable to register as a handler\n");
        exit(EXIT_FAILURE);
    }

    if (signal(SIGINT, signalHandler) == SIG_ERR) {
        printf("Server failed to register for signal handler\n");
        exit(EXIT_FAILURE);
    }

    status = mkfifo(SERVER_FIFO_NAME, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
    if (status == -1) {
        printf("Server failed to create request FIFO.\n");
        exit(EXIT_FAILURE);
    }

    requestFifoDescriptor = open(SERVER_FIFO_NAME, O_RDONLY);
    if (requestFifoDescriptor == -1) {
        printf("Server failed to open request FIFO for reading.\n");
        exit(EXIT_FAILURE);
    }

    dummyDescriptor = open(SERVER_FIFO_NAME, O_WRONLY);
    if (dummyDescriptor == -1) {
        printf("Server failed to open the dummy descriptor.\n");
        exit(EXIT_FAILURE);
    }

    while (TRUE) {
        Request *requestHandlerInput = (Request *) malloc(sizeof(Request));
        if (requestHandlerInput == NULL) {
            printf("Failed to allocate memory\n");
            continue; // Or handle the error as per your application's requirements
        }

        int numRead = read(requestFifoDescriptor, requestHandlerInput, sizeof(Request));
        if (numRead == -1) {
            printf("Reader failed: server failed to read request\n");
            free(requestHandlerInput); // Free memory in case of read failure
            continue; // Or handle the error as per your application's requirements
        } else if (numRead != sizeof(Request)) {
            printf("Incomplete read\n");
            free(requestHandlerInput);
            continue;
        }

        status = pthread_create(&requestHandlerThread, NULL, giveOutTickets, (void *) requestHandlerInput);
        if (status != 0) {
            printf("Failed to create thread\n");
            free(requestHandlerInput);
            continue; // Or handle the error as per your application's requirements
        }

        status = pthread_detach(requestHandlerThread);
        if (status != 0) {
            printf("Failed to detach thread\n");
            // Thread creation succeeded but detaching failed
            // Consider how to handle this situation in your application
        }
    }

    // Code to close FIFOs and other cleanup if necessary
    close(requestFifoDescriptor);
    close(dummyDescriptor);

    exit(EXIT_SUCCESS);
}
