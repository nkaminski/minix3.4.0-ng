#include <unistd.h>
#include <stdio.h>
#include <string.h>
const int group_nr = 0;
int main(int argc, char** args) {
	int rv;
	char msg[128];
	memset(msg,0x00,128);
   printf("Receiving from group 0 without OpenGroup.\n");
	rv = mreceive(group_nr, msg, 127);
	printf("returned %i, message is %s\n",rv,msg);
   return 0;
}