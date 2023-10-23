#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "func.h"

typedef struct {
    int id;         // Unique identifier
    int num_loops;  // Number of times to increment glob
} ThreadArgs;

static int glob = 0;

static void *threadFunc(void *arg) {
    ThreadArgs *args = (ThreadArgs *)arg;
    int loc, j;

    for (j = 0; j < args->num_loops; j++) {
        loc = glob;
        loc++;
        glob = loc;

        printf("THREAD ID: %d, new value of glob: %d\n", args->id, glob);

    }

    printf("Thread %d finished. Glob is now %d\n", args->id, glob);
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t t1, t2;
    int loops, s;

    loops = (argc > 1) ? getInt(argv[1], GN_GT_0, "num-loops") : 10000000;

    ThreadArgs args1 = {1, loops};
    ThreadArgs args2 = {2, loops};

    s = pthread_create(&t1, NULL, threadFunc, &args1);
    if (s != 0)
        errExitEN(s, "pthread_create");

    s = pthread_create(&t2, NULL, threadFunc, &args2);
    if (s != 0)
        errExitEN(s, "pthread_create");

    s = pthread_join(t1, NULL);
    if (s != 0)
        errExitEN(s, "pthread_join");

    s = pthread_join(t2, NULL);
    if (s != 0)
        errExitEN(s, "pthread_join");

    printf("Final glob = %d\n", glob);
    exit(EXIT_SUCCESS);
}
