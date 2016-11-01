#include <unistd.h>
#include <stdio.h>
const int group_nr = 1;
const char msg[] = "EngineeringInReverse!";
int main(int argc, char** args) {
	int rv;
	printf("testing msend syscall to group %d\n",group_nr);
	rv = msend(group_nr, msg, sizeof(msg));
	printf("returned %i\n",rv);
	return 0;
}
