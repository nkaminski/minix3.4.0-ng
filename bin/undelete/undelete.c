#include <unistd.h>
#include <stdio.h>
int main(int argc, char **argv){
   if(argc != 2){
           printf("Usage: %s <path of file to undelete>\n",argv[0]);
           return 1;
   }
   return undelete(argv[1]);
}
