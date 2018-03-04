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
    
    /* determine the size of each group */
    int size_block = 1024 << superblock_summary.s_log_block_size;
    int blocks_per_group = superblock_summary.s_blocks_per_group;
    int size_group = size_block * blocks_per_group;
    
    /* get the total number of groups in the file system */
    int num_groups = superblock_summary.s_blocks_count / superblock_summary.s_blocks_per_group + 1;
    
    /* number of blocks in last group */
    int block_count_last = superblock_summary.s_blocks_count % superblock_summary.s_blocks_per_group;
    /* number of inodes in last group */
    int inode_count_last = superblock_summary.s_inodes_count % superblock_summary.s_inodes_per_group;
    
    /* declare struct for the block group descriptor tables */
    struct ext2_group_desc bgdt[num_groups];
    
    int i, block_count, inode_count;
    for(i = 0; i < num_groups; i++) {
        
        pread(fd, &bgdt[i], sizeof(struct ext2_group_desc), BGDT_OFFSET + i*sizeof(struct ext2_group_desc));
        
        if(i == num_groups - 1) {
            block_count = block_count_last;
            inode_count = inode_count_last;
        }
        else {
            block_count = superblock_summary.s_blocks_count;
            inode_count = superblock_summary.s_inodes_count;
        }
        
        fprintf(stdout, "GROUP,%d,%d,%d,%d,%d,%d,%d,%d\n",
                i,
                block_count,
                inode_count,
                bgdt[i].bg_free_blocks_count,
                bgdt[i].bg_free_inodes_count,
                bgdt[i].bg_block_bitmap,
                bgdt[i].bg_inode_bitmap,
                bgdt[i].bg_inode_table);
    }
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
