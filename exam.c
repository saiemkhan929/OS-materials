#include <stdio.h>
#include <wait.h>
#include <unistd.h>

volatile int locked = 1;

void lockSpinLock() {

   while (1) {
      if (locked)
         continue;

      locked =1;
      break;
   }
}

void freeSpinLock() {
   
   locked = 0;
}

int main() {
     printf("Start---\n");
    lockSpinLock();
    freeSpinLock();
     printf("End---\n");

    return 0;
}
/*
p=1519 // parent process
ppid=1514 // Grandparent id
pid=1519 // parent
ppid=1519 //parent
pid=1515
*/
