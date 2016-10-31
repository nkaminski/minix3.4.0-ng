#define MAX_GROUPS 32
#ifndef _MCAST_TYPES
#define _MCAST_TYPES
#include <minix/config.h>
#include <stdbool.h>
//int ProcessList[NR_PROCS];

typedef struct {
	unsigned char valid;
	endpoint_t pid;
	unsigned char blocked;

	struct vir_addr dataptr;
	int datasize;
} mc_member_t;

typedef mc_member_t *mc_member_list_t[NR_PROCS];

typedef struct {
	//group number implied as index in list
	unsigned char valid;
	int nmembers;
	mc_member_list_t member_list;
} mc_group_t;

typedef mc_group_t group_list_t[MAX_GROUPS];
//mc_group_t mc_group_list_t[MAX_GROUPS];
extern group_list_t group_list;
#endif
