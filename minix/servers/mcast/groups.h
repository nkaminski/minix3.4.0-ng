#include <string.h>
#include <stdlib.h>

int msend(endpoint_t pid, const char *src, size_t size, int gid);
int mrecv(endpoint_t pid, void *dest, size_t size, int gid);
int valid_index(int index);
int msg_add(void *src);
int opengroup(endpoint_t pid, int gid, int *index);
int closegroup(int gid,int index);
void add_group(int gid);
void rm_group(int gid);
void add_member(endpoint_t pid, int gid, int *index);
void rm_member(int gid, int index);
int valid_index(int index);
int valid_gid(int gid);
int valid_member(int gid, int index);
size_t get_next_size(int index);
void *get_next(int index);
int msg_add(void *src);
void init_groups();
mc_member_t* find_member_index( endpoint_t pid, int gid);
