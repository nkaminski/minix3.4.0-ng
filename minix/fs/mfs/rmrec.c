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

//struct buf *get_block_map(register struct inode *rip, u64_t position);
//Set buf to inode list
//Return size of buffer = #inodes pointers * sizeof(uint32_t)
struct buf get_recovery(void *buf, dev_t dev)
{
	struct super_block *sp;//get super here(read)
	sp->s_dev = dev;

	if(read_super(sp) != OK)
		panic("Couldnt Read Super for get Recovery");

	size_t size = (sp->s_block_size/sizeof(uint32_t))-1; //Size of list
	uint32_t rcinode = sp->s_rcdir_inode;  /* inode that stores rcdir list */

	struct buf sbuf = get_block_map((ino_t)rcinode,0);
	buf = sbuf->data;
	assert(size == sbuf->lmfs_bytes);
	//give inode list in buf?	
	return size;
}
void put_recovery(struct *sbuf, dev_t dev)
{
	struct super_block *sp;//get super here(read)
	sp->s_dev = dev;

	if(read_super(sp) != OK)
		panic("Couldnt Read Super for put Recovery");

	uint32_t rcinode = sp->s_rcdir_inode;  /* inode that stores rcdir list */
	size_t size = (sp->s_block_size/sizeof(uint32_t))-1; //Size of list

	put_block(buf);
	//put inode list back
	//write list to block
}
//Return status for success
int recovery_add(ino_t inode)
{
	return OK;
}
void recovery_remove(ino_t inode)
{

}
