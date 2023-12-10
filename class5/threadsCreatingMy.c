#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

void* threadFunction(void* input) {
    int index, value;
    value = *(int*) input;
    for (index = 0; index < 10000; index++) {
        printf("Thread %d\n", value);
    }
    return NULL;
}

int main(int argc, char** argv) {
    if (argc != 2 || strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
        printf("<usage>: %s <number_of_threads>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    int numThread = atoi(argv[1]);
    pthread_t threads[numThread];
    int thread_args[numThread];

    for (int i = 0; i < numThread; i++) {
        thread_args[i] = i;
        int status = pthread_create(&threads[i], NULL, threadFunction, &thread_args[i]);
        if (status != 0) {
            printf("pthread_create for thread %d failed.\n", i);
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < numThread; i++) {
        int status = pthread_join(threads[i], NULL);
        if (status != 0) {
            printf("pthread_join for thread %d failed.\n", i);
            exit(EXIT_FAILURE);
        }
    }
    exit(EXIT_SUCCESS);
}
