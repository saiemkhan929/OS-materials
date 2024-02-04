#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <sched.h>
#include <errno.h>

#define ATIME 1.6
#define NUM 60
#define FALSE 0
#define TRUE (!FALSE)

pthread_mutex_t mutex;
unsigned char myArray[NUM + 1];
volatile sig_atomic_t flag = FALSE;

void *thread2(void *);
void sigh(int signo) {
    flag = TRUE;
}

int main(void) {
    pthread_t tid;
    int i;
    unsigned char *ptr = NULL;

    // Initialize mutex
    if (pthread_mutex_init(&mutex, NULL) != 0) {
        fprintf(stderr, "Error pthread_mutex_init(): %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    // Create thread2
    if (pthread_create(&tid, NULL, thread2, NULL) != 0) {
        fprintf(stderr, "Error pthread_create(): %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    // Allocate memory for ptr
    if ((ptr = (unsigned char *)malloc((NUM + 1) * sizeof(unsigned char))) == NULL) {
        fprintf(stderr, "Error malloc(): %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    // Set signal handler for SIGALRM
    signal(SIGALRM, sigh);

    printf("Creating pattern - please wait...\n");

    // Initialize ptr with vertical lines
    memset(ptr, '|', NUM);
    *(ptr + NUM) = '\0';

    // Raise SIGALRM after ATIME seconds
    alarm(ATIME);

    

    while (1) {

     
        // Acquire the mutex lock
        if (pthread_mutex_lock(&mutex) != 0) {
            fprintf(stderr, "Error pthread_mutex_lock(): %s\n", strerror(errno));
            return EXIT_FAILURE;
        }
        

        // Copy ptr to myArray
        for (i = 0; i < NUM; i++)
            myArray[i] = ptr[i];

        // Copy myArray back to ptr
        for (i = 0; i < NUM; i++)
            ptr[i] = myArray[i];

        // Release the mutex lock
        if (pthread_mutex_unlock(&mutex) != 0) {
            fprintf(stderr, "Error pthread_mutex_unlock(): %s\n", strerror(errno));
            return EXIT_FAILURE;
        }

           if (flag) {
            printf("Wins Array: %s\n", ptr);
            
            break;
        }

       
    }
    //pthread_join(tid, NULL);

    return EXIT_SUCCESS;
}

void *thread2(void *data) {
    printf("came till here");
    int i;
    unsigned char *ptr = NULL;

    // Allocate memory for ptr
    if ((ptr = (unsigned char *)malloc((NUM + 1) * sizeof(unsigned char))) == NULL) {
        fprintf(stderr, "Error malloc(): %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Initialize ptr with horizontal lines
    memset(ptr, '-', NUM);
    *(ptr + NUM) = '\0';

    while (1) {
        // Acquire the mutex lock
        if (pthread_mutex_lock(&mutex) != 0) {
            fprintf(stderr, "Error pthread_mutex_lock(): %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }

        // Copy ptr to myArray
        for (i = 0; i < NUM; i++)
            myArray[i] = ptr[i];

        // Copy myArray back to ptr
        for (i = 0; i < NUM; i++)
            ptr[i] = myArray[i];

        // Release the mutex lock
        if (pthread_mutex_unlock(&mutex) != 0) {
            fprintf(stderr, "Error pthread_mutex_unlock(): %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
    }
}
