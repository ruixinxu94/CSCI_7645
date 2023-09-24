#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int square(int x) {
    return x*x;
}

int cube(int x) {
    return x*x*x;
}

void printArray(int *numbers, size_t size) {
    for (int i = 0; i < size; i++) {
        printf("  %d", numbers[i]);
    }
    printf("\n");
}

void secondOrder(int* numbers, size_t size, int (*visitor)(int)) {
    for (int i = 0; i < size; i++) {
        numbers[i] = visitor(numbers[i]);
    }
}

int  main(int argc, char* argv[]) {
    if (argc != 2) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }
    int size = atoi(argv[1]);
    int* numbers;
    numbers = malloc(size * sizeof(int));
    if (NULL == numbers) {
        exit(1);
    }
    for (int i = 0; i < size; i++) {
        numbers[i] = i+1;
    }
    secondOrder(numbers, size, square);
    printArray(numbers, size);
    secondOrder(numbers, size, cube);
    printArray(numbers, size);
    free(numbers);
    exit(EXIT_SUCCESS);
}