#include <sys/un.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>

// The number of pending connections that can be queued up by the socket.
/**
 * Creating and Binding a Socket: To set up a communication endpoint for clients to connect to.
Removing Old Socket File: To ensure the server can bind to the desired path without issues from previous runs.
Listening and Accepting Connections: To handle incoming client requests.
Reading and Printing Data: To process data sent by clients.
Infinite Loop for Accepting Connections: To continuously serve multiple clients over time.
Error Handling: To make the server robust and avoid crashes or undefined behavior.
 */
#define BACKLOG 5
#define SV_SOCK_PATH "/tmp/us_xfr"
#define BUF_SIZE 100

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
            continue;
        }

        while ((numRead = read(cfd, buf, BUF_SIZE)) > 0) {
            // Convert to uppercase
            for (int i = 0; i < numRead; i++) {
                buf[i] = toupper((char) buf[i]);
            }

            // Send back to client
            numWritten = write(cfd, buf, numRead);
            if (numWritten != numRead) {
                printf("Error in write/n");
                break;
            }
        }

        if (numRead == -1) {
            printf("Error in read");
            exit(EXIT_FAILURE);
        }

        if (close(cfd) == -1) {
            printf("Error in close");
            exit(EXIT_FAILURE);
        }
    }
    exit(EXIT_SUCCESS);
}
