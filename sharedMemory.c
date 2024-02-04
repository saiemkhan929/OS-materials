#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/mman.h>

#define MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)
#define MSGLEN 100

int main(void) {
    char *memfile = "any";
    int fd;
    int wstat;
    pid_t npid;
    char *ptr = NULL, *hptr = NULL;

    if ((fd = shm_open(memfile, O_RDWR | O_CREAT, MODE)) == -1) {
        printf("Error: 'shm_open(create)' doesn't work!\n");
        return EXIT_FAILURE;
    }

    if (ftruncate(fd, sizeof(*ptr) + MSGLEN) == -1) {
        printf("Error: 'ftruncate()' doesn't work!\n");
        return EXIT_FAILURE;
    }

    if ((ptr = (char *)mmap(0, sizeof(*ptr) + MSGLEN, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == (char *)-1) {
        printf("Error: 'mapping' impossible!\n");
        return EXIT_FAILURE;
    }

    close(fd);

    hptr = ptr + 1;
    *ptr = 0; /* ugly hack: first byte == 0 indicates 'freedom to write to memory block' */

    npid = fork();

    if (npid) {
        usleep(100);
        printf("Parent process: Please type a message:\n");
        *hptr = '\0';
        fflush(stdin);
        scanf("%[^\n]", hptr);
        *ptr = 1; /* ugly hack: first byte == 1 indicates 'Memory contains new data' */
        wait(&wstat);
        printf("Parent process: EXIT\n");
        return EXIT_SUCCESS;
    } else {
        printf("Child process: Waiting for a message...\n");
        while (*ptr == 0) {
            /* infinite loop */
        }
        printf("Child process: I received this:\n%s\n", hptr);
        munmap(ptr, sizeof(*ptr) + MSGLEN);
        if (shm_unlink(memfile) == -1) {
            printf("Error: 'Unlink' impossible!\n");
            return EXIT_FAILURE;
        }
        printf("Child process: EXIT\n");
        return EXIT_SUCCESS;
    }
}
