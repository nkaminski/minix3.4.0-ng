#include "fs.h"
#include <assert.h>
#include <string.h>
#include "buf.h"
#include "inode.h"
#include "super.h"
#include <stdlib.h>

static struct inode *ino;
static struct buf *sbuf;
const char emptystr[] = "";
struct rc_entry {
        uint32_t i_file; //the hidden file's inode number
        uint32_t i_pdir; //the inode number of the containing directory
};




/*-------------------------------------Yi's code starts here---------------------------------------*/
static char changed=1;
/* 	About state variable changed:	0 is unchanged, 1 is changed
	Things you have to do :			Whenever you add a new i-node to the list, mark it to be 1
*/

size_t heap_size = 0;

struct heap_entry{
	uint32_t i_file;
	uint32_t i_pdir;
	u32_t i_mtime;//the only field needed is last modified time
};

static struct heap_entry *inoheap=NULL;//global heap since I have a recursive heapify call

void print_heap(int size)
{
	int i;
	for (i=1;i<=size;i++) printf("%d  ",inoheap[i].i_file);
	puts("");
}

// Heapify procedure
void heapify(int current, int size)
{
	if (current*2>size) return;				// Current entry has no child
	struct heap_entry tmp;
	if (size>=current*2+1)					// Current entry has 2 children
	{
		if ((inoheap[current].i_mtime>inoheap[current*2].i_mtime)||(inoheap[current].i_mtime>inoheap[current*2+1].i_mtime))
		{
			if ((inoheap[current*2].i_mtime<inoheap[current*2+1].i_mtime))
			{
				tmp=inoheap[current];
				inoheap[current]=inoheap[current*2];
				inoheap[current*2]=tmp;
				heapify(current*2,size);
			}
			else
			{
				tmp=inoheap[current];
				inoheap[current]=inoheap[current*2+1];
				inoheap[current*2+1]=tmp;
				heapify(current*2+1,size);
			}
		}
	}
	else									// Current entry only has left child
	{
		if (inoheap[current].i_mtime>inoheap[current*2].i_mtime)
		{
			tmp=inoheap[current];
			inoheap[current]=inoheap[current*2];
			inoheap[current*2]=tmp;
			heapify(current*2,size);
		}
	}
	return;
}


//Frees the oldest hidden i-node in the list. Returns 0 if no more space can be freed, 1 on success.
int gc_undeletable(dev_t dev){
	int i;
   ino_t it_temp;
   register struct inode *ip;
   printf("Space is needed, performing a GC\n");
	if (changed==1)						// New i-node in list, rescan the list and heapify
	{
		printf("changes were made in GC\n");
      get_recovery(dev);
      printf("got recovery\n");
		sbuf = get_block_map(ino, NORMAL);
      printf("got list block\n");
		struct rc_entry *inols = sbuf->data;
		heap_size = ((sbuf->lmfs_bytes)/sizeof(struct rc_entry))-1;
		if (heap_size==0){
              put_block(sbuf);
              put_recovery();
              return 0;
      }
      if(inoheap != NULL)
		   free(inoheap);
		inoheap=malloc(sizeof(struct heap_entry)*(heap_size+1));//size+1 : binary heap uses index starting from 1.
      assert(inoheap != NULL);

		for (i=0;i<heap_size;i++)
		{
         if(inols[i].i_file==0)
                 break;
			inoheap[i+1].i_file=inols[i].i_file;
			inoheap[i+1].i_pdir=inols[i].i_pdir;
         printf("getting inode %d for mtime compare\n", inoheap[i+1].i_file);
         ip = get_inode(dev,inoheap[i+1].i_file);
         printf("got inode %d for mtime compare\n", inoheap[i+1].i_file);
         if(ip != NULL){
			   inoheap[i+1].i_mtime=ip->i_mtime;
            put_inode(ip);
         } else {
            panic("Unable to find inode to get mtime\n");
         }
		}
      heap_size = i;
      printf("copied list\n");
      put_block(sbuf);
      put_recovery();
      printf("put back recovery\n");

		for (i=(int)(heap_size/2);i>0;i--)
		{
			heapify(i,heap_size);
		}
		print_heap(heap_size);
		changed=0;						// Mark the list as unchanged.

	}
   if (heap_size==0){
           return 0;
   }
	struct rc_entry target;				// Information of i-node to be freed is stored in target.
	target.i_file=inoheap[1].i_file;
	target.i_pdir=inoheap[1].i_pdir;
   //get parent dir's inode from disk/cache and remove the dirent
   ip = get_inode(dev, target.i_pdir);
   if(ip != NULL){
           printf("parent dirent removed\n");
           //Parent dir is valid
           it_temp = target.i_file;
           search_dir_expand(ip, emptystr, &it_temp, I_DELETE, 1);
           IN_MARKDIRTY(ip);
           put_inode(ip);
   } else {
           printf("Parent dir inode not found in GC\n");
   }
   ip = get_inode(dev, target.i_file); //Get file's inode
   if(ip != NULL){
           printf("file unlinked\n");
           if(ip->i_nlinks>0)
               ip->i_nlinks--;
           printf("file has link count %d, ref count %d\n", ip->i_nlinks, ip->i_count);
           //assert(ip->i_count == 1);
           IN_MARKDIRTY(ip);
           //shouldnt be needed
           while(ip->i_count != 0)
               put_inode(ip);
   } else {
           printf("File inode not found in GC\n");
   }
   /* Redundant! called inside search_dir_expand 
   printf("calling recovery_remove in GC\n");
   recovery_remove(dev, target.i_file);
   */
   inoheap[1].i_file=inoheap[heap_size].i_file;
	inoheap[1].i_pdir=inoheap[heap_size].i_pdir;
	heap_size--;
   printf("new heap is of size %d\n",heap_size);
	heapify(1,heap_size);
   return 1;
}
/*-------------------------------------Yi's code ends here---------------------------------------*/


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
   printf("got recovery: adding pdir %d and file %d\n",inode_nr_pdir, inode_nr_file);

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
   else
   	changed=1;		// Mark the list changed upon successful add.
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
