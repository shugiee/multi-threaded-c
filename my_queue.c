#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 3

// Why do I have to include the * before the function name?
void *thread_function(void *arg) {
    int *idx = (int *)arg;
    printf("Hello from thread #%d\n", *idx);
    sleep(1);
    free(arg);
    return NULL;
}

/**
 * This passes &i to each thread, so that they can log their index.
 * This isn't great, since all threads are getting the same pointer! So, we'll
 * get weird access and weird logs
 */
int thread_init_naive() {
    pthread_t threads[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        int *idx = malloc(sizeof(int));
        *idx = i;
        pthread_create(&threads[i], NULL, thread_function, idx);
        printf("Created naive thread #%d\n", i);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
        printf("Joined naive thread #%d\n", i);
    }
    return 0;
}

/**
 * Allocates a new value to pass to each thread, so they don't all get the same
 * pointer. This is perfectly valid
 */
int thread_init_reallocate() {
    pthread_t threads[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        int *idx = malloc(sizeof(int));
        *idx = i;
        pthread_create(&threads[i], NULL, thread_function, idx);
        printf("Created thread #%d\n", i);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
        printf("Joined thread #%d\n", i);
    }
    return 0;
}

int main() {
    thread_init_naive();
    thread_init_reallocate();
    return 0;
}
