/* This file contains the main program of the mcast server. It is responsible
 * for implementing the multicast IPC functions.
 */

#define _MAIN
#define MIN(a, b) (((a)<(b)) ? (a) : (b))
#include "mcast.h"

extern int (* const call_vec[])(void);

/* Declare some local functions. */
static void sef_local_startup(void);
static void reply(endpoint_t who_e, message *m_ptr);

/* globals */
message m_in,m_out;	/* the incoming message itself is kept here. */
int call_nr;	/* system call number */
int who_e;	/* caller's endpoint */
int result;	/* result to system call */
endpoint_t bproc;
vir_bytes dstaddr;
int dstlen;
/*===========================================================================*
 *				handlers				     *
 *===========================================================================*/

int do_test(void){
	printf("test of mcast server process\n");
	return(OK);
}
int do_msend(void){
	int rv;
	printf("msend called\n");
	if(bproc != -1){
		printf("msend unblock %d\n", bproc);
		m_out.m_type = OK;           /* build reply message */
		m_out.m_source = MCAST_PROC_NR;
		/* copy from the sender to the receiver */
		printf("Attempt to copy %d bytes \n", MIN(dstlen,m_in.m_m1.m1i2));
		rv = sys_datacopy(who_e, (vir_bytes)m_in.m_m1.m1p1, bproc, dstaddr, MIN(dstlen,(vir_bytes)m_in.m_m1.m1i2));		
		reply(bproc, &m_out);
		bproc = -1;
	}
	return rv;
}
int do_mreceive(void){
	printf("mreceive called\n");
	/* save parameters of caller */
	bproc = who_e;
	dstaddr = (vir_bytes)m_in.m_m1.m1p1;
	dstlen = m_in.m_m1.m1i2;
	return(SUSPEND);
}
int do_opengroup(void){
	printf("opengroup called\n");
	return(OK);
}
int do_closegroup(void){
	printf("closegroup called\n");
	return(OK);
}
int do_recovergroup(void){
	printf("recovergroup called\n");
	return(OK);
}
/*===========================================================================*
 *				main loop				     *
 *===========================================================================*/

int main(void)
{
	int rv;
	unsigned int call_index;
	/* SEF local startup. */
	sef_local_startup();
	printf("starting mcast server process\n");
	bproc = -1;
	/* This is MCAST's main loop - get work and do it, forever and forever. */
	while (TRUE) {
		int ipc_status;

		/* Wait for the next message and extract useful information from it. */
		if (sef_receive_status(ANY, &m_in, &ipc_status) != OK)
			panic("MCAST sef_receive error");
		who_e = m_in.m_source;	/* who sent the message */
		call_nr = m_in.m_type;	/* system call number */

		/* Check for system notifications first. Special cases. */
		if (is_ipc_notify(ipc_status)) {
			switch(who_e){
				/* ignore for now */
				default :
					break;
			}

			continue; /* Don't reply. */
		}
		/* If the system call number is valid, perform the call. */
		if(IS_MCAST_CALL(call_nr) && (call_nr - MCAST_BASE) < NR_MCAST_CALLS ){	
			call_index = (unsigned int) (call_nr - MCAST_BASE);
			result = (*call_vec[call_index])();
		} else {
			result = no_sys(who_e, call_nr);
		}

		/* Send reply. */
		if (result != SUSPEND) {
			m_in.m_type = result;  		/* build reply message */
			reply(who_e, &m_in);		/* send it away */
		}
	}
	return(OK);
}

/*===========================================================================*
 *				reply					     *
 *===========================================================================*/
static void reply(endpoint_t who_e, message *m_ptr)
{
	int s = ipc_send(who_e, m_ptr);    /* send the message */
	if (OK != s)
		printf("MCAST: unable to send reply to %d: %d\n", who_e, s);
}

/*===========================================================================*
 *			       sef_local_startup			     *
 *===========================================================================*/
static void sef_local_startup(void)
{
	/* Register init callbacks. */

	/* No signal callbacks for now. */

	/* Let SEF perform startup. */
	sef_startup();
}

