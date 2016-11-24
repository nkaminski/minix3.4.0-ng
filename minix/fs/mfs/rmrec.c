#include "fs.h"
#include <assert.h>
#include <string.h>
#include "buf.h"
#include "inode.h"
#include "super.h"

static struct inode *ino;
static struct buf *sbuf;

struct rc_entry {
        uint32_t i_file; //the hidden file's inode number
        uint32_t i_pdir; //the inode number of the containing directory
};

//TODO implement!
/* This will get called one or more times when space is needed, return nonzero if you were able to free at least one disk block or zero if you cant */
int gc_undeletable(dev_t dev){

/* 0 means NO space was able to be reclaimed,
non zero means at least one zone was freed */
return 0;
}

/* Gets the recoverable file list from the disk referenced by dev and points sbuf at it
   its size is (sbuf->lmfs_bytes/sizeof(struct rc_entry))-1)
   and it contains entries of type struct rc_entry.
   
   The end of the list is denoted by reaching an entry with the i_file field equal to 0.
   
   put_recovery() MUST be called EXACTLY ONE time prior to returning from a function that has called get_recovery!
*/
void get_recovery(dev_t dev)
{
	struct super_block sp;//get super here(read)
	sp.s_dev = dev;

	if(read_super(&sp) != OK)
		panic("Couldnt Read Super for get Recovery");

	size_t size = (sp.s_block_size/sizeof(struct rc_entry))-1; //Size of list
	ino_t rcinode = sp.s_rcdir_inode;  /* inode that stores rcdir list */

	ino = get_inode(dev,rcinode);
	sbuf = get_block_map(ino, NORMAL);
   printf("sbuf->lmfs_bytes=%d, size=%d\n",sbuf->lmfs_bytes,size); 
   assert(sbuf != NULL);
	assert(size == (sbuf->lmfs_bytes/sizeof(struct rc_entry))-1);
	//give inode list in buf?	
}
/* decrements the ref count and syncs the changes to the cache and disk */
void put_recovery()
{
   
	MARKDIRTY(sbuf);
   IN_MARKDIRTY(ino);
	put_block(sbuf);
	put_inode(ino);
	//put inode list back
	//write list to block
}
//Adds a hidden(deleted) file and containing dir pair to the rc list
//Return status for success
int recovery_add(dev_t dev,uint32_t inode_nr_file, uint32_t inode_nr_pdir)
{
   get_recovery(dev);
   printf("got recovery\n");

	//add to list here
   size_t size = ((sbuf->lmfs_bytes)/sizeof(struct rc_entry))-1;

	int r = 1;
	struct rc_entry *inols = sbuf->data;
	size_t i;
	for(i = 0; i < size; i++)
	{
	   printf("Inols[%d] = %d\n",i,inols[i].i_file);
		if(inols[i].i_file == 0)
		{
			inols[i].i_file = inode_nr_file;
			inols[i].i_pdir = inode_nr_pdir;
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

//Deletes a hidden(deleted) file and containing dir pair from the rc list by file inode number
void recovery_remove(dev_t dev,uint32_t inode_nr_file)
{
   get_recovery(dev);

	size_t size = ((sbuf->lmfs_bytes)/sizeof(struct rc_entry))-1;
	//remove from list here
   struct rc_entry *inols = sbuf->data;
	size_t i;
	int rmi = -1;
	int end = -1;
	int r = 1;
	for(i = 0; i < size; i++)
	{
		if(inols[i].i_file == inode_nr_file)	
		{
			rmi = i;
		}
		if(inols[i].i_file != 0)
		{
			end = i;
		}
		if(inols[i].i_file == 0)
			break;
	}
	if(rmi != -1)
	{
		inols[rmi] = inols[end];
		inols[end].i_file = 0;
	}
	else if(rmi == -1)
	{
		printf("Inode not in recovery list\n");
		//Not in list
	}
	
	put_recovery();
}
