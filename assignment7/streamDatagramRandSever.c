#include <sys/un.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define SV_SOCK_PATH "/tmp/ud_ucase"

typedef struct {
    float nums[10];
} Request;

typedef struct {
    float result;
} Response;

int main() {
    struct sockaddr_un svaddr, claddr;
    int sfd;
    ssize_t numBytes;
    socklen_t len;
    Request request;
    Response response;

    // Create server socket
    sfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sfd == -1) {
        printf("Error in socket creation\n");
        exit(EXIT_FAILURE);
    }

    // Construct well-known address and bind server socket to it
    if (remove(SV_SOCK_PATH) == -1 && errno != ENOENT) {
        printf("Error removing socket path\n");
        exit(EXIT_FAILURE);
    }

    memset(&svaddr, 0, sizeof(struct sockaddr_un));
    svaddr.sun_family = AF_UNIX;
    strncpy(svaddr.sun_path, SV_SOCK_PATH, sizeof(svaddr.sun_path) - 1);

    if (bind(sfd, (struct sockaddr *) &svaddr, sizeof(struct sockaddr_un)) == -1) {
        printf("Error in bind\n");
        exit(EXIT_FAILURE);
    }

    while (1) {
        len = sizeof(struct sockaddr_un);
        numBytes = recvfrom(sfd, &request, sizeof(Request), 0, (struct sockaddr *) &claddr, &len);

        if (numBytes == -1) {
            printf("Error in recvfrom\n");
            continue;
        }

        // Calculate the average
        float sum = 0;
        for (int i = 0; i < 10; i++) {
            sum += request.nums[i];
        }
        response.result = sum / 10;

        // Send response back to the client
        numBytes = sendto(sfd, &response, sizeof(Response), 0, (struct sockaddr *) &claddr, len);
        if (numBytes == -1) {
            printf("Error in sendto\n");
            continue;
        }
    }
    exit(EXIT_SUCCESS);
}
