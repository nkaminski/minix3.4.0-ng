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
struct buf get_recovery(dev_t dev,register struct inode *ino)
{
	struct super_block *sp;//get super here(read)
	sp->s_dev = dev;

	if(read_super(sp) != OK)
		panic("Couldnt Read Super for get Recovery");

	size_t size = (sp->s_block_size/sizeof(uint32_t))-1; //Size of list
	uint32_t rcinode = sp->s_rcdir_inode;  /* inode that stores rcdir list */

	*ino = get_inode(dev,(ino_t)rcinode);
	struct buf sbuf = get_block_map(*ino,0);

	assert(size == (sbuf->lmfs_bytes)/sizeof(uint32_t));
	MARKDIRTY(sbuf);
	//give inode list in buf?	
	return sbuf;
}
void put_recovery(dev_t dev, struct *sbuf, register struct inode *ino)
{
	struct super_block *sp;//get super here(read)
	sp->s_dev = dev;

	if(read_super(sp) != OK)
		panic("Couldnt Read Super for put Recovery");

	uint32_t rcinode = sp->s_rcdir_inode;  /* inode that stores rcdir list */
	size_t size = (sp->s_block_size/sizeof(uint32_t))-1; //Size of list

	put_block(sbuf);
	put_inode(ino);
	//put inode list back
	//write list to block
}
//Return status for success
int recovery_add(dev_t dev,ino_t inode)
{
	register struct *ino;  
	*ino = get_inode(dev,inode);
	struct buf sbuf = get_recovery(dev,ino);

	//add to list here
	size_t size = (sbuf->lmfs_bytes)/sizeof(uint32_t);

	int r = 1;
	ino_t *inols = sbuf->data;
	size_t i;
	for(i = 0; i < size; i++)
	{
		if(inols[i] == NULL)
		{
			inols[i] = node;
			r = OK;
			break;
		}
	}

	put_recovery(dev,sbuf,ino);
	put_inode(ino);
	return r;
}
void recovery_remove(dev_t dev,ino_t inode)
{
	register struct *ino;  
	*ino = get_inode(dev,inode);
	struct buf sbuf = get_recovery(dev,ino);
	size_t size = (sbuf->lmfs_bytes)/sizeof(uint32_t);
	//remove from list here
	size_t i;
	size_t rmi = NULL;
	size_t end = NULL;
	int r = 1;
	for(i = 0; i < size; i++)
	{
		if(inols[i] == inode)	
		{
			rmi = i;
		}
		if(inols[i] != NULL)
		{
			end = i;
		}
		if(inols[i] == NULL)
			break;
	}
	if(rmi == NULL)
	{
		//Not in list
	}
	if(end == NULL)
	{
		//List full
	}
	else if(rmi != NULL)
	{
		inols[rmi] = NULL;
		inols[rmi] = inols[end];
		inols[end] = NULL;
	}
	
	put_recovery(dev,sbuf,ino);
	put_inode(ino);
}
