/*
 * startupfiletool - Shantonu Sen <ssen@opendarwin.org>
 * Tool to set HFS+ Startup File
 * cc -o startupfiletool startupfiletool.c -O0 -g -Wall
 */

#include <libc.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <sys/disk.h>
#include <util.h>
#include <err.h>

#include <hfs/hfs_format.h>
#include <libkern/OSByteOrder.h>

int verbose = 0;

void usage(void);
int writeStartupFile(char *device, char *filedata);
int allocateBlocks(int fd, HFSPlusForkData *allocationFile,
		   uint32_t allocationBlockSize, uint32_t totalBlocks,
		   uint32_t *allocatedBlocks, uint32_t *allocatedStart);
uint64_t mapForkOffset(HFSPlusForkData *fork, uint32_t allocationBlockSize,
			uint32_t blockNumber);

int main(int argc, char *argv[]) {
	char *device = NULL;
	char *file = NULL;
	int ch;
	
	if(argc == 1)
		usage();
	
	while ((ch = getopt(argc, argv, "v")) != -1) {
		switch(ch) {
			case 'v':
				verbose = 1;
				break;
			case 'h':
			default:
				usage();
				break;
		}
		
	}
	argc -= optind;
	argv += optind;

	if(argc != 2)
	  usage();

	
	device = argv[0];
	file = argv[1];
			
	return writeStartupFile(device, file);
	
	return 0;
}

void usage(void)
{

	fprintf(stderr, "Usage: %s [-v] <raw device> <file>\n", getprogname());
	
	exit(1);
}

#define kSectorSize 512

int writeStartupFile(char *device, char *filedata)
{
  int fd, datafd;
  int ret;
  ssize_t bytesRead, bytesWritten;
  uint32_t allocationBlockSize = 0;
  uint32_t totalBlocks = 0;
  uint64_t logicalSize = 0;
  uint32_t deviceBlockSize = 0;
  uint64_t deviceBlockCount = 0;
  char *datapayload = NULL;
  struct stat sb;
  char buffer[kSectorSize];
  HFSPlusVolumeHeader *vh = (HFSPlusVolumeHeader *)buffer;
  uint32_t allocatedBlocks = 0, allocatedStart = 0;
  ssize_t iobytes;

  fd = opendev(device, O_RDWR, 0, NULL);
  if(fd < 0)
    err(1, "opendev(%s)", device);

  ret = ioctl(fd, DKIOCGETBLOCKSIZE, &deviceBlockSize);
  if(ret)
    err(1, "ioctl(DKIOCGETBLOCKSIZE)");

  ret = ioctl(fd, DKIOCGETBLOCKCOUNT, &deviceBlockCount);
  if(ret)
    err(1, "ioctl(DKIOCGETBLOCKCOUNT)");


  bytesRead = pread(fd, buffer, sizeof(buffer), 2*kSectorSize);
  if(bytesRead != sizeof(buffer))
    err(1, "pread");

  switch(OSSwapBigToHostInt16(vh->signature)) {
  case kHFSPlusSigWord:
    printf("HFS+ filesystem detected\n");
    break;
  case kHFSXSigWord:
    printf("HFSX filesystem detected\n");
    break;
  case kHFSSigWord: {
    HFSMasterDirectoryBlock *mdb = (HFSMasterDirectoryBlock *)vh;
    if(OSSwapBigToHostInt16(mdb->drEmbedSigWord)
       == kHFSPlusSigWord) {
      errx(1, "Wrapped HFS+ filesystem not supported");
    } else {
      errx(1, "HFS (plain) filesystem not supported");
    }
  }
    break;
  default:
    errx(1, "Unrecognized filesystem");
  }

  if((OSSwapBigToHostInt32(vh->attributes) & kHFSVolumeUnmountedMask)
     == 0) {
    errx(1, "Filesystem not cleanly unmounted");
  }
  
  allocationBlockSize = OSSwapBigToHostInt32(vh->blockSize);

  datafd = open(filedata, O_RDONLY, 0400);
  if(datafd < 0)
    err(1, "open(%s)", filedata);

  ret = fstat(datafd, &sb);
  if(ret)
    err(1, "fstat(%s)", filedata);

  logicalSize = sb.st_size;
  totalBlocks = (logicalSize + allocationBlockSize - 1)/allocationBlockSize;

  ret = allocateBlocks(fd, &vh->allocationFile, 
		       allocationBlockSize, totalBlocks,
		       &allocatedBlocks, &allocatedStart);
  if(ret)
    errc(1, ret, "allocateBlocks of %u failed", totalBlocks);

  printf("allocated blocks %u at start %u\n",
	 allocatedBlocks, allocatedStart);

  vh->freeBlocks = OSSwapHostToBigInt32(OSSwapBigToHostInt32(vh->freeBlocks)
					- allocatedBlocks);
  vh->writeCount = OSSwapHostToBigInt32(OSSwapBigToHostInt32(vh->writeCount)
					+ 1);

  vh->startupFile.extents[0].startBlock = 
    OSSwapHostToBigInt32(allocatedStart);
  vh->startupFile.extents[0].blockCount = 
    OSSwapHostToBigInt32(allocatedBlocks);

  vh->startupFile.logicalSize = OSSwapHostToBigInt64(logicalSize);
  vh->startupFile.clumpSize = OSSwapHostToBigInt32(allocationBlockSize);
  vh->startupFile.totalBlocks= OSSwapHostToBigInt32(allocatedBlocks);

  bytesWritten = pwrite(fd, buffer, sizeof(buffer), 2*kSectorSize);
  if(bytesWritten != sizeof(buffer))
    err(1, "pwrite HFS+ VH");

  bytesWritten = pwrite(fd, buffer, sizeof(buffer),
			((uint64_t)deviceBlockSize) * deviceBlockCount - 2*kSectorSize);
  if(bytesWritten != sizeof(buffer))
    err(1, "pwrite HFS+ Alt-VH");


  iobytes = ((size_t)sb.st_size + allocationBlockSize-1)/allocationBlockSize*allocationBlockSize;
  datapayload = malloc(iobytes);
  if(read(datafd, datapayload, (size_t)sb.st_size) != (size_t)sb.st_size)
    err(1, "short read of datafd");

  if(pwrite(fd, datapayload, iobytes, ((off_t)allocatedStart)*((off_t)allocationBlockSize)) != iobytes)
    err(1, "short write of payload");

  free(datapayload);

  ret = close(datafd);
  if(ret)
    err(1, "close");

  ret = close(fd);
  if(ret)
    err(1, "close");

  return 0;
}

// we traverse the primary extents for the allocation bitmap
// to find space for the startup file.
int allocateBlocks(int fd, HFSPlusForkData *allocationFile,
		   uint32_t allocationBlockSize, uint32_t totalBlocks,
		   uint32_t *allocatedBlocks, uint32_t *allocatedStart)
{
  // total number of empty allocation words we need.
  uint32_t allocationWords = (totalBlocks + 32 - 1)/32;
  HFSPlusForkData bitmapFork;
  int i;
  uint32_t j;
  size_t bufferSize = allocationBlockSize; // read a block at a time
  char * buffer = valloc(bufferSize);

  printf("Looking for %u words free\n", allocationWords);

  if(buffer == NULL)
    err(1, "valloc failed");

  bitmapFork = *allocationFile;
  bitmapFork.logicalSize = OSSwapBigToHostInt64(bitmapFork.logicalSize);
  bitmapFork.clumpSize = OSSwapBigToHostInt32(bitmapFork.clumpSize);
  bitmapFork.totalBlocks = OSSwapBigToHostInt32(bitmapFork.totalBlocks);
  for(i=0; i < kHFSPlusExtentDensity; i++) {
    bitmapFork.extents[i].startBlock =
      OSSwapBigToHostInt32(bitmapFork.extents[i].startBlock);
    bitmapFork.extents[i].blockCount =
      OSSwapBigToHostInt32(bitmapFork.extents[i].blockCount);
  }

  for(j=0; j < bitmapFork.totalBlocks; j++) {
    ssize_t readBytes, writeBytes;
    int contigWordsEmpty = 0;
    uint32_t *ptr;
    int k;
    int foundit = 0;
    off_t readOffset = mapForkOffset(&bitmapFork,
				     allocationBlockSize,
				     j);
    printf("reading %qu,%d\n", readOffset, allocationBlockSize);
    readBytes = pread(fd, buffer, allocationBlockSize, readOffset);
    if(readBytes != allocationBlockSize)
      err(1, "pread(%d,%qu)", allocationBlockSize, readOffset);

    ptr = (uint32_t *)buffer;
    for(k=0; k < allocationBlockSize/sizeof(uint32_t); k++) {
      if(ptr[k] == 0x0) {
	contigWordsEmpty++;
	if(contigWordsEmpty == allocationWords) {
	  // got it
	  int startk = k + 1 - contigWordsEmpty;
	  int endk = k;
	  *allocatedBlocks = contigWordsEmpty*32;
	  *allocatedStart = j*allocationBlockSize*8
	    + startk*32;
	  for(; startk <= endk; startk++) {
	    printf("Marking word %d\n", startk);
	    ptr[startk] = 0xFFFFFFFF;
	  }
	  foundit = 1;
	  break;
	}
      } else {
	contigWordsEmpty = 0;
      }
    }
    
    if(foundit) {
      // marked the bits as used. Need to write back bitmap
      printf("writing back %qu,%d\n", readOffset, allocationBlockSize);
      writeBytes = pwrite(fd, buffer, allocationBlockSize, readOffset);
      // writeBytes = allocationBlockSize;
      if(writeBytes != allocationBlockSize)
	err(1, "pwrite(%d,%qu)", allocationBlockSize, readOffset);

      break;
    }

  }

  if(j == bitmapFork.totalBlocks) {
    // didn't allocate it
    return 1;
  } else {
    return 0;
  }
}

uint64_t mapForkOffset(HFSPlusForkData *fork, uint32_t allocationBlockSize,
			uint32_t blockNumber)
{
  int i;
  uint64_t offset = 0xFFFFFFFFFFFFFFFFULL;

  for(i=0; i < kHFSPlusExtentDensity; i++) {
    if(blockNumber < fork->extents[i].blockCount) {
      // in this extent
      offset =
	((uint64_t)(fork->extents[i].startBlock + blockNumber))*((uint64_t)allocationBlockSize);
      break;
    } else {
      // make relative to rest of file
      blockNumber -= fork->extents[i].blockCount;
    }
  }

  if(i == kHFSPlusExtentDensity)
    errx(1, "Could not map block in fork");

  return offset;
}
