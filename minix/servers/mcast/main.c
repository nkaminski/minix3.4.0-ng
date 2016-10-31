/* This file contains the main program of the mcast server. It is responsible
 * for implementing the multicast IPC functions.
 */

#define _MAIN
#define MIN(a, b) (((a)<(b)) ? (a) : (b))
#include "mcast.h"
#include "reply.h"
#include "groups.h"

extern int (* const call_vec[])(void);

/* Declare some local functions. */
static void sef_local_startup(void);
static int sef_cb_init_fresh(int, sef_init_info_t *);
static void sef_cb_signal_handler(int);

/* globals */
message m_in;	/* the incoming message itself is kept here. */
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
/*
int do_msend(void){
	int rv;
	printf("msend called\n");
	if(bproc != -1){
		printf("msend unblock %d\n", bproc);
			 copy from the sender to the receiver 
		printf("Attempt to copy %d bytes \n", MIN(dstlen,m_in.m_m1.m1i2));
		rv = sys_datacopy(who_e, (vir_bytes)m_in.m_m1.m1p1, bproc, dstaddr, MIN(dstlen,(vir_bytes)m_in.m_m1.m1i2));
		wake_up(bproc,OK);	
		bproc = -1;
	}
	return rv;
}
int do_mreceive(void){
	printf("mreceive called\n");
	 save parameters of caller 
	bproc = who_e;
	dstaddr = (vir_bytes)m_in.m_m1.m1p1;
	dstlen = m_in.m_m1.m1i2;
	return(SUSPEND);
}
*/
int do_msend(void){
	return(OK);	
}

int do_mreceive(void){
	return(OK);
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
 *			       sef_local_startup			     *
 *===========================================================================*/
static void sef_local_startup()
{
  /* Register init callbacks. */
  sef_setcb_init_fresh(sef_cb_init_fresh);
//  sef_setcb_init_lu(sef_cb_init_fresh);
  sef_setcb_init_restart(sef_cb_init_fresh);

  /* Register signal callbacks. */
  sef_setcb_signal_handler(sef_cb_signal_handler);

  /* Let SEF perform startup. */
  sef_startup();
}

/*===========================================================================*
 *		            sef_cb_init_fresh                                *
 *===========================================================================*/
static int sef_cb_init_fresh(int UNUSED(type), sef_init_info_t *UNUSED(info))
{
/* Initialize mcast server */
	init_groups();

  return(OK);
}

/*===========================================================================*
 *		            sef_cb_signal_handler                            *
 *===========================================================================*/
static void sef_cb_signal_handler(int signo)
{
  /* Only check for termination signal, ignore anything else. */
  if (signo != SIGTERM) return;
  printf("MCAST: exiting on SIGTERM\n");
  /* Shutting down. */ 

  exit(0);
}

