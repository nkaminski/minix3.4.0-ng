#include <sys/cdefs.h>
#include "namespace.h"
#include <lib.h>

#include <string.h>
#include <unistd.h>

#ifdef __weak_alias
__weak_alias(mtest, _mtest)
#endif

int mtest(void)
{
  message m;

  memset(&m, 0, sizeof(m));
  return(_syscall(MCAST_PROC_NR, MCAST_TEST, &m));
}
