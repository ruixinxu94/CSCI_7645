#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <semaphore.h>

#define NAME_SIZE 20
#define PERSON_COUNT 10

typedef struct {
    char firstName[NAME_SIZE];
    char lastName[NAME_SIZE];
} Person;

int main(int argc, char *argv[]) {
    int shmDescriptor;
    int status;
    void *addr;
    sem_t *mySemaphore;
    struct stat shmMetadata;
    int *pointerToSharedVariable;

    mySemaphore = sem_open("/MySemaphore", 0);
    if (mySemaphore == SEM_FAILED) {
        printf("Failed to open semaphore.\n");
        exit(EXIT_FAILURE);
    }

    shmDescriptor = shm_open("/MySharedMemory", O_RDONLY, 0);
    if (shmDescriptor == -1) {
        printf("Failed to open shared memory.\n");
        exit(EXIT_FAILURE);
    }

    status = fstat(shmDescriptor, &shmMetadata);
    if (status == -1) {
        printf("Failed to get size of the shared memory.\n");
        exit(EXIT_FAILURE);
    }

    addr = (Person *) mmap(NULL, shmMetadata.st_size,
                           PROT_READ, MAP_SHARED, shmDescriptor, 0);
    if (addr == MAP_FAILED) {
        printf("Failed to map shared memory.\n");
        exit(EXIT_FAILURE);
    }

    status = close(shmDescriptor);
    if (status == -1) {
        printf("Failed to close shared memory.\n");
        exit(EXIT_FAILURE);

    }
    pointerToSharedVariable = (int *) addr;
    status = sem_wait(mySemaphore);
    if (status != 0) {
        printf("Failed to wait semaphore.\n");
        exit(EXIT_FAILURE);
    }
    Person *person = (Person *) pointerToSharedVariable;

    status = sem_post(mySemaphore);
    if (status != 0) {
        printf("Failed to unlock semaphore.\n");
        exit(EXIT_FAILURE);
    }

    status = sem_close(mySemaphore);
    if (status == -1) {
        printf("Failed to close semaphore.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < PERSON_COUNT; i++) {
        printf("Person %d: %s %s\n", i, person[i].firstName, person[i].lastName);
    }

    exit(EXIT_SUCCESS);
}
