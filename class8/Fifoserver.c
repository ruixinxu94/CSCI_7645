#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>


#define TRUE 1
#define FALSE 0
#define SERVER_FIFO_NAME "RequestFifo"
#define CLIENT_FIFO_TEMPLATE "ResponsFifo.%d"
#define MAX_LENGTH 100

/** define the protocol */
typedef struct {
    pid_t clientId;
    unsigned numTickets;
} Request;


typedef struct {
    unsigned firstTicket;
    unsigned lastTicket;
} Response;

// include the signal handler for ctrl c
void signalHandler(int dummyInput) {
    exit(EXIT_SUCCESS);
}


// create exit handler for removing FIFO
void removeFIFO() {
    int status = unlink(SERVER_FIFO_NAME);
    if (status == -1) {
        printf("server failed to remove request FIFO.\n");
//        exit(EXIT_FAILURE); shouldn't call it again since it would trigger infnite handler
    }
}


int main(int argc, char **argv) {
    int status, requestFifoDesciptor, responseFifoDescriptor, dummyDescriptor;
    Request request;
    Response response;
    int numRead, numWritten;
    char responseFifoName[MAX_LENGTH];
    unsigned nextTicket = 1;
    // register the exit handler
    status = atexit(removeFIFO);
    if (status != 0) {
        printf("server was unable to register as a handler");
        exit(EXIT_FAILURE);
    }
    // register the signal handler to remove the pipe
    if (signal(SIGINT, signalHandler) == SIG_ERR) {
        printf("Server failed to register for signal handler");
        exit(EXIT_FAILURE);
    }

    /** create the request FIFO */
    status = mkfifo(SERVER_FIFO_NAME, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
    if (status == -1) {
        printf("Server failed to create reuqest FIFO .\n");
        exit(EXIT_FAILURE);
    }

    requestFifoDesciptor = open(SERVER_FIFO_NAME, O_RDONLY);
    if (requestFifoDesciptor == -1) {
        printf("Server failed to open request FIFO for reading .\n");
        exit((EXIT_FAILURE));
    }

    dummyDescriptor = open(SERVER_FIFO_NAME, O_WRONLY);
    if (dummyDescriptor == -1) {
        printf("Server failed to open the dummy descriptor.\n");
        exit((EXIT_FAILURE));
    }

    while (TRUE) {
        /** read the next request */
        numRead = read(requestFifoDesciptor, &request, sizeof(Request));
        if (numRead == -1) {
            printf("reader is failed: server failed to read request");
            exit(EXIT_FAILURE);
        }

        printf("server received a request from client %d for %d tickets\n", request.clientId, request.numTickets);

        /** calculate the response */
        response.firstTicket = nextTicket;
        response.lastTicket = response.firstTicket + request.numTickets - 1;
        nextTicket = response.lastTicket + 1;

        /** send the response to the correct client */

        /** calculate the NAME of the RESPONSE Fifo */
        snprintf(responseFifoName, MAX_LENGTH, CLIENT_FIFO_TEMPLATE, request.clientId);

        /** open  the repsonse fifo in the write onlyy mode*/
        responseFifoDescriptor = open(responseFifoName, O_RDONLY);
        if (responseFifoDescriptor == -1) {
            printf("Server failed to open response Fifo descriptor .\n");
            exit((EXIT_FAILURE));
        }

        // write the response
        numWritten = write(responseFifoDescriptor, &response, sizeof(Response));
        if (numWritten < sizeof(Response)) {
            printf("Server encountered partial write error.\n");
            exit(EXIT_FAILURE);
        }

        // close the response FIFO so that client can delete it
        status = close(responseFifoDescriptor);
        if (status == -1) {
            printf("server failed to close resoibse Fufi. \n");
            exit(EXIT_FAILURE);
        }
    }

    exit(EXIT_SUCCESS);


}