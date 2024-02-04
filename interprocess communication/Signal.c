#include <stdio.h>
#include <signal.h>

#define FALSE 0
#define TRUE (!FALSE)

volatile sig_atomic_t flag = FALSE;

void sigh(int); /* handler declaration */

int main() {
    signal(SIGINT, sigh); /* assign handler function */
    printf("Press ^C to call the signal handler\n");

    while (!flag)
        ;

    printf("Program will be terminated!\n");

    return 0;
}

void sigh(int signum) {
    flag = TRUE;
}
