#include <string.h>
#include <stdlib.h>

int msend(endpoint_t pid, const char *src, size_t size, int gid);
int mrecv(endpoint_t pid, void *dest, size_t size, int gid);
int valid_index(int index);
int msg_add(void *src);
int opengroup(endpoint_t pid, int gid);
int closegroup(endpoint_t pid, int gid);
void recovergroup(int gid);
void add_group(int gid);
void rm_group(int gid);
void add_member(endpoint_t pid, int gid);
void rm_member(endpoint_t pid, gid);
int valid_gid(int gid);
int valid_member(endpoint_t pid,int gid);
int msg_add(void *src);
void init_groups();
mc_member_t* find_member_index(endpoint_t pid, int gid, int *index);
