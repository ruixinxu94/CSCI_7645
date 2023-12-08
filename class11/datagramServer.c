#include <sys/un.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#define BUF_SIZE 10 // Maximum size of messages exchanged between client to server
#define SV_SOCK_PATH "/tmp/ud_ucase"

int main(int argc, char *argv[]) {
    struct sockaddr_un svaddr, claddr;
    int sfd, j;
    ssize_t numBytes;
    socklen_t len;
    char buf[BUF_SIZE];

    // Create server socket
    sfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sfd == -1) {
        printf("Error in socket creation: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Construct well-known address and bind server socket to it
    if (remove(SV_SOCK_PATH) == -1 && errno != ENOENT) {
        printf("Error removing socket path: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    memset(&svaddr, 0, sizeof(struct sockaddr_un));
    svaddr.sun_family = AF_UNIX;
    strncpy(svaddr.sun_path, SV_SOCK_PATH, sizeof(svaddr.sun_path) - 1);

    if (bind(sfd, (struct sockaddr *) &svaddr, sizeof(struct sockaddr_un)) == -1) {
        printf("Error in bind: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Receive messages, convert to uppercase, and return to client
    while (1) {
        len = sizeof(struct sockaddr_un);
        numBytes = recvfrom(sfd, buf, BUF_SIZE, 0, (struct sockaddr *) &claddr, &len);
        if (numBytes == -1) {
            printf("Error in recvfrom: %s\n", strerror(errno));
            continue; // Continue processing next message
        }

        printf("Server received %ld bytes from %s\n", (long) numBytes, claddr.sun_path);

        for (j = 0; j < numBytes; j++)
            buf[j] = toupper((unsigned char) buf[j]);

        if (sendto(sfd, buf, numBytes, 0, (struct sockaddr *) &claddr, len) != numBytes) {
            printf("Error in sendto: partial/failed send\n");
            continue; // Continue processing next message
        }
    }

    exit(EXIT_SUCCESS);
}
