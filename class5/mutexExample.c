/**
 * Chapter 30
* Goal is see an exmaple of mutex to protect the critical section
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

volatile int globalVariable = 0;
// creates the mutex varaible and sets the varaible to unclocked
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *increment(void *input) {
    int index, status;

    for (index = 0; index < 1000000; index++) {
        /** lock the mutex **/
        status = pthread_mutex_lock(&mutex);
        if (status != 0) {
            printf("mutex lock failed");
            exit(EXIT_FAILURE);
        }

        /** critical section **/
        globalVariable++;

        /** unlock the mutex **/
        status = pthread_mutex_unlock(&mutex);
        if (status != 0) {
            printf("mutex unlock failed, blocked state, program forced to quit");
            exit(EXIT_FAILURE);
        }
    }
    return NULL;

}

int main(int argc, char **argv) {
    pthread_t thread1, thread2;

    void *result;
    /** return value **/
    int status;


    /** Create the first thread **/
    status = pthread_create(&thread1, NULL, increment, NULL);
    if (status != 0) {
        printf("failed to create the first thread");
        exit(EXIT_FAILURE);
    }

    /** Create the second thread **/
    status = pthread_create(&thread2, NULL, increment, NULL);
    if (status != 0) {
        printf("failed to create the second thread");
        exit(EXIT_FAILURE);
    }

    /**
     * Wait for the first thread to complete
     */
    status = pthread_join(thread1, &result);
    if (status != 0) {
        printf("failed to join the first thread");
        exit(EXIT_FAILURE);
    }

    /**
 * Wait for the second thread to complete
 */
    status = pthread_join(thread2, &result);
    if (status != 0) {
        printf("failed to join the second thread");
        exit(EXIT_FAILURE);
    }

    printf("final value of the global variable is %d\n", globalVariable);

    exit(EXIT_SUCCESS);


}