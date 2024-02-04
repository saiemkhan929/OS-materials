#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void f1() {
    printf("f1 called by %d\n", getpid());
}

void f2() {
    printf("f2 called by %d\n", getpid());
}

int main() {
    // custom codes
    int pipe1[2];
    int pipe2[2];
    char message1[20] = "anything";
    char message2[20] = "Hello";

    if (pipe(pipe1) < 0 || pipe(pipe2) < 0) {
        perror("Pipe");
        return EXIT_FAILURE;
    }

    switch (fork()) {
        case -1:
            perror("fork1");
            exit(1);
        case 0:
            f1();

            // Tell the other process that the call to f1 is done
            write(pipe1[1], message1, sizeof(message1));

            // Wait for the done message from the other process
            read(pipe2[0], message2, sizeof(message2) );


            f2();
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
            write(pipe2[1], message2, sizeof(message2));

            // Wait for the done message from the other process
            read(pipe1[0], message1, sizeof(message1));

            f2();
            exit(0);
    }

    exit(0);
}
