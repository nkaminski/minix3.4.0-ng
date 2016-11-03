/* This file contains some utility routines for MCAST.
 *
 * The entry points are:
 *   no_sys:		called for invalid system call numbers
 *   mcast_isokendpt:		verifies if a given endpoint is valid
 */

#include "mcast.h"
#include <minix/endpoint.h>
#include "utility.h"

/*===========================================================================*
 *				no_sys					     *
 *===========================================================================*/
int no_sys(int who_e, int call_nr)
{
        /* A system call number not implemented by MCAST has been requested. */
        printf("MCAST: in no_sys, call nr %d from %d\n", call_nr, who_e);
        return(ENOSYS);
}
/*===========================================================================*
 *          mcast_isokendpt                                                  *
 *===========================================================================*/
int mcast_isokendpt(endpoint_t endpoint, int *proc)
{
        if(endpoint == NONE)
                return EINVAL;
        *proc = getnpid(endpoint);
        printf("isokendpt npid=%d\n", *proc);
        if(*proc < 0)
                return EINVAL;
        return OK;
}
