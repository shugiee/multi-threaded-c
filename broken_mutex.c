#include <pthread.h>
#include <stdio.h>

#define THREAD_COUNT 10

int counter = 0;

void *thread_target(void *vargs) {
    for (int i = 0; i < 1000000; i++) {
        counter += 1;
    }

    printf("Counter is %d\n", counter);
}

int main(int argc, char **argv) {
    pthread_t threads[THREAD_COUNT];

    int i = 0;
    for (i = 0; i < THREAD_COUNT; i++) {
        // Note that we're not doing anything here to lock the value, each
        // thread can access the variable willy nilly They each add a copy of it
        // to their register, add to it, then set it back, with no timing
        // restrictions
        // Ways to counter this:
        //     1 - Mutex: mutual exclusion
        //     2 - Semaphore: mulitiple concurrent access
        if (pthread_create(&threads[i], NULL, thread_target, NULL)) {
            return -1;
        }
    }

    for (i = 0; i < THREAD_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
