#include "mcast.h"
#include "reply.h"
#include <assert.h>
#include <string.h>
/*===========================================================================*
 *				wake_up					     *
 *===========================================================================*/

int wake_up(endpoint_t ep, int retcode){
	message m_out;
	memset(&m_out,0x00,sizeof(message));
	assert(retcode != (SUSPEND));
	m_out.m_type = retcode;;           /* build reply message */
	m_out.m_source = MCAST_PROC_NR;
	return reply(ep, &m_out);	
}

int reply(endpoint_t who_e, message *m_ptr)
{
	int s = ipc_send(who_e, m_ptr);    /* send the message */
	if (OK != s){
		printf("MCAST: unable to send reply to %d: %d\n", who_e, s);
		return(-EAGAIN);
	}
	return (OK);
}


