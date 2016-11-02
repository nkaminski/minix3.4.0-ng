#define MAX_GROUPS 32
#ifndef _MCAST_TYPES
#define _MCAST_TYPES
#include <minix/config.h>
#include <stdbool.h>
//int ProcessList[NR_PROCS];

typedef struct {
   endpoint_t pid;
	unsigned char blocked;
	unsigned char pending;
	vir_bytes dataptr;
	size_t numgroups; //Number of groups member is part of
	size_t datasize;
} mc_member_t;

typedef mc_member_t *mc_member_list_t[NR_PROCS];

typedef struct {
	//group number implied as index in list
	mc_member_t b_sender;
   int s_valid;
	int npending;
	int nmembers;
	mc_member_list_t member_list;
} mc_group_t;

typedef mc_group_t group_list_t[MAX_GROUPS];
//mc_group_t mc_group_list_t[MAX_GROUPS];
extern group_list_t group_list;
#endif
