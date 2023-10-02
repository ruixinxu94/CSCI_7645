#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// function pointer
// there is a function called visitor who is input is an int, and return int
void transformElements(int *x, size_t count, int (*visitor)(int)) {
    int index;
    for (index = 0; index < count; index++) {
        x[index] = visitor(x[index]);
    }
}

int square(int x) {
    return x * x;
}

int cube(int x) {
    return x * x * x;
}

// same code
void cubeAllElements(int *x, size_t count) {
    int index;
    for (index = 0; index < count; index++) {
        x[index] = x[index] * x[index] * x[index];
    }
}

void squareAllElements(int *x, size_t count) {
    int index;

    for (index = 0; index < count; index++) {
        x[index] = x[index] * x[index];
    }
}

// arg[0] is the "./printnum"
// arg[1] is the number you input
// argv[] is the array of strings
// in java ir is called public static void main(String[] args)
//./printnumbers 10
int main(int argc, char *argv[]) {
    // loop index
    int index;
    // how many things to print
    int size;
    // array
    int *numbers;
    // check if the number of the command line is correct
    // argv[1]=="--help" is wrong, using the.equals function for a string compare
    if (argc != 2 || strcmp(argv[1], "--help") == 0) {
        printf("Usage: %s <n>\n", argv[0]);
        exit(EXIT_FAILURE); // exit(1)
    }

    size = atoi(argv[1]);
//    if (num < 0) {
//        printf("the num is negative");
//        exit(1);
//    }

    numbers = malloc(sizeof(int) * size);
    if (numbers == NULL) {
        printf("fail to allocate memory");
        exit(EXIT_FAILURE);
    }
    for (index = 0; index < size; index++) {
//        printf("the num is %d\n", index);
        numbers[index] = index + 1;

    }

    transformElements(numbers, size, cube);

    for (index = 0; index < size; index++) {
        printf("the num is %d\n", numbers[index]);

    }

    free(numbers);

    // the integer return is the exit code
    exit(EXIT_SUCCESS);  // exit(0)
}