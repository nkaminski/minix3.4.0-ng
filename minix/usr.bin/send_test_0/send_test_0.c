#include <unistd.h>
#include <stdio.h>
const int group_nr = 3;
const int alt_group_nr = 7;
const char msg[] = "SettingTheRightBitsInTheRightPlaces!";
int main(int argc, char** args) {
	int rv;
	printf("testing msend syscall to group 3\n");
	rv = msend(group_nr, msg, sizeof(msg));
	printf("returned %i\n",rv);
	printf("testing msend syscall to group 7\n");
	rv = msend(alt_group_nr, msg, sizeof(msg));
	printf("returned %i\n",rv);
	return 0;
}
