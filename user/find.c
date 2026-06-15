#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "kernel/param.h"
/*Thoughts - we need to start at path and if path is directory (use a while loop into the files inside and recurse), and repeat for subdirectories until we get only files and then strcmp paths
    */
char*
fmtname(char *path) // this takes mnt/cache/dir and gives dir
{
  // static char buf[DIRSIZ+1];
  char *p; 

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--) // set p to end of path and then go down till u hit backslash
    ;
  p++; //  p is pointer to s in saddsawd
    return p;
}

void
find (char *path, char *to_find, int execute, char *function,  char *args[])
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, O_RDONLY)) < 0){
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }
  
  switch(st.type){
  case T_DEVICE:
  case T_FILE:
    if (strcmp(to_find, fmtname(path)) == 0) { // path should be absolute, so we need to pull out the last file name and then compare
        if (execute) {
        int pid = fork();
        if (pid < 0) {
            fprintf(2, "fork failed");
            close(fd);
            return;
        }
        else if (pid == 0) {
            int i = 0;
            while (args[i] != 0) {
                i++;
            }
            args[i] = path;
            args[i + 1] = 0;
            exec(function, args);
            exit(1);
        }
        else {
            wait(&pid);
        } }
        else {
        printf("%s\n", path); }
    // we found the file (print the path of the file we located)
 }  
  break; 
  case T_DIR:
    strcpy(buf, path);
    p = buf+strlen(buf); // taking the parent path and adding na / for exxample mnt/cache -> mnt/cache/
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
        if((strcmp(".", de.name) == 0)|| (strcmp(de.name,"..") == 0)) {
        continue;
    }
      if(de.inum == 0) {
        continue; }

        memmove(p, de.name, DIRSIZ); // since p has the entire file path here, all we need to is compare and return
        p[DIRSIZ] = 0; // setting as null termination
        find(buf, to_find, execute, function, args);
    }
    break;
  }
  close(fd);
}

int
main(int argc, char *argv[])
{

  if(argc < 3){
    exit(1);
  }
  if (argc > MAXARG) {
    printf("TOO MANY ARGUMENTS");
    exit(1);
  }
  char *args[MAXARG];
  char *function = argv[3];


  if (argc > 3) {
      if (strcmp(argv[3], "-exec") != 0){
    printf("only exec with find");
    exit(1);
  }
      function = argv[4];
args[0] = argv[4]; // args[0] must be the program name ("grep")

int i = 5;
while(i < argc) {
   args[i - 4] = argv[i]; // Copies "hello" and any other trailing flags
   i++;
}
// This leaves the array looking like: ["grep", "hello"]
        find(argv[1], argv[2], 1, function, args);
  }
    else {
        find(argv[1], argv[2], 0, function, args);
    }
  exit(0);
}
