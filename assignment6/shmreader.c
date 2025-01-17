#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 3 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
        printf("Usage: ./shmreader <shared memory name> <semaphore name>\n");
        exit(EXIT_FAILURE);
    }

    const char *sharedMemoryName = argv[1];
    const char *semaphoreName = argv[2];

    sem_t *sem = sem_open(semaphoreName, 0);
    if (sem == SEM_FAILED) {
        printf("Error opening semaphore\n");
        exit(EXIT_FAILURE);
    }

    int shmDescriptor = shm_open(sharedMemoryName, O_RDONLY, 0);
    if (shmDescriptor == -1) {
        printf("Error opening shared memory\n");
        sem_close(sem);
        exit(EXIT_FAILURE);
    }

    struct stat shmMetadata;
    if (fstat(shmDescriptor, &shmMetadata) == -1) {
        printf("Error getting shared memory size\n");
        sem_close(sem);
        close(shmDescriptor);
        exit(EXIT_FAILURE);
    }

    int *shared_memory = mmap(NULL, shmMetadata.st_size,
                              PROT_READ, MAP_SHARED, shmDescriptor, 0);
    if (shared_memory == MAP_FAILED) {
        printf("Error mapping shared memory\n");
        sem_close(sem);
        close(shmDescriptor);
        exit(EXIT_FAILURE);
    }

    sem_wait(sem);
    int num_values = shared_memory[0];
    for (int i = 0; i < num_values; i++) {
        printf("%d\n", shared_memory[i + 1]);
    }
    sem_post(sem);

    munmap(shared_memory, shmMetadata.st_size);
    close(shmDescriptor);
    sem_close(sem);

    exit(EXIT_SUCCESS);
}
