#include <sys/un.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#define BACKLOG 5
#define SV_SOCK_PATH "/tmp/us_xfr"
#define BUF_SIZE 100

int main(int argc, char *argv[]) {
    struct sockaddr_un addr;
    int sfd, cfd;
    ssize_t numRead;
    char buf[BUF_SIZE];

    sfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sfd == -1) {
        printf("Error in socket creation: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (remove(SV_SOCK_PATH) == -1 && errno != ENOENT) {
        printf("Error removing socket path: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SV_SOCK_PATH, sizeof(addr.sun_path) - 1);

    if (bind(sfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) == -1) {
        printf("Error in bind: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (listen(sfd, BACKLOG) == -1) {
        printf("Error in listen: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    while (1) { // Replaced 'for' with 'while'
        cfd = accept(sfd, NULL, NULL);
        if (cfd == -1) {
            printf("Error in accept: %s\n", strerror(errno));
            continue; // Continue accepting new connections
        }

        while ((numRead = read(cfd, buf, BUF_SIZE)) > 0) {
            if (write(STDOUT_FILENO, buf, numRead) != numRead) {
                printf("Error in write: partial/failed write\n");
                break;
            }
        }

        if (numRead == -1) {
            printf("Error in read: %s\n", strerror(errno));
        }

        if (close(cfd) == -1) {
            printf("Error in close: %s\n", strerror(errno));
        }
    }

    exit(EXIT_SUCCESS);
}
