#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>

#define MAX_MESSAGES 1024
#define SHM_NAME "/MySharedMemory"
#define MUTEX_NAME "/my_mutex_semaphore"
#define EMPTY_NAME "/my_empty_semaphore"
#define FULL_NAME "/my_full_semaphore"

sem_t *mutex, *empty, *full;
int *numMessages;

void producer() {
    while (1) {
        if (sem_wait(empty) != 0) {
            printf("Failed to wait on empty semaphore.\n");
            exit(EXIT_FAILURE);
        }
        if (sem_wait(mutex) != 0) {
            printf("Failed to wait on mutex semaphore.\n");
            exit(EXIT_FAILURE);
        }

        if (*numMessages < MAX_MESSAGES) {
            (*numMessages)++;
            printf("producer increased by 1: %d", *numMessages);
        }

        if (sem_post(mutex) != 0) {
            printf("Failed to post to mutex semaphore.\n");
            exit(EXIT_FAILURE);
        }
        if (sem_post(full) != 0) {
            printf("Failed to post to full semaphore.\n");
            exit(EXIT_FAILURE);
        }
    }
}

void consumer() {
    while (1) {
        if (sem_wait(full) != 0) {
            printf("Failed to wait on full semaphore.\n");
            exit(EXIT_FAILURE);
        }
        if (sem_wait(mutex) != 0) {
            printf("Failed to wait on mutex semaphore.\n");
            exit(EXIT_FAILURE);
        }

        if (*numMessages > 0) {
            (*numMessages)--;
            printf("consumer decreased by 1: %d\n", *numMessages);
        }

        if (sem_post(mutex) != 0) {
            printf("Failed to post to mutex semaphore.\n");
            exit(EXIT_FAILURE);
        }
        if (sem_post(empty) != 0) {
            printf("Failed to post to empty semaphore.\n");
            exit(EXIT_FAILURE);
        }
    }
}


int main() {
    int shmDescriptor;
    pid_t pid;

    // Create shared memory
    shmDescriptor = shm_open(SHM_NAME, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (shmDescriptor == -1) {
        printf("Failed to open shared memory.\n");
        exit(EXIT_FAILURE);
    }
    if (ftruncate(shmDescriptor, sizeof(int)) == -1) {
        printf("Failed to set size of the shared memory.\n");
        exit(EXIT_FAILURE);
    }

    numMessages = mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shmDescriptor, 0);
    if (numMessages == MAP_FAILED) {
        printf("Failed to map shared memory.\n");
        exit(EXIT_FAILURE);
    }
    *numMessages = 0;

    // Initialize semaphores
    mutex = sem_open(MUTEX_NAME, O_CREAT, S_IRUSR | S_IWUSR, 1);
    empty = sem_open(EMPTY_NAME, O_CREAT, S_IRUSR | S_IWUSR, MAX_MESSAGES);
    full = sem_open(FULL_NAME, O_CREAT, S_IRUSR | S_IWUSR, 0);

    if (mutex == SEM_FAILED || empty == SEM_FAILED || full == SEM_FAILED) {
        printf("Failed to open semaphores.\n");
        exit(EXIT_FAILURE);
    }

    // Forking processes
    pid = fork();
    if (pid == -1) {
        printf("Failed to fork process.\n");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Child process - Consumer
        consumer();
        exit(EXIT_SUCCESS);
    } else {
        // Parent process - Producer
        producer();
        wait(NULL); // Wait for the child process to finish
    }

    if (close(shmDescriptor) != 0) {
        printf("Failed to close shared memory file descriptor.\n");
        exit(EXIT_FAILURE);
    }
    if (sem_close(mutex) != 0 || sem_close(empty) != 0 || sem_close(full) != 0) {
        printf("Failed to close semaphores.\n");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}
