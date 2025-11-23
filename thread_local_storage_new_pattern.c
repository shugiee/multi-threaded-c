#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

// __thread is a keyword
// Creates a global variable that's unique to a thread
// Main diff is that thread keywords is handled at compile-time
__thread int x_thd;

void *wait_for_event(void *arg) {
    x_thd++;
    printf("%d\n", x_thd);
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t threads[10];

    for (int i = 0; i < 10; i++) {
        pthread_create(&threads[i], NULL, wait_for_event, NULL);
    }

    for (int i = 0; i < 10; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
