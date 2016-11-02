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
  _syscall(MCAST_PROC_NR, MCAST_TEST, &m);
  return m.m_m1.m1i1;
}
