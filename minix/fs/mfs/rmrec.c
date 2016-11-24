#include "fs.h"
#include <assert.h>
#include <string.h>
#include "buf.h"
#include "inode.h"
#include "super.h"


int gc_undeletable(dev_t dev){

/* 0 means NO space was able to be reclaimed,
non zero means at least one zone was freed */
return 0;

}

//TODO Edit read_super, write_super, rw_super? to read/write s_rcdir_inode

//struct buf *get_block_map(register struct inode *rip, u64_t position);
//Set buf to inode list
//Return size of buffer = #inodes pointers * sizeof(uint32_t)
void get_recovery(dev_t dev, struct buf *sbuf, register struct inode *ino)
{
	struct super_block sp;//get super here(read)
	sp.s_dev = dev;

	if(read_super(&sp) != OK)
		panic("Couldnt Read Super for get Recovery");

	size_t size = (sp.s_block_size/sizeof(uint32_t))-1; //Size of list
	ino_t rcinode = sp.s_rcdir_inode;  /* inode that stores rcdir list */

	ino = get_inode(dev,rcinode);
	sbuf = get_block_map(ino, NORMAL);
   
   assert(sbuf != NULL);
	assert(size == (sbuf->lmfs_bytes/sizeof(uint32_t))-1);
	//give inode list in buf?	
}
void put_recovery(struct buf *sbuf, register struct inode *ino)
{
   /* not needed
	struct super_block *sp;//get super here(read)
	sp->s_dev = dev;

	if(read_super(sp) != OK)
		panic("Couldnt Read Super for put Recovery");

	uint32_t rcinode = sp->s_rcdir_inode;   inode that stores rcdir list 
	size_t size = (sp->s_block_size/sizeof(uint32_t))-1; //Size of list
   */
   
	MARKDIRTY(sbuf);
   IN_MARKDIRTY(ino);
	put_block(sbuf);
	put_inode(ino);
	//put inode list back
	//write list to block
}
//Return status for success
int recovery_add(dev_t dev,ino_t inode)
{
	register struct inode *ino;  
	struct buf sbuf;
	ino = get_inode(dev,inode);
   get_recovery(dev,&sbuf,ino);

	//add to list here
	size_t size = ((sbuf.lmfs_bytes)/sizeof(uint32_t))-1;

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
	if(r != OK)
		printf("Recovery list is full, cannot add entry\n");

	put_recovery(&sbuf,ino);
	put_inode(ino);
	return r;
}
void recovery_remove(dev_t dev,ino_t inode)
{
	register struct inode *ino;  
	struct buf sbuf;
	ino = get_inode(dev,inode);
   get_recovery(dev,&sbuf,ino);

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
	if(rmi != NULL)
	{
		inols[rmi] = NULL;
		inols[rmi] = inols[end];
		inols[end] = NULL;
	}
	else if(rmi == NULL)
	{
		printf("Inode not in recovery list\n");
		//Not in list
	}
	
	put_recovery(&sbuf,ino);
	put_inode(ino);
}
