#include "types.h"
extern bool Send[NR_PROCS][NR_PROCS];
extern bool Receive[NR_PROCS];
extern mc_member_list_t ProcessList;				//A list of processes

int CircleCheckSend(int PIndex, int GIndex);
//int CircleCheckReceive(int PIndex, int GIndex);
int FindIndex(int pid);
int ProcessRegister(mc_member_t *p);
int ProcessDelete(int pid);
int SendSafe(int pid, int GIndex);
int ReceiveSafe(int pid, int GIndex);
int EnterSend(int pid, int GIndex);
int EnterReceive(int pid, int GIndex);
int ExitSend(int pid, int GIndex);
int ExitReceive(int pid, int GIndex);
int ProcessActive(int pid);
void deadlock_init();


void printAll();
void printSendMatrix();
void printReceiveMatrix();
void printGroup();
void printProcessList();


