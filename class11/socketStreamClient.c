#include <sys/un.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define SV_SOCK_PATH "/tmp/us_xfr"
#define BUF_SIZE 100

int main(int argc, char *argv[]) {
    struct sockaddr_un addr;
    int sfd;
    ssize_t numRead;
    char buf[BUF_SIZE];

    // Create client socket
    sfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sfd == -1) {
        printf("Error in socket creation: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Construct server address, and make the connection
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SV_SOCK_PATH, sizeof(addr.sun_path) - 1);

    if (connect(sfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) == -1) {
        printf("Error in connect: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Copy stdin to socket
    while ((numRead = read(STDIN_FILENO, buf, BUF_SIZE)) > 0) {
        if (write(sfd, buf, numRead) != numRead) {
            printf("Error in write: partial/failed write\n");
            exit(EXIT_FAILURE);
        }
    }

    if (numRead == -1) {
        printf("Error in read: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS); // Closes our socket; server sees EOF
}
