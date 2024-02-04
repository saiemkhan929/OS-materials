#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>

void *thread1(void *data){
   
    for(int i = 0; i<10;i++ ){
        printf("Thread 1 \n");
        sleep(1);
    }
}

void *thread2(void *data){
    for(int i = 0; i<10;i++ ){
        printf("Thread 2 \n");
        sleep(1);
    }
}

int main(){
    pthread_t tid1, tid2;

    pthread_create(&tid1, NULL, thread1, NULL);
    pthread_create(&tid2, NULL, thread2, NULL);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    return 0;
}