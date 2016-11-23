#include "fs.h"
#include <sys/param.h>
#include <stdlib.h>
#include <minix/libminixfs.h>
#include "super.h"
#include "inode.h"

int gc_undeletable(dev_t dev){

/* 0 means NO space was able to be reclaimed,
non zero means at least one zone was freed */
return 0;

}
