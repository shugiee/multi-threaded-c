#include <pthread.h>
#include <stdio.h>

#define THREAD_COUNT 10

void *thread_target(void *arg) { printf("Hello from thread!\n"); }

int main(int argc, char **argv) {
    pthread_t threads[THREAD_COUNT];

    int i = 0;
    for (i = 0; i < THREAD_COUNT; i++) {
        // Create a new thread
        if (pthread_create(&threads[i], NULL, thread_target, NULL)) {
            return -1;
        }
    }

    for (i = 0; i < THREAD_COUNT; i++) {
        // Wait for the thread to finish; without this, the main program may
        // exit before threads complete
        pthread_join(threads[i], NULL);
    }

    return 0;
}
