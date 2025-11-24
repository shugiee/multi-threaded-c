#include "../inc/threadpool.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void threadpool_function(void *threadpool) {
    threadpool_t *pool = (threadpool_t *)threadpool;

    while (1) {
        // Lock the `lock` field. We treat this as the lock for the whole object
        // though
        pthread_mutex_lock(&(pool->lock));

        // If there is nothing in the queue, wait to be notified
        while (pool->queue_size == 0 && !pool->stop) {
            // Until 'notify' is live, unlock the mutext and put our thread to
            // sleep
            pthread_cond_wait(&(pool->notify), &(pool->lock));
        }

        // Make sure the pool isn't stopped, since we did wait for some time
        if (pool->stop) {
            pthread_mutex_unlock(&(pool->lock));
            pthread_exit(NULL);
        }

        task_t task = pool->task_queue[pool->queue_front];
        pool->queue_front = (pool->queue_front + 1) & QUEUE_SIZE;
        pool->queued--;

        // We only need to lock when accessing shared data, i.e. grabbing a task
        // When executing the task, we don't need the lock! Relinquish it
        pthread_mutex_unlock(&(pool->lock));

        (*(task.fn))(task.arg);
    }
    return;
}

void threadpool_init(threadpool_t *pool) {
    pool->queued = 0;
    pool->queue_front = 0;
    pool->queue_back = 0;
    pool->stop = 0;

    pthread_mutex_init(&(pool->lock), NULL);
    pthread_cond_init(&(pool->notify), NULL);

    for (int i = 0; i < THREADS; i++) {
        pthread_create(&(pool->threads[i]), NULL, threadpool_function, pool);
    }
}

void threadpool_destroy(threadpool_t *pool) {
    pthread_mutex_lock(&(pool->lock));
    pool->stop = 1;

    // Use 'broadcast' to tell all threads that are currently waiting on that
    // conditional
    pthread_cond_broadcast(&(pool->notify));
    pthread_mutex_unlock(&(pool->lock));

    // Wait for all threads to exit
    // TODO: Why is it called 'join'?
    // I guess this is similar to a Promise.all to wrap things up
    for (int i = 0; i < THREADS; i++) {
        pthread_join(pool->threads[i], NULL);
    }
    pthread_mutex_destroy(&(pool->lock));
    pthread_cond_destroy(&(pool->notify));
}

void example_task(void *arg) {
    int *num = (int *)arg;
    printf("Processing task %d\n", *num);
    sleep(1);
    free(arg);
}

void threadpool_add_task(threadpool_t *pool, void (*function)(void *),
                         void *arg) {
    pthread_mutex_lock(&(pool->lock));

    int next_rear = (pool->queue_back + 1) % QUEUE_SIZE;
    if (pool->queued < QUEUE_SIZE) {
        pool->task_queue[pool->queue_back].fn = function;
        pool->task_queue[pool->queue_back].arg = arg;
        pool->queue_back = next_rear;
        pool->queued++;
        // Tell the next thread to go ahead
        pthread_cond_signal(&(pool->notify));
    } else {
        printf("Task queue is full! Cannot add more tasks.\n");
    }

    pthread_mutex_unlock(&(pool->lock));
}
