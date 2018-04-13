/* =============================================================================
 * Copyright (c) 2001 Apple Computer, Inc.  All rights reserved.
 * compile with:
 * gcc -o iofindwholemedia -framework IOKit -framework CoreFoundation \
 * 					iofindwholemedia.c 
 *
 * $Id: iofindwholemedia.c,v 1.6 2004/06/03 17:25:50 fkr Exp $
 */

#include <IOKit/IOKitLib.h>
#include <IOKit/IOBSD.h>
#include <IOKit/storage/IOMedia.h>
#include <sys/disk.h>
#include <sys/ioctl.h>
#include <sys/fcntl.h>

#include <CoreFoundation/CoreFoundation.h>

#include <unistd.h>

static void servicesHaveMatched(io_iterator_t services);
static void show(io_registry_entry_t service);

static int disknum = 0;
static int showdisk = 0;

int
main(int argc, char *argv[])
{
     CFMutableDictionaryRef description = 0; // (needs release)
     mach_port_t            iokitPort   = 0; // (don't release)
     io_iterator_t          services    = 0; // (needs release)
     kern_return_t          status      = KERN_SUCCESS;

     status = IOMasterPort(bootstrap_port, &iokitPort);
     assert(status == KERN_SUCCESS);

     description = IOServiceMatching(kIOMediaClass);

     CFDictionaryAddValue(description, CFSTR(kIOMediaWholeKey), 
kCFBooleanTrue);
     CFDictionaryAddValue(description, CFSTR(kIOMediaWritableKey),
                                 kCFBooleanTrue);

     status = IOServiceGetMatchingServices(iokitPort, description, 
&services);
     assert(status == KERN_SUCCESS);

     if (argc == 2) {
         showdisk = atoi(argv[1]);
     }
     servicesHaveMatched(services);

     IOObjectRelease(services);

     return 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void servicesHaveMatched(io_iterator_t services)
{
     io_registry_entry_t service = 0; // (needs release)

     while ((service = IOIteratorNext(services))) {
         show(service);
         IOObjectRelease(service);
     }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

static void show(io_registry_entry_t service)
{
     io_name_t       name;           // (don't release)
     const char *    node       = 0; // (needs release)
     CFMutableDictionaryRef properties = 0; // (needs release)
     kern_return_t   status     = KERN_SUCCESS;
     CFStringRef     string     = 0; // (don't release)
     double          bytes      = 0.0;
     int64_t         bytes64    = 0LL;
     const char *    morg       = "MB";
     char *          space      = NULL;

     char            buf[32];
     int             devBlkSize,fd;
     long long       devBlkCnt;

     status = IORegistryEntryGetName(service, name);
     assert(status == KERN_SUCCESS);

     status = IORegistryEntryCreateCFProperties(service, &properties,
                 kCFAllocatorDefault, kNilOptions );
     assert(status == KERN_SUCCESS);
     assert(CFGetTypeID(properties) == CFDictionaryGetTypeID());

     string = (CFStringRef) CFDictionaryGetValue(properties, CFSTR(kIOBSDNameKey));
     if (string) {
         assert(CFGetTypeID(string) == CFStringGetTypeID());
         node = CFStringGetCStringPtr(string, 
	 CFStringGetSystemEncoding());
     }

     sprintf(buf, "/dev/r%s", node);
     fd = open (buf, O_RDONLY);
     if (fd < 0) {
         perror("open");
         exit(1);
     }
     if (ioctl(fd, DKIOCGETBLOCKSIZE, &devBlkSize) < 0) {
         perror("ioctl(DKIOCGETBLOCKSIZE)");
         exit(1);
     }
     if (ioctl(fd, DKIOCGETBLOCKCOUNT, &devBlkCnt) < 0) {
         perror("ioctl(DKIOCGETBLOCKCOUNT)");
         exit(1);
     }
     close(fd);
     bytes64 = (int64_t)devBlkSize * devBlkCnt;

     if (showdisk) {
         if (++disknum == showdisk) {
                 printf("/dev/%s\n", node);
         }
         CFRelease(properties);
         return;
     }
     bytes = (double)bytes64 / 1024.0 / 1024.0;
     if (bytes > 1024.0) {
         morg = "GB";
         bytes /= 1024.0;
     }

     // Strip out spaces and "Media"
     space = strrchr(name, ' ');
     if (space) while (*space == ' ') *space-- = '\0';

     printf("%d. %s @ %s (%.1f %s)\n", ++disknum, name, node, bytes, morg);
     CFRelease(properties);
}
