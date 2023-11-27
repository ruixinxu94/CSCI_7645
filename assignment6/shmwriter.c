#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 4 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
        printf("Usage: ./shmwriter <shared memory name> <semaphore name> <n1> <n2> ...");
        exit(EXIT_FAILURE);
    }

    const char *shm_name = argv[1];
    const char *sem_name = argv[2];
    int shmDescriptor;
    sem_t *mySemaphore;
    int num_values = argc - 3;

    mySemaphore = sem_open("/MySemaphore", O_CREAT, S_IRUSR | S_IWUSR, 1);
    if (mySemaphore == SEM_FAILED) {
        printf("Failed to create semaphore.\n");
        exit(EXIT_FAILURE);
    }

    /* Open the shared variable (create if it doesn't exist)*/
    shmDescriptor = shm_open(
            "/MySharedMemory", O_CREAT | O_RDWR, // cannot have O_WRONLY here
            S_IRUSR | S_IWUSR); // cannot have S_IXUSR
    if (shmDescriptor == -1) {
        printf("Failed to create shared memory.\n");
        exit(EXIT_FAILURE);
    }

    // Configure the size of the shared memory object
    if (ftruncate(shmDescriptor, num_values * sizeof(int)) == -1) {
        printf("Error configuring shared memory size\n");
        sem_close(mySemaphore);
        shm_unlink(shm_name);
        exit(EXIT_FAILURE);
    }

    int *shared_memory = mmap(NULL, num_values * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shmDescriptor, 0);
    if (shared_memory == MAP_FAILED) {
        printf("Error mapping shared memory\n");
        sem_close(mySemaphore);
        shm_unlink(shm_name);
        exit(EXIT_FAILURE);
    }

    sem_wait(mySemaphore);
    for (int i = 0; i < num_values; i++) {
        shared_memory[i] = atoi(argv[i + 3]);
    }
    sem_post(mySemaphore);

    munmap(shared_memory, num_values * sizeof(int));
    close(shmDescriptor);
    sem_close(mySemaphore);

    exit(EXIT_SUCCESS);
}
