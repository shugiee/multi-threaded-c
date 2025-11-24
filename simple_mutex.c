#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 10

int counter = 0;
pthread_mutex_t lock;

void *thread_function(void *arg) {
    pthread_mutex_lock(&lock);
    printf("Thread #%d\n", counter++);
    sleep(1);
    pthread_mutex_unlock(&lock);
    return NULL;
}

/**
 * Allocates a new value to pass to each thread, so they don't all get the same
 * pointer. This is perfectly valid
 */
int thread_init() {
    pthread_t threads[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, thread_function, NULL);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    return 0;
}

int main() {
    pthread_mutex_init(&lock, NULL);
    thread_init();
    return 0;
}
