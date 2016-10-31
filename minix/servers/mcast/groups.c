#include <string.h>
#include <stdlib.h>
#include "types.h"
#include "deadlock.c"

/* Decide Messages Structure
 * Each member in group should have pointer
 * to next object to get
 */
//unistd.h for msend

int msend(char *src, size_t size, int gid);
int mrecv(void *dest, int index, int gid);
int valid_index(int index);
int msg_add(void *src);
int msend(char *src, size_t size, int gid);
int mrecv(void *dest, int index, int gid);
void opengroup(int gid, int *index);
void closegroup(int gid,int index);
void add_group(int gid);
void rm_group(int gid);
void add_member(int gid, int *index);
void rm_member(int gid, int index);
int valid_index(int index);
int valid_gid(int gid);
int valid_member(int gid, int index);
size_t get_next_size(int index);
void *get_next(int index);
int msg_add(void *src);
void init();

int main(int argv, char **argc)
{
	init();
	return 0;
}
void init()
{
	int i;
	for(i = 0; i < MAX_GROUPS; i++)
		group_list[i].nmembers = 0;
	deadlock_init();
}

//MESSAGE APPENDING MUST BE ATOMIC
/* src - Source data
 * type - datatype
 * index - who sent from
 * gid - group id
 *
 * return: 0 on success, -1 failure
 */
int msend(endpoint_t pid, char *src, size_t size, int gid)
{
	//ensure valid type,src,index gid
	int t=FindIndex((int)pid);
	if (t==-1)
		return -1;
	//Check if sender is in the process list

	if(!valid_gid(gid))
		return -1;
	//>0 members in grpu
	//Check if src is valid?
	
	//ATOMIC {
	//Add message to msg list
	if(msg_add(src) != 0)
		return -1;

	if (SendSafe(t,gid)==0)
	{
		EnterSend(t,gid);
		//Do the message delivery between EnterSend() and ExitSend()


	//For each process in grp
	//if blocked waiting
	//deliver msg and unblock
	//else mark as pending
	//
	//if any process was marked as pending
	//block sender

	//index msg pointer should skip own data
	//advance index pointer to next msg
	//}


		ExitSend(t,gid);
	}
	return 0;
}
/*Dest is destination location
 *index is index of calling process in group
 *gid is group id
 */
int mrecv(endpoint_t pid, void *dest, int index, int gid)
{
	int t=FindIndex((int)pid);
	if (t==-1)
		return -1;
	//Check if sender is in the process list

	if(!valid_gid(gid))
		return -1;
	if(!valid_member(gid,index))	
		return -1;

	if (ReceiveSafe(t,gid)==0)
	{
		EnterReceive(t,gid);
		//Do the message delivery between EnterSend() and ExitSend()

		
	//Will need to be changed
	size_t size = get_next_size(index);
	const void *src = get_next(index);

	//if msg pending for caller
	//deliver msg
		//if no more members pending
		//unblock sender, do not block caller
	//else
	//set state as blocked & block caller

	//i
	//memcpy(dest,src,size);
		ExitReceive(t,gid);
	}
	return 0;
}
//Returns a index to process
void opengroup(int gid, int *index)
{
	if(valid_gid(gid))
		add_member(gid,index);
	else
	{
		add_group(gid);
		add_member(gid,index);
	}
}
void closegroup(int gid,int index)
{
	if(valid_gid(gid) && valid_member(gid,index))
	{
		rm_member(gid,index);
		/*
		if(group_list[gid].nmembers == 0)
			rm_group(gid);
			*/
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
void add_member(int gid, int *index)
{
	if(!valid_member(gid,*index)) //Not already a member	
	{
		group_list[gid].nmembers++;
		int i;
		for(i = 0; i < MAX_PROCS; i++)
		{
			if(group_list[gid].member_list[i] == NULL)
			{
				group_list[gid].member_list[i] = malloc(sizeof(mc_member_t));
				*index = i; //Set index, index
				//INSTANTIATE VARS HERE?
				break;
			}
		}
	}
}
void rm_member(int gid, int index)
{
	if(valid_member(gid,index)) //Ensure its a member
	{
		group_list[gid].nmembers--;
		free(group_list[gid].member_list[index]);
		group_list[gid].member_list[index] = NULL;
	}
}
int valid_index(int index)
{
	return 0;
}
int valid_gid(int gid)
{
	if(gid < 32)
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
size_t get_next_size(int index)
{
	return 0;
}
void *get_next(int index)
{
	void *p;
	return p;
}
int msg_add(void *src)
{
	return 0;
}
