#define MAX_GROUPS 32
#define MAX_PROCS 100
int ProcessList[MAX_PROCS];

typedef struct {
	unsigned char valid;
	endpoint_t pid;
	unsigned char blocked;

	struct vir_addr dataptr;
	int datasize;
} mc_member_t;

typedef mc_member_t *member_list_t[MAX_PROCS];

typedef struct {
	//group number implied as index in list
	unsigned char valid;
	int nmembers;
	member_list_t member_list;
} mc_group_t;

typedef mc_group_t group_list_t[MAX_GROUPS];
//mc_group_t mc_group_list_t[MAX_GROUPS];
group_list_t group_list;
