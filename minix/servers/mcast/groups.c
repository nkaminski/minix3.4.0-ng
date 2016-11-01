#include "mcast.h"
#include "types.h"
#include "deadlock.h"
#include "reply.h"
#include "groups.h"
#include "utility.h"

group_list_t group_list;
/* Decide Messages Structure
 * Each member in group should have pointer
 * to next object to get
 */
//unistd.h for msend
/* int main(int argv, char **argc)
   {
   init();
   return 0;
   }
 */
void init_groups()
{
        int i,j;
        for(i = 0; i < MAX_GROUPS; i++){
                group_list[i].nmembers = 0;
                group_list[i].b_sender.pid = -1;
            for(j = 0;
        }
}

//MESSAGE APPENDING MUST BE ATOMIC
/* src - Source data
 * type - datatype
 * index - who sent from
 * gid - group id
 *
 * return: (OK) on success, (SUSPEND) to block sender, - on failure
 */
int msend(endpoint_t pid, const char *src, size_t size, int gid)
{
        int i, procnr;
        size_t com_size;

        /* Register the sender */
        group_list[gid].b_sender.pid = pid;
        ProcessRegister(group_list[gid].b_sender);

        //ensure valid type,src,index gid
        int t=FindIndex((int)pid);

        if (t==-1)
                return (EGENERIC);
        //Check if sender is in the process list

        if(!valid_gid(gid))
                return (EINVAL);
        //>0 members in grpu
        //Check if src is valid?

        //ATOMIC {
        //Add message to msg list
        /* if(msg_add(src) != 0)
           return -EINVAL;
         */
        if(group_list[gid].b_sender.pid > 0){
                printf("One sender at a time can send to a group!\n");
                return (EAGAIN);
        }
        if (SendSafe(t,gid)!=0)
        {
                return (ELOCKED);
        }
        EnterSend(t,gid);
        //For each process in grp
        for(i=0;i<NR_PROCS;i++){
                if(group_list[gid].member_list[i] == NULL)
                        continue;
                //if blocked waiting
                if(group_list[gid].member_list[i]->blocked < 1){
                        //else mark as pending
                        printf("Process %d not blocked, block sender after this!\n", group_list[gid].member_list[i]->pid);
                        group_list[gid].member_list[i]->pending = 1;
                        group_list[gid].npending++;
                        continue;
                }
                /* interrupted system call? */
                if(mcast_isokendpt(group_list[gid].member_list[i]->pid, &procnr) != OK){
                        /* Do NOT deliver! Deallocate everything pertaining to that proc. copying is unsafe since the proc is gone! */
                        printf("MCAST: msend detected interrupted receiver. Deallocating.\n");
                        if(group_list[gid].member_list[i]->pending > 0){
                                group_list[gid].npending--;
                        }
                        ExitReceive(FindIndex(group_list[gid].member_list[i]->pid),gid);
                        ProcessDelete((int)(group_list[gid].member_list[i]->pid));
                        group_list[gid].member_list[i]=NULL;
                        continue;
                }

                /* proc is blocked */
                //deliver msg and unblock
                com_size = MIN(group_list[gid].member_list[i]->datasize,size);
                printf("Attempting to copy %d bytes to ep %d\n", com_size, group_list[gid].member_list[i]->pid);
                sys_datacopy(pid, (vir_bytes)src, group_list[gid].member_list[i]->pid, group_list[gid].member_list[i]->dataptr, com_size);
                /* Clear blocked flag */
                group_list[gid].member_list[i]->blocked = 0;
                /* reciever is going to exit so tell deadlock detector */
                ExitReceive(FindIndex(group_list[gid].member_list[i]->pid),gid);
                /* unblock receiver */
                wake_up(group_list[gid].member_list[i]->pid, (OK));
        }
        //if any process is/was marked as pending
        if(group_list[gid].npending > 0){
                //block sender
                group_list[gid].b_sender.blocked = 1;
                group_list[gid].b_sender.dataptr = (vir_bytes)src;
                group_list[gid].b_sender.datasize = size;
                return (SUSPEND);
        }
        //else return
        ExitSend(t,gid);
        if(ProcessDelete(ProcessList[t]->pid) != 0){
                printf("Unable to deregister sender from process list after send completed!\n");
                return (EGENERIC);
        }  
        return (OK);

        //Do the message delivery between EnterSend() and ExitSend()


        //index msg pointer should skip own data
        //advance index pointer to next msg
        //}

}
/*Dest is destination location
 *gid is group id
 */
int mrecv(endpoint_t pid, void *dest, size_t size, int gid)
{
        int t=FindIndex((int)pid);
        int procnr;
        size_t com_size;
        if (t==-1)
                return -1;
        //Check if sender is in the process list

        if(!valid_gid(gid))
                return EINVAL;
        if(!valid_member(gid,t))   
                return EINVAL;

        //Does the rx have a message pending?
        if(group_list[gid].b_sender.pid > 0 && ProcessList[t]->pending > 0){
                assert(group_list[gid].npending > 0);
                /* has the sender been interrupted? */
                if(mcast_isokendpt(group_list[gid].b_sender.pid,&procnr) == OK){

                        /* yes it does, deliver immediately (i.e. dont block) */
                        com_size = MIN(size, group_list[gid].b_sender.datasize);
                        printf("MRECV: immediate delivery, attempting to copy %d bytes to ep %d\n", com_size, group_list[gid].b_sender.pid);
                        sys_datacopy(group_list[gid].b_sender.pid, group_list[gid].b_sender.dataptr, pid, (vir_bytes)dest, com_size);
                        ProcessList[t]->pending = 0;
                        group_list[gid].npending--;
                        /* all pending messages handled? */
                        if(group_list[gid].npending == 0){
                                /* unblock sender */
                                ExitSend(FindIndex(group_list[gid].b_sender.pid),gid);
                                group_list[gid].b_sender.pid = -1;
                                group_list[gid].b_sender.blocked = 0;
                                wake_up(group_list[gid].b_sender.pid, OK);
                        }
                        /* receive has been handled, return. */
                        return (OK);
                }
                else {
                        printf("MCAST: Blocked sender is no longer valid, handling as a blocking msend()\n");
                }       
        }
        /* Otherwise we are going to wait, begin deadlock safety checks */
        if (ReceiveSafe(t,gid)!=0)
        {
                return (ELOCKED);
        }
        EnterReceive(t,gid);
        //Do the message delivery between EnterReceive() and ExitReceive()
        /* store info and block receiver */
        ProcessList[t]->dataptr = (vir_bytes)dest;
        ProcessList[t]->datasize = size;
        /* Caller is getting blocked, receiver will ExitReceive() in the msend() handler */
        return (SUSPEND);

        //Will need to be changed
        // size_t size = get_next_size(index);
        // const void *src = get_next(index);

        //memcpy(dest,src,size);

}

int opengroup(endpoint_t pid, int gid, int *index)
{
        if(gid > MAX_GROUPS || index == NULL || *index < 0 || *index > NR_PROCS){
                return EINVAL;
        }
        if(valid_gid(gid))
                add_member(pid, gid, index);
		/*
        else
        {
                add_group(gid);
                add_member(pid, gid, index);
        }
		*/
        return OK;
}
int closegroup(endpoint_t pid, int gid)
{

        if(valid_gid(gid) && valid_member(gid,index))
        {
			rm_member(gid,index);
				find_member_index(pid,gid) = NULL;
                //rm_member(gid,index);
                /*
                   if(group_list[gid].nmembers == 0)
                   rm_group(gid);
                 */
                return OK;
        } else {
                return EINVAL;
        }
}
mc_member_t* find_member_index( endpoint_t pid, int gid)
{
	mc_member_t *p;
	for(p = group_list[gid].member_list; p != NULL; p++)
	{
		if(p->pid == pid)
			return p;
	}
	return NULL;
}

}
/*
void add_group(int gid)
{
        if(gid < 32)
        {
                mc_group_t group;
                group.nmembers = 0;
                group_list[gid] = group;
        }
}
*/
/*
void rm_group(int gid)
{
        group_list[gid].valid = 0;
}
*/
void add_member(endpoint_t pid, int gid, int *index)
{
        if(!valid_member(gid,*index)) //Not already a member  
        {
                group_list[gid].nmembers++;
                int i;
                for(i = 0; i < NR_PROCS; i++)
                {
                        if(group_list[gid].member_list[i] == NULL)
                        {
							//TODO only malloc if it isnt in the process list
							int mindex = FindIndex(pid); 
							if(FindIndex(pid) == -1)
							{
                                group_list[gid].member_list[i] = malloc(sizeof(mc_member_t));
                                *index = i; //Set index, index
                                //INSTANTIATE VARS
                                group_list[gid].member_list[i]->pending = 0;
                                group_list[gid].member_list[i]->blocked = 0;
                                group_list[gid].member_list[i]->pid = pid;
                                group_list[gid].member_list[i]->numgroups = 1;
                                //TODO check for -2 error code and propagate up
                                ProcessRegister(group_list[gid].member_list[i]);
                                break;
							}
							else
							{
								group_list[gid].member_list[i] = ProcessList[mindex];
                                group_list[gid].member_list[i]->numgroups++; 
								break;
							}
                        }
                }
        }
}
int recovergroup(int gid){
        int i;
        /* Handle invalid group properly */
        if(gid < 0 || gid > MAX_GROUPS)
                return (EINVAL);
        /* fail all blocked senders and receivers with ELOCKED */
        for(i=0; i<NR_PROCS; i++){
                if(group_list[gid].member_list[i] == NULL)
                        continue;
                /* clear all pending requests */
                if(group_list[gid].member_list[i].pending > 0){
                        group_list[gid].member_list[i].pending = 0;
                        group_list[gid].npending--;
                }
                /* wake all blocked receivers with ELOCKED */
                if(group_list[gid].member_list[i]->blocked == 1){
                        group_list[gid].member_list[i]->blocked = 0;
                        reply(group_list[gid].member_list[i]->pid, (ELOCKED));
                }
                /* is there a blocked sender? If so wake and return ELOCKED. */
                if(group_list[gid].b_sender.pid != -1){
                        reply(group_list[gid].b_sender.pid, (ELOCKED));
                        group_list[gid].b_sender.pid = -1;
                }
        }
        return (OK);
}
void rm_member(int gid, int index)
{
        if(valid_member(gid,index)) //Ensure its a member
        {
				int mindex = FindIndex(pid); 
				assert(mindex != -1);
				assert(group_list[gid].member_list[index]->numgroups != 0);
				if(group_list[gid].member_list[index]->numgroups = 1)
				{
					group_list[gid].member_list[index]->numgroups = 0;
                	ProcessDelete(group_list[gid].member_list[index]->pid);
				}
                group_list[gid].nmembers--;
                group_list[gid].member_list[index] = NULL;
        }
}
int valid_index(int index)
{
        return 0;
}
int valid_gid(int gid)
{
        if(gid < MAX_GROUPS && gid >= 0)
                return 1;
        return 0;
}
//Checks if gid is valid, and if member of index exists, and within bounds
int valid_member(int gid, int index)
{
        if(!valid_gid(gid))
                return 0;
        if(index >= 100)
                return 0;
        if(group_list[gid].member_list[index] != NULL)
                return 1;
        return 0;
}
void *get_next(int index)
{
        void *p;
        return p;
}
