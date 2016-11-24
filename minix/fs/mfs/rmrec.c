#include "fs.h"
#include <assert.h>
#include <string.h>
#include "buf.h"
#include "inode.h"
#include "super.h"

static struct inode *ino;
static struct buf *sbuf;

int gc_undeletable(dev_t dev){

/* 0 means NO space was able to be reclaimed,
non zero means at least one zone was freed */
return 0;

}

//TODO Edit read_super, write_super, rw_super? to read/write s_rcdir_inode

//struct buf *get_block_map(register struct inode *rip, u64_t position);
//Set buf to inode list
//Return size of buffer = #inodes pointers * sizeof(uint32_t)
void get_recovery(dev_t dev)
{
	struct super_block sp;//get super here(read)
	sp.s_dev = dev;

	if(read_super(&sp) != OK)
		panic("Couldnt Read Super for get Recovery");

	size_t size = (sp.s_block_size/sizeof(uint32_t))-1; //Size of list
	ino_t rcinode = sp.s_rcdir_inode;  /* inode that stores rcdir list */

	ino = get_inode(dev,rcinode);
	sbuf = get_block_map(ino, NORMAL);
   printf("sbuf->lmfs_bytes=%d, size=%d\n",sbuf->lmfs_bytes,size); 
   assert(sbuf != NULL);
	assert(size == (sbuf->lmfs_bytes/sizeof(uint32_t))-1);
	//give inode list in buf?	
}
void put_recovery()
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
int recovery_add(dev_t dev,uint32_t inode_nr)
{
   get_recovery(dev);
   printf("got recovery\n");

	//add to list here
   size_t size = ((sbuf->lmfs_bytes)/sizeof(uint32_t))-1;

	int r = 1;
	uint32_t *inols = sbuf->data;
	size_t i;
	for(i = 0; i < size; i++)
	{
	   printf("Inols[%d] = %d\n",i,inols[i]);
		if(inols[i] == 0)
		{
			inols[i] = inode_nr;
			r = OK;
			break;
		}
	}
	if(r != OK)
		printf("Recovery list is full, cannot add entry\n");
   
	put_recovery();
   printf("put recovery\n");
	return r;
}
void recovery_remove(dev_t dev,uint32_t inode_nr)
{
   get_recovery(dev);

	size_t size = ((sbuf->lmfs_bytes)/sizeof(uint32_t))-1;
	//remove from list here
   uint32_t *inols = sbuf->data;
	size_t i;
	int rmi = -1;
	int end = -1;
	int r = 1;
	for(i = 0; i < size; i++)
	{
		if(inols[i] == inode_nr)	
		{
			rmi = i;
		}
		if(inols[i] != 0)
		{
			end = i;
		}
		if(inols[i] == 0)
			break;
	}
	if(rmi != -1)
	{
		inols[rmi] = inols[end];
		inols[end] = 0;
	}
	else if(rmi == -1)
	{
		printf("Inode not in recovery list\n");
		//Not in list
	}
	
	put_recovery();
}
