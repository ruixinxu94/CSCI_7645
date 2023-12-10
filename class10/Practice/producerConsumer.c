#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>

#define MAX_MESSAGES 1024
#define SHM_NAME "/mySharedMemory1"
#define MUTEX_NAME "/my_mutex_semaphore1"
#define EMPTY_NAME "/my_empty_semaphore1"
#define FULL_NAME "/my_full_semaphore1"

sem_t *mutex, *empty, *full;
int *numMessages;

void cleanup() {
    munmap(numMessages, sizeof(int));
    shm_unlink(SHM_NAME);
    sem_close(mutex);
    sem_unlink(MUTEX_NAME);
    sem_close(empty);
    sem_unlink(EMPTY_NAME);
    sem_close(full);
    sem_unlink(FULL_NAME);
}

void producer() {
    while (1) {
        sem_wait(empty);
        sem_wait(mutex);

        if (*numMessages < MAX_MESSAGES) {
            (*numMessages)++;
            printf("Producer increased: %d\n", *numMessages);
        }

        sem_post(mutex);
        sem_post(full);
    }
}

void consumer() {
    while (1) {
        sem_wait(full);
        sem_wait(mutex);

        if (*numMessages > 0) {
            (*numMessages)--;
            printf("Consumer decreased: %d\n", *numMessages);
        }

        sem_post(mutex);
        sem_post(empty);
    }
}
int main() {
    int shmDescriptor;
    pid_t pid;

    shmDescriptor = shm_open(SHM_NAME, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (shmDescriptor == -1) {
        printf("Failed to open shared memory\n");
        exit(EXIT_FAILURE);
    }

    if (ftruncate(shmDescriptor, sizeof(int)) == -1) {
        printf("Failed to set size of the shared memory\n");
        close(shmDescriptor);
        exit(EXIT_FAILURE);
    }

    numMessages = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shmDescriptor, 0);
    if (numMessages == MAP_FAILED) {
        printf("Failed to map shared memory\n");
        close(shmDescriptor);
        exit(EXIT_FAILURE);
    }
    *numMessages = 0;

    mutex = sem_open(MUTEX_NAME, O_CREAT, S_IRUSR | S_IWUSR, 1);
    if (mutex == SEM_FAILED) {
        printf("Failed to open mutex semaphore\n");
        cleanup();
        exit(EXIT_FAILURE);
    }

    empty = sem_open(EMPTY_NAME, O_CREAT, S_IRUSR | S_IWUSR, MAX_MESSAGES);
    if (empty == SEM_FAILED) {
        printf("Failed to open empty semaphore\n");
        cleanup();
        exit(EXIT_FAILURE);
    }

    full = sem_open(FULL_NAME, O_CREAT, S_IRUSR | S_IWUSR, 0);
    if (full == SEM_FAILED) {
        printf("Failed to open full semaphore\n");
        cleanup();
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid == -1) {
        printf("Failed to fork process\n");
        cleanup();
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        consumer();
        exit(EXIT_SUCCESS);
    } else {
        producer();
        wait(NULL);
        cleanup();
    }

    close(shmDescriptor);
    exit(EXIT_SUCCESS);
}
