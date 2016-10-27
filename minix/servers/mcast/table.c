/* This file contains the table used to map system call numbers onto the
 * routines that perform them.
 */

#define _TABLE

#include "mcast.h"

#define CALL(n) [((n) - MCAST_BASE)]

int (* const call_vec[NR_MCAST_CALLS])(void) = {
	CALL(MCAST_TEST)	= do_test,
	CALL(MCAST_MSEND)	= do_msend,
	CALL(MCAST_MRECEIVE)	= do_mreceive,
	CALL(MCAST_OPENGROUP)	= do_opengroup,
	CALL(MCAST_CLOSEGROUP)	= do_closegroup,
	CALL(MCAST_RECOVERGROUP)	= do_recovergroup
};
