#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define _GNU_SOURCE
#include <errno.h>
#include <sched.h>

void *thread_function(void *arg) {
    pthread_t tid = pthread_self();
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);

    if (pthread_getaffinity_np(tid, sizeof(cpu_set_t), &cpuset) == 0) {
        printf("Thread %lu running on CPUs: ", (unsigned long)tid);
        for (int i = 0; i < CPU_SETSIZE; i++) {
            if (CPU_ISSET(i, &cpuset)) {
                printf("%d", i);
            }
        }
        printf("\n");
    }
    return NULL;
}

int main(void) {
    pthread_t thread;
    pthread_attr_t attr;
    cpu_set_t cpuset;
    struct sched_param param;

    pthread_attr_init(&attr);
    pthread_attr_setstacksize(&attr, 1024 * 1024);
    pthread_attr_setschedpolicy(&attr, SCHED_RR);
    param.sched_priority = sched_get_priority_max(SCHED_RR);
    pthread_attr_setschedparam(&attr, &param);
    pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);

    CPU_ZERO(&cpuset);
    CPU_SET(0, &cpuset);
    pthread_attr_setaffinity_np(&attr, sizeof(cpu_set_t), &cpuset);

    if (pthread_create(&thread, &attr, thread_function, NULL) != 0) {
        perror("Failed to create thread");
        return 1;
    }

    pthread_join(thread, NULL); // wait for the thread to print
    return 0;
}
