// kernel/sysinfo.h
#ifndef _KERNEL_SYSINFO_H_
#define _KERNEL_SYSINFO_H_

#include "types.h"

struct sysinfo {
  uint64 freemem; // bytes of free memory
  int nproc;      // number of processes with state != UNUSED
};

#endif // _KERNEL_SYSINFO_H_