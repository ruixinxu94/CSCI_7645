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
        printf("Usage: ./shmreader <shared memory name> <semaphore name>");
        exit(EXIT_FAILURE);
    }

    const char *shm_name = argv[1];
    const char *sem_name = argv[2];

    sem_t *sem = sem_open(sem_name, 0);
    if (sem == SEM_FAILED) {
        printf("Error opening semaphore\n");
        exit(EXIT_FAILURE);
    }

    int shmDescriptor = shm_open(shm_name, O_RDONLY, 0);
    if (shmDescriptor == -1) {
        printf("Error opening shared memory\n");
        sem_close(sem);
        exit(EXIT_FAILURE);
    }

    struct stat sb;
    if (fstat(shmDescriptor, &sb) == -1) {
        printf("Error getting shared memory size\n");
        sem_close(sem);
        close(shmDescriptor);
        exit(EXIT_FAILURE);
    }

    int *shared_memory = mmap(NULL, sb.st_size, PROT_READ, MAP_SHARED, shmDescriptor, 0);
    if (shared_memory == MAP_FAILED) {
        printf("Error mapping shared memory\n");
        sem_close(sem);
        close(shmDescriptor);
        exit(EXIT_FAILURE);
    }

    sem_wait(sem);
    int num_values = sb.st_size / sizeof(int);
    for (int i = 0; i < num_values; i++) {
        printf("%d\n", shared_memory[i]);
    }
    sem_post(sem);

    // Cleanup
    munmap(shared_memory, sb.st_size);
    close(shmDescriptor);
    sem_close(sem);

    exit(EXIT_SUCCESS);
}
