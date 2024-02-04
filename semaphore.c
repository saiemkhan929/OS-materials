#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_THREADS 5

sem_t semaphore;

void* thread_function(void* arg) {
    int thread_id = *(int*)arg;

    printf("Thread %d is trying to access the shared resource.\n", thread_id);
    
    sem_wait(&semaphore); // Decrease semaphore value (acquire)
    sleep(2);

    printf("Thread %d has acquired the shared resource.\n", thread_id);
    // Access and use the shared resource

    printf("Thread %d is releasing the shared resource.\n", thread_id);
    sem_post(&semaphore); // Increase semaphore value (release)

    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    sem_init(&semaphore, 0, 5); // Initialize semaphore with initial value 1

    for (int i = 0; i < NUM_THREADS; ++i) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, thread_function, &thread_ids[i]);
    }

    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&semaphore); // Destroy the semaphore

    return 0;
}
