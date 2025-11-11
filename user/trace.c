#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int i;
  char *nargv[MAXARG];

  if(argc < 3){
    fprintf(2, "Usage: trace mask command\n");
    exit(1);
  }

  // Gọi system call trace() với mask được cung cấp
  if (trace(atoi(argv[1])) < 0) {
    fprintf(2, "trace failed\n");
    exit(1);
  }
  
  for(i = 2; i < argc && i < MAXARG; i++){
    nargv[i-2] = argv[i];
  }
  nargv[i-2] = 0; // Đánh dấu kết thúc mảng đối số

  exec(nargv[0], nargv);
  
  // exec chỉ trả về nếu có lỗi
  fprintf(2, "exec %s failed\n", nargv[0]);
  exit(1);
}