#include <sys/un.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>

#define BACKLOG 5
#define SV_SOCK_PATH "/tmp/us_xfr"
#define BUF_SIZE 100

typedef struct {
    float nums[10];
} Request;

typedef struct {
    float result;
} Response;

int main(int argc, char *argv[]) {
    struct sockaddr_un addr;
    int sfd, cfd;
    ssize_t numRead, numWritten;
    char buf[BUF_SIZE];

    sfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sfd == -1) {
        printf("Error in socket creation\n");
        exit(EXIT_FAILURE);
    }

    if (remove(SV_SOCK_PATH) == -1 && errno != ENOENT) {
        printf("Error removing socket path\n");
        exit(EXIT_FAILURE);
    }

    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SV_SOCK_PATH, sizeof(addr.sun_path) - 1);
    //  assigns an address to a socket.
    if (bind(sfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) == -1) {
        printf("Error in bind\n");
        exit(EXIT_FAILURE);
    }
    // Marks the socket as a passive socket that will be used to accept incoming connection requests.
    if (listen(sfd, BACKLOG) == -1) {
        printf("Error in listen\n");
        exit(EXIT_FAILURE);
    }

    while (1) {
        cfd = accept(sfd, NULL, NULL);
        if (cfd == -1) {
            printf("Error in accept\n");
            continue; // Continue to next iteration
        }

        numRead = read(cfd, buf, sizeof(Request));
        if (numRead == -1) {
            printf("Error in read\n");
            close(cfd);
            continue; // Continue to next iteration
        }

        // Cast the buffer to Request and calculate the average
        Request *req = (Request *) buf;
        float sum = 0.0;
        for (int i = 0; i < 10; i++) {
            sum += req->nums[i];
        }
        Response resp;
        resp.result = sum / 10;

        // Send the response back to the client
        numWritten = write(cfd, &resp, sizeof(Response));
        if (numWritten == -1) {
            printf("Error in write\n");
        }

        close(cfd); // Close the connection
    }

    exit(EXIT_SUCCESS);
}
