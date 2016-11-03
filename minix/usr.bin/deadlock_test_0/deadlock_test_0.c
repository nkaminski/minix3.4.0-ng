#include <unistd.h>
#include <stdio.h>
#include <errno.h>
const int group_nr = 0;
const char msg[] = "SettingTheRightBitsInTheRightPlaces!";
int main(int argc, char** args) {
	int rv;
   rv = openGroup(group_nr);
	printf("opengroup returned %i\n",rv);
	printf("testing msend syscall to group %d\n",group_nr);
	rv = msend(group_nr, msg, sizeof(msg));
	printf("msend returned %i\n",rv);
   if(rv != -ELOCKED){
           printf("Deadlock detection not functional\n");
   }
	rv = closeGroup(group_nr);
	printf("closegroup returned %i\n",rv);
   return 0;
}
