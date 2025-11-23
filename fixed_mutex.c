#include <pthread.h>
#include <stdio.h>

#define THREAD_COUNT 10

int counter = 0;
pthread_mutex_t counter_lock = PTHREAD_MUTEX_INITIALIZER;

// This constitutes a "critical section" where if multiple threads try to mutate
// data at the same time, things can go critically wrong
void *thread_target(void *args) {
    // 1 - Claim the mutex
    //      - If another thread tries to call this, it'll wait until it can
    //        claim the mutex
    //      - We cannot control which thread gets here first. If it prints the
    //        right order in this case, it's just luck
    //      - This approach has SIGNIFICANT performance implications due to
    //      waiting, "Starvation"
    //      - When waiting, the OS will put that thread to sleep, allowing it to
    //      do other things
    //      - When it sleeps, it invalidates the cache. When coming back online,
    //      it misses the cache and has to repopulate
    //      - Ideally, we'll have as few memory-access points as possible. Do
    //      all the work independently, then finally write to a value!
    pthread_mutex_lock(&counter_lock);

    for (int i = 0; i < 1000000; i++) {
        counter += 1;
    }

    pthread_mutex_unlock(&counter_lock);

    printf("Counter is %d\n", counter);
}

int main(int argc, char **argv) {
    pthread_t threads[THREAD_COUNT];

    pthread_mutex_init(&counter_lock, NULL);

    int i = 0;
    for (i = 0; i < THREAD_COUNT; i++) {
        if (pthread_create(&threads[i], NULL, thread_target, NULL)) {
            return -1;
        }
    }

    for (i = 0; i < THREAD_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
