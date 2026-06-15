#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"
#include <stdint.h>

void memdump(char *fmt, char *data);

int
main(int argc, char *argv[])
{
  if(argc == 1){
    printf("Example 1:\n");
    int a[2] = { 61810, 2025 };
    memdump("ii", (char*) a);
    
    printf("Example 2:\n");
    memdump("S", "a string");
    
    printf("Example 3:\n");
    char *s = "another";
    memdump("s", (char *) &s);

    struct sss {
      char *ptr;
      int num1;
      short num2;
      char byte;
      char bytes[8];
    } example;

    example.ptr = "hello";
    example.num1 = 1819438967;
    example.num2 = 100;
    example.byte = 'z';
    strcpy(example.bytes, "xyzzy");
    
    printf("Example 4:\n");
    memdump("pihcS", (char*) &example);
    
    printf("Example 5:\n");
    memdump("sccccc", (char*) &example);
  } else if(argc == 2){
    // format in argv[1], up to 512 bytes of data from standard input.
    char data[512];
    int n = 0;
    memset(data, '\0', sizeof(data));
    while(n < sizeof(data)){
      int nn = read(0, data + n, sizeof(data) - n);
      if(nn <= 0)
        break;
      n += nn;
    }
    memdump(argv[1], data);
  } else {
    printf("Usage: memdump [format]\n");
    exit(1);
  }
  exit(0);
}

void
memdump(char *fmt, char *data)
{
  // printf("%ld", sizeof(*data)); -> since we are abstracting away the struct, we can't access it's size here.
  // I guess we need to directly rely on the truth of fmt
  int i = 0;
  int x = 0;
  while (fmt[i] != '\0') {
    char c = fmt[i];
    // printf("%c", c);
    if (c == 'i') {

    unsigned int val = ((unsigned char)data[x])        |
                       ((unsigned char)data[x + 1] << 8)  |
                       ((unsigned char)data[x + 2] << 16) |
                       ((unsigned char)data[x + 3] << 24);
    printf("%d \n", (int)val);

    x += 4;
  }

   else if(c == 'p') {
    
        uint64_t val = ((uint64_t)(unsigned char)data[x])        |
                       ((uint64_t)(unsigned char)data[x + 1] << 8)  |
                       ((uint64_t)(unsigned char)data[x + 2] << 16) |
                       ((uint64_t)(unsigned char)data[x + 3] << 24) |
                       ((uint64_t)(unsigned char)data[x + 4] << 32) | 
                       ((uint64_t)(unsigned char)data[x + 5] << 40) |
                       ((uint64_t)(unsigned char)data[x + 6] << 48) |
                       ((uint64_t)(unsigned char)data[x + 7] << 56)
                       ;
        x += 8;
        printf("%x \n", (int) val);
   }

  else  if (c == 'h') {
      uint16_t val = ((unsigned char)data[x])        |
                       ((unsigned char)data[x + 1] << 8);
      printf("%d \n", val);

    x += 2;
  }
   
  else if(c == 'c') {
      printf("%c \n", data[x]);
      x++;
  }
  else if (c == 's') {
    uint64_t val = ((uint64_t)(unsigned char)data[x])        |
                       ((uint64_t)(unsigned char)data[x + 1] << 8)  |
                       ((uint64_t)(unsigned char)data[x + 2] << 16) |
                       ((uint64_t)(unsigned char)data[x + 3] << 24) |
                       ((uint64_t)(unsigned char)data[x + 4] << 32) | 
                       ((uint64_t)(unsigned char)data[x + 5] << 40) |
                       ((uint64_t)(unsigned char)data[x + 6] << 48) |
                       ((uint64_t)(unsigned char)data[x + 7] << 56)
                       ;
    printf("%s \n", (char *)val);
    x += 8;
  }
  else if (c == 'S') {
    while (data[x] != '\0') {
      printf("%c", data[x]);
      x++;
    }
    printf("\n");
  }
  i++;
  }

}
