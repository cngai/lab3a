#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ext2_fs.h"

/* define offsets that correspond to the positions of the elements that we want to examine */
#define SUPERBLOCK_OFFSET 1024
#define SUPERBLOCK_SIZE 1024

/* get values for superblock summary */
void get_sbs(int fd) {

    /* declare super_block and inode structs */
    struct ext2_super_block block_summary;
    struct ext2_inode inode_summary;
    
    /* read from superblock and inode offsets and write to their respective structs */
    pread(fd, &block_summary, sizeof(ext2_super_block), SUPERBLOCK_OFFSET);
    pread(fd, &inode_summary, sizeof(inode_summary), block_summary.s_first_ino);

    /* print the data based on the data given in the structs */
    fprintf(stdout, "SUPERBLOCK,%d,%d,%d,%d,%d,%d,%d\n",
            block_summary.s_blocks_count,
            block_summary.s_inodes_count,
            1024 << block_summary.s_log_block_size,
            inode_summary.i_size,
            block_summary.s_blocks_per_group,
            block_summary.s_inodes_per_group,
            block_summary.s_first_data_block);

    return;
}

/* get values for group summary */
void get_gs(int fd) {
    
    /*
     
     Scan each of the groups in the file system. For each group, produce a new-line terminated line for each group, each comprised of nine comma-separated fields (with no white space), summarizing its contents.
     
     GROUP
     group number (decimal, starting from zero)
     total number of blocks in this group (decimal)
     total number of i-nodes in this group (decimal)
     number of free blocks (decimal)
     number of free i-nodes (decimal)
     block number of free block bitmap for this group (decimal)
     block number of free i-node bitmap for this group (decimal)
     block number of first block of i-nodes in this group (decimal)
     
     */
    
    
    
  return;
}

/* get values for free block entries */
void get_fbe(int fd) {
  return;
}

/* get values for free i-node entries */
void get_fie(int fd) {
  return;
}

/* get values for i-node summary */
void get_is(int fd) {
  return;
}

/* get values for directory entries */
void get_de(int fd) {
  return;
}

/* get values for indirect block references */
void get_ibr(int fd) {
  return;
}

int main(int argc, char * argv[]) {

    /* check to make sure correct number of arguments */
    if (argc != 2){
        fprintf(stderr, "Error: incorrect number of arguments.\n");
        exit(1);
    }
    /* open the file system image */
    int fd = open(argv[1], O_RDONLY);

    get_sbs(fd);
    get_gs(fd);
    get_fbe(fd);
    get_fie(fd);
    get_is(fd);
    get_de(fd);
    get_ibr(fd);
    exit(0);
}
