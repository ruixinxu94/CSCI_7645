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
    sem_t *mySemaphore;
    Person *sharedPeople;

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

    sharedPeople = (Person *) mmap(NULL, sizeof(Person) * PERSON_COUNT, PROT_READ, MAP_SHARED, shmDescriptor, 0);
    if (sharedPeople == MAP_FAILED) {
        printf("Failed to map shared memory.\n");
        exit(EXIT_FAILURE);
    }

    if (sem_wait(mySemaphore) != 0) {
        printf("Failed to wait semaphore.\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < PERSON_COUNT; i++) {
        printf("Person %d: %s %s\n", i, sharedPeople[i].firstName, sharedPeople[i].lastName);
    }
    if (sem_post(mySemaphore) != 0) {
        printf("Failed to unlock semaphore.\n");
        exit(EXIT_FAILURE);
    }

    if (munmap(sharedPeople, sizeof(Person) * PERSON_COUNT) == -1) {
        printf("Failed to unmap shared memory.\n");
        exit(EXIT_FAILURE);
    }

    if (close(shmDescriptor) == -1) {
        printf("Failed to close shared memory descriptor.\n");
        exit(EXIT_FAILURE);
    }

    if (sem_close(mySemaphore) == -1) {
        printf("Failed to close semaphore.\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
