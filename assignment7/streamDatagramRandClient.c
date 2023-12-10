#include <sys/un.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

#define BUF_SIZE 10 // Maximum size of messages exchanged between client to server
#define SV_SOCK_PATH "/tmp/ud_ucase"

typedef struct {
    float nums[10];
} Request;

typedef struct {
    float result;
} Response;

int main(int argc, char *argv[]) {
    struct sockaddr_un svaddr, claddr;
    int sfd, i;
    ssize_t numBytes;
    char resp[BUF_SIZE];
    Request request;
    Response  response;
    // Create client socket; bind to unique pathname (based on PID)
    sfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sfd == -1) {
        printf("Error in socket creation: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // generate random number
    srand(time(NULL) ^ getpid());

    for (int i = 0; i < 10; i++) {
        request.nums[i] = rand() % 2;
    }

    memset(&claddr, 0, sizeof(struct sockaddr_un));
    claddr.sun_family = AF_UNIX;
    snprintf(claddr.sun_path, sizeof(claddr.sun_path), "/tmp/ud_ucase_cl.%ld", (long) getpid());

    if (bind(sfd, (struct sockaddr *) &claddr, sizeof(struct sockaddr_un)) == -1) {
        printf("Error in bind: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Construct address of server
    memset(&svaddr, 0, sizeof(struct sockaddr_un));
    svaddr.sun_family = AF_UNIX;
    strncpy(svaddr.sun_path, SV_SOCK_PATH, sizeof(svaddr.sun_path) - 1);

    // Send Request to server
    if (sendto(sfd, &request, sizeof(Request), 0, (struct sockaddr *) &svaddr,
               sizeof(struct sockaddr_un)) != sizeof(Request)) {
        printf("Error in sendto: partial/failed send\n");
        remove(claddr.sun_path); // Remove client socket pathname
        exit(EXIT_FAILURE);
    }

    // Receive Response from server
    numBytes = recvfrom(sfd, &response, sizeof(Response), 0, NULL, NULL);
    if (numBytes == -1) {
        printf("Error in recvfrom: %s\n", strerror(errno));
        remove(claddr.sun_path); // Remove client socket pathname
        exit(EXIT_FAILURE);
    }

    printf("Received response: %f\n", response.result);

    remove(claddr.sun_path); // Remove client socket pathname
    exit(EXIT_SUCCESS);
}
