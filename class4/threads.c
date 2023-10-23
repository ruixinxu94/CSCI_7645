/**
 *
 * compile using: cc threads.c -o threads -pthread
* Main therad creates two child threads, T1 and T2
 * T1 keeps printing "Thread 1" 1,000,000 times
 * T2 pints "Thread 2" 1,000,000 times
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void* threadFunction(void* input) {
    int index, value;
    value = *(int*) input;
    for (index = 0; index < 1000000; index++) {
        printf("Thread %d\n", value);
    }
    return NULL;
}


int main(int argc, char** argv) {
    if (argv[1] == " -h" || argv[1] == "--help" || argc != 2) {
        printf("<usage>");
    }
    pthread_t thread1, thread2;
    int status;
    int input1, input2;

    void* result1;

    void* result2;
    input1 = 1;
    input2 = 2;
    status = pthread_create(&thread1, NULL, threadFunction, (void*) &input1);
    if (status != 0) {
        printf("pthread_create for the first thread failed.\n");
        exit(EXIT_FAILURE);
    }

    status = pthread_create(&thread2, NULL, threadFunction, (void*) &input2);
    if (status != 0) {
        printf("pthread_create for the second thread failed.\n");
        exit(EXIT_FAILURE);
    }
    status = pthread_join(thread1, &result1);

    if (status != 0) {
        printf("pthread_join for the first thread failed.\n");
        exit(EXIT_FAILURE);
    }

    status = pthread_join(thread2, &result2);
    if (status != 0) {
        printf("pthread_join for the first thread failed.\n");
        exit(EXIT_FAILURE);
    }


}