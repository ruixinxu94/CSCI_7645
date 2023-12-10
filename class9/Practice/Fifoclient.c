#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

/* Define the protocol */

// seq 1 10 | xargs -n 1 -P 5 ./fifoclient
// cat tickets.txt | xargs -n 1 ./Fifoclient
// cat tickets.txt | xargs -n 1 -p 5 ./Fifoclient
//

typedef struct {
    pid_t clientID;
    double nums[10];
} Request;

typedef struct {
    double result;
} Response;

/* FIFO names */

#define SERVER_FIFO_NAME "RequestFifo"
#define CLIENT_FIFO_TEMPLATE "ResponseFifo.%d"

#define MAX_LENGTH 100

/* Create exit handler for removing response FIFO */
void removeResponseFifo() {
    int status;
    char responseFifoName[MAX_LENGTH];

    snprintf(responseFifoName, MAX_LENGTH, CLIENT_FIFO_TEMPLATE, getpid());

    status = unlink(responseFifoName);
    if (status == -1) {
        printf("Client failed to remove response FIFO.\n");
    }

}

int main(int argc, char *argv[]) {
    int status, requestFifoDescriptor, responseFifoDescriptor;
    Request request;
    Response response;
    int numRead, numWritten;
    char responseFifoName[MAX_LENGTH];

    /* Register the exit handler */
    status = atexit(removeResponseFifo);
    if (status != 0) {
        printf("Client was unable to register exit handler.\n");
        exit(EXIT_FAILURE);
    }

    /* Create the response FIFO */
    snprintf(responseFifoName, MAX_LENGTH, CLIENT_FIFO_TEMPLATE, getpid());

    status = mkfifo(
            responseFifoName,
            S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP
    );
    if (status == -1) {
        printf("Client failed to create response FIFO.\n");
        exit(EXIT_FAILURE);
    }

    /* Prepare the request */
    request.clientID = getpid();

    double array[10];

    // Seed the random number generator
    srand(time(NULL) ^ getpid());
    // Generate random numbers
    for (int i = 0; i < 10; i++) {
        array[i] = rand() % 2;
    }

    // Print the array & assign the number
    printf("Random Array: \n");
    for (int i = 0; i < 10; i++) {
        // assign double number to reuqest
        request.nums[i] = array[i];
        printf("%.1f ", array[i]);
    }
    printf("\n");


    /* Put the request on the request FIFO */
    requestFifoDescriptor = open(SERVER_FIFO_NAME, O_WRONLY);
    if (requestFifoDescriptor == -1) {
        printf("Client cannot reach server.\n");
        exit(EXIT_FAILURE);
    }

    numWritten = write(requestFifoDescriptor, &request, sizeof(Request));
    if (numWritten < sizeof(Request)) {
        printf("Client encountered partial write error while writing the request.\n");
        exit(EXIT_FAILURE);
    }

    status = close(requestFifoDescriptor);
    if (status == -1) {
        printf("Client failed to close the request FIFO.\n");
        exit(EXIT_FAILURE);
    }

    /* Read the response */
    responseFifoDescriptor = open(responseFifoName, O_RDONLY);
    if (responseFifoDescriptor == -1) {
        printf("Client failed to open response FIFO.\n");
        exit(EXIT_FAILURE);
    }

    numRead = read(responseFifoDescriptor, &response, sizeof(Response));
    if (numRead == -1) {
        printf("Client failed to read response.\n");
        exit(EXIT_FAILURE);
    }

    printf(
            "The response is %f\n",
            response.result
    );

    status = close(responseFifoDescriptor);
    if (status == -1) {
        printf("Client failed to close response FIFO.\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);

}