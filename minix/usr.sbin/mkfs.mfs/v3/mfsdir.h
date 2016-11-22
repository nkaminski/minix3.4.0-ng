#ifndef _MFSDIR_H
#define _MFSDIR_H

#define UNDELETE_NONE 0x00
#define UNDELETE_RECOVERABLE 0x01
#define UNDELETE_HIDDEN 0x02

/* Maximum Minix MFS on-disk directory filename.
 * MFS uses 'struct direct' to write and parse 
 * directory entries, so this can't be changed
 * without breaking filesystems.
 */

#define MFS_DIRSIZ	59

struct direct {
  uint32_t d_ino; //4 bytes
  uint8_t d_rc_flags;
  char d_name[MFS_DIRSIZ]; // 60 bytes
} __packed;

#endif /* _MFSDIR_H */
