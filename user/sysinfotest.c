#include "kernel/sysinfo.h"
#include "user/user.h"
#include "kernel/types.h"

int
main(int argc, char *argv[])
{
  struct sysinfo si;
  if(sysinfo(&si) < 0){
    printf("sysinfotest: sysinfo failed\n");
    exit(1);
  }
  // basic checks: nproc > 0, freemem reasonable
  if(si.nproc > 0 && si.freemem > 0){
    printf("sysinfotest: OK\n");
    exit(0);
  } else {
    printf("sysinfotest: FAILED freemem=%ld nproc=%d\n", si.freemem, si.nproc);
    exit(1);
  }
}