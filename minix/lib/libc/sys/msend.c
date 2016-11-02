#include <sys/cdefs.h>
#include <lib.h>
#include "namespace.h"

#include <string.h>
#include <unistd.h>

#ifdef __weak_alias
__weak_alias(msend, _msend)
#endif

int msend(int group, const char *buffer, size_t nbytes)
{
  message m;

  memset(&m, 0, sizeof(m));
  m.m_m1.m1i1 = group;
  m.m_m1.m1i2 = nbytes;
  m.m_m1.m1p1 = (vir_bytes)buffer;
  _syscall(MCAST_PROC_NR, MCAST_MSEND, &m);
  return m.m_m1.m1i1;
}
