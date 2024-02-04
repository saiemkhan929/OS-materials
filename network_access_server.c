#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define PORT 5000

int main() {
    int sock;
    struct sockaddr_in server;
    int msgsock;
    char buf[1024];
    int rval;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Stream Socket");
        return EXIT_FAILURE;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    if (bind(sock, (struct sockaddr *)&server, sizeof(server))) {
        perror("Port assignment");
        return EXIT_FAILURE;
    }

    printf("Server: Listening on port #%d\n", ntohs(server.sin_port));

    listen(sock, 5);

    do {
        msgsock = accept(sock, NULL, 0);
        if (msgsock == -1) {
            perror("Connection establishment");
            return EXIT_FAILURE;
        } else {
            memset(buf, 0, sizeof(buf));

            if ((rval = read(msgsock, buf, 1024)) < 0) {
                perror("Data receipt");
            } else if (rval == 0) {
                printf("Server: Connection terminated\n");
            } else {
                printf("Client: %s\n", buf);

                if(strcmp(buf, "isavailable") == 0){ // 0 means equal
                    /* Do server's job here */
                    const char *response = "https://facebook.com"; // none or URL
                    write(msgsock, response, strlen(response));
                }else{
                    
                    /* Do server's job here */
                    const char *response = "Query invalid";
                    write(msgsock, response, strlen(response));
                
                }

                
            }
        }
        close(msgsock);
    } while (1);

    return 0;
}
