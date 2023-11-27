#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
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
    Person *sharedPeople;
    sem_t *mySemaphore;

    mySemaphore = sem_open("/MySemaphore", O_CREAT, S_IRUSR | S_IWUSR, 1);
    if (mySemaphore == SEM_FAILED) {
        printf("Failed to create semaphore.\n");
        exit(EXIT_FAILURE);
    }
    shmDescriptor = shm_open("/MySharedMemory", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (shmDescriptor == -1) {
        printf("Failed to create shared memory.\n");
        exit(EXIT_FAILURE);
    }

    status = ftruncate(shmDescriptor, sizeof(Person) * PERSON_COUNT);
    if (status == -1) {
        printf("Failed to set the size of the shared memory.\n");
        exit(EXIT_FAILURE);
    }

    addr = mmap(NULL, sizeof(Person) * PERSON_COUNT, PROT_READ | PROT_WRITE, MAP_SHARED, shmDescriptor, 0);
    if (addr == MAP_FAILED) {
        printf("Failed to map shared memory into address space.\n");
        exit(EXIT_FAILURE);
    }

    sharedPeople = (Person *) addr;

    status = sem_wait(mySemaphore);
    if (status != 0) {
        printf("Failed to lock semaphore.\n");
        exit(EXIT_FAILURE);
    }

    // Writing sample data to shared memory
    for (int i = 0; i < PERSON_COUNT; i++) {
        snprintf(sharedPeople[i].firstName, NAME_SIZE, "FirstName%d", i);
        snprintf(sharedPeople[i].lastName, NAME_SIZE, "LastName%d", i);
    }

    status = sem_post(mySemaphore);
    if (status != 0) {
        printf("Failed to unlock semaphore.\n");
        exit(EXIT_FAILURE);
    }

    // Close the shared memory descriptor and semaphore
    status = close(shmDescriptor);
    if (status == -1) {
        printf("Failed to close shared memory.\n");
        exit(EXIT_FAILURE);
    }

    status = sem_close(mySemaphore);
    if (status != 0) {
        printf("Failed to close semaphore.\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
