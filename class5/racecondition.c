/**
* Goal is to see what a race condition looks like
 * a race condition when critical sections happen
 * and how to prevent it
 * Compile using: cc racecondition.c -o racecondition -pthread
 * Run using ./racecondition
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// telling the compiler is not to compile it
// reading the variable being updated at one functions
// declaring it outside any functions
/**
 * two threads will both increment this varaibale 1000,000 times
 * the two threads would accecss to the variable at the same time
 */
volatile int globalVariable = 0;

//int globalVariable = 0;

void *increment(void *input) {
    int index;

    for (index = 0; index < 1000000; index++) {
        globalVariable++;

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