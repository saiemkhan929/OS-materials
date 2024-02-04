#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t npid;
    int i;

    for (i = 0; i < 3; i++) {
        
        npid = fork();
        
        
        if (npid > 0 && i == 0) {
            printf("Process %d: I'm your father, Luke!\n", getpid());
        } else if (npid == 0) {
            printf("Process %d: Output child process.\n", getpid());
        } else {
            
            return 1;
        }
    }

    return 0;
}
