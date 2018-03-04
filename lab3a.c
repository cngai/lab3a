#include <stdio.h>
#include <stdlib.h>

/* header files needed for open() in C99 */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/* header file needed for pread() */
#include <unistd.h>

/* header file needed to facilitate access to EXT2 file system data */
#include "ext2_fs.h"

/* define offsets that correspond to the positions of the elements that we want to examine */
#define SUPERBLOCK_OFFSET 1024
#define SUPERBLOCK_SIZE 1024
#define BGDT_OFFSET 2048

/* declare super_block and inode structs */
struct ext2_super_block superblock_summary;
struct ext2_inode inode_summary;


/* get values for superblock summary */
void get_sbs(int fd) {
    
    /* read from superblock and inode offsets and write to their respective structs */
    pread(fd, &superblock_summary, sizeof(struct ext2_super_block), SUPERBLOCK_OFFSET);
    pread(fd, &inode_summary, sizeof(struct ext2_inode), superblock_summary.s_first_ino);
    
    /* print the data based on the data given in the structs */
    fprintf(stdout, "SUPERBLOCK,%d,%d,%d,%d,%d,%d,%d\n",
            superblock_summary.s_blocks_count,
            superblock_summary.s_inodes_count,
            1024 << superblock_summary.s_log_block_size,
            inode_summary.i_size,
            superblock_summary.s_blocks_per_group,
            superblock_summary.s_inodes_per_group,
            superblock_summary.s_first_data_block);
    
    return;
}

/* get values for group summary */
void get_gs(int fd) {
    
    /* WE WILL NEED THE GROUP SIZE AND ADDR OF FIRST BLOCK TO TRAVERSE THROUGH THE GROUPS */
    
    /* determine the size of each group */
    int size_block = 1024 << superblock_summary.s_log_block_size;
    int blocks_per_group = superblock_summary.s_blocks_per_group;
    int size_group = size_block * blocks_per_group;
    
    /* get the address of the first block */
    int addr_first_block = superblock_summary.s_first_data_block;
    
    /* get the total number of groups in the file system */
    int num_groups = superblock_summary.s_blocks_count / superblock_summary.s_blocks_per_group;
    
    /* declare struct for the block group descriptor table */
    struct ext2_group_desc bdgt;
    
    int i, offset;
    for(i = 0, offset = addr_first_block; i < num_groups; i++, offset += size_group) {
        pread(fd, &bgdt, sizeof(struct ext2_group_desc), offset);
        // implement fprintf statement
    }
    
    
    
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
