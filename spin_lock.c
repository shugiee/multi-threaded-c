#include <pthread.h>
#include <stdio.h>

#define THREAD_COUNT 10

int counter = 0;
pthread_spinlock_t counter_lock;

/**
 * A spin lock is very similar to a mutex
 *
 * With a mutex, you starve until you get the lock and thread goes to sleep.
 * With a spinlock, instead of going to sleep, it loops until the lock is
 * available.
 *
 * So, this has unique usecases where you're really trying to prevent sleep
 * overhead, so I imagine it's best on high-throughput logic, where the wait
 * time is near-zero
 */

void *thread_target(void *vargs) {

    // 1 thread gets to pass
    // 9 threads that are in starvation
    pthread_spin_lock(&counter_lock);

    for (int i = 0; i < 1000000; i++) {
        counter += 1;
    }

    pthread_spin_unlock(&counter_lock);

    printf("Counter is \d\n", counter);

    return NULL;
}

int main(int argc, char **argv) {
    pthread_t threads[THREAD_COUNT];

    pthread_spin_init(&counter_lock, NULL);

    int i = 0;
