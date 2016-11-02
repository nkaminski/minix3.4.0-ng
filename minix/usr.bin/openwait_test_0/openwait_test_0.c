#include <unistd.h>
#include <stdio.h>
#include <string.h>
const int group_nr = 0;
int main(int argc, char** args) {
	int rv;
   printf("calling opengroup(0)\n");
	rv = openGroup(group_nr);
   printf("waiting 5 seconds\n");
   usleep(5E6);
   printf("calling closegroup(0)\n");
	rv = closeGroup(group_nr);
   printf("returned %i\n",rv);
   return 0;
}
