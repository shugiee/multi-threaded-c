#include "../inc/threadpool.h"
#include <pthread.h>

int main() {
    // Initialize the pool
    threadpool_t pool;
    threadpool_init(&pool);

    for (int i = 0; i < 100; i++) {
        // Use a heap pointer to pass integer in
        int *task_num = malloc(sizeof(int));
        *task_num = i;
        threadpool_add_task(&pool, example_task, task_num);
    }

    sleep(5);

    threadpool_destory(&(pool));

    return 0;
}
