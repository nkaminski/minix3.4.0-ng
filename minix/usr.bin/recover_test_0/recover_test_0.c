#include <unistd.h>
#include <stdio.h>
#include <string.h>
const int group_nr = 0;
int main(int argc, char** args) {
	int rv;
   printf("calling recovergroup(0)\n");
	rv = recoverGroup(group_nr);
   printf("returned %i\n",rv);
   return 0;
}
