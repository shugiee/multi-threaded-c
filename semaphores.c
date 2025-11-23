#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

// Mutex -> mutual exclusion (access)
// Semaphore -> multiple access (signaling)

sem_t sem;

void *signal_event(void *arg) {
    printf("Doing some work\n...");
    sleep(2);
    printf("Signaling event completion\n");

    // Add a unit to the semaphore
    sem_post(&sem);

    return NULL;
}

void *wait_for_event(void *arg) {
    printf("Waiting for event...\n");

    // Wait for an available unit; since we started with 0, this means we're
    // waiting for signal_event to add one to the semaphore
    sem_wait(&sem);
    printf("Event has been triggered!\n");

    return NULL;
}

int main(int argc, char **argv) {
    pthread_t t1, t2;

    // Start the semaphore with 0 available units
    sem_init(&sem, 0, 0);

    pthread_create(&t1, NULL, wait_for_event, NULL);
    pthread_create(&t2, NULL, signal_event, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    sem_destroy(&sem);

    return 0;
}
