/*
 * BUAA MIPS OS Kernel file system format
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>

/* Prevent inc/types.h, included from inc/fs.h,
 * From attempting to redefine types defined in the host's inttypes.h. */
#define _INC_TYPES_H_
#define BY2PG       4096        // bytes to a page
#include "../include/fs.h"

#define nelem(x)    (sizeof(x) / sizeof((x)[0]))
typedef struct Super Super;
typedef struct File File;

#define NBLOCK 1024 // The number of blocks in the disk.
uint32_t nbitblock; // the number of bitmap blocks.
uint32_t nextbno;   // next availiable block.

struct Super super; // super block.

enum {
    BLOCK_FREE  = 0,
    BLOCK_BOOT  = 1,
    BLOCK_BMAP  = 2,
    BLOCK_SUPER = 3,
    BLOCK_DATA  = 4,
    BLOCK_FILE  = 5,
    BLOCK_INDEX = 6,
};

struct Block {
    uint8_t data[BY2BLK];
    uint32_t type;
} disk[NBLOCK];

// Initial the disk. Do some work with bitmap and super block.
void init_disk() {
    int i, r, diff;

    // Step 1: Mark boot sector block.
    disk[0].type = BLOCK_BOOT;

    // Step 2: Initialize boundary.
    nbitblock = (NBLOCK + BIT2BLK - 1) / BIT2BLK;
    nextbno = 2 + nbitblock;

    // Step 2: Initialize bitmap blocks.
    for(i = 0; i < nbitblock; ++i) {
        disk[2+i].type = BLOCK_BMAP;
    }
    for(i = 0; i < nbitblock; ++i) {
        memset(disk[2+i].data, 0xff, NBLOCK/8);
    }
    if(NBLOCK != nbitblock * BY2BLK) {
        diff = NBLOCK % BY2BLK / 8;
        memset(disk[2+(nbitblock-1)].data+diff, 0x00, BY2BLK - diff);
    }

    // Step 3: Initialize super block.
    disk[1].type = BLOCK_SUPER;
    super.s_magic = FS_MAGIC;
    super.s_nblocks = NBLOCK;
    super.s_root.f_type = FTYPE_DIR;
    strcpy(super.s_root.f_name, "/");
}

// Get next block id, and set `type` to the block's type.
int next_block(int type) {
    disk[nextbno].type = type;
    return nextbno++;
}

// Flush disk block usage to bitmap.
void flush_bitmap() {
    int i;
    // update bitmap, mark all bit where corresponding block is used.
    for(i = 0; i < nextbno; ++i) {
        ((uint32_t *)disk[2+i/BIT2BLK].data)[(i%BIT2BLK)/32] &= ~(1<<(i%32));
    }
}

// Finish all work, dump block array into physical file.
void finish_fs(char *name) {
    int fd, i, k, n, r;
    uint32_t *p;

    // Prepare super block.
    memcpy(disk[1].data, &super, sizeof(super));

    // Dump data in `disk` to target image file.
    fd = open(name, O_RDWR|O_CREAT, 0666);
    for(i = 0; i < 1024; ++i) {
        write(fd, disk[i].data, BY2BLK);
    }

    // Finish.
    close(fd);
}

// Save block link.
void save_block_link(struct File *f, int nblk, int bno)
{
    assert(nblk < NINDIRECT); // if not, file is too large !

    if(nblk < NDIRECT) {
        f->f_direct[nblk] = bno;
    }
    else {
        if(f->f_indirect == 0) {
            // create new indirect block.
            f->f_indirect = next_block(BLOCK_INDEX);
        }
        ((uint32_t *)(disk[f->f_indirect].data))[nblk] = bno;
    }
}

// Make new block contians link to files in a directory.
int make_link_block(struct File *dirf, int nblk) {
    save_block_link(dirf, nblk, nextbno);
    dirf->f_size += BY2BLK;
    return next_block(BLOCK_FILE);
}

// Overview:
//      Create new block pointer for a file under sepcified directory.
//      Notice that when we delete a file, we do not re-arrenge all 
//      other file pointers, so we should be careful of existing empty
//      file pointers
//      
// Post-Condition:
//      We ASSUM that this function will never fail

struct File *create_file(struct File *dirf) {
    struct File *dirblk;
    int i, bno, found;
    int nblk = dirf->f_size / BY2BLK;
    
    // Your code here
	if(nblk > 0) {
		void *ptr = disk[dirf->f_direct[nblk - 1]].data;
		for(i = 0; i < FILE2BLK; ++i, ptr += BY2FILE) {
			dirblk = ptr;
			if(strlen(dirblk->f_name) <= 0) {
				return dirblk;
			}
		}
	}
	bno = make_link_block(dirf, nblk);
	return (struct File *)disk[bno].data;
}

// Write file to disk under specified dir.
void write_file(struct File *dirf, const char *path) {
    printf("write_file %x %s\n", dirf, path);
    int iblk = 0, r = 0, n = sizeof(disk[0].data);
    uint8_t buffer[n+1], *dist;
    struct File *target = create_file(dirf);
    int fd = open(path, O_RDONLY);
    
    // Get file name with no path prefix.
    const char *fname = strrchr(path, '/');
    if(fname)
        fname++;
    else
        fname = path;
    strcpy(target->f_name, fname);
    
    target->f_size = lseek(fd, 0, SEEK_END);
    target->f_type = FTYPE_REG;
    
    // Start reading file.
    lseek(fd, 0, SEEK_SET);
    while((r = read(fd, disk[nextbno].data, n)) > 0) {
        save_block_link(target, iblk++, next_block(BLOCK_DATA));
    }
    close(fd); // Close file descriptor.

	// printf("write_file: %x, %s(%s), %x\n", dirf, path, target->f_name, nextbno);
}

// Overview:
//      Write directory to disk under specified dir.
//      Notice that we may use standard library functions to operate on
//      directory to get file infomation.
//      
// Post-Condition:
//      We ASSUM that this funcion will never fail
void write_directory(struct File *dirf, char *name) {
    // Your code here
	DIR *dir = opendir(name);
	assert(dir);

	struct dirent *ent;
	while(ent = readdir(dir)) {
		if(strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
			continue;
		char path[MAXPATHLEN];
		snprintf(path, MAXPATHLEN, "%s/%s", name, ent->d_name);
		if(ent->d_type == DT_DIR) {
			struct File *dfile = create_file(dirf);
			dfile->f_type = FTYPE_DIR;
		    strcpy(dfile->f_name, ent->d_name);
			// printf("write_directory: %x, %s(%s)\n", dirf, name, dfile->f_name);
			write_directory(dfile, path);
		} else {
			write_file(dirf, path);
		}
	}
	closedir(dir);

}

int main(int argc, char **argv) {
    int i;

    init_disk();

    if(argc < 3 || (strcmp(argv[2], "-r") == 0 && argc != 4)) {
        fprintf(stderr, "\
Usage: fsformat <image file> files...\n\
       fsformat <image file> -r DIR\n");
        exit(0);
    }

    if(strcmp(argv[2], "-r") == 0) {
        for (i = 3; i < argc; ++i) {
            write_directory(&super.s_root, argv[i]);
        }
    } else {
        for(i = 2; i < argc; ++i) {
            write_file(&super.s_root, argv[i]);
        }
    }

    flush_bitmap();
    finish_fs(argv[1]);

    return 0;
}
