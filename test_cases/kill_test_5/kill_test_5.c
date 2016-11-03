#include <unistd.h>
#include <stdio.h>
const int group_nr = 0;
const char msg[] = "DeliverThisBeforeImKilled";
int main(int argc, char** args) {
	int rv;
	rv = msend(group_nr, msg, sizeof(msg));
	printf("Sender killed during sending to group %d\n",group_nr);
	printf("returned %i\n",rv);
	return 0;
}