#include <assert.h>
#include "proto.h"

//TODO 
//Implement find_rinode, to give recovered inode struct
//Implement is_recoverable if needed 
//Implement parent()
//Implement undelete_dir && undelete_file
//-These may require inode structures instead of inode numbers
//CALL undelete from fs_undelete()

//struct inode;
//int search_dir(ldir_ptr, string, numb, flag);
//register struct inode *ldir_ptr; /* ptr to inode for dir to search */
//const char *string;		 /* component to search for */
//ino_t *numb;			 /* pointer to inode number */
//int flag;			 /* LOOK_UP, ENTER, DELETE or IS_EMPTY */
/* Include in MINIX
#define LOOK_UP_UNDELETABLE 1
	int flag;
	if(flag == 1)
	{
		undelete(char *file_name);
	}
*/
/* Required Edits,
 * is_directory relies on find_inode, which
 * would need to be able to find a recoverable directory.
 * May implement find_rinode
 */
int undelete(ino_t inode,char *file_name)
{
	register struct inode *ldir_ptr; /* ptr to inode for dir to search */
	//may use find_rinode, to search our own inode entires to get struct inode
	//*
	ldir_ptr = find_inode(dev_t dev, ino_t inode);

	//ino_t pdir = find(parent(ip)); //Parent directory

	if(!is_recoverable(inode))
	{
		printf("Not a recoverable file/directory\n");
		return EINVAL;
	}

	if(pdir == NULL)
	{
		printf("Parent is not recovered\n");
		return EINVAL;
	}

	if(is_directory(ldir_ptr))
	{
		undelete_dir(inode);
	}
	else
	{
		undelete_file(inode);	
	}
	return OK;
}
//Return the parent inode
ino_t parent()
{
	return //.. inode	
}
//Given inode, determine if recoverable
int is_recoverable(ino_t inode)
{
	
}
//Find the inode for a recoverable inode in our own structures
struct inode *find_rinode(ino_t inode)
{
	//return null if not recoverable or exists,
	//then is_recoverable is not needed.
}
//See if inode is a directory
int is_directory(struct inode *ldir_ptr)
{
	if ((ldir_ptr->i_mode & I_TYPE) != I_DIRECTORY) 
		return 0;
	else return 1;
}
//Maximum recoverable inodes for a directory
#define MAXRINODES 64
//Undelete directory,
//Will need to create a new directory for each recoverable inode
int undelete_dir(ino_t inode)
{

	//Ensure that dir is recoverable
	assert(is_recoverable(inode));
	ino_t *ls = malloc(MAXRINODES*sizeof(ino_t *));
	//Create list of all sub files/directories
	//use fs_getdents
	ino_t *p;
	int errno;

	//Recover directory here
	//Create new directory
	//Needed to create new inode
	char *name;
	mode_t mode;
	uid_t uid;
	gid_t gid;
	struct fsdriver_node *node;
	errno = fs_rcmkdir(inode,name,mode,uid,gid,node);
	if(errno != OK)
		return errno;
	//then recover all sub directories/files in list;
	
	for(p = ls; p != NULL; p++)
	{
		if(is_directory(*p))
		{
			errno = undelete_dir(*p);
		}
		else
		{
			errno = undelete_file(*p)
		}

		if(errno != OK)
			return errno;
	}
	return errno;
}
int undelete_file(ino_t inode)
{
	assert(is_recoverable(inode));
	//Recover file here
	//Needed to create new inode
	char *name;
	mode_t mode;
	uid_t uid;
	gid_t gid;
	struct fsdriver_node *node;
	//Create new file in the directory it should be in, should be done by
	//fs_create
	int errno = fs_create(inode,name,mode,uid,gid,node);
	//Add new inode in recoverable directory to recoverable files!
	return errno;
}
