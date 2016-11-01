#include "mcast.h"
#include "deadlock.h"

int total=0;										//Total number of registered processes on list
void printProcessList()								//print all registered processes
{
	int i;
	printf("Total processes = %d\n",total);
	printf("Process list:\n");
	for (i=0;i<total;i++)
		printf("Pid %2d: %d\n",i,(int)ProcessList[i].pid);
}

void printGroup()									//Print all valid groups
{
	printf("Group list:\n");
	int i,j;
	for (i=0;i<MAX_GROUPS;i++)
		if (group_list[i].nmembers!=0)
		{
			printf("Group %d: ",i);
			for (j=0;j<group_list[i].nmembers;j++)
				printf("%d ",(int)group_list[i].member_list[j]->pid);
			puts("");
		}
}

void printSendMatrix()								//Print the sending matrix
{
	int i,j;
	puts("Send matrix:");
	printf("     |");
	for (i=0;i<total;i++) printf("%5d|",(int)ProcessList[i].pid);
	puts("");
	for (i=0;i<total;i++)
	{
		printf("%5d|",(int)ProcessList[i].pid);
		for (j=0;j<total;j++)
			printf("%5d|",(int)Send[i][j]);
		puts("");
	}
}

void printReceiveMatrix()							//Print the receiving matrix
{
	int i,j;
	puts("Receive matrix:");
	for (i=0;i<=total;i++) printf("------");
	puts("");
	printf("     |");
	for (i=0;i<total;i++) printf("%5d|",(int)ProcessList[i].pid);
	puts("");
	for (i=0;i<=total;i++) printf("------");
	puts("");
	for (i=0;i<total;i++)
	{
		printf("%5d|",(int)ProcessList[i].pid);
		for (j=0;j<total;j++)
			printf("%5d|",(int)Receive[i][j]);
		puts("");
	}
	for (i=0;i<=total;i++) printf("------");
	puts("");
}

void do_deadlock_test2()
{
	mc_member_t p0,p1,p2,p3,p4,p5,p6;
	p0.pid=(int)100;
	p1.pid=(int)101;
	p2.pid=(int)102;
	p3.pid=(int)103;
	p4.pid=(int)104;
	p5.pid=(int)105;
	ProcessRegister(p0);
	ProcessRegister(p1);
	ProcessRegister(p2);
	ProcessRegister(p3);
	ProcessRegister(p4);
	ProcessRegister(p5);
	printProcessList();

	ProcessDelete(100);
	printProcessList();

	/*OpenGroup(100,0);
	OpenGroup(101,0);
	OpenGroup(102,1);
	OpenGroup(103,1);
	OpenGroup(104,2);
	OpenGroup(105,2);*/
	printGroup();

	//CloseGroup(102,1);
	printGroup();

	//OpenGroup(p2,1);
	

	int t;
	/*t=SendSafe((int)p0.pid,1);
	EnterSend((int)p0.pid,1);
	printSendMatrix();

	t=SendSafe((int)p1.pid,1);
	EnterSend((int)p1.pid,1);
	printSendMatrix();

	t=SendSafe((int)p2.pid,2);
	if (t==0) EnterSend((int)p2.pid,2);
	printSendMatrix();

	t=SendSafe((int)p4.pid,1);
	if (t==0) EnterSend((int)p4.pid,1);
	printSendMatrix();*/

	t=ReceiveSafe((int)p0.pid,1);
	if (t==0) EnterReceive((int)p0.pid,1);
	printReceiveMatrix();

	t=ReceiveSafe((int)p1.pid,1);
	if (t==0) EnterReceive((int)p1.pid,1);
	printReceiveMatrix();

	t=ReceiveSafe((int)p2.pid,2);
	if (t==0) EnterReceive((int)p2.pid,2);
	printReceiveMatrix();

	t=ReceiveSafe((int)p4.pid,1);
	if (t==0) EnterReceive((int)p4.pid,1);
	printReceiveMatrix();
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
	for (i=0;i<group_list[GIndex].nmembers;i++)
	{
		k=FindIndex((int)group_list[GIndex].member_list[i]->pid);
		Send[t][k]=1;
	}
	return 0;
}

int EnterReceive(int pid, int GIndex)				//Enter receiving blocking
{
	int i,t,k;
	t=FindIndex(pid);
	for (i=0;i<group_list[GIndex].nmembers;i++)
	{
		k=FindIndex((int)group_list[GIndex].member_list[i]->pid);
		Receive[t][k]=1;
	}
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
	int i,t;
	t=FindIndex(pid);
	for (i=0;i<total;i++) Receive[t][i]=0;
	return 0;
}

int SendSafe(int pid, int GIndex)					//Check if it is safe to send. Returns 0 if safe, -1 if unsafe
{
	if (group_list[GIndex].nmembers==0)	return -1;
	int t;
	t=FindIndex(pid);
	if (t==-1) return -1;
	return CircleCheckSend(t,GIndex);
}

int ReceiveSafe(int pid, int GIndex)				//Check if it is safe to receive. Returns 0 if safe, -1 if unsafe
{
	if (group_list[GIndex].nmembers==0) return -1;
	int t;
	t=FindIndex(pid);
	if (t==-1) return -1;
	return CircleCheckReceive(t,GIndex);
}

int CircleCheckSend(int PIndex, int GIndex)			//Circle checking for sending. Returns 0 if safe, -1 if unsafe
{
	int head,tail,i,t;
	int queue[NR_PROCS];
	head=0;
	tail=0;
	queue[head]=PIndex;
	for (i=0;i<group_list[GIndex].nmembers;i++)
	{
		t=FindIndex((int)group_list[GIndex].member_list[i]->pid);
		if (t==PIndex) return -1;
		tail++;
		queue[tail]=t;
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

int CircleCheckReceive(int PIndex, int GIndex)		//Circle checking for receiving. Returns 0 if safe, -1 if unsafe
{
	int queue[NR_PROCS];
	int head,tail,i,t;
	head=0;
	tail=0;
	queue[head]=PIndex;
	for (i=0;i<group_list[GIndex].nmembers;i++)
	{
		t=FindIndex((int)group_list[GIndex].member_list[i]->pid);
		if (t==PIndex) return -1;
		tail++;
		queue[tail]=t;
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

int FindIndex(int pid)								//Find the index in process list of a given Pid, returns -1 if not found
{
	int i;
	for (i=0;i<total;i++)
		if ((int)ProcessList[i].pid==pid) return i;
	return -1;
}

int ProcessRegister(mc_member_t p)					//Register a new process into process list
{
	if (FindIndex((int)p.pid)!=-1) return -1;					//Error : process already in list
	if (total==NR_PROCS) return -1;							//Error : max number of processes
	ProcessList[total]=p;
	total++;
	int i;
	for (i=0;i<total;i++)
	{
		Send[i][total]=0;
		Send[total][i]=0;
		Receive[i][total]=0;
		Receive[total][i]=0;
	}
	return 0;
}

int ProcessDelete(int pid)							//Delete a process from process list
{
	int t;
	t=FindIndex(pid);
	if (t==-1) return -1;										//Error : Pid not found
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
	for (i=0;i<MAX_GROUPS;i++)									//Delete from group list
	{
		if (group_list[i].nmembers>0)
		{
			for (j=0;j<group_list[i].nmembers;j++)
				if (group_list[i].member_list[j]->pid==pid) break;
			for (k=j;k<group_list[i].nmembers-1;k++)
				group_list[i].member_list[k]=group_list[i].mc_member_list[k+1];
			group_list[i].nmembers--;
		}
	}
	for (i=t;i<total-1;i++)										//Delete from process list
		ProcessList[i]=ProcessList[i+1];
	total--;
	return 0;
}

/*int OpenGroup(int pid, int GIndex)					//Open a new group for a list of processes. Returns the index of new group on success, -1 on failure
{
	int t=FindIndex(pid);
	if (t==-1) return -1;										//Error : Pid not found
	int i;
	for (i=0;i<group_list[GIndex].nmembers;i++)
		if ((int)group_list[GIndex].member_list[i]->pid==pid) return -1;	//Error : process already in group
	group_list[GIndex].member_list[group_list[GIndex].nmembers]=&ProcessList[t];
	group_list[GIndex].nmembers++;
	return 0;
}

int CloseGroup(int pid, int GIndex)					//Close a group. Returns 1 on success, -1 if the group doesn't exist
{
	int t=FindIndex(pid);
	if (t==-1) return -1;										//Error : Pid not found
	if (PorcessActive(pid)==-1) return -1;						//Error : Process still active
	int i;
	for (i=0;i<group_list[GIndex].nmembers;i++)
		if ((int)group_list[GIndex].member_list[i]->pid==pid)
		{
			int j;
			for (j=i;j<group_list[GIndex].nmembers-1;j++)
				group_list[GIndex].member_list[j]=group_list[GIndex].mc_member_list_t[j+1];
			group_list[GIndex].nmembers--;
			return 0;											//Success
		}
	return -1;													//Error : process not in this group
}*/

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

/*int main()
{
	deadlock_init();
	do_deadlock_test2();
}*/
