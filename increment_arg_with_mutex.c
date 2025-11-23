#include <pthread.h>
#include <stdlib.h>

pthread_mutex_t thread_lock = PTHREAD_MUTEX_INITIALIZER;

void startup_function() { pthread_mutex_init(&thread_lock, NULL); }

// Since this is called by pthread, we have to have this signature! We cannot
// say the arg is an int pointer. It has to be a void *. So, we have to cast its
// type before incrementing. c has no generics, or else we might be able to
// express this
// void * is the only universal pointer type in c
void *thread_function(void *arg) {
    pthread_mutex_lock(&thread_lock);
    // arg is a void *, so we cannot simply dereference it; c wouldn't know what
    // it's pointing to First, cast it to an int, then dereference it and assign
    // a value
    int *num = (int *)arg;
    (*num)++;
    pthread_mutex_unlock(&thread_lock);
    return NULL;
}
