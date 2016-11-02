#include <sys/cdefs.h>
#include <lib.h>
#include "namespace.h"

#include <string.h>
#include <unistd.h>

#ifdef __weak_alias
__weak_alias(openGroup, _openGroup)
#endif

int openGroup(int group)
{
  message m;

  memset(&m, 0, sizeof(m));
  m.m_m1.m1i1 = group;
  _syscall(MCAST_PROC_NR, MCAST_OPENGROUP, &m);
  return m.m_m1.m1i1;
}
