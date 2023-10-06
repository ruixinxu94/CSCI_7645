#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    FILE *fp = fopen("example.txt", "w");
    if (fp == NULL) {
        perror("Failed to open file");
        return 1;
    }

    fprintf(fp, "Hello, World!");

//     Uncomment the line below for exit()
     exit(0);

//     Uncomment the line below for _exit()
//     _exit(0);

//    return 0;
}
