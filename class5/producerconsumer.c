/**
 * this is to show the producer-consumer problem, busy waiting
* MAX_BUFFER_SIZE is the maximum number of items in the buffer
 * numberOfUnits is the current number if items in the buffer
 * REQUIREMENTS:
 * (1) numberOfUnits must be protected by mutual exclusion
 * (2)numberOfUnits cannot be negative i.e., no underflow
 *  (3)numberOfUnits cannot be greater than MAX_BUFFER_SIZE
 *
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define FALSE 0
#define TRUE 1
#define MAX_BUFFER_SIZE 5

volatile int numberOfUnits = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *producer(void *input) {
    int status;
    while (TRUE) {
        status = pthread_mutex_lock(&mutex);
        if (status != 0) {
            printf("producer failed to lock mutext.\n");
            exit(EXIT_FAILURE);
        }

        if (numberOfUnits < MAX_BUFFER_SIZE) {
            numberOfUnits++;
            printf("producer increased numberOfUnits to %d\n", numberOfUnits);
        } else {
            printf("producer is waiting\n");
        }

        status = pthread_mutex_unlock(&mutex);
        if (status != 0) {
            printf("producer failed to unlock mutext.\n");
            exit(EXIT_FAILURE);
        }
        sleep(2);
    }
    return NULL;
}


void *consumer(void *input) {
    int status;
    while (TRUE) {
        status = pthread_mutex_lock(&mutex);
        if (status != 0) {
            printf("consumer failed to lock mutext.\n");
            exit(EXIT_FAILURE);
        }
        if (numberOfUnits > 0) {
            numberOfUnits--;
            printf("consumer decreased numberOfUnits to %d\n", numberOfUnits);
        } else {
            printf("consumer is waiting\n");
        }
        status = pthread_mutex_unlock(&mutex);
        if (status != 0) {
            printf("consumer failed to unlock mutext.\n");
            exit(EXIT_FAILURE);
        }
        sleep(1);
    }
    return NULL;
}

int main(int argc, char **argv) {
    pthread_t thread1, thread2;

    void *result;
    /** return value **/
    int status;


    /** Create the first thread **/
    status = pthread_create(&thread1, NULL, producer, NULL);
    if (status != 0) {
        printf("failed to create the producer thread\n");
        exit(EXIT_FAILURE);
    }

    /** Create the second thread **/
    status = pthread_create(&thread2, NULL, consumer, NULL);
    if (status != 0) {
        printf("failed to create the consumer thread\n");
        exit(EXIT_FAILURE);
    }

    /**
     * Wait for the first thread to complete
     */
    status = pthread_join(thread1, &result);
    if (status != 0) {
        printf("failed to join the consumer thread\n");
        exit(EXIT_FAILURE);
    }

    /**
 * Wait for the second thread to complete
 */
    status = pthread_join(thread2, &result);
    if (status != 0) {
        printf("failed to join the producer thread\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);

}



