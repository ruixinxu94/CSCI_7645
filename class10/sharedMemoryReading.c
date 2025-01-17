/*
 * Read the value of the shared variable /MySharedMemory
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include <semaphore.h>

int main(int argc, char *argv[]) {
    int shmDescriptor;
    int status;
    void *addr;
    int *pointerToSharedVariable;
    int valueOfSharedVariable;
    struct stat shmMetadata;
    sem_t *mySemaphore;
    char* sharedMemoryName = "/MySharedMemory2";
    char* semaphoreName = "/MySemaphore2";

    /* Open the existing semaphore */
    mySemaphore = sem_open(semaphoreName, 0);
    if (mySemaphore == SEM_FAILED) {
        printf("Failed to open semaphore.\n");
        exit(EXIT_FAILURE);
    }

    /* Open the shared variable for reading */
    shmDescriptor = shm_open(sharedMemoryName,  O_RDONLY, 0);
    if (shmDescriptor == -1) {
        printf("Failed to open shared memory.\n");
        exit(EXIT_FAILURE);
    }

    /* Get metadata about the shared variable */
    status = fstat(shmDescriptor, &shmMetadata);
    if (status == -1) {
        printf("Failed to get size of the shared memory.\n");
        exit(EXIT_FAILURE);
    }

    /* Map this shared memory into my address space */
    addr = mmap(
            NULL, shmMetadata.st_size, PROT_READ, MAP_SHARED, shmDescriptor, 0);
    if (addr == MAP_FAILED) {
        printf("Failed to map shared memory into address space.\n");
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
        printf("Failed to lock semaphore.\n");
        /* Cleanup before exiting */
        munmap(addr, shmMetadata.st_size);
        sem_close(mySemaphore);
        exit(EXIT_FAILURE);
    }
    valueOfSharedVariable = *pointerToSharedVariable; // CRITICAL SECTION
    status = sem_post(mySemaphore);
    if (status != 0) {
        printf("Failed to unlock semaphore.\n");
        /* Cleanup before exiting */
        munmap(addr, shmMetadata.st_size);
        sem_close(mySemaphore);
        exit(EXIT_FAILURE);
    }

    status = sem_close(mySemaphore);
    if (status != 0) {
        printf("Failed to close semaphore.\n");
        exit(EXIT_FAILURE);
    }

    munmap(addr, shmMetadata.st_size);

    printf("The value of the shared memory = %d\n", valueOfSharedVariable);
    exit(EXIT_SUCCESS);
}