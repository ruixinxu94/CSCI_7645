#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

volatile int globalVariable = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct {
    int value;
    char operation[sizeof(char)];
} ThreadData;

void *cal(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    int value = data->value;
    char *operation = data->operation;
    int index, status;

    for (index = 0; index < value; index++) {
        status = pthread_mutex_lock(&mutex);
        if (status != 0) {
            printf("mutex lock failed\n");
            exit(EXIT_FAILURE);
        }
        if (strcmp("+", operation) == 0) {
            globalVariable++;
        } else {
            globalVariable--;
        }
        status = pthread_mutex_unlock(&mutex);
        if (status != 0) {
            printf("mutex unlock failed, blocked state, program forced to quit\n");
            exit(EXIT_FAILURE);
        }
    }
    return NULL;
}

int main(int argc, char **argv) {
    if (argc != 3 || strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
        printf("Usage: ./threads <n1> <n2>\n");
        exit(EXIT_FAILURE);
    }
    pthread_t thread1, thread2;

    ThreadData data1 = {atoi(argv[1]), "+"};
    ThreadData data2 = {atoi(argv[2]), "-"};

    int status;

    status = pthread_create(&thread1, NULL, cal, &data1);
    if (status != 0) {
        printf("failed to create the first thread\n");
        exit(EXIT_FAILURE);
    }

    status = pthread_create(&thread2, NULL, cal, &data2);
    if (status != 0) {
        printf("failed to create the second thread\n");
        exit(EXIT_FAILURE);
    }

    status = pthread_join(thread1, NULL);
    if (status != 0) {
        printf("failed to join the first thread\n");
        exit(EXIT_FAILURE);
    }

    status = pthread_join(thread2, NULL);
    if (status != 0) {
        printf("failed to join the second thread\n");
        exit(EXIT_FAILURE);
    }

    printf("Final value of globalVariable = %d\n", globalVariable);

    exit(EXIT_SUCCESS);
}
