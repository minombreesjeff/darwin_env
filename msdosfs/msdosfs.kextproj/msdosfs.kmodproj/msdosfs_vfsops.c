/*
 * Copyright (c) 2000 Apple Computer, Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 * 
 * Copyright (c) 1999-2003 Apple Computer, Inc.  All Rights Reserved.
 * 
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this
 * file.
 * 
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@
 */
/* $FreeBSD: src/sys/msdosfs/msdosfs_vfsops.c,v 1.63 2000/05/05 09:58:36 phk Exp $ */
/*	$NetBSD: msdosfs_vfsops.c,v 1.51 1997/11/17 15:36:58 ws Exp $	*/

/*-
 * Copyright (C) 1994, 1995, 1997 Wolfgang Solfrank.
 * Copyright (C) 1994, 1995, 1997 TooLs GmbH.
 * All rights reserved.
 * Original code by Paul Popelka (paulp@uts.amdahl.com) (see below).
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by TooLs GmbH.
 * 4. The name of TooLs GmbH may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY TOOLS GMBH ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL TOOLS GMBH BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*
 * Written by Paul Popelka (paulp@uts.amdahl.com)
 *
 * You can do anything you want with this software, just don't say you wrote
 * it, and don't remove this notice.
 *
 * This software is provided "as is".
 *
 * The author supplies this software to be publicly redistributed on the
 * understanding that the author is not responsible for the correct
 * functioning of this software in any circumstances and is not liable for
 * any damages caused by this software.
 *
 * October 1992
 */
#include <sys/param.h>
#include <sys/systm.h>
#include <sys/conf.h>
#include <sys/proc.h>
#include <sys/kernel.h>
#include <sys/vnode.h>
#include <sys/mount.h>
#include <sys/buf.h>
#include <sys/fcntl.h>
#include <sys/malloc.h>
#include <sys/stat.h> 				/* defines ALLPERMS */
#include <sys/ubc.h>
#include <sys/utfconv.h>
#include <sys/disk.h>
#include <mach/kmod.h>
#include <kern/thread.h>

#include <miscfs/specfs/specdev.h>

#include "bpb.h"
#include "bootsect.h"
#include "direntry.h"
#include "denode.h"
#include "msdosfsmount.h"
#include "fat.h"

#define MSDOSFS_DFLTBSIZE       4096

extern u_int16_t dos2unicode[32];

extern long msdos_secondsWest;	/* In msdosfs_conv.c */

static int	update_mp __P((struct mount *mp, struct msdosfs_args *argp));
static int	mountmsdosfs __P((vnode_t devvp, struct mount *mp,
				  vfs_context_t context, struct msdosfs_args *argp));
static int	msdosfs_fhtovp __P((struct mount *, struct fid *, vnode_t *, vfs_context_t));
static int	msdosfs_mount __P((struct mount *mp, vnode_t devvp, caddr_t data, vfs_context_t));
static int	msdosfs_root __P((struct mount *, vnode_t *, vfs_context_t));
static int	msdosfs_statfs __P((struct mount *, struct vfsstatfs *, vfs_context_t));
static int	msdosfs_sync __P((struct mount *, int, vfs_context_t));
static int	msdosfs_unmount __P((struct mount *, int, vfs_context_t));
static int	msdosfs_vptofh __P((vnode_t, struct fid *, vfs_context_t));

static int	get_root_label(struct mount *mp, vfs_context_t context);

/*ARGSUSED*/
static int 
msdosfs_init(vfsp)
	struct vfsconf *vfsp;
{
	msdosfs_hash_init();
	msdosfs_fat_init();
	return 0;
}


static int
msdosfs_uninit(vfsp)
	struct vfsconf *vfsp;
{
	msdosfs_fat_uninit();
	msdosfs_hash_uninit();
	return 0;
}


static int
update_mp(mp, argp)
	struct mount *mp;
	struct msdosfs_args *argp;
{
	struct msdosfsmount *pmp = VFSTOMSDOSFS(mp);

	pmp->pm_gid = argp->gid;
	pmp->pm_uid = argp->uid;
	pmp->pm_mask = argp->mask & ALLPERMS;
	pmp->pm_flags |= argp->flags & MSDOSFSMNT_MNTOPT;
	if (argp->flags & MSDOSFSMNT_SECONDSWEST)
		msdos_secondsWest = argp->secondsWest;

	if (argp->flags & MSDOSFSMNT_LABEL)
		bcopy(argp->label, pmp->pm_label, sizeof(pmp->pm_label));

	return 0;
}


/*
 * mp - path - addr in user space of mount point (ie /usr or whatever)
 * data - addr in user space of mount params including the name of the block
 * special file to treat as a filesystem.
 */
static int
msdosfs_mount(mp, devvp, data, context)
	struct mount *mp;
	vnode_t devvp;
	caddr_t data;
	vfs_context_t context;
{
	struct msdosfs_args args; /* will hold data from mount request */
	/* msdosfs specific mount control block */
	struct msdosfsmount *pmp = NULL;
	int error, flags;

	error = copyin(CAST_USER_ADDR_T(data), (caddr_t)&args, sizeof(struct msdosfs_args));
	if (error)
		return (error);
	if (args.magic != MSDOSFS_ARGSMAGIC)
		args.flags = 0;

	/*
	 * If updating, check whether changing from read-only to
	 * read/write; if there is no device name, that's all we do.
	 */
	if (vfs_isupdate(mp)) {
		pmp = VFSTOMSDOSFS(mp);
		error = 0;
		if (!(pmp->pm_flags & MSDOSFSMNT_RONLY) && vfs_isrdonly(mp)) {
			/* Downgrading from read/write to read-only */
			/* ¥ Is vflush() sufficient?  Is there more we should flush out? */
			flags = WRITECLOSE;
			if (vfs_isforce(mp))
				flags |= FORCECLOSE;
			error = vflush(mp, NULLVP, flags);
		}
		if (!error && vfs_isreload(mp))
			/* not yet implemented */
			error = EOPNOTSUPP;
		if (error)
			return (error);
		if ((pmp->pm_flags & MSDOSFSMNT_RONLY) && vfs_iswriteupgrade(mp)) {
			/* ¥ Assuming that VFS has verified we can write to the device */

			pmp->pm_flags &= ~MSDOSFSMNT_RONLY;

			/* Now that the volume is modifiable, mark it dirty */
			error = markvoldirty(pmp, 1, context);
			if (error) {
				pmp->pm_flags |= MSDOSFSMNT_RONLY;
				return error;
			}
		}
		if (devvp == 0) {	/* ¥ Is that the correct test for exports? */
#ifdef	__notyet__		/* doesn't work correctly with current mountd	XXX */
			if (args.flags & MSDOSFSMNT_MNTOPT) {
				pmp->pm_flags &= ~MSDOSFSMNT_MNTOPT;
				pmp->pm_flags |= args.flags & MSDOSFSMNT_MNTOPT;
			}
			/*
			 * Process export requests.
			 */
			return (vfs_export(mp, &pmp->pm_export, &args.export));
#endif
		}
	}

	if ( !vfs_isupdate(mp))
		error = mountmsdosfs(devvp, mp, context, &args);

	if (error == 0)
		error = update_mp(mp, &args);

	if (error == 0)
		(void) msdosfs_statfs(mp, vfs_statfs(mp), context);

	if (error)
		msdosfs_unmount(mp, MNT_FORCE, context);

	return error;
}

static int
mountmsdosfs(devvp, mp, context, argp)
	vnode_t devvp;
	struct mount *mp;
	vfs_context_t context;
	struct msdosfs_args *argp;
{
	struct msdosfsmount *pmp;
	struct buf *bp;
	dev_t dev = vnode_specrdev(devvp);
	union bootsector *bsp;
	struct byte_bpb33 *b33;
	struct byte_bpb50 *b50;
	struct byte_bpb710 *b710;
	u_int8_t SecPerClust;
	u_long clusters;
	int	ronly, error;
	struct vfsstatfs *vfsstatfs;
	
	/*
	 * Disallow multiple mounts of the same device.
	 * Disallow mounting of a device that is currently in use
	 * (except for root, which might share swap device for miniroot).
	 * Flush out any old buffers remaining from a previous use.
	 *
	 *¥ Obsolete?

	error = vfs_mountedon(devvp);
	if (error)
		return (error);
	if (vcount(devvp) > 1 && devvp != rootvp)
		return (EBUSY);
	 */

	error = buf_invalidateblks(devvp, BUF_WRITE_DATA, 0, 0);
	if (error)
		return (error);

	ronly = vfs_isrdonly(mp);

	bp  = NULL; /* both used in error_exit */
	pmp = NULL;

	/*
	 * Read the boot sector of the filesystem, and then check the
	 * boot signature.  If not a dos boot sector then error out.
	 *
	 * NOTE: 2048 is a maximum sector size in current...
	 */
	error = (int)buf_meta_bread(devvp, 0, 2048, vfs_context_ucred(context), &bp);
	if (error)
		goto error_exit;
	buf_markaged(bp);
	bsp = (union bootsector *)buf_dataptr(bp);
	b33 = (struct byte_bpb33 *)bsp->bs33.bsBPB;
	b50 = (struct byte_bpb50 *)bsp->bs50.bsBPB;
	b710 = (struct byte_bpb710 *)bsp->bs710.bsBPB;


	/* [2699033]
	 *
	 * The first three bytes are an Intel x86 jump instruction.  It should be one
	 * of the following forms:
	 *    0xE9 0x?? 0x??
	 *    0xEC 0x?? 0x90
	 * where 0x?? means any byte value is OK.
	 */
	if (bsp->bs50.bsJump[0] != 0xE9
		&& (bsp->bs50.bsJump[0] != 0xEB || bsp->bs50.bsJump[2] != 0x90))
	{
		error = EINVAL;
		goto error_exit;
	}

	MALLOC(pmp, struct msdosfsmount *, sizeof(*pmp), M_TEMP, M_WAITOK);
	bzero((caddr_t)pmp, sizeof *pmp);
	pmp->pm_mountp = mp;

	/*
	 * Compute several useful quantities from the bpb in the
	 * bootsector.  Copy in the dos 5 variant of the bpb then fix up
	 * the fields that are different between dos 5 and dos 3.3.
	 */
	SecPerClust = b50->bpbSecPerClust;
	pmp->pm_BytesPerSec = getushort(b50->bpbBytesPerSec);
	pmp->pm_ResSectors = getushort(b50->bpbResSectors);
	pmp->pm_FATs = b50->bpbFATs;
	pmp->pm_RootDirEnts = getushort(b50->bpbRootDirEnts);
	pmp->pm_Sectors = getushort(b50->bpbSectors);
	pmp->pm_FATsecs = getushort(b50->bpbFATsecs);
	pmp->pm_SecPerTrack = getushort(b50->bpbSecPerTrack);
	pmp->pm_Heads = getushort(b50->bpbHeads);
	pmp->pm_Media = b50->bpbMedia;
	pmp->pm_label_cluster = CLUST_EOFE;	/* Assume there is no label in the root */

	/* calculate the ratio of sector size to device block size */
	error = VNOP_IOCTL(devvp, DKIOCGETBLOCKSIZE, (caddr_t) &pmp->pm_BlockSize, 0, context);
	if (error) {
		error = ENXIO;
		goto error_exit;
	}
	pmp->pm_BlocksPerSec = pmp->pm_BytesPerSec / pmp->pm_BlockSize;

	if (pmp->pm_Sectors == 0) {
		pmp->pm_HugeSectors = getulong(b50->bpbHugeSectors);
	} else {
		pmp->pm_HugeSectors = pmp->pm_Sectors;
	}

	if (pmp->pm_RootDirEnts == 0) {
		if (pmp->pm_Sectors != 0
		    || pmp->pm_FATsecs != 0
		    || getushort(b710->bpbFSVers) != 0) {
			error = EINVAL;
			printf("mountmsdosfs(): bad FAT32 filesystem\n");
			goto error_exit;
		}
		pmp->pm_fatmask = FAT32_MASK;
		pmp->pm_fatmult = 4;
		pmp->pm_fatdiv = 1;
		pmp->pm_FATsecs = getulong(b710->bpbBigFATsecs);
		if (getushort(b710->bpbExtFlags) & FATMIRROR)
			pmp->pm_curfat = getushort(b710->bpbExtFlags) & FATNUM;
		else
			pmp->pm_flags |= MSDOSFS_FATMIRROR;
	} else
		pmp->pm_flags |= MSDOSFS_FATMIRROR;

	/*
	 * Check a few values (could do some more):
	 * - logical sector size: power of 2, >= block size
	 * - sectors per cluster: power of 2, >= 1
	 * - number of sectors:   >= 1, <= size of partition
	 */
	if ( (SecPerClust == 0)
	  || (SecPerClust & (SecPerClust - 1))
	  || (pmp->pm_BytesPerSec < DEV_BSIZE)
	  || (pmp->pm_BytesPerSec & (pmp->pm_BytesPerSec - 1))
	  || (pmp->pm_HugeSectors == 0)
	) {
		error = EINVAL;
		goto error_exit;
	}

	if (FAT32(pmp)) {
		pmp->pm_rootdirblk = getulong(b710->bpbRootClust);
		pmp->pm_firstcluster = pmp->pm_ResSectors
			+ (pmp->pm_FATs * pmp->pm_FATsecs);
		pmp->pm_fsinfo = getushort(b710->bpbFSInfo);
	} else {
                /*
                 * Compute the root directory and first cluster as sectors
                 * so that pm_maxcluster will be correct, below.
                 */
		pmp->pm_rootdirblk = (pmp->pm_ResSectors + (pmp->pm_FATs * pmp->pm_FATsecs));
		pmp->pm_rootdirsize = (pmp->pm_RootDirEnts * sizeof(struct direntry)
				       + pmp->pm_BytesPerSec - 1)
			/ pmp->pm_BytesPerSec; /* in sectors */
		pmp->pm_firstcluster = pmp->pm_rootdirblk + pmp->pm_rootdirsize;
                
                /* Change the root directory values to physical (device) blocks */
                pmp->pm_rootdirblk *= pmp->pm_BlocksPerSec;
                pmp->pm_rootdirsize *= pmp->pm_BlocksPerSec;
	}

	pmp->pm_maxcluster = (pmp->pm_HugeSectors - pmp->pm_firstcluster) /
	    SecPerClust + 1;

        pmp->pm_firstcluster *= pmp->pm_BlocksPerSec;	/* Convert to physical (device) blocks */

	if (pmp->pm_fatmask == 0) {
		if (pmp->pm_maxcluster
		    <= ((CLUST_RSRVD - CLUST_FIRST) & FAT12_MASK)) {
			/*
			 * This will usually be a floppy disk. This size makes
			 * sure that one fat entry will not be split across
			 * multiple blocks.
			 */
			pmp->pm_fatmask = FAT12_MASK;
			pmp->pm_fatmult = 3;
			pmp->pm_fatdiv = 2;
		} else {
			pmp->pm_fatmask = FAT16_MASK;
			pmp->pm_fatmult = 2;
			pmp->pm_fatdiv = 1;
		}
	}

	/* Compute number of clusters this FAT could hold based on its total size */
	clusters = pmp->pm_FATsecs * pmp->pm_BytesPerSec;	/* Size of FAT in bytes */
	clusters *= pmp->pm_fatdiv;
	clusters /= pmp->pm_fatmult;				/* Max number of clusters, rounded down */
        
	if (pmp->pm_maxcluster >= clusters) {
		printf("Warning: number of clusters (%ld) exceeds FAT "
		    "capacity (%ld)\n", pmp->pm_maxcluster + 1, clusters);
		pmp->pm_maxcluster = clusters - 1;
	}


	if (FAT12(pmp))
		pmp->pm_fatblocksize = 3 * pmp->pm_BytesPerSec;
	else
		pmp->pm_fatblocksize = MSDOSFS_DFLTBSIZE;

	pmp->pm_fatblocksec = pmp->pm_fatblocksize / pmp->pm_BytesPerSec;
	pmp->pm_bnshift = ffs(pmp->pm_BlockSize) - 1;

	/*
	 * Compute mask and shift value for isolating cluster relative byte
	 * offsets and cluster numbers from a file offset.
	 */
	pmp->pm_bpcluster = (u_int32_t) SecPerClust * (u_int32_t) pmp->pm_BytesPerSec;
	pmp->pm_crbomask = pmp->pm_bpcluster - 1;
	pmp->pm_cnshift = ffs(pmp->pm_bpcluster) - 1;

	/*
	 * Check for valid cluster size
	 * must be a power of 2
	 */
	if (pmp->pm_bpcluster ^ (1 << pmp->pm_cnshift)) {
		error = EINVAL;
		goto error_exit;
	}

	/* Copy volume label from boot sector into mount point */
	{
		struct extboot *extboot;
		int i;
		u_char uc;
		
		/* Start out assuming no label (empty string) */
		pmp->pm_label[0] = '\0';

		if (FAT32(pmp)) {
			extboot = (struct extboot *) bsp->bs710.bsExt;
		} else {
			extboot = (struct extboot *) bsp->bs50.bsExt;
		}
		
		if (extboot->exBootSignature == EXBOOTSIG) {
			/*
			 * Copy the label from the boot sector into the mount point.
			 *
			 * We don't call dos2unicodefn() because it assumes the last three
			 * characters are an extension, and it will put a period before the
			 * extension.
			 */
			for (i=0; i<11; i++) {
				uc = extboot->exVolumeLabel[i];
				if (i==0 && uc == SLOT_E5)
					uc = 0xE5;
				pmp->pm_label[i] = (uc < 0x80 || uc > 0x9F ? uc : dos2unicode[uc - 0x80]);
			}

			/* Remove trailing spaces, add NUL terminator */
			for (i=10; i>=0 && pmp->pm_label[i]==' '; --i)
				;
			pmp->pm_label[i+1] = '\0';
		}
	}
        
	/*
	 * Release the bootsector buffer.
	 */
	buf_brelse(bp);
	bp = NULL;

	/*
	 * Check FSInfo.
	 */
	if (pmp->pm_fsinfo) {
		struct fsinfo *fp;

		/*
		 * The FSInfo sector occupies pm_BytesPerSec bytes on disk,
		 * but only 512 of those have meaningful contents.  There's no point
		 * in reading all pm_BytesPerSec bytes if the device block size is
		 * smaller.  So just use the device block size here.
		 */
		error = buf_meta_bread(devvp, pmp->pm_fsinfo, pmp->pm_BlockSize, vfs_context_ucred(context), &bp);
		if (error)
			goto error_exit;
		fp = (struct fsinfo *)buf_dataptr(bp);
		if (!bcmp(fp->fsisig1, "RRaA", 4)
		    && !bcmp(fp->fsisig2, "rrAa", 4)
		    && !bcmp(fp->fsisig3, "\0\0\125\252", 4))
			pmp->pm_nxtfree = getulong(fp->fsinxtfree);
		else
			pmp->pm_fsinfo = 0;
		buf_brelse(bp);
		bp = NULL;
	}

	/*
	 * Check and validate (or perhaps invalidate?) the fsinfo structure?		XXX
	 */

	/*
	 * If they want fat updates to be synchronous then let them suffer
	 * the performance degradation in exchange for the on disk copy of
	 * the fat being correct just about all the time.  I suppose this
	 * would be a good thing to turn on if the kernel is still flakey.
	 */
	if (vfs_issynchronous(mp))
		pmp->pm_flags |= MSDOSFSMNT_WAITONFAT;

	/*
	 * msdosfs_fat_init_vol() needs pm_devvp.
	 */
	pmp->pm_dev = dev;
	pmp->pm_devvp = devvp;

	/*
	 * Set up the per-volume FAT structures, including
	 * the in-use map.
	 */
	error = msdosfs_fat_init_vol(pmp, context);
	if (error != 0)
		goto error_exit;

	/*
	 * Finish up.
	 */
	if (ronly)
		pmp->pm_flags |= MSDOSFSMNT_RONLY;
	else {
		/* [2753891] Mark the volume dirty while it is mounted read/write */
		if ((error = markvoldirty(pmp, 1, context)) != 0)
			goto error_exit;

		pmp->pm_fmod = 1;
	}
	vfs_setfsprivate(mp, (void *)pmp);

	/*
	 * Fill in the statvfs fields that are constant (not updated by msdosfs_statfs)
	 */
	vfsstatfs = vfs_statfs(mp);
	vfsstatfs->f_bsize = pmp->pm_bpcluster;
	vfsstatfs->f_iosize = pmp->pm_bpcluster;
	vfsstatfs->f_blocks = pmp->pm_maxcluster + 1;
	vfsstatfs->f_fsid.val[0] = (long)dev;
	vfsstatfs->f_fsid.val[1] = vfs_typenum(mp);

	/* vfs_setflags(mp, MNT_LOCAL);		¥ Is this needed?  Is it set by vfs_fsadd? */
	vnode_setmountedon(devvp);

	(void) get_root_label(mp, context);

	return 0;

error_exit:
	if (bp)
		buf_brelse(bp);
	if (pmp) {
		msdosfs_fat_uninit_vol(pmp);

		FREE(pmp, M_TEMP);

		vfs_setfsprivate(mp, (void *)NULL);
	}
	return (error);
}

/*
 * Make a filesystem operational.
 * Nothing to do at the moment.
 */
/* ARGSUSED */
static int
msdosfs_start(mp, flags, p)
	struct mount *mp;
	int flags;
	proc_t p;
{
	return (0);
}

/*
 * Unmount the filesystem described by mp.
 */
static int
msdosfs_unmount(mp, mntflags, context)
	struct mount *mp;
	int mntflags;
	vfs_context_t context;
{
	struct msdosfsmount *pmp;
	int error, flags;
	int force;

	flags = 0;
	force = 0;
	if (mntflags & MNT_FORCE) {
		flags |= FORCECLOSE;
		force = 1;
	}

	error = vflush(mp, NULLVP, flags);
	if (error && !force)
		return (error);
	pmp = VFSTOMSDOSFS(mp);

	/* [2753891] If the volume was mounted read/write, mark it clean now */
	if ((pmp->pm_flags & MSDOSFSMNT_RONLY) == 0) {
		error = markvoldirty(pmp, 0, context);
		if (error && !force)
			return (error);
	}
	
	/*¥ Flush the device here? */
	
	msdosfs_fat_uninit_vol(pmp);
	FREE(pmp, M_TEMP);

	vfs_setfsprivate(mp, (void *)NULL);

	return (0);
}

static int
msdosfs_root(mp, vpp, context)
	struct mount *mp;
	vnode_t *vpp;
	vfs_context_t context;
{
	struct msdosfsmount *pmp = VFSTOMSDOSFS(mp);
	struct denode *ndep;
	int error;

	error = deget(pmp, MSDOSFSROOT, MSDOSFSROOT_OFS, NULLVP, NULL, &ndep, context);
	if (error)
		return (error);
	*vpp = DETOV(ndep);
	return (0);
}


/*
 * Do operations associated with quotas
 */
static int msdosfs_quotactl(mp, cmds, uid, arg, segflg, p)
	struct mount *mp;
	int cmds;
	uid_t uid;
	caddr_t arg;
	enum uio_seg segflg;
	proc_t p;
{
    return (EOPNOTSUPP);
}


static int
msdosfs_statfs(mp, sbp, context)
	struct mount *mp;
	struct vfsstatfs *sbp;
	vfs_context_t context;
{
	struct msdosfsmount *pmp;

	pmp = VFSTOMSDOSFS(mp);
	/*
	 * ¥ VFS fills in everything from a cached copy.
	 * We only need to fill in fields that can change.
	 */
	sbp->f_bfree = pmp->pm_freeclustercount;
	sbp->f_bavail = pmp->pm_freeclustercount;
	sbp->f_bused = sbp->f_blocks - sbp->f_bavail;
	sbp->f_files = pmp->pm_RootDirEnts;			/* XXX */
	sbp->f_ffree = 0;	/* what to put in here? */
	vfs_name(mp, sbp->f_fstypename);

	return (0);
}


struct msdosfs_sync_cargs {
	vfs_context_t context;
	int		waitfor;
	int		error;
};


static int
msdosfs_sync_callback(vnode_t vp, void *cargs)
{
	struct denode *dep;
	struct msdosfs_sync_cargs *args;
	int error;

	args = (struct msdosfs_sync_cargs *)cargs;

	dep = VTODE(vp);

	if ((dep->de_flag & (DE_ACCESS | DE_CREATE | DE_UPDATE | DE_MODIFIED)) || vnode_hasdirtyblks(vp)) {
		error = msdosfs_fsync_internal(vp, args->waitfor==MNT_WAIT, args->context);

		if (error)
			args->error = error;
	}
	return (VNODE_RETURNED);
}


static int
msdosfs_sync(mp, waitfor, context)
	struct mount *mp;
	int waitfor;
	vfs_context_t context;
{
	struct msdosfsmount *pmp = VFSTOMSDOSFS(mp);
	int error, allerror = 0;
	struct msdosfs_sync_cargs args;
        
	/*
	 * If we ever switch to not updating all of the fats all the time,
	 * this would be the place to update them from the first one.
	 */
	if (pmp->pm_fmod != 0) {
		if (pmp->pm_flags & MSDOSFSMNT_RONLY)
			panic("msdosfs_sync: rofs mod");
		else {
			/* update fats here */
		}
	}
	/*
	 * Write back each (modified) denode.
	 */
	args.context = context;
	args.waitfor = waitfor;
	args.error = 0;
	/*
	 * msdosfs_sync_callback will be called for each vnode
	 * hung off of this mount point... the vnode will be
	 * properly referenced and unreferenced around the callback
	 */
	vnode_iterate(mp, 0, msdosfs_sync_callback, (void *)&args);

	if (args.error)
		allerror = args.error;

	/*¥ Do we need to flush the FATs? */
	
	/*¥ Do we need to flush the boot sector or FSInfo sector? */

	/*
	 * Flush filesystem control info.
	 */
	error = VNOP_FSYNC(pmp->pm_devvp, waitfor, context);
	if (error)
		allerror = error;

	return (allerror);
}


static int
msdosfs_vget(mp, ino, vpp, context)
	struct mount *mp;
	void *ino;
	vnode_t *vpp;
	vfs_context_t context;
{
	return (EOPNOTSUPP);
}


/*
 *¥	File handle and deget() should probably also take parent directory's
 *	start cluster.  This way, we could always find a file's parent.
 */
static int
msdosfs_fhtovp(mp, fhp, vpp, context)
	struct mount *mp;
	struct fid *fhp;
	vnode_t *vpp;
	vfs_context_t context;
{
	struct msdosfsmount *pmp = VFSTOMSDOSFS(mp);
	struct defid *defhp = (struct defid *) fhp;
	struct denode *dep;
	int error;

	return (EOPNOTSUPP);

	error = deget(pmp, defhp->defid_dirclust, defhp->defid_dirofs, NULLVP, NULL, &dep, context);
	if (error) {
		*vpp = NULLVP;
		return (error);
	}
	*vpp = DETOV(dep);
	return (0);
}


static int
msdosfs_vptofh(vp, fhp, context)
	vnode_t vp;
	struct fid *fhp;
	vfs_context_t context;
{
	struct denode *dep;
	struct defid *defhp;

	dep = VTODE(vp);
	defhp = (struct defid *)fhp;
	defhp->defid_len = sizeof(struct defid);
	defhp->defid_dirclust = dep->de_dirclust;
	defhp->defid_dirofs = dep->de_diroffset;
	/* defhp->defid_gen = dep->de_gen; */
	return (0);
}

static int
msdosfs_sysctl(name, namelen, oldp, oldlenp, newp, newlen, context)
     int * name;
     u_int namelen;
     void* oldp;
     size_t * oldlenp;
     void * newp;
     size_t newlen;
     vfs_context_t context;
{
     return (EOPNOTSUPP);
}

struct vfsops msdosfs_vfsops = {
	msdosfs_mount,
	msdosfs_start,
	msdosfs_unmount,
	msdosfs_root,
	msdosfs_quotactl,
	msdosfs_statfs,
	msdosfs_sync,
	msdosfs_vget,
	msdosfs_fhtovp,
	msdosfs_vptofh,
	msdosfs_init,
	msdosfs_sysctl
};

extern struct vnodeopv_desc msdosfs_vnodeop_opv_desc;
static struct vnodeopv_desc *msdosfs_vnodeop_opv_desc_list[1] =
{
	&msdosfs_vnodeop_opv_desc
};


/*
 *¥ vfs_fsadd: second parameter should be (void **)?
 * If so, then the following should be a (void *).
 */
static struct vfsconf msdosfs_vfsconf;

__private_extern__ int
msdosfs_module_start(struct kmod_info_t *ki, void *data)
{
#pragma unused(data)
	errno_t error;
	struct vfs_fsentry vfe;
	
	vfe.vfe_vfsops = &msdosfs_vfsops;
	vfe.vfe_vopcnt = 1;		/* We just have vnode operations for regular files and directories */
	vfe.vfe_opvdescs = msdosfs_vnodeop_opv_desc_list;
	strcpy(vfe.vfe_fsname, "msdos");
	vfe.vfe_flags = VFS_TBLNOTYPENUM | VFS_TBLLOCALVOL;
	vfe.vfe_reserv[0] = 0;
	vfe.vfe_reserv[1] = 0;
	
	error = vfs_fsadd(&vfe, &msdosfs_vfsconf);
	return error ? KERN_FAILURE : KERN_SUCCESS;
}

__private_extern__ int  
msdosfs_module_stop(kmod_info_t *ki, void *data)
{
#pragma unused(ki)
#pragma unused(data)
	errno_t error;

	error = vfs_fsremove(&msdosfs_vfsconf);
	
	return error ? KERN_FAILURE : KERN_SUCCESS;
}



/*
 * Look through the root directory for a volume label entry.
 * If found, use it to replace the label in the mount point.
 */
static int get_root_label(struct mount *mp, vfs_context_t context)
{
    int error;
    struct msdosfsmount *pmp;
    vnode_t rootvp = NULL;
    struct buf *bp = NULL;
    u_long frcn;	/* file relative cluster number in root directory */
    daddr64_t bn;		/* block number of current dir block */
    u_long cluster;	/* cluster number of current dir block */
    u_long blsize;	/* size of current dir block */
    int blkoff;		/* dir entry offset within current dir block */
    struct direntry *dep = NULL;
    struct denode *root;
    u_int16_t unichars;
    u_int16_t ucfn[12];
    u_char uc;
    int i;
    size_t outbytes;
    char *bdata;

    pmp = VFSTOMSDOSFS(mp);

    error = msdosfs_root(mp, &rootvp, context);
    if (error)
        return error;
    root = VTODE(rootvp);
    
    for (frcn=0; ; frcn++) {
        error = pcbmap(root, frcn, 1, &bn, &cluster, &blsize, context);
        if (error)
            goto not_found;

        error = (int)buf_meta_bread(pmp->pm_devvp, bn, blsize, vfs_context_ucred(context), &bp);
        if (error) {
            goto not_found;
        }
		bdata = (char *)buf_dataptr(bp);

        for (blkoff = 0; blkoff < blsize; blkoff += sizeof(struct direntry)) {
            dep = (struct direntry *) (bdata + blkoff);

            /* Skip deleted directory entries */
            if (dep->deName[0] == SLOT_DELETED)
                continue;
            
            /* Stop if we hit the end of the directory (a never used entry) */
            if (dep->deName[0] == SLOT_EMPTY) {
                goto not_found;
            }

            /* Skip long name entries */
            if (dep->deAttributes == ATTR_WIN95)
                continue;
            
            if (dep->deAttributes & ATTR_VOLUME) {
                pmp->pm_label_cluster = cluster;
                pmp->pm_label_offset = blkoff;

                /*
                 * Copy the dates from the label to the root vnode.
                 */
                root->de_CHun = dep->deCHundredth;
                root->de_CTime = getushort(dep->deCTime);
                root->de_CDate = getushort(dep->deCDate);
                root->de_ADate = getushort(dep->deADate);
                root->de_MTime = getushort(dep->deMTime);
                root->de_MDate = getushort(dep->deMDate);

				/*
                 * We don't call dos2unicodefn() because it assumes the last three
                 * characters are an extension, and it will put a period before the
                 * extension.
                 */
				for (i=0; i<11; i++) {
					uc = dep->deName[i];
					if (i==0 && uc == SLOT_E5)
						uc = 0xE5;
					ucfn[i] = (uc < 0x80 || uc > 0x9F ? (u_int16_t)uc : dos2unicode[uc - 0x80]);
				}
				for (i=10; i>=0 && ucfn[i]==' '; --i)
					;
				unichars = i+1;
				
				/* translate the name in ucfn into UTF-8 */
				error = utf8_encodestr(ucfn, unichars * 2,
								pmp->pm_label, &outbytes,
								sizeof(pmp->pm_label), 0, UTF_DECOMPOSED);
                goto found;
            }
        }
        
        buf_brelse(bp);
        bp = NULL;
    }

found:
not_found:
    if (bp)
        buf_brelse(bp);

    if (rootvp)
        vnode_put(rootvp);

    return error;
}
