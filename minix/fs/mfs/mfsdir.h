#ifndef _MFSDIR_H
#define _MFSDIR_H
#define HIDDEN 0xFF00
#define UNDELETE_NONE 0x00
#define UNDELETE_RECOVERABLE 0x01
#define UNDELETE_HIDDEN 0x02

#include <sys/cdefs.h>
#include <sys/types.h>

/* Maximum Minix MFS on-disk directory filename.
 * MFS uses 'struct direct' to write and parse 
 * directory entries, so this can't be changed
 * without breaking filesystems.
 */

#define MFS_DIRSIZ	59

struct direct {
  uint32_t mfs_d_ino;
  uint8_t mfs_rcdir_flags;
  char mfs_d_name[MFS_DIRSIZ];
} __packed;

#endif /* _MFSDIR_H */
