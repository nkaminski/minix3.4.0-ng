#include "proto.h"
#include "inode.h"
//Remove recoverable files
/*
 * if mkdir or mkfile does not have space,
 * call rmrec with size and call again.
 */

//size - size of memory needed
//TODO lazy way of holding dir inode #, would limit number of allowable
//rcmkdirs
#define MAXRCDIRS 64
ino_t rcdirs[MAXRCDIRS];
int rmrec(size_t size)
{
	struct inode pdir = find_oldest_dir(); //oldest dir
	if(dir == NULL)
		return EINVAL; //No dirs
	else
	{
		ino_t rmnode;
		if(find_rmfile(pdir) != OK)//No files in dir, possible?
		{
			return EINVAL;
		}
		else
		{
			//fs_unlink(rmnode, ... , ... )
		}
	}
	//if can remove enough memory to allow file/dir to be created,
	//do it
	//else
	//return error: not enough memory
	
}
//follow rm_dir call on how to traverse directories to delete all files
//nvm, remove dir requires empty dir
int find_rmfile(struct inode *pdir)
{
		
}
struct inode *find_oldest_dir()
{
	int i;
	int first = 1; //first flag to set min
	int mindex;
	struct inode *min = NULL;
	for(i = 0; i < MAXRCDIRS; i++)	
	{
		if(rcdirs[i] != NULL)
		{
			//get struct inode of dir
			struct inode *node = find_inode(dev_t dev,rcdirs[i]);
			if(node == NULL)
				break;
			if(first == 1)
			{
				mindex = i;
				min = node;
				first = 0;
				continue;
			}
			//TODO find out how to compare i_updates
			if(node->i_update < min->i_update)
			{
				min = node;
			}
			//check i_update to find least recently updated
			//
		}
	}
	return min;
}
