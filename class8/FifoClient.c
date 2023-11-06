#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define SERVER_FIFO_NAME "RequestFifo"
#define CLIENT_FIFO_TEMPLATE "ResponsFifo.%d"
#define MAX_LENGTH 100

typedef struct {
    pid_t clientId;
    unsigned numTickets;
} Request;

typedef struct {
    unsigned firstTicket;
    unsigned lastTicket;
} Response;

int main(int argc, char **argv) {
    int numTickets, requestFifoDescriptor, responseFifoDescriptor;
    Request request;
    Response response;
    int numRead, numWritten;
    char responseFifoName[MAX_LENGTH];

    if (argc != 2) {
        printf("Usage: %s <number_of_tickets>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    numTickets = atoi(argv[1]);
    if (numTickets <= 0) {
        printf("Number of tickets must be positive\n");
        exit(EXIT_FAILURE);
    }

    // Prepare the request
    request.clientId = getpid();
    request.numTickets = (unsigned) numTickets;

    // Create the response FIFO name
    snprintf(responseFifoName, MAX_LENGTH, CLIENT_FIFO_TEMPLATE, request.clientId);

    // Create the response FIFO
    if (mkfifo(responseFifoName, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP) == -1) {
        perror("Client failed to create response FIFO");
        exit(EXIT_FAILURE);
    }

    // Put the request on the request FIFO
    requestFifoDescriptor = open(SERVER_FIFO_NAME, O_WRONLY);
    if (requestFifoDescriptor == -1) {
        perror("Cannot reach the server");
        unlink(responseFifoName); // Clean up FIFO
        exit(EXIT_FAILURE);
    }

    numWritten = write(requestFifoDescriptor, &request, sizeof(request));
    if (numWritten != sizeof(request)) {
        perror("Client encountered partial write error while writing to request FIFO");
        close(requestFifoDescriptor);
        unlink(responseFifoName); // Clean up FIFO
        exit(EXIT_FAILURE);
    }
    close(requestFifoDescriptor); // Request sent, can close the descriptor

    // Open the response FIFO for reading
    responseFifoDescriptor = open(responseFifoName, O_RDONLY);
    if (responseFifoDescriptor == -1) {
        perror("Client failed to open response FIFO");
        unlink(responseFifoName); // Clean up FIFO
        exit(EXIT_FAILURE);
    }

    // Read the response
    numRead = read(responseFifoDescriptor, &response, sizeof(response));
    if (numRead != sizeof(response)) {
        perror("Client failed to read full response from FIFO");
    } else {
        printf("First ticket = %u, last ticket = %u\n", response.firstTicket, response.lastTicket);
    }

    // Cleanup: Close FIFO and delete it
    close(responseFifoDescriptor);
    if (unlink(responseFifoName) == -1) {
        perror("Client failed to delete response FIFO");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS); // Corrected the exit call with the appropriate argument
}
