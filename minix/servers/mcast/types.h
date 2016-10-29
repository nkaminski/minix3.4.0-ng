#define MAX_GROUPS 32
int ProcessList[MAX_PROCS];

typedef struct {
	unsigned char valid;
	endpoint_t pid;
	unsigned char blocked;
	vir_addr dataptr;
	int datasize;
} mc_member_t;

typedef struct {
	int groupnr;
	int nmembers;
	mc_member_t[MAX_PROCS];
} mc_group_t;

typedef mc_group_t[MAX_GROUPS] mc_group_list_t;

 
