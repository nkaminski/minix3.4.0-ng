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

//TODO Edit read_super, write_super, rw_super? to read/write s_rcdir_inode

//Set buf to inode list
//Return size of buffer = #inodes pointers * sizeof(uint32_t)
size_t get_recovery(void *buf)
{
	struct super_block *sp;//get super here(read)
	read_super(sp);
	size_t size = sp->s_ninodes*sizeof(uint32_t); //Size of list
	uint32_t rcinode = sp->s_rcdir_inode;  /* inode that stores rcdir list */

	struct fsdriver_data *data;
	off_t position; //TODO what does this need to be
	fs_readwrite((ino_t)rcinode,data,size,position,FSC_READ);
	
	//give inode list in buf?	
	return size;
}
void put_recovery(void *buf)
{
	struct super_block *sp;//get super here(read)
	read_super(sp);
	uint32_t rcinode = sp->s_rcdir_inode;  /* inode that stores rcdir list */
	size_t size = sp->s_ninodes*sizeof(uint32_t);

	struct fs_driver_data *data = buf; //TODO not sure if correct
	off_t position; //TODO what does this need to be
	//put inode list back
	fs_readwrite((ino_t)rcinode,data,size,position,FSC_WRITE);
	//put back (write)super block
	write_super(sp);
}
