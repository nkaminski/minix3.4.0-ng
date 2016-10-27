/* This file contains some utility routines for MCAST.
 *
 * The entry points are:
 *   no_sys:		called for invalid system call numbers
 */

#include "mcast.h"

/*===========================================================================*
 *				no_sys					     *
 *===========================================================================*/
int no_sys(int who_e, int call_nr)
{
/* A system call number not implemented by MCAST has been requested. */
  printf("MCAST: in no_sys, call nr %d from %d\n", call_nr, who_e);
  return(ENOSYS);
}
