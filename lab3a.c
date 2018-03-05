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

/* header file needed to use strerror() */
#include <errno.h>
#include <string.h>

/* header file needed to use *gmtime() */
#include <time.h>

/* define offsets that correspond to the positions of the elements that we want to examine */
#define SUPERBLOCK_OFFSET 1024
#define SUPERBLOCK_SIZE 1024
#define BGDT_OFFSET 2048

/* hold value of error number */
int errno;

/* declare super_block and inode structs */
struct ext2_super_block superblock_summary;
struct ext2_inode inode_summary;

/* declare block-group descriptor */
struct ext2_group_desc* bgdt;

/* total number of groups in the file system */
int num_groups;


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
}

/* get values for group summary */
void get_gs(int fd) {
    
    /* determine the size of each group */
    /* int size_block = 1024 << superblock_summary.s_log_block_size;            //commented this out b/c you don't use any of these variables
    int blocks_per_group = superblock_summary.s_blocks_per_group;
    int size_group = size_block * blocks_per_group; */
    
    /* get the total number of groups in the file system */
    num_groups = superblock_summary.s_blocks_count / superblock_summary.s_blocks_per_group + 1;
    
    /* number of blocks in last group */
    int block_count_last = superblock_summary.s_blocks_count % superblock_summary.s_blocks_per_group;
    /* number of inodes in last group */
    int inode_count_last = superblock_summary.s_inodes_count % superblock_summary.s_inodes_per_group;
    
    /* initialize struct for the block group descriptor tables */
    bgdt = malloc(num_groups * sizeof(struct ext2_group_desc));
    if (bgdt == NULL){
        fprintf(stderr, "Error allocating memory. %s\n", strerror(errno));
    }
    
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
}

/* get values for free block entries */
void get_fbe(int fd) {
    int i, j, k;
    /* iterate through each group */
    for (i = 0; i < num_groups; i++){
        /* iterate through each block in each group */
        for (j = 0; j < (1024 << superblock_summary.s_log_block_size); j++){
            /* read byte by byte from block bitmap */
            uint8_t byte;
            pread(fd, &byte, 1, (bgdt[i].bg_block_bitmap * (1024 << superblock_summary.s_log_block_size)) + j);

            /* iterate through all 8 bits of byte to find free block */
            for (k = 0; k < 8; k++){
                /* free block represented by '0' and used block represented by '1' */
                if ((byte & (1 << k)) == 0){
                    fprintf(stdout, "BFREE,%d\n", (i * superblock_summary.s_blocks_per_group) + (j * 8) + (k + 1));
                }
            }
        }
    }
}

/* get values for free i-node entries */
void get_fie(int fd) {
    int i, j, k;
    /* iterate through each group */
    for (i = 0; i < num_groups; i++){
        /* iterate through each block in each group */
        for (j = 0; j < (1024 << superblock_summary.s_log_block_size); j++){
            /* read byte by byte from block bitmap */
            uint8_t byte;
            pread(fd, &byte, 1, (bgdt[i].bg_inode_bitmap * (1024 << superblock_summary.s_log_block_size)) + j);

            /* iterate through all 8 bits of byte to find free i-node */
            for (k = 0; k < 8; k++){
                /* free i-node represented by '0' and used i-node represented by '1' */
                if ((byte & (1 << k)) == 0){
                    fprintf(stdout, "IFREE,%d\n", (i * superblock_summary.s_inodes_per_group) + (j * 8) + (k + 1));
                }
            }
        }
    }
}

/* get time and put in specified format */
void get_time(uint32_t c_time, uint32_t m_time, uint32_t a_time, char* c_array, char* m_array, char* a_array){
    const time_t change_time = c_time;
    const time_t mod_time = m_time;
    const time_t acc_time = a_time;

    /* broken-down times */
    const struct tm bd_c_time = *gmt(change_time);
    const struct tm bd_m_time = *gmt(mod_time);
    const struct tm bd_a_time = *gmt(acc_time);

    /* format time and place into appropriate char array */
    if (strftime(c_array, 20, "%m/%d/%y %H:%M:%S", &bd_c_time) == 0){
        fprintf(stderr, "Error: unable to format change time. %s.\n", strerror(errno));
        exit(1);
    }
    if (strftime(m_array, 20, "%m/%d/%y %H:%M:%S", &bd_m_time) == 0){
        fprintf(stderr, "Error: unable to format modified time. %s.\n", strerror(errno));
        exit(1);
    }
    if (strftime(a_array, 20, "%m/%d/%y %H:%M:%S", &bd_a_time) == 0){
        fprintf(stderr, "Error: unable to format access time. %s.\n", strerror(errno));
        exit(1);
    }
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
