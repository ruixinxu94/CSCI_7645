#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

volatile int globalVariable = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *increment(void *input) {
    int index, status;

    for (index = 0; index < 1000000; index++) {
        status = pthread_mutex_lock(&mutex);
        if (status != 0) {
            printf("mutex lock failed");
            exit(EXIT_FAILURE);
        }

        globalVariable++;

        status = pthread_mutex_unlock(&mutex);
        if (status != 0) {
            printf("mutex unlock failed, blocked state, program forced to quit");
            exit(EXIT_FAILURE);
        }

    }

    return NULL;
}

int main(int argc, char **argv) {
    if (argc != 2 || strcmp(argv[0], "-h") == 0 || strcmp(argv[0], "--help")) {
        printf("Usage: %s <number_of_threads>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int numThreads = atoi(argv[1]);
    pthread_t *threads = malloc(numThreads * sizeof(pthread_t));
    if (!threads) {
        printf("Failed to allocate memory for threads.\n");
        exit(EXIT_FAILURE);
    }
    int status;
    for (int i = 0; i < numThreads; i++) {
        status = pthread_create(&threads[i], NULL, increment, NULL);
        if (status != 0) {
            printf("Failed to create thread %d\n", i);
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < numThreads; i++) {
        status = pthread_join(threads[i], NULL);
        if (status != 0) {
            printf("Failed to join thread %d\n", i);
            exit(EXIT_FAILURE);
        }
    }

    printf("Final value of the global variable is %d\n", globalVariable);

    free(threads);
    exit(EXIT_SUCCESS);
}
