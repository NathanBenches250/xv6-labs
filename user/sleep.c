#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#include <stdarg.h>


int sleep_user (int seconds)  {
    return pause(seconds); // i used my sleep call but testing is not happy with this and wants a pause systemcall
}
int main (int argc, char *argv[]) {
    if(argc != 2) {
        exit(1);
    }
    int result = sleep_user(atoi(argv[1]));
    if (result == 0) {
        // printf("sucessful \n");
    }
    // printf("I am awake \n");
    exit(0);
}   