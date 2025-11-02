#include "kernel/types.h"
#include "user/user.h"

int
main(void)
{
  int p1[2]; // pipe cha -> con
  int p2[2]; // pipe con -> cha
  char buf[1];
  
  pipe(p1);
  pipe(p2);

  int pid = fork();

  if(pid == 0) {
    // Process con
    read(p1[0], buf, 1); // đọc từ cha
    printf("%d: received ping\n", getpid());
    write(p2[1], buf, 1); // gửi lại cho cha
    exit(0);
  } else {
    // Process cha
    write(p1[1], "a", 1); // gửi 'a' cho con
    read(p2[0], buf, 1);  // đọc lại từ con
    printf("%d: received pong\n", getpid());
    exit(0);
  }
}
