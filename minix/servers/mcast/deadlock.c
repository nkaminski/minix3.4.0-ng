#include "mcast.h"
#include "deadlock.h"

int total=0;										//Total number of registered processes on list
void printProcessList()								//print all registered processes
{
	int i;
	printf("Total processes = %d\n",total);
	printf("Process list:\n");
	for (i=0;i<total;i++)
		printf("Pid %2d: %d\n",i,(int)ProcessList[i]->pid);
}

void printGroup()									//Print all valid groups
{
	printf("Group list:\n");
	int i,j;
	for (i=0;i<MAX_GROUPS;i++)
		if (group_list[i].nmembers!=0)
		{
			printf("Group %d: ",i);
			for (j=0;j<NR_PROCS;j++)
				if (group_list[i].member_list[j]!=NULL)
					printf("%d ",(int)group_list[i].member_list[j]->pid);
			puts("");
		}
}

void printSendMatrix()								//Print the sending matrix
{
	int i,j;
	puts("Send matrix:");
	if (total>0)
	{
		for (i=0;i<=total;i++) printf("------");
		puts("");
		printf("     |");
		for (i=0;i<total;i++) printf("%5d|",(int)ProcessList[i]->pid);
		puts("");
		for (i=0;i<=total;i++) printf("------");
		puts("");
		for (i=0;i<total;i++)
		{
			printf("%5d|",(int)ProcessList[i]->pid);
			for (j=0;j<total;j++)
				printf("%5d|",(int)Send[i][j]);
			puts("");
		}
		for (i=0;i<=total;i++) printf("------");
		puts("");
	}
}

void printReceiveMatrix()							//Print the receiving matrix
{
	int i,j;
	puts("Receive matrix:");
	if (total>0)
	{
		for (i=0;i<total;i++)
		{
			printf("Pid : %7d | %d\n",ProcessList[i]->pid,Receive[i]);
		}
	}
}

void printAll()										//For debug
{
	printProcessList();
	printGroup();
	printSendMatrix();
	printReceiveMatrix();
}

int ProcessActive(int pid)							//Check if a process is active. Returns -1 if active, 
{													//0 if pid not found or inactive(safe to Delete or CloseGroup)
	int t;
	t=FindIndex(pid);
	if (t==-1) return 0;
	int i;
	for (i=0;i<total;i++)
	{
		if (Send[t][i]==1) return -1;
		if (Send[i][t]==1) return -1;
		if (Receive[t][i]==1) return -1;
		if (Receive[i][t]==1) return -1;
	}
	return 0;
}

int EnterSend(int pid, int GIndex)					//Enter sending blocking
{
	int i,t,k;
	t=FindIndex(pid);
	for (i=0;i<NR_PROCS;i++)
	{
		if (group_list[GIndex].member_list[i]!=NULL)
		{
			k=FindIndex((int)group_list[GIndex].member_list[i]->pid);
			Send[t][k]=1;
		}
	}
	return 0;
}

int EnterReceive(int pid, int GIndex)				//Enter receiving blocking
{
	int t;
	t=FindIndex(pid);
	Receive[t]=1;
	return 0;
}

int ExitSend(int pid, int GIndex)					//Exit sending blocking
{
	int i,t;
	t=FindIndex(pid);
	for (i=0;i<total;i++) Send[t][i]=0;
	return 0;
}

int ExitReceive(int pid, int GIndex)				//Exit receiving blocking
{
	int t;
	t=FindIndex(pid);
	Receive[t]=0;
	return 0;
}

int SendSafe(int pid, int GIndex)					//Check if it is safe to send. Returns 0 if safe, -1 if unsafe
{
	if (group_list[GIndex].nmembers==0)	return 0;
	int t;
	t=FindIndex(pid);
	if (t==-1) return -1;
	return CircleCheckSend(t,GIndex);
}

int ReceiveSafe(int pid, int GIndex)				//Check if it is safe to receive. Returns 0 if safe, -1 if unsafe
{
	if (valid_member((endpoint_t)pid,GIndex)==1) return -1;
	else return 0;
}

int CircleCheckSend(int PIndex, int GIndex)			//Circle checking for sending. Returns 0 if safe, -1 if unsafe
{
	int head,tail,i,t;
	int queue[NR_PROCS];
	head=0;
	tail=0;
	queue[head]=PIndex;
	for (i=0;i<NR_PROCS;i++)
	{
		if (group_list[i].nmembers!=0)
		{
			t=FindIndex((int)group_list[GIndex].member_list[i]->pid);
			if (t==PIndex) return -1;
			tail++;
			queue[tail]=t;
		}
	}
	head++;
	while(head<=tail)
	{
		for (i=0;i<NR_PROCS;i++)
		{
			if (Send[queue[head]][i]!=0)
			{
				if (i==PIndex) return -1;
				tail++;
				queue[tail]=i;

			}
		}
		head++;
	}
	return 0;
}

/*
int CircleCheckReceive(int PIndex, int GIndex)		//Circle checking for receiving. Returns 0 if safe, -1 if unsafe
{
	int queue[NR_PROCS];
	int head,tail,i,t;
	head=0;
	tail=0;
	queue[head]=PIndex;
	for (i=0;i<NR_PROCS;i++)
	{
		if (group_list[i].nmembers!=0)
		{
			t=FindIndex((int)group_list[GIndex].member_list[i]->pid);
			if (t==PIndex) return -1;
			tail++;
			queue[tail]=t;
		}
	}
	head++;
	while(head<=tail)
	{
		for (i=0;i<NR_PROCS;i++)
		{
			if (Receive[queue[head]][i]!=0)
			{
				if (i==PIndex) return -1;
				tail++;
				queue[tail]=i;

			}
		}
		head++;
	}
	return 0;
}
*/


int FindIndex(int pid)								//Find the index in process list of a given Pid, returns -1 if not found
{
	int i;
	for (i=0;i<total;i++)
		if ((int)ProcessList[i]->pid==pid) return i;
	return -1;
}

int ProcessRegister(mc_member_t *p)					//Register a new process into process list
{
	if (FindIndex((int)p->pid)!=-1) return -1;					//Error : process already in list
	if (total==NR_PROCS) return -2;							//Error : max number of processes
	ProcessList[total]=p;
	total++;
	int i;
	for (i=0;i<total;i++)
	{
		Send[i][total-1]=0;
		Send[total-1][i]=0;
		Receive[i][total-1]=0;
		Receive[total-1][i]=0;
	}
	return 0;
}

int ProcessDelete(int pid)							//Delete a process from process list
{
	int t;
	mc_member_t *p;
	t=FindIndex(pid);
	if (t==-1) return -1;
	p=ProcessList[t];										//Error : Pid not found
	if (ProcessActive(pid)==-1) return -1;						//Error : Process still active
	int i,j,k;
	for (i=t;i<total-1;i++)										//Delete from sending and receiving matrix
		for (j=0;j<total;j++)									//
		{														//
			Send[i][j]=Send[i+1][j];							//
			Receive[i][j]=Receive[i+1][j];						//
		}														//
	for (j=0;j<total;j++)										//
		for (i=t;i<total-1;i++)									//
		{														//
			Send[j][i]=Send[j][i+1];							//
			Receive[j][i]=Receive[j][i+1];						//
		}														//
	for (i=0;i<total;i++)										//
	{															//
		Send[i][total-1]=0;										//
		Send[total-1][i]=0;										//
		Receive[i][total-1]=0;									//
		Receive[total-1][i]=0;									//Delete from sending and receiving matrix
	}
	for (i=t;i<total-1;i++)										//Delete from process list
		ProcessList[i]=ProcessList[i+1];
	total--;
	free(p);
	return 0;
}

void deadlock_init()								//Initialize all lists
{
	int i,j;
	total=0;
	for (i=0;i<NR_PROCS;i++)									//Sending and receiving matrix
		for (j=0;j<NR_PROCS;j++)
		{
			Send[i][j]=0;
			Receive[i][j]=0;
		}
}
