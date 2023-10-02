#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char* argv[]) {
    printf("my process id is %d\n", getpid());
    printf("my parent's process id is %d\n", getppid());
}