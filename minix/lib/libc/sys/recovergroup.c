#include <sys/cdefs.h>
#include <lib.h>
#include "namespace.h"

#include <string.h>
#include <unistd.h>

#ifdef __weak_alias
__weak_alias(recoverGroup, _recoverGroup)
#endif

int recoverGroup(int group)
{
  message m;

  memset(&m, 0, sizeof(m));
  m.m_m1.m1i1 = group;
  _syscall(MCAST_PROC_NR, MCAST_RECOVERGROUP, &m);
  return m.m_m1.m1i1;
}
