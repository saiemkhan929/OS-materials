/* Program example_fork_exec2 (child process) */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[], char* envp[]) {
    int i = 0;
    printf("Output child process \"%s\" (PID: %d)!\n", argv[0], getpid());
    printf("Environment:\n");
    while (envp[i] != NULL) {
        printf("%d. %s\n", i + 1, envp[i]);
        i++;
    }
    return EXIT_SUCCESS;
}