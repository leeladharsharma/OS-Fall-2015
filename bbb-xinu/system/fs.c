#include <fs.h>
#include <xinu.h>
#include <kernel.h>
#include <stddef.h>
#include <stdio.h>


static struct fsystem fsd;
int dev0_numblocks;
int dev0_blocksize;
char *dev0_blocks;

extern int dev0;

char block_cache[512];

#define SB_BLK 0
#define BM_BLK 1
#define RT_BLK 2

#define NUM_FD 16
struct filetable oft[NUM_FD];
int next_open_fd = 0;
int inode_id=1;

struct inode blank_inode;
static int next_free_inode = 0;
static int next_free_data_block;

struct filetable *fileTablePtr;

#define INODES_PER_BLOCK (fsd.blocksz / sizeof(struct inode))
#define NUM_INODE_BLOCKS (( (fsd.ninodes % INODES_PER_BLOCK) == 0) ? fsd.ninodes / INODES_PER_BLOCK : (fsd.ninodes / INODES_PER_BLOCK) + 1)
#define FIRST_INODE_BLOCK 2
#define FIRST_DATA_BLOCK (2 + NUM_INODE_BLOCKS)

int fs_fileblock_to_diskblock(int dev, int fd, int fileblock);


int fs_fileblock_to_diskblock(int dev, int fd, int fileblock) {
  int diskblock;

  if (fileblock >= INODEBLOCKS - 2) {
    printf("Indirect block support not present\n");
    return SYSERR;
  }

  diskblock = oft[fd].in.blocks[fileblock]; //get the logical block address

  return diskblock;
}

/* read in an inode and fill in the pointer */
int fs_get_inode_by_num(int dev, int inode_number, struct inode *in) {
  int bl, inn;
  int inode_off;

  if (dev != 0) {
    printf("Unsupported device\n");
    return SYSERR;
  }
  if (inode_number > fsd.ninodes) {
    printf("fs_get_inode_by_num: inode %d out of range\n", inode_number);
    return SYSERR;
  }

  bl = inode_number / INODES_PER_BLOCK;
  inn = inode_number % INODES_PER_BLOCK;
  bl += FIRST_INODE_BLOCK;

  inode_off = inn * sizeof(struct inode);

  /*
  printf("in_no: %d = %d/%d\n", inode_number, bl, inn);
  printf("inn*sizeof(struct inode): %d\n", inode_off);
  */

  bs_bread(dev0, bl, 0, &block_cache[0], fsd.blocksz);
  memcpy(in, &block_cache[inode_off], sizeof(struct inode));

  return OK;

}

int
fs_put_inode_by_num(int dev, int inode_number, struct inode *in) {
  int bl, inn;

  if (dev != 0) {
    printf("Unsupported device\n");
    return SYSERR;
  }
  if (inode_number > fsd.ninodes) {
    printf("fs_put_inode_by_num: inode %d out of range\n", inode_number);
    return SYSERR;
  }

  bl = inode_number / INODES_PER_BLOCK;
  inn = inode_number % INODES_PER_BLOCK;
  bl += FIRST_INODE_BLOCK;

  /*
  printf("in_no: %d = %d/%d\n", inode_number, bl, inn);
  */

  bs_bread(dev0, bl, 0, block_cache, fsd.blocksz);
  memcpy(&block_cache[(inn*sizeof(struct inode))], in, sizeof(struct inode));
  bs_bwrite(dev0, bl, 0, block_cache, fsd.blocksz);

  return OK;
}
     
int fs_mkfs(int dev, int num_inodes) {
  int i;
  
  if (dev == 0) {
    fsd.nblocks = dev0_numblocks;
    fsd.blocksz = dev0_blocksize;
  }
  else {
    printf("Unsupported device\n");
    return SYSERR;
  }

  if (num_inodes < 1) {
    fsd.ninodes = DEFAULT_NUM_INODES;
  }
  else {
    fsd.ninodes = num_inodes;
  }

  i = fsd.nblocks;
  while ( (i % 8) != 0) {i++;}
  fsd.freemaskbytes = i / 8; 
  
  if ((fsd.freemask = getmem(fsd.freemaskbytes)) == (void *)SYSERR) {
    printf("fs_mkfs memget failed.\n");
    return SYSERR;
  }
  
  /* zero the free mask */
  for(i=0;i<fsd.freemaskbytes;i++) {
    fsd.freemask[i] = '\0';
  }
  
  fsd.inodes_used = 0;
  
  /* write the fsystem block to SB_BLK, mark block used */
  fs_setmaskbit(SB_BLK);
  bs_bwrite(dev0, SB_BLK, 0, &fsd, sizeof(struct fsystem));
  
  /* write the free block bitmask in BM_BLK, mark block used */
  fs_setmaskbit(BM_BLK);
  bs_bwrite(dev0, BM_BLK, 0, fsd.freemask, fsd.freemaskbytes);

  return 1;
}

void fs_print_fsd(void) {

  printf("fsd.ninodes: %d\n", fsd.ninodes);
  printf("struct inode Size: %d\n", sizeof(struct inode));
  printf("INODES_PER_BLOCK: %d\n", INODES_PER_BLOCK);
  printf("NUM_INODE_BLOCKS: %d\n", NUM_INODE_BLOCKS);
}

/* specify the block number to be set in the mask */
int fs_setmaskbit(int b) {
  int mbyte, mbit;
  mbyte = b / 8;
  mbit = b % 8;

  fsd.freemask[mbyte] |= (0x80 >> mbit);
  return OK;
}

/* specify the block number to be read in the mask */
int fs_getmaskbit(int b) {
  int mbyte, mbit;
  mbyte = b / 8;
  mbit = b % 8;

  return( ( (fsd.freemask[mbyte] << mbit) & 0x80 ) >> 7);
  return OK;

}

/* specify the block number to be unset in the mask */
int fs_clearmaskbit(int b) {
  int mbyte, mbit, invb;
  mbyte = b / 8;
  mbit = b % 8;

  invb = ~(0x80 >> mbit);
  invb &= 0xFF;

  fsd.freemask[mbyte] &= invb;
  return OK;
}

/* This is maybe a little overcomplicated since the lowest-numbered
   block is indicated in the high-order bit.  Shift the byte by j
   positions to make the match in bit7 (the 8th bit) and then shift
   that value 7 times to the low-order bit to print.  Yes, it could be
   the other way...  */
void fs_printfreemask(void) {
  int i,j;

  for (i=0; i < fsd.freemaskbytes; i++) {
    for (j=0; j < 8; j++) {
      printf("%d", ((fsd.freemask[i] << j) & 0x80) >> 7);
    }
    if ( (i % 8) == 7) {
      printf("\n");
    }
  }
  printf("\n");
}

int fs_open(char *filename, int flags)
{
	return 0;
}

int fs_create(char *filename, int mode)
{
	struct dirent * ptrDirent;
	int i;			

	/* check if file already exists */
	for (i = 0; i < DIRECTORY_SIZE; i++) {
		ptrDirent = &fsd.root_dir.entry[i];
		if (strncmp(ptrDirent->name, filename, FILENAMELEN) == 0)
			break;
	}
	if (i != DIRECTORY_SIZE) {
		printf("file %s already exists\n", filename);
		return SYSERR;
	}

	/* Allocate next dir entry and initialize it to empty file */
	ptrDirent = &fsd.root_dir.entry[fsd.root_dir.numentries++];
	strncpy(ptrDirent->name, filename, FILENAMELEN-1);
	ptrDirent->name[FILENAMELEN-1]='\0';
	ptrDirent->inode_num=-1;

	/* search for empty slot in open file table */
	for(i=0; i<NUM_FD; i++) {
		if(oft[next_open_fd].state == FSTATE_CLOSED) {
			break;
		}
		next_open_fd++;
		if(next_open_fd >= NUM_FD)
			next_open_fd = 0;
	}
	if(oft[next_open_fd].state != FSTATE_CLOSED) {
		printf("max open files reached\n");
		return SYSERR;			
	}

	/* Make an entry in open file table. */
	fileTablePtr = &oft[next_open_fd];
	fileTablePtr->state = FSTATE_OPEN;	
	fileTablePtr->fileptr = 0;
	fileTablePtr->de = ptrDirent;

	/* search for an empty inode */
	for(i=0; i<fsd.ninodes; i++) {
		fs_get_inode_by_num(0, next_free_inode, &fileTablePtr->in);
		if(fileTablePtr->in.id < 0 || fileTablePtr->in.id >= fsd.ninodes) {
			break;
		}
		next_free_inode++;
		if(next_free_inode >= fsd.ninodes)
			next_free_inode = 0;
	}

	if(!(fileTablePtr->in.id < 0 || fileTablePtr->in.id >= fsd.ninodes)) {
		printf("max files limit reached\n");
		return SYSERR;			
	}

	fileTablePtr->in.id = next_free_inode;
	fileTablePtr->in.type = INODE_TYPE_FILE;
	fileTablePtr->in.nlink = 0;
	fileTablePtr->in.device = 0;
	fileTablePtr->in.size = 0;

	ptrDirent->inode_num = next_free_inode;

	fsd.inodes_used++;
	/* write inode to disk */	
	fs_put_inode_by_num(dev0, fileTablePtr->in.id, &fileTablePtr->in);

	printf("\nFile %s created and opened successfully.\n",ptrDirent->name);
	return next_open_fd;
}

int fs_read(int fd, void *buf, int nbytes)
{
	
	int block, offset, length;
	int i, m;
	int num_bytes_read = nbytes;
	int bytes_transferred = 0;

	// verify arguments
	fileTablePtr = &oft[fd];
	
	/* check if the file is open */
	if(fileTablePtr->state != FSTATE_OPEN) {
		printf("file is not open\n");
		return SYSERR;	
	}

	if ( nbytes <= 0 ) {
		printf("bytes to read is less than 1\n");
		return SYSERR;
	}
	
	/* check if reading from file will exceed EOF */
	if( (nbytes + fileTablePtr->fileptr) > fileTablePtr->in.size ) {
		printf("INFO: bytes to read was greater than file size. data is read only till the end of file.\n");
		nbytes = fileTablePtr->in.size - fileTablePtr->fileptr;
	}

	/* check if reading from file will exceed the max file length */
	if( (nbytes + fileTablePtr->fileptr) >= (MDEV_BLOCK_SIZE * INODEDIRECTBLOCKS) ) {
		printf("bytes to read is greater than file size\n");
		return SYSERR;
	}

	// read current block
	// get current block number and offset
	block = fileTablePtr->in.blocks[fileTablePtr->fileptr/MDEV_BLOCK_SIZE];
	offset =fileTablePtr->fileptr % MDEV_BLOCK_SIZE;
	length = nbytes < (MDEV_BLOCK_SIZE - offset) ? nbytes : (MDEV_BLOCK_SIZE - offset);

	if (bs_bread(dev0, block, offset, buf, length)!=OK) {
		printf("block read failed\n");
		return SYSERR;
	}
	fileTablePtr->fileptr += length;
	nbytes -= length;
	bytes_transferred += length;

	// extract each block from disk and copy to buffer	
	while(nbytes > 0) {
		block = fileTablePtr->in.blocks[fileTablePtr->fileptr/MDEV_BLOCK_SIZE];
		offset = 0;
		length = nbytes < MDEV_BLOCK_SIZE ? nbytes : MDEV_BLOCK_SIZE;
		if (block == -1)
			break;
		if (bs_bread(dev0, block, offset, buf+bytes_transferred, length)!=OK) {
			printf("block write failed\n");
			return SYSERR;
		}
		fileTablePtr->fileptr += length;
		nbytes -= length;
		bytes_transferred += length;
	}
	
	return num_bytes_read;
}
int fs_write(int fd, void *buf, int nbytes)
{
    	int block, offsetVal, length;
	int i, m;	
	int eofInt = 0;
	int bytes_transferred = 0;	

	int bytesCount = nbytes;
	fileTablePtr = &oft[fd];
	if(fileTablePtr->state != FSTATE_OPEN) {
		printf("file is not open\n");
		return SYSERR;	
	}
	if ((nbytes + fileTablePtr->fileptr) > fileTablePtr->in.size) {
		eofInt = 1;
	}
	block = fileTablePtr->in.blocks[fileTablePtr->fileptr/MDEV_BLOCK_SIZE];
	offsetVal = fileTablePtr->fileptr % MDEV_BLOCK_SIZE;
	length = nbytes < (MDEV_BLOCK_SIZE - offsetVal) ? nbytes : (MDEV_BLOCK_SIZE - offsetVal);

	printf("writing: %s\n", (char *) buf);

	if (bs_bwrite(dev0, block, offsetVal, buf, length)!=OK) {
		printf("block write failed\n");
		return SYSERR;
	}
	fileTablePtr->fileptr += length;
	nbytes -= length;
	bytes_transferred += length;
	while(nbytes > 0) 
	{
		for(i=FIRST_DATA_BLOCK; i<fsd.nblocks; i++) {
			m = fs_getmaskbit(next_free_data_block);
			if(m == 0) {
				break;
			}
			next_free_data_block++;
			if(next_free_data_block >= MDEV_NUM_BLOCKS)
				next_free_data_block = FIRST_DATA_BLOCK;
		}
		if(m != 0) {
			printf("disk full\n");
			return SYSERR;			
		}
		fs_setmaskbit(next_free_data_block);
		/* update bitmask block */
		fs_setmaskbit(BM_BLK);
		bs_bwrite(dev0, BM_BLK, 0, fsd.freemask, fsd.freemaskbytes);

		fileTablePtr->in.blocks[fileTablePtr->fileptr/MDEV_BLOCK_SIZE] = next_free_data_block;

		// fill the allocated block
		block = fileTablePtr->in.blocks[fileTablePtr->fileptr/MDEV_BLOCK_SIZE];
		offsetVal = 0;
		length = nbytes < MDEV_BLOCK_SIZE ? nbytes : MDEV_BLOCK_SIZE;
		if (bs_bwrite(dev0, block, offsetVal, buf+bytes_transferred, length)!=OK) {
			printf("block write failed\n");
			return SYSERR;
		}
		fileTablePtr->fileptr += length;
		nbytes -= length;
		bytes_transferred += length;
	}
	if (eofInt) {
		fileTablePtr->in.size = fileTablePtr->fileptr;
	}
	
	/* write inode to disk */
	fs_put_inode_by_num(dev0, fileTablePtr->in.id, &fileTablePtr->in);
	return bytesCount;
}

int fs_close(int fd)
{
    fileTablePtr= &oft[fd];
	
	/* check if the file is open */
	if(fileTablePtr->state != FSTATE_OPEN) {
		printf("file is not open\n");
		return SYSERR;	
	}
	
	fileTablePtr->state = FSTATE_CLOSED;

	return OK;
}
int get_freeinode_num() {
    int i;
    for (i = 0; i < fsd.freemaskbytes; i++) {
        if (fs_getmaskbit(i) != 1) {
            return i;
        }
    }
    return SYSERR;
}
void printrootdirectory(void)
{
	printf("Root directory name :%s",fsd.root_dir.entry[0].name);
}
