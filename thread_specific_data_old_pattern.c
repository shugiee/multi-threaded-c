#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// Thread-Specific Data (TSD)

// Global key
// The value stored under the key can differ per thread
// Think of it as a global dictionary
pthread_key_t key;

void array_destructor(void *arr) {
    free(arr);
    printf("Array freed for a thead\n");
}

void *thread_function(void *arg) {
    int *my_array = (int *)malloc(10 * sizeof(int));

    // For me (thread) treat the value at key as my_array
    pthread_setspecific(key, my_array);

    for (int i = 0; i < 10; i++) {
        my_array[i] = i;
    }

    for (int i = 0; i < 10; i++) {
        printf("my_array value: %d\n", my_array[i]);
    }

    int *my_array2 = pthread_getspecific(key);
    for (int i = 0; i < 10; i++) {
        printf("key value: %d\n", my_array2[i]);
    }

    printf("\n");
    return NULL;
}

int main() {
    pthread_t thread1, thread2;

    pthread_key_create(&key, array_destructor);

    pthread_create(&thread1, NULL, thread_function, NULL);
    pthread_create(&thread2, NULL, thread_function, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    pthread_key_delete(key);

    return 0;
}
