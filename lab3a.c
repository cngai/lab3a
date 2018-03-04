#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ext2_fs.h"

/* define offsets that correspond to the positions of the elements that we want to examine */
#define SUPERBLOCK_OFFSET 1024
#define SUPERBLOCK_SIZE 1024

/* get values for superblock summary */
void get_sbs(int fd) {

    struct ext2_super_block block_summary;
    
    pread(fd, &block_summary, sizeof(ext2_super_block), SUPERBLOCK_OFFSET);
    fprintf(stdout, "SUPERBLOCK,%d,%d,%d,%d,%d,%d,%d\n",
            block_summary.s_blocks_count,
            block_summary.s_inodes_count,
            1024 << block_summary.s_log_block_size,
            block_summary.s_inodes_count / block_summary.s_blocks_count * (1024 << block_summary.s_log_block_size),
            block_summary.s_blocks_per_group,
            block_summary.s_inodes_per_group,
            block_summary.s_first_data_block);
    
    /* NOTE: i-node size calculation is most likely incorrect. I just left it in the
     printf statement as a placeholder for when we figure out how to actually calculate it. */

    return;
}

/* get values for group summary */
void get_gs(int fd) {
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
