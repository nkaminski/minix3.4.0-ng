#include <unistd.h>
#include <stdio.h>
#include <string.h>
const int group_nr = 3;
const int alt_group_nr = 7;
int main(int argc, char** args) {
	int rv;
	char msg[128];
	memset(msg,0x00,128);
	printf("testing mreceive syscall from group 3\n");
	rv = mreceive(group_nr, msg, 127);
	printf("returned %i, message is %s\n",rv,msg);

	memset(msg,0x00,128);
	printf("testing mreceive syscall to group 7\n");
	rv = mreceive(alt_group_nr, msg, 127);
	printf("returned %i, message is %s\n",rv,msg);
	return 0;
}
