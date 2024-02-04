#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#define MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)


void f1() {
    printf("f1 called by %d\n", getpid());
}

void f2() {
    printf("f2 called by %d\n", getpid());
}

int main() {
    // custom codes
    int fifo1;
    int fifo2;
    char message1[20] = "anything";
    char message2[20] = "Hello";
    char* FIFO1_NAME = "any_name";
    char* FIFO2_NAME = "here_FIFO";

    //make sure no FIFO exists
    remove(FIFO1_NAME);
    remove(FIFO2_NAME);

    if (mkfifo(FIFO1_NAME, MODE) < 0) {
        perror("FIFO1: ");
        return EXIT_FAILURE;
    }

     if (mkfifo(FIFO2_NAME, MODE)  < 0) {
        perror("FIFO2: ");
        return EXIT_FAILURE;
    }

    switch (fork()) {
        case -1:
            perror("fork1");
            exit(1);
        case 0:
            f1();

            // Tell the other process that the call to f1 is done
            if ( (fifo1 = open(FIFO1_NAME, O_WRONLY)) == -1) {
                printf("Child process: Couldn't open FIFO for reading !\n");
                return EXIT_FAILURE;
            }

            write(fifo1, message1, sizeof(message1));

            // Wait for the done message from the other process

            if ( (fifo2 = open(FIFO2_NAME, O_RDONLY)) == -1) {
                printf("Child process: Couldn't open FIFO for reading !\n");
                return EXIT_FAILURE;
            }
            read(fifo2, message2, sizeof(message2) );
            remove(FIFO2_NAME);

            f2();

            close(fifo1);
            close(fifo2);
            exit(0);
    }

    // Subprocess 2 is symmetric
    switch (fork()) {
        case -1:
            perror("fork2");
            exit(1);
        case 0:
            f1();

            // Tell the other process that the call to f1 is done
            if ( (fifo2 = open(FIFO2_NAME, O_WRONLY)) == -1) {
                printf("Child process: Couldn't open FIFO for reading !\n");
                return EXIT_FAILURE;
            }

            write(fifo2, message2, sizeof(message2));

            // Wait for the done message from the other process

            if ( (fifo1 = open(FIFO1_NAME, O_RDONLY)) == -1) {
                printf("Child process: Couldn't open FIFO for reading !\n");
                return EXIT_FAILURE;
            }
            read(fifo1, message1, sizeof(message1));
            remove(FIFO1_NAME);

            f2();

            close(fifo1);
            close(fifo2);
            exit(0);
    }

    
    exit(0);
}
