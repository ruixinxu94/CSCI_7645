#include <sys/un.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SV_SOCK_PATH "/tmp/us_xfr"
#define BUF_SIZE 100

int main(int argc, char *argv[]) {
    struct sockaddr_un addr;
    int sfd;
    ssize_t numWritten, numRead;
    char buf[BUF_SIZE];
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <string>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    // create socket, sfd is the socket descriptor
    sfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sfd == -1) {
        printf("Error in socket creation\n");
        exit(EXIT_FAILURE);
    }

    // assign socket address
    memset(&addr, 0, sizeof(struct sockaddr_un));
    // address family of socket
    addr.sun_family = AF_UNIX;
    // copy the socket path
    strncpy(addr.sun_path, SV_SOCK_PATH, sizeof(addr.sun_path) - 1);
    // sfd says who is trying to connect the socket,
    if (connect(sfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) == -1) {
        printf("Error in connect\n");
        exit(EXIT_FAILURE);
    }

    // Iterate over each command-line argument and send it
    for (int i = 1; i < argc; ++i) {
        numWritten = write(sfd, argv[i], strlen(argv[i]));
        if (numWritten == -1) {
            perror("Error in write");
            exit(EXIT_FAILURE);
        }
        if (numWritten < strlen(argv[i])) {
            printf("Partial write\n");
            exit(EXIT_FAILURE);
        }

        // Read response from server
        numRead = read(sfd, buf, BUF_SIZE - 1);
        if (numRead == -1) {
            perror("Error in read");
            exit(EXIT_FAILURE);
        }
        buf[numRead] = '\0'; // Null-terminate the received string
        printf("Response from server: %s\n", buf);
    }
    // Close socket
    close(sfd);
    exit(EXIT_SUCCESS);
}
