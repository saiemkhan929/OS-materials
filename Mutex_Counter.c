#include <stdio.h>
#include <pthread.h>

int shared_variable = 0;
pthread_mutex_t mutex;

void* thread_function(void* arg) {
    int thread_id = *(int*)arg;

    // Enter the critical section
    pthread_mutex_lock(&mutex);

    // Critical Section
    shared_variable++; // Increment the shared variable
    printf("Thread %d: Shared Variable = %d\n", thread_id, shared_variable);

    // Exit the critical section
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main() {
    pthread_t threads[5];
    int thread_ids[5];

    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < 5; ++i) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, thread_function, &thread_ids[i]);
    }

    for (int i = 0; i < 5; ++i) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);

    return 0;
}
