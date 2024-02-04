#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
 #include<sys/stat.h>  
 #include<sys/mman.h>

#define ZMAX 80
#define BUFLEN 100
#define MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)
#define MSGLEN 500

volatile sig_atomic_t flag = 0;

void sighandler(int signum) {
    flag = 1;
}

int main(int argc, char *argv[]) {
    char string[ZMAX] = "isavailable"; // query | keep it empty if no query
    char response[500];
    int sock;
    struct sockaddr_in server;
    struct hostent *hp = NULL;

    // communication between process (pipe)
    pid_t npid;
    int i;
    int status;

    //create shared memory
    int fd;
    char *memfile = "shtrans";
    char *ptr=NULL;

    // Create the shared memory
    fd=shm_open(memfile, O_RDWR|O_CREAT, MODE);
    ftruncate(fd, sizeof(*ptr)+MSGLEN);
    ptr= (char*) mmap(0, sizeof(*ptr)+MSGLEN, PROT_READ|PROT_WRITE, MAP_SHARED,fd,0);

    // Register the signal handler
    signal(SIGUSR1, sighandler);

    for (i = 0; i < 30; i++) {
        npid = fork(); // creating processes

        if (npid > 0 && i == 0) { // parent process run only once
            if (argc != 3) {
                fprintf(stderr, "Usage: %s hostname portnumber\n", argv[0]);
                return EXIT_FAILURE;
            }

            sock = socket(AF_INET, SOCK_STREAM, 0);
            if (sock < 0) {
                perror("Socket");
                return EXIT_FAILURE;
            }

            server.sin_family = AF_INET;
            hp = gethostbyname(argv[1]);
            if (hp == NULL) {
                fprintf(stderr, "%s: %s\n", argv[1], hstrerror(h_errno));
                return EXIT_FAILURE;
            }

            memcpy(&server.sin_addr, hp->h_addr_list[0], hp->h_length);
            server.sin_port = htons(atoi(argv[2]));

            if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
                perror("Establishing connection");
                return EXIT_FAILURE;
            }

            // Sending query
            if (write(sock, string, sizeof(string)) < 0)
                perror("Sending data");

            memset(response, 0, sizeof(response));
            read(sock, response, 500);

            if (strcmp(response, "none") == 0) {
                printf("Response: no URL\n");
            } else {
                printf("Response: %s | Transmitting to all processes\n", response);

                // Writing to shared memory
                memcpy(ptr, response, MSGLEN);

                // Sending signal to child processes
                kill(0, SIGUSR1);
            }

            // close the socket in parent process
            close(sock);

        } else if (npid == 0) { // child process
            printf("Child Process %d: Output child process [%d].\n", getpid(), i + 1);

            // Wait for signal from parent process
            while (!flag)
                ;

            // Read from shared memory
            printf("CHILD URL: %s\n", ptr);


            return 454;
        }
    }

    // Wait for all child processes to finish
    while (wait(&status) > 0)
        ;

    // clear memory
    munmap(ptr, sizeof(*ptr)+MSGLEN);
    shm_unlink(memfile);

    return EXIT_SUCCESS;
}
