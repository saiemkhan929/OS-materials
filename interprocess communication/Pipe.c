#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

int main(void) {
    pid_t npid;
    size_t anz;
    int fds[2];
    char msgbuf[100] = "\0";

    if (pipe(fds) < 0) {
        perror("Pipe");
        return EXIT_FAILURE;
    }

    npid = fork();

    if (npid == -1) {
        perror("Fork");
        return EXIT_FAILURE;
    } else if (npid > 0) {
        printf("Parent process: Please type a message:\n");
        fflush(stdin);
        scanf("%[^\n]", msgbuf);
        anz = strlen(msgbuf) + 1;
        write(fds[1], msgbuf, anz);
        printf("Parent process: EXIT\n");
    } else {
        printf("Child process: Waiting for message...\n");
        if ((anz = read(fds[0], msgbuf, sizeof(msgbuf))) != -1) {
            printf("Child process: I received this message:\n%s\n", msgbuf);
            printf("Child process: EXIT %d\n", anz);
        } else {
            printf("Child process: No message for me! (error: %s)!\n", strerror(errno));
        }
    }

    return 0;
}
