#include <stdio.h>
#include <pthread.h>

#define BUFFER_SIZE 1000

int buffer[BUFFER_SIZE];
int count = 0; // number of items in the buffer

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *producer(void *arg) {
    for (int i = 0; i < 1000000; i++) {
        pthread_mutex_lock(&mutex);

        while (count == BUFFER_SIZE) { // buffer is full
            pthread_cond_wait(&cond, &mutex); // wait for the buffer to have space
        }

        buffer[count] = i; // produce an item
        printf("Produced %d\n", i);
        count++;

        pthread_cond_signal(&cond); // signal the consumer that an item is produced
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void *consumer(void *arg) {
    for (int i = 0; i < 1000000; i++) {
        pthread_mutex_lock(&mutex);

        while (count == 0) { // buffer is empty
            pthread_cond_wait(&cond, &mutex); // wait for an item to be produced
        }

        int item = buffer[count - 1]; // consume an item
        printf("Consumed %d\n", item);
        count--;

        pthread_cond_signal(&cond); // signal the producer that an item is consumed
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    pthread_t prodThread, consThread;

    pthread_create(&prodThread, NULL, producer, NULL);
    pthread_create(&consThread, NULL, consumer, NULL);

    pthread_join(prodThread, NULL);
    pthread_join(consThread, NULL);

    return 0;
}
