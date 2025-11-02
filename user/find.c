#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#ifndef DIRSIZ
#define DIRSIZ 14
#endif

struct dirent {
  ushort inum;
  char name[DIRSIZ];
};

#define MAXPATH 512

void
find(const char *path, const char *target)
{
  char buf[MAXPATH];
  int fd;
  struct dirent de;
  struct stat st;

  if(stat(path, &st) < 0){
    return;
  }

  if(st.type == T_FILE){
    // lấy tên file cuối trong đường dẫn
    int last = 0;
    for(int i = 0; path[i]; i++) if(path[i] == '/') last = i+1;
    if(strcmp(path + last, target) == 0){
      printf("%s\n", path);
    }
    return;
  }

  // O_RDONLY = 0 trong xv6-labs
  if((fd = open(path, 0)) < 0){
    return;
  }

  while(read(fd, &de, sizeof(de)) == sizeof(de)){
    if(de.inum == 0)
      continue;

    char name[DIRSIZ+1];
    memmove(name, de.name, DIRSIZ);
    name[DIRSIZ] = 0;

    if(strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
      continue;

    int n = 0;
    for(; path[n] && n < MAXPATH-1; n++)
      buf[n] = path[n];

    if(n > 0 && buf[n-1] != '/'){
      if(n+1 >= MAXPATH-1) continue;
      buf[n++] = '/';
    }

    int i = 0;
    for(; name[i] && n < MAXPATH-1; i++)
      buf[n++] = name[i];
    buf[n] = 0;

    if(stat(buf, &st) < 0){
      continue;
    }

    if(strcmp(name, target) == 0){
      printf("%s\n", buf);
    }

    if(st.type == T_DIR){
      find(buf, target);
    }
  }

  close(fd);
}

int
main(int argc, char *argv[])
{
  if(argc != 3){
    printf("Usage: find <path> <name>\n");
    exit(1);
  }

  find(argv[1], argv[2]);
  exit(0);
}
