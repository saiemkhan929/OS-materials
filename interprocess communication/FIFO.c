#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#define TFIFO "tfifo"
#define BUFLEN 100
#define MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)

int main(void) {
    pid_t npid;
    size_t anz;
    int fds;
    char *fifo_nam = TFIFO;
    char msgbuf[BUFLEN] = "\0";

    if (mkfifo(fifo_nam, MODE) < 0) {
        printf("Error creating FIFO \n");
        return EXIT_FAILURE;
    }

    npid = fork();

    if (npid) {
        if ((fds = open(fifo_nam, O_WRONLY)) == -1) {
            printf("Parent process: Couldn't open FIFO for writing !\n" );
            return EXIT_FAILURE;
        }

        printf("Parent process: Enter a message:\n");
        fflush(stdin);
        scanf("%[^\n]", msgbuf);
        anz = strlen(msgbuf) + 1;
        write(fds, msgbuf, anz);
        printf("Parent process: EXIT\n");
    } else {
        if ((fds = open(fifo_nam, O_RDONLY)) == -1) {
            printf("Child process: Couldn't open FIFO for reading !\n");
            return EXIT_FAILURE;
        }

        printf("Child process: Waiting for a message...\n");

        if ((anz = read(fds, msgbuf, sizeof(msgbuf))) != -1) {
            printf("Child process: I received this message:\n%s\n", msgbuf);
            remove(fifo_nam);
            printf("Child process: EXIT\n");
        } else {
            printf("Child process: No message for me!\n");
        }
    }

    return 0;
}
