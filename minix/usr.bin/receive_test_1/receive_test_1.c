#include <unistd.h>
#include <stdio.h>
#include <string.h>
const int group_nr = 1;
int main(int argc, char** args) {
	int rv;
	char msg[128];
	memset(msg,0x00,128);
	printf("testing mreceive syscall from group %d\n", group_nr);
	rv = mreceive(group_nr, msg, 127);
	printf("returned %i, message is %s\n",rv,msg);
	return 0;
}
