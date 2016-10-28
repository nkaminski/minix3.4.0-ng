#include <unistd.h>
#include <stdio.h>
int main(int argc, char** args) {
	int rv;
	printf("testing mtest syscall\n");
	rv = mtest();
	printf("returned %i\n",rv);
	return 0;
}
