#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

int main(int args, char *argv[]) {
    char sep[] = " -\r\t\n./,";
    char digits_set[] = "1234567890";

    if (args < 2) {
        printf("failed sixfive\n");
        return -1;
    }

    for (int f = 1; f < args; f++) {
        int fd = open(argv[f], O_RDONLY);
        if (fd < 0) {
            printf("failed sixfive\n");
            return -1;
        }

        char collect[16];
        int len = 0;
        char tmp;

        collect[0] = '\0';
        while (read(fd, &tmp, 1) == 1) {
            if (strchr(sep, tmp)) {
                collect[len] = '\0';
                if (len > 0) {
                    int all_digits = 1;
                    for (int j = 0; j < len; j++) {
                        if (strchr(digits_set, collect[j]) == 0)
                            all_digits = 0;
                    }
                    if (all_digits) {
                        int val = atoi(collect);
                        if ((val % 5 == 0) || (val % 6 == 0))
                            printf("%d\n", val);
                    }
                }
                len = 0;
            } else if (len < 15) {
                collect[len++] = tmp;
            }
        }

        collect[len] = '\0';
        if (len > 0) {
            int all_digits = 1;
            for (int j = 0; j < len; j++) {
                if (strchr(digits_set, collect[j]) == 0)
                    all_digits = 0;
            }
            if (all_digits) {
                int val = atoi(collect);
                if ((val % 5 == 0) || (val % 6 == 0))
                    printf("%d\n", val);
            }
        }
        close(fd);
    }
    return 0;
}