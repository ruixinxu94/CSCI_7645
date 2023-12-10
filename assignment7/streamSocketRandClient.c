#include <sys/un.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define SV_SOCK_PATH "/tmp/us_xfr"

typedef struct {
    float nums[10];
} Request;

typedef struct {
    float result;
} Response;

int main(int argc, char *argv[]) {
    struct sockaddr_un addr;
    int sfd;
    ssize_t numWritten, numRead;
    Request request;
    Response response;
    // Generate random number
    srand(time(NULL) ^ getpid());

    for (int i = 0; i < 10; i++) {
        request.nums[i] = rand() % 2;
    }

    // Create socket, sfd is the socket descriptor
    sfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sfd == -1) {
        printf("Error in socket creation\n");
        exit(EXIT_FAILURE);
    }

    // Assign socket address
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SV_SOCK_PATH, sizeof(addr.sun_path) - 1);

    if (connect(sfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) == -1) {
        printf("Error in connect\n");
        close(sfd);
        exit(EXIT_FAILURE);
    }

    // Send request to server
    numWritten = write(sfd, &request, sizeof(Request));
    if (numWritten == -1) {
        printf("Error in write\n");
        close(sfd);
        exit(EXIT_FAILURE);
    }

    // Read response from server
    numRead = read(sfd, &response, sizeof(Response));
    if (numRead == -1) {
        printf("Error in read\n");
        close(sfd);
        exit(EXIT_FAILURE);
    }

    printf("Received response: %f\n", response.result);

    // Close socket
    close(sfd);
    exit(EXIT_SUCCESS);
}
