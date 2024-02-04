#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define FALSE 0
#define TRUE (!FALSE)

void sigh(int); /* Handler declaration */

void f1(){
    printf("F1 -----------\n");
}

void f2(){
    printf("F2 -----------\n");
}


int main(void) {
    pid_t npid;
    int status;

    npid = fork();

    if (npid) {
        f1();
        printf("Parent process: F1() accessed. Now waiting for child process to complete\n");
        kill(npid, SIGUSR1); // sending signal to child
        wait(&status);
        f2();
        printf("Parent process: Done\n");
        
        return 0;
    } else {
        f1();
        printf("Child process: F1() accessed. Now waiting for Parent process to complete\n");
        signal(SIGUSR1, sigh); /* Assign SIGUSR1 to handler */
        pause();
        f2();
        printf("Child process: Done\n");
        return 10;
    }
}

void sigh(int signum) {
    /* Signal handler for SIGUSR1 */
    /* Attention: Only reentrant-resistant functions here... */
}
