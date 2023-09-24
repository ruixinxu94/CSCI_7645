#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void  swap(int* first, int* second) {
    int temp = *first;
    *first = *second;
    *second = temp;
}


int main(int argc, char* argv[]) {
    if (argc != 3 || strcmp(argv[1], "--help") == 0) {
        printf("--usage: ......");
        exit(EXIT_FAILURE);
    }
    int x = atoi(argv[1]);
    int y = atoi(argv[2]);
    printf("before swap, x is %d, y is %d\n", x, y);
    int* a = &x;
    int* b = &y;
    swap(a, b);
    printf("after swap, x is %d, y is %d\n", x, y);
    exit(EXIT_SUCCESS);
}