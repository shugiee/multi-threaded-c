#include <pthread.h>
#include <stdio.h>

#define THREAD_COUNT 10

typedef struct {
    int arg1;
    short arg2;
} thread_arg_t;

// How to pass args to threads!
// Note how we have to use void *, then cast that into a pointer of the type we
// want
void *thread_target(void *vargs) {
    thread_arg_t *args = (thread_arg_t *)vargs;
    printf("Hello from thread %d\n", args->arg1);
}

int main(int argc, char **argv) {
    pthread_t threads[THREAD_COUNT];

    thread_arg_t myargs;

    int i = 0;
    for (i = 0; i < THREAD_COUNT; i++) {
        myargs.arg1 = i;
        // Initialize threads
        if (pthread_create(&threads[i], NULL, thread_target, &myargs)) {
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
