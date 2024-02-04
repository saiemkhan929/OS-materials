#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define PROGNAM "exec_child"

int main(int argc, char* argv[]) {
    pid_t npid;
    int rval;
    char* argvk[] = { PROGNAM, NULL };
    char* envmt[] = { "PROG=exec", "TEST=text", NULL };

    npid = fork();

    if (npid > 0) {
        printf("Output parent process \"%s\" (PID: %d)!\n", argv[0], getpid());
        wait(&rval);
        return EXIT_SUCCESS;
    }
    else if (npid == 0) {
        printf("Parent: Output child process \"%s\" (PID: %d)!\n", argv[0], getpid());
        execve(PROGNAM, argvk, envmt);
        return EXIT_FAILURE;
    }
    else {
        fprintf(stderr, "Error in fork(): %s\n", strerror(errno));
        return EXIT_FAILURE;
    }
}