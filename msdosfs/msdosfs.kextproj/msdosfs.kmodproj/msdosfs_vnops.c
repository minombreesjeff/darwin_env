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
/* $FreeBSD: src/sys/msdosfs/msdosfs_vnops.c,v 1.99 2000/05/05 09:58:36 phk Exp $ */
/*	$NetBSD: msdosfs_vnops.c,v 1.68 1998/02/10 14:10:04 mrg Exp $	*/

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
#include <sys/vnode.h>
#include <sys/resourcevar.h>	/* defines plimit structure in proc struct */
#include <sys/kernel.h>
#include <sys/stat.h>
#include <sys/buf.h>
#include <sys/proc.h>
#include <sys/mount.h>
#include <sys/unistd.h>
#include <sys/vnode.h>
#include <miscfs/specfs/specdev.h>
#include <sys/malloc.h>
#include <sys/dirent.h>
#include <sys/signalvar.h>
#include <sys/ubc.h>
#include <sys/utfconv.h>
#include <sys/attr.h>
#include <sys/namei.h>

#include <machine/spl.h>

#include "bpb.h"
#include "direntry.h"
#include "denode.h"
#include "msdosfsmount.h"
#include "fat.h"
#include "msdosfs_lockf.h"

#define	DOS_FILESIZE_MAX	0xffffffff

#define	GENERIC_DIRSIZ(dp) \
    ((sizeof (struct dirent) - (MAXNAMLEN+1)) + (((dp)->d_namlen+1 + 3) &~ 3))

extern int groupmember(gid_t gid, ucred_t cred);

/*
 * Prototypes for MSDOSFS vnode operations
 */
static int msdosfs_create __P((struct vnop_create_args *));
static int msdosfs_mknod __P((struct vnop_mknod_args *));
static int msdosfs_close __P((struct vnop_close_args *));
static int msdosfs_access __P((struct vnop_access_args *));
static int msdosfs_getattr __P((struct vnop_getattr_args *));
static int msdosfs_setattr __P((struct vnop_setattr_args *));
static int msdosfs_read __P((struct vnop_read_args *));
static int msdosfs_write __P((struct vnop_write_args *));
static int msdosfs_pagein __P((struct vnop_pagein_args *));
static int msdosfs_fsync __P((struct vnop_fsync_args *));
static int msdosfs_remove __P((struct vnop_remove_args *));
static int msdosfs_rename __P((struct vnop_rename_args *));
static int msdosfs_mkdir __P((struct vnop_mkdir_args *));
static int msdosfs_rmdir __P((struct vnop_rmdir_args *));
static int msdosfs_readdir __P((struct vnop_readdir_args *));
static int msdosfs_strategy __P((struct vnop_strategy_args *));
static int msdosfs_pathconf __P((struct vnop_pathconf_args *ap));
static int msdosfs_advlock(struct vnop_advlock_args *ap);

int msdosfs_getattrlist(struct vnop_getattrlist_args *ap);	/* In msdosfs_attrlist.c */
int msdosfs_setattrlist(struct vnop_setattrlist_args *ap);	/* In msdosfs_attrlist.c */

/*
 * Some general notes:
 *
 * In the ufs filesystem the inodes, superblocks, and indirect blocks are
 * read/written using the vnode for the filesystem. Blocks that represent
 * the contents of a file are read/written using the vnode for the file
 * (including directories when they are read/written as files). This
 * presents problems for the dos filesystem because data that should be in
 * an inode (if dos had them) resides in the directory itself.  Since we
 * must update directory entries without the benefit of having the vnode
 * for the directory we must use the vnode for the filesystem.  This means
 * that when a directory is actually read/written (via read, write, or
 * readdir, or seek) we must use the vnode for the filesystem instead of
 * the vnode for the directory as would happen in ufs. This is to insure we
 * retreive the correct block from the buffer cache since the hash value is
 * based upon the vnode address and the desired block number.
 */


/*
 * Return the owning user ID for a given volume.  If the volume was mounted
 * with "unknown permissions", then the user ID making the request is the
 * owner.  This is the "everyone is an owner" model.
 */
__private_extern__ uid_t
get_pmuid(struct msdosfsmount *pmp, uid_t current_user)
{
	if (vfs_flags(pmp->pm_mountp) & MNT_UNKNOWNPERMISSIONS)
		return current_user;
	else
		return pmp->pm_uid;
}

/*
 * Create a regular file.
 */
static int
msdosfs_create(ap)
	struct vnop_create_args /* {
		struct vnodeop_desc *a_desc;
		vnode_t a_dvp;
		vnode_t *a_vpp;
		struct componentname *a_cnp;
		struct vattr *a_vap;
		vfs_context_t a_context;
	} */ *ap;
{
	vnode_t dvp = ap->a_dvp;
	struct denode *pdep = VTODE(dvp);
	struct componentname *cnp = ap->a_cnp;
	vfs_context_t context = ap->a_context;
	struct denode ndirent;
	struct denode *dep;
	struct timespec ts;
	int error;
	u_long va_flags;

	/*
	 * If this is the root directory and there is no space left we
	 * can't do anything.  This is because the root directory can not
	 * change size.  (FAT12 and FAT16 only)
	 *
	 * On FAT32, we can grow the root directory, and de_StartCluster
	 * will be the actual cluster number of the root directory.
	 */
	if (pdep->de_StartCluster == MSDOSFSROOT
	    && pdep->de_fndoffset >= pdep->de_FileSize)
	{
		error = ENOSPC;
		goto exit;
	}

	/*
	 * Create a directory entry for the file, then call createde() to
	 * have it installed. NOTE: DOS files are always executable.
	 * The supplied mode is ignored (DOS doesn't store mode, so
	 * all files on a volume have a constant mode).  The immutable
	 * flag is used to set DOS's read-only attribute.
	 */
	bzero(&ndirent, sizeof(ndirent));
	error = uniqdosname(pdep, cnp, ndirent.de_Name, &ndirent.de_LowerCase, context);
	if (error)
		goto exit;

	// Set read-only attribute if one of the immutable bits is set.
	// Always set the "needs archive" attribute on newly created files.
	va_flags = ap->a_vap->va_flags;
	if (va_flags != VNOVAL && (va_flags & (SF_IMMUTABLE | UF_IMMUTABLE)) != 0)
		ndirent.de_Attributes = ATTR_ARCHIVE | ATTR_READONLY;
	else
		ndirent.de_Attributes = ATTR_ARCHIVE;
        
	ndirent.de_StartCluster = 0;
	ndirent.de_FileSize = 0;
	ndirent.de_dev = pdep->de_dev;
	ndirent.de_devvp = pdep->de_devvp;
	ndirent.de_pmp = pdep->de_pmp;
	ndirent.de_flag = DE_ACCESS | DE_CREATE | DE_UPDATE;
	getnanotime(&ts);
	DETIMES(&ndirent, &ts, &ts, &ts);
	error = createde(&ndirent, pdep, &dep, cnp, context);
	if (error == 0)
		*ap->a_vpp = DETOV(dep);

exit:
	return (error);
}

static int
msdosfs_mknod(ap)
	struct vnop_mknod_args /* {
		vnode_t a_dvp;
		vnode_t *a_vpp;
		struct componentname *a_cnp;
		struct vattr *a_vap;
		vfs_context_t a_context;
	} */ *ap;
{
	/* We don't support special files */
	return EINVAL;
}

static int
msdosfs_open(ap)
	struct vnop_open_args /* {
		vnode_t a_vp;
		int a_mode;
		vfs_context_t a_context;
	} */ *ap;
{
	return 0;
}

static int
msdosfs_close(ap)
	struct vnop_close_args /* {
		vnode_t a_vp;
		int a_fflag;
		vfs_context_t a_context;
	} */ *ap;
{
	vnode_t vp = ap->a_vp;
	struct denode *dep = VTODE(vp);
	struct timespec ts;

/*	if (vp->v_usecount > (UBCINFOEXISTS(vp) ? 2 : 1)) { */
	getnanotime(&ts);
	DETIMES(dep, &ts, &ts, &ts);
/*	} */
	
	/*¥ Should we cluster_push(vp, IO_CLOSE) here? */
	return 0;
}

static int
msdosfs_access(ap)
	struct vnop_access_args /* {
		vnode_t a_vp;
		int a_mode;
		vfs_context_t a_context;
	} */ *ap;
{
	vnode_t vp = ap->a_vp;
	ucred_t cred = vfs_context_ucred(ap->a_context);
	mode_t mode = ap->a_mode;
	struct denode *dep = VTODE(vp);
	struct msdosfsmount *pmp = dep->de_pmp;
	mode_t mask, file_mode;
	gid_t *gp;
	int i;
	int error = 0;

	file_mode = ALLPERMS & pmp->pm_mask;
    
	/*
	 * Disallow write attempts on read-only file systems;
	 * unless the file is a socket, fifo, or a block or
	 * character device resident on the file system.
	 */
	if (mode & VWRITE) {
		switch (vnode_vtype(vp)) {
		case VDIR:
		case VLNK:
		case VREG:
			if (vnode_vfsisrdonly(vp))
			{
				error = EROFS;
				goto exit;
			}
			break;
		default:
			break;
		}
	}

    /* If ATTR_READONLY (immutable) is set, nobody gets write access. */
    if ((mode & VWRITE) && (dep->de_Attributes & ATTR_READONLY))
    {
    	error = EPERM;
    	goto exit;
	}
	
	/* User id 0 always gets access. */
	if (cred->cr_uid == 0)
		goto exit;

	mask = 0;

	/* Otherwise, check the owner. */
	/* And allow for unkown permissions (everyone is the owner) */
	if (cred->cr_uid == get_pmuid(pmp, cred->cr_uid)) {
		if (mode & VEXEC)
			mask |= S_IXUSR;
		if (mode & VREAD)
			mask |= S_IRUSR;
		if (mode & VWRITE)
			mask |= S_IWUSR;
		error = (file_mode & mask) == mask ? 0 : EACCES;
		goto exit;
	}

	/* Otherwise, check the groups. */
	for (i = 0, gp = cred->cr_groups; i < cred->cr_ngroups; i++, gp++)
		if (pmp->pm_gid == *gp) {
			if (mode & VEXEC)
				mask |= S_IXGRP;
			if (mode & VREAD)
				mask |= S_IRGRP;
			if (mode & VWRITE)
				mask |= S_IWGRP;
			error = (file_mode & mask) == mask ? 0 : EACCES;
			goto exit;
		}

	/* Otherwise, check everyone else. */
	if (mode & VEXEC)
		mask |= S_IXOTH;
	if (mode & VREAD)
		mask |= S_IROTH;
	if (mode & VWRITE)
		mask |= S_IWOTH;
	error = (file_mode & mask) == mask ? 0 : EACCES;

exit:
	return error;
}

static int
msdosfs_getattr(ap)
	struct vnop_getattr_args /* {
		vnode_t a_vp;
		struct vnode_vattr *a_vap;
		vfs_context_t a_context;
	} */ *ap;
{
	vnode_t vp = ap->a_vp;
	struct denode *dep = VTODE(vp);
	struct msdosfsmount *pmp = dep->de_pmp;
	struct vnode_vattr *vap = ap->a_vap;
	struct timespec ts;

	getnanotime(&ts);
	DETIMES(dep, &ts, &ts, &ts);
	vap->va_fsid = dep->de_dev;

	vap->va_fileid = defileid(dep);
	vap->va_mode = ALLPERMS & pmp->pm_mask;
	vap->va_uid = get_pmuid(pmp, vfs_context_ucred(ap->a_context)->cr_uid);
	vap->va_gid = pmp->pm_gid;
	vap->va_nlink = 1;
	vap->va_rdev = 0;
	vap->va_size = dep->de_FileSize;
	dos2unixtime(dep->de_MDate, dep->de_MTime, 0, &vap->va_mtime);
	dos2unixtime(dep->de_ADate, 0, 0, &vap->va_atime);
	dos2unixtime(dep->de_CDate, dep->de_CTime, dep->de_CHun, &vap->va_ctime);
	vap->va_flags = 0;
	if ((dep->de_Attributes & ATTR_ARCHIVE) == 0)	// DOS: flag set means "needs to be archived"
		vap->va_flags |= SF_ARCHIVED;				// BSD: flag set means "has been archived"
    if (dep->de_Attributes & ATTR_READONLY)			// DOS read-only becomes user immutable
        vap->va_flags |= UF_IMMUTABLE;
	vap->va_gen = 0;
	vap->va_blocksize = pmp->pm_bpcluster;
	vap->va_bytes =
	    (dep->de_FileSize + pmp->pm_crbomask) & ~pmp->pm_crbomask;
	vap->va_type = vnode_vtype(vp);
	vap->va_filerev = dep->de_modrev;
	
	return 0;
}

static int
msdosfs_setattr(ap)
	struct vnop_setattr_args /* {
		vnode_t a_vp;
		struct vnode_vattr *a_vap;
		vfs_context_t a_context;
	} */ *ap;
{
	vnode_t vp = ap->a_vp;
	struct denode *dep = VTODE(ap->a_vp);
	struct vnode_vattr *vap = ap->a_vap;
	vfs_context_t context = ap->a_context;
	ucred_t cred = vfs_context_ucred(context);
	proc_t p = vfs_context_proc(context);
	struct msdosfsmount *pmp = dep->de_pmp;
	uid_t pmuid;	// Effective owner during this call
	int error;

	/*
	 * Check for unsettable attributes.
	 */
	if ((vap->va_type != VNON) || (vap->va_nlink != VNOVAL) ||
	    (vap->va_fsid != VNOVAL) || (vap->va_fileid != VNOVAL) ||
	    (vap->va_blocksize != VNOVAL) || (vap->va_rdev != VNOVAL) ||
	    (vap->va_bytes != VNOVAL) || (vap->va_gen != VNOVAL))
	{
		error = EINVAL;
		goto exit;
	}

	pmuid = get_pmuid(pmp, cred->cr_uid);
	
    if (vap->va_flags != VNOVAL) {
		if (vnode_vfsisrdonly(vp))
		{
			error = EROFS;
			goto exit;
		}
		
		/*¥ What's the right way to test for "change permissions" permission? */
		if (cred->cr_uid != pmuid &&
		    (error = suser(cred, &p->p_acflag)))
		{
			goto exit;
		}
		
		/*
		 * We are very inconsistent about handling unsupported
		 * attributes.  We ignored the access time and the
		 * mode bits.  We were strict for the other attributes.
		 *
		 * Here we are strict, stricter than ufs in not allowing
		 * users to attempt to set SF_SETTABLE bits or anyone to
		 * set unsupported bits.  However, we ignore attempts to
		 * set ATTR_ARCHIVE for directories `cp -pr' from a more
		 * sensible file system attempts it a lot.
		 */
		/*¥ What's the right way to test for "set super user flags" permission? */
		if (cred->cr_uid != 0 && (vap->va_flags & SF_SETTABLE))
		{
			error = EPERM;
			goto exit;
		}
        
		if (vap->va_flags & ~(SF_ARCHIVED | SF_IMMUTABLE | UF_IMMUTABLE))
		{
			error = EOPNOTSUPP;
			goto exit;
		}
            
		if (vap->va_flags & SF_ARCHIVED)
			dep->de_Attributes &= ~ATTR_ARCHIVE;
		else if (!(dep->de_Attributes & ATTR_DIRECTORY))
			dep->de_Attributes |= ATTR_ARCHIVE;

        /* For files, copy the immutable flag to read-only attribute. */
        /* Ignore immutable bit for directories. */
        if (!(dep->de_Attributes & ATTR_DIRECTORY))
        {
            if (vap->va_flags & (SF_IMMUTABLE | UF_IMMUTABLE))
                dep->de_Attributes |= ATTR_READONLY;
            else
                dep->de_Attributes &= ~ATTR_READONLY;
        }
        
		dep->de_flag |= DE_MODIFIED;
	}

	if (vap->va_uid != (uid_t)VNOVAL || vap->va_gid != (gid_t)VNOVAL) {
		uid_t uid;
		gid_t gid;

		if (vnode_vfsisrdonly(vp))
		{
			error = EROFS;
			goto exit;
		}
		uid = vap->va_uid;
		if (uid == (uid_t)VNOVAL)
			uid = pmuid;
		gid = vap->va_gid;
		if (gid == (gid_t)VNOVAL)
			gid = pmp->pm_gid;
		if ((cred->cr_uid != pmuid || uid != pmuid ||
		    (gid != pmp->pm_gid && !groupmember(gid, cred))) &&
		    (error = suser(cred, &p->p_acflag)))
		{
			goto exit;
		}
		if (uid != pmuid || gid != pmp->pm_gid)
		{
			error = EINVAL;
			goto exit;
		}
	}

	if (vap->va_size != VNOVAL) {
		/*
		 * Disallow write attempts on read-only file systems;
		 * unless the file is a socket, fifo, or a block or
		 * character device resident on the file system.
		 */
		switch (vnode_vtype(vp)) {
		case VDIR:
			error = EISDIR;
			goto exit;
			/* NOT REACHED */
		case VLNK:
		case VREG:
			if (vnode_vfsisrdonly(vp))
			{
				error = EROFS;
				goto exit;
			}
			break;
		default:
			break;
		}
		error = detrunc(dep, vap->va_size, 0, context);
		if (error) goto exit;
	}
	if (vap->va_atime.tv_sec != VNOVAL || vap->va_mtime.tv_sec != VNOVAL) {
		if (vnode_vfsisrdonly(vp))
		{
			error = EROFS;
			goto exit;
		}
		if (cred->cr_uid != pmuid &&
		    (error = suser(cred, &p->p_acflag)) &&
		    ((vap->va_vaflags & VA_UTIMES_NULL) == 0 ||
		    (error = vn_access(ap->a_vp, VWRITE, ap->a_context))))
		{
			goto exit;
		}
		if ( !vnode_isdir(vp)) {
			if (vap->va_atime.tv_sec != VNOVAL)
				unix2dostime(&vap->va_atime, &dep->de_ADate, NULL, NULL);
			if (vap->va_mtime.tv_sec != VNOVAL)
				unix2dostime(&vap->va_mtime, &dep->de_MDate, &dep->de_MTime, NULL);
			dep->de_Attributes |= ATTR_ARCHIVE;
			dep->de_flag |= DE_MODIFIED;
		}
	}
	/*
	 * DOS files only have the ability to have their writability
	 * attribute set, so we use the owner write bit to set the readonly
	 * attribute.
	 */
    /*
     * DOS doesn't store mode flags.  The code below only serves to
     * check for some error conditions where you wouldn't be able
     * to set the mode anyway.
     */
	if (vap->va_mode != (mode_t)VNOVAL) {
		if (vnode_vfsisrdonly(vp))
		{
			error = EROFS;
			goto exit;
		}
		if (cred->cr_uid != pmuid &&
		    (error = suser(cred, &p->p_acflag)))
		{
			goto exit;
		}
	}

	error = deupdat(dep, 1, context);

exit:
	return error;
}

static int
msdosfs_read(ap)
	struct vnop_read_args /* {
		vnode_t a_vp;
		struct uio *a_uio;
		int a_ioflag;
		vfs_context_t a_context;
	} */ *ap;
{
	int error = 0;
	int orig_resid;
	vnode_t vp = ap->a_vp;
	struct uio *uio = ap->a_uio;
	vfs_context_t context = ap->a_context;
	struct denode *dep = VTODE(vp);
	struct msdosfsmount *pmp = dep->de_pmp;

	if (uio->uio_offset < 0)
		return EINVAL;

	if ((u_int64_t)uio->uio_offset > DOS_FILESIZE_MAX)
		return 0;

	/* If they didn't ask for any data, then we are done. */
	orig_resid = uio->uio_resid;
	if (orig_resid <= 0)
		return 0;

	if (vnode_isreg(vp)) {
		error = cluster_read(vp, uio, (off_t)dep->de_FileSize, 0);
		if (error == 0)
			dep->de_flag |= DE_ACCESS;
	}
	else
	{
		u_long blsize;
		u_int n;
		u_long diff;
		u_long on;
		daddr64_t lbn;
		buf_t bp;

		/* The following code is only used for reading directories */
		
		do {
			if (uio->uio_offset >= dep->de_FileSize)
				break;
			lbn = de_cluster(pmp, uio->uio_offset);
			/*
			 * If we are operating on a directory file then be sure to
			 * do i/o with the vnode for the filesystem instead of the
			 * vnode for the directory.
			 */
			/* convert cluster # to block # */
			error = pcbmap(dep, lbn, 1, &lbn, NULL, &blsize, context);
			if (error == E2BIG) {
				error = EINVAL;
				break;
			} else if (error)
				break;
			error = (int)buf_meta_bread(pmp->pm_devvp, lbn, blsize, vfs_context_ucred(context), &bp);
			if (error) {
				buf_brelse(bp);
				break;
			}
			on = uio->uio_offset & pmp->pm_crbomask;
			diff = pmp->pm_bpcluster - on;
			n = diff > uio->uio_resid ? uio->uio_resid : diff;
			diff = dep->de_FileSize - uio->uio_offset;
			if (diff < n)
				n = diff;
			diff = blsize - buf_resid(bp);
			if (diff < n)
				n = diff;
			error = uiomove((char *)buf_dataptr(bp) + on, (int) n, uio);
			buf_brelse(bp);
		} while (error == 0 && uio->uio_resid > 0 && n != 0);
	}

	return error;
}

/*
 * Write data to a file or directory.
 */
static int
msdosfs_write(ap)
	struct vnop_write_args /* {
		vnode_t a_vp;
		struct uio *a_uio;
		int a_ioflag;
		vfs_context_t a_context;
	} */ *ap;
{
	int error;
	vnode_t vp = ap->a_vp;
	struct uio *uio = ap->a_uio;
	int ioflag = ap->a_ioflag;
	vfs_context_t context = ap->a_context;
	proc_t p = vfs_context_proc(context);
	struct denode *dep = VTODE(vp);
	struct msdosfsmount *pmp = dep->de_pmp;
	off_t resid;
	off_t zero_off;
	u_int32_t osize;
	u_int32_t count;
	u_int32_t lastcn;
	u_int32_t filesize;
	int   lflag;

	switch (vnode_vtype(vp)) {
	case VREG:
		break;
	case VDIR:
		return EISDIR;
	default:
		panic("msdosfs_write: bad file type");
		return EINVAL;
	}

	if (ioflag & IO_APPEND)
		uio->uio_offset = dep->de_FileSize;

	if (uio->uio_offset < 0)
		return EFBIG;

	if (uio->uio_resid == 0)
		return 0;

	/*
	 * If they've exceeded their filesize limit, tell them about it.
	 */
	if (p &&
	    ((u_int64_t)uio->uio_offset + uio->uio_resid >
	    p->p_rlimit[RLIMIT_FSIZE].rlim_cur)) {
		psignal(p, SIGXFSZ);
		return EFBIG;
	}

	if ((u_int64_t)uio->uio_offset + uio->uio_resid > DOS_FILESIZE_MAX)
		return EFBIG;

	/*
	 * If the offset we are starting the write at is beyond the end of
	 * the file, then they've done a seek.  Unix filesystems allow
	 * files with holes in them, DOS doesn't so we must fill the hole
	 * with zeroed blocks.
	 */
	/*
	 * Remember some values in case the write fails.
	 */
	resid = uio->uio_resid;
	osize = dep->de_FileSize;

	/*
	 * If we write beyond the end of the file, extend it to its ultimate
	 * size ahead of the time to hopefully get a contiguous area.
	 */
    if (uio->uio_offset + resid > osize) {
        count = de_clcount(pmp, uio->uio_offset + resid) -
        		de_clcount(pmp, osize);
        error = extendfile(dep, count, context);
        if (error &&  (error != ENOSPC || (ioflag & IO_UNIT)))
            goto errexit;
        lastcn = dep->de_fc[FC_LASTFC].fc_frcn;
    } else
		lastcn = de_clcount(pmp, osize) - 1;

	if (uio->uio_offset + resid > osize)
		filesize = uio->uio_offset + resid;
	else
		filesize = osize;

	lflag = (ioflag & IO_SYNC);

	if (uio->uio_offset > osize) {
		zero_off = osize;
		lflag   |= IO_HEADZEROFILL;
	} else
		zero_off = 0;
	
	/*
	 * if the write starts beyond the current EOF then
	 * we we'll zero fill from the current EOF to where the write begins
	 */
	error = cluster_write(vp, uio, (off_t)osize, (off_t)filesize,
				(off_t)zero_off,
				(off_t)0, lflag);
	
	if (uio->uio_offset > dep->de_FileSize) {
		dep->de_FileSize = uio->uio_offset;
		ubc_setsize(vp, (off_t)dep->de_FileSize);
	}

	if (resid > uio->uio_resid)
		dep->de_flag |= DE_UPDATE;
	
	/*
	 * If the write failed and they want us to, truncate the file back
	 * to the size it was before the write was attempted.
	 */
errexit:
	if (error) {
		if (ioflag & IO_UNIT) {
			detrunc(dep, osize, ioflag & IO_SYNC, context);
			uio->uio_offset -= resid - uio->uio_resid;
			uio->uio_resid = resid;
		} else {
			detrunc(dep, dep->de_FileSize, ioflag & IO_SYNC, context);
			if (uio->uio_resid != resid)
				error = 0;
		}
	} else if (ioflag & IO_SYNC)
		error = deupdat(dep, 1, context);

	return error;
}

static int
msdosfs_pagein(ap)
	struct vnop_pagein_args /* {
		vnode_t a_vp;
		upl_t a_pl;
		vm_offset_t a_pl_offset;
		off_t a_f_offset;
		size_t a_size;
		int a_flags;
		vfs_context_t a_context;
	} */ *ap;
{
	vnode_t vp = ap->a_vp;
	struct denode *dep = VTODE(vp);
	int error;
	
	error = cluster_pagein(vp, ap->a_pl, ap->a_pl_offset, ap->a_f_offset,
				ap->a_size, (off_t)dep->de_FileSize,
				ap->a_flags);
	
	return error;
}

static int
msdosfs_pageout(ap)
	struct vnop_pageout_args /* {
		vnode_t a_vp;
		upl_t a_pl;
		vm_offset_t a_pl_offset;
		off_t a_f_offset;
		size_t a_size;
		int a_flags;
		vfs_context_t a_context;
	} */ *ap;
{
	vnode_t vp = ap->a_vp;
	struct denode *dep = VTODE(vp);
	int error;

	error = cluster_pageout(vp, ap->a_pl, ap->a_pl_offset, ap->a_f_offset,
				ap->a_size, (off_t)dep->de_FileSize,
				ap->a_flags);

	return error;
}

__private_extern__ int
msdosfs_fsync_internal(vnode_t vp, int sync, vfs_context_t context)
{
	/*
	 * First of all, write out any clusters.
	 */
	cluster_push(vp, 0);

	/*
	 * Flush all dirty buffers associated with a vnode.
	 */
	buf_flushdirtyblks(vp, sync, 0, (char *)"msdosfs_fsync_internal");

	return deupdat(VTODE(vp), sync, context);
}

/*
 * Flush the blocks of a file to disk.
 *
 * This function is worthless for vnodes that represent directories. Maybe we
 * could just do a sync if they try an fsync on a directory file.
 */
static int
msdosfs_fsync(ap)
	struct vnop_fsync_args /* {
		vnode_t a_vp;
		int a_waitfor;
		vfs_context_t a_context;
	} */ *ap;
{
	int error;
	vnode_t vp = ap->a_vp;
	
	error = msdosfs_fsync_internal(vp, ap->a_waitfor == MNT_WAIT, ap->a_context);

	return error;
}

/*
 * Remove (unlink) a file.
 *
 * On entry, vp has been suspended, so there are no pending
 * calls using it.
 *
 */
static int
msdosfs_remove(ap)
	struct vnop_remove_args /* {
		vnode_t a_dvp;
		vnode_t a_vp;
		struct componentname *a_cnp;
		vfs_context_t a_context;
	} */ *ap;
{
    vnode_t vp = ap->a_vp;
    vnode_t dvp = ap->a_dvp;
    struct denode *dep = VTODE(vp);
    struct denode *ddep = VTODE(dvp);
    off_t eof = 0;
    int error;

    if (vnode_isdir(ap->a_vp))
    {
        error = EPERM;
        goto exit;
    }

    /* Make sure the file isn't read-only */
    if (dep->de_Attributes & ATTR_READONLY)
    {
        error = EPERM;
        goto exit;
    }

#if 0
	/*¥ Check the NODELETEBUSY flag in a_cnp->cn_flags? */
	/* This is new, never tested code */
	if ((cnp->cn_flags & NODELETEBUSY) && vnode_isinuse(vp, 1))
	{
		error = EBUSY;
		goto exit;
	}
#endif

	/*¥ Call cache_purge(vp)? */
	
	eof = dep->de_FileSize;
    error = removede(ddep, dep, ap->a_context);

exit:

	return error;
}

/*
 * Renames on files require moving the denode to a new hash queue since the
 * denode's parent is used to compute which hash queue to put the file
 * in. Unless it is a rename in place.  For example "mv a b".
 *
 * What follows is the basic algorithm:
 *
 * if (file move) {
 *	if (dest file exists) {
 *		remove dest file
 *	}
 *	if (dest and src in same directory) {
 *		rewrite name in existing directory slot
 *	} else {
 *		write new entry in dest directory
 *		update offset and dirclust in denode
 *		move denode to new hash chain
 *		clear old directory entry
 *	}
 * } else {
 *	directory move
 *	if (dest directory exists) {
 *		if (dest is not empty) {
 *			return ENOTEMPTY
 *		}
 *		remove dest directory
 *	}
 *	if (dest and src in same directory) {
 *		rewrite name in existing entry
 *	} else {
 *		be sure dest is not a child of src directory
 *		write entry in dest directory
 *		update "." and ".." in moved directory
 *		clear old directory entry for moved directory
 *	}
 * }
 *
 * Locking:
 *	On entry, VFS has locked all of the vnodes in an
 *	arbitrary, but consistent, order.  For example,
 *	it may lock the parent with the smallest vnode pointer,
 *	then its child, then the parent with the larger vnode
 *	pointer, then its child.  These locks prevent either
 *	the source or destination (if any) from changing, and
 *	prevents changes to either the source or destination
 *	parent directories.
 *
 *	Traditionally, one of the hardest parts about rename's
 *	locking is when the source is a directory, and we
 *	have to verify that the destination parent isn't a
 *	descendant of the source.  We need to walk up the
 *	directory hierarchy from the destination parent up to
 *	the root.  If any of those directories is the source,
 *	the operation is invalid.  But walking up the hierarchy
 *	violates the top-down lock order; to avoid deadlock, we
 *	must not have two nodes locked at the same time.
 *
 *	But there is a solution: a mutex on rename operations for
 *	the entire volume (that is, only one rename at a time
 *	per volume).  During the walk up the hierarchy, we must
 *	prevent any change to the hierarchy that could cause the
 *	destination parent to become or stop being an ancestor of
 *	the source.  Any operation on a file can't affect the
 *	ancestry of directories.  Directory create operations can't
 *	affect the ancestry of pre-existing directories.
 *	Directory delete operations outside the ancestry path
 *	don't matter, and deletes within the ancestry path will
 *	fail as long as the directories remain locked (the delete
 *	will fail because the directory is locked, or not empty, or
 *	both).  The only operation that can affect the ancestry is
 *	other rename operations on the same volume (and even then,
 *	only if the source parent is not the destination parent).
 *
 *	It is sufficient if the rename mutex is taken only
 *	when the source parent is not the destination parent.
 */
static int
msdosfs_rename(ap)
	struct vnop_rename_args /* {
		vnode_t a_fdvp;
		vnode_t a_fvp;
		struct componentname *a_fcnp;
		vnode_t a_tdvp;
		vnode_t a_tvp;
		struct componentname *a_tcnp;
		vfs_context_t a_context;
	} */ *ap;
{
	vnode_t tdvp = ap->a_tdvp;
	vnode_t fvp = ap->a_fvp;
	vnode_t fdvp = ap->a_fdvp;
	vnode_t tvp = ap->a_tvp;
	struct componentname *tcnp = ap->a_tcnp;
	struct componentname *fcnp = ap->a_fcnp;
	vfs_context_t context = ap->a_context;
	u_char toname[11], oldname[11];
	u_long to_diroffset;
	u_int32_t from_offset;
	u_int8_t new_deLowerCase;	/* deLowerCase corresponding to toname */
	int doingdirectory = 0, newparent = 0;
    int change_case;
	int error;
	u_long cn;
	daddr64_t bn = 0;
	struct denode *fddep;	/* from file's parent directory	 */
	struct denode *fdep;	/* from file or directory	 */
	struct denode *tddep;	/* to file's parent directory	 */
	struct denode *tdep;	/* to file or directory		 */
	struct msdosfsmount *pmp;
	struct buf *bp;
	
	fddep = VTODE(ap->a_fdvp);
	fdep = VTODE(ap->a_fvp);
	tddep = VTODE(ap->a_tdvp);
	tdep = tvp ? VTODE(tvp) : NULL;
	pmp = fddep->de_pmp;

	pmp = VFSTOMSDOSFS(vnode_mount(fdvp));

	/*
	 * Check for cross-device rename.
	 *¥Should VFS do this?
	 */
	if ((vnode_mount(fvp) != vnode_mount(tdvp)) ||
	    (tvp && (vnode_mount(fvp) != vnode_mount(tvp))))
	{
		return EXDEV;
	}

	/*
	 * If source and dest are the same, then it is a rename
	 * that may be changing the upper/lower case of the name.
	 */
    change_case = 0;
	if (tvp == fvp) {
        /* Pretend the destination doesn't exist. */
        tvp = NULL;
        
        /* Remember we're changing case, so we can skip uniqdosname() */
        change_case = 1;
        
        /*
         * Lookup left tdvp's de_fndoffset and de_fndcnt pointing at
         * the existing name (fvp).  We need to find some unused space
         * in the directory so createde() can create the new name without
         * overwriting the old name.  As a side effect, we get the
         * updated lower case flags that the new entry will need.
         *
         * Note: we can't necessarily re-use the existing directory entry
         * because the old name might not have a long name (all upper case
         * or all lower case with LC_xx flags set) and the new name
         * needs a long name (which takes up more entries).
         */
        error = findslots(tddep, ap->a_tcnp, &new_deLowerCase, context);
        if (error)
            goto exit;
	}

    /* Make sure the destination vnode (if any) can be deleted */
    if (tdep && (tdep->de_Attributes & ATTR_READONLY))
    {
        error = EPERM;
        goto exit;
    }
    
    /* Make sure the source vnode can be changed */
    if (fdep->de_Attributes & ATTR_READONLY)
    {
        error = EPERM;
        goto exit;
    }
    
	/*
	 * Be sure we are not renaming ".", "..", or an alias of ".". This
	 * leads to a crippled directory tree.  It's pretty tough to do a
	 * "ls" or "pwd" with the "." directory entry missing, and "cd .."
	 * doesn't work if the ".." entry is missing.
	 */
	if (fdep->de_Attributes & ATTR_DIRECTORY) {
		/*
		 * Avoid ".", "..", and aliases of "." for obvious reasons.
		 *¥Do we need the explicit test for name == "."?  Wouldn't
		 * the fddep==fdep check find the same thing?
		 */
		if (/* (fcnp->cn_namelen == 1 && fcnp->cn_nameptr[0] == '.') || */
		    fddep == fdep ||
		    (fcnp->cn_flags & ISDOTDOT) ||
		    (tcnp->cn_flags & ISDOTDOT)) {
			error = EINVAL;
			goto exit;
		}
		doingdirectory = 1;
	}

	/*
	 * Remember direntry place to use for destination
	 *¥Assumes VFS did the lookup on the destination
	 * after the lookup of the source.
	 */
	to_diroffset = tddep->de_fndoffset;

	/*
	 * If ".." must be changed (ie the directory gets a new
	 * parent) then the source directory must not be in the
	 * directory heirarchy above the target, as this would
	 * orphan everything below the source directory. Also
	 * the user must have write permission in the source so
	 * as to be able to change "..".
	 */
	if (VTODE(fdvp)->de_StartCluster != VTODE(tdvp)->de_StartCluster)
		newparent = 1;
	if (doingdirectory && newparent) {
		error = vn_access(fvp, VWRITE, ap->a_context);
		if (error) goto exit;

		error = doscheckpath(fdep, tddep, context);
		if (error) goto exit;
	}

	/*
	 * Remember the offset of fdep within fddep.
	 */
	if (doingdirectory)
	{
		error = msdosfs_lookupdir(fddep, fdep, &from_offset, context);
		if (error) goto exit;
	}
	else
		from_offset = fdep->de_diroffset;

	if (tvp != NULL) {
		/*
		 * Target must be empty if a directory and have no links
		 * to it. Also, ensure source and target are compatible
		 * (both directories, or both not directories).
		 *
		 *¥ This doesn't work for renaming non-empty directories
		 * to change the case of the name.
		 */
		if (tdep->de_Attributes & ATTR_DIRECTORY) {
			if (!dosdirempty(tdep, context)) {
				error = ENOTEMPTY;
				goto exit;
			}
			if (!doingdirectory) {
				error = ENOTDIR;
				goto exit;
			}
			cache_purge(tdvp);
		} else if (doingdirectory) {
			error = EISDIR;
			goto exit;
		}
		error = removede(tddep, tdep, context);
		if (error) goto exit;
	}

	/*
	 * Convert the filename in tcnp into a dos filename. We copy this
	 * into the denode and directory entry for the destination
	 * file/directory.
     *
     * NOTE: uniqdosname also makes sure that the short name does not
     * already exist in the destination directory.  When changing case,
     * that short name already exists (for the source object), so we
     * have to skip the call to uniqdosname.  If we're changing case,
     * we keep the short name as-is, and the call to findslots()
     * set up new_deLowerCase for the new dir entry.
	 */
	if (!change_case) {
        error = uniqdosname(tddep, tcnp, toname, &new_deLowerCase, context);
        /*¥ What if we get an error and we already deleted the target? */
        if (error) goto exit;
    }

    if (change_case) {
    	/*
         * Since we're just changing case, the short name should stay
         * the same, and we couldn't call uniqdosname() above to set
         * up "toname".  So, set it up now.
         *
         * For a file, the short name is already in the denode, and
         * we can just copy it.  But for a directory, the denode actually
         * points at the "." entry in the directory, and the name stored
         * is ".".  So, for a directory, we have to go read in the
         * short name entry as it exists in the parent directory.
         */
        if (doingdirectory)
        {
        	u_long blsize;
        	struct direntry *direntp;
        	
        	/* Read in fdep's entry in fddep */
        	error = pcbmap(fddep, de_cluster(pmp, from_offset), 1, &bn, NULL, &blsize, context);
        	if (error) goto exit;
        	/*¥ On error, we had already removed tdep */
        	
        	error = (int)buf_meta_bread(pmp->pm_devvp, bn, blsize, vfs_context_ucred(context), &bp);
        	if (error) {
        		buf_brelse(bp);
        		/*¥ On error, we had already removed tdep */
        		goto exit;
        	}
        	
        	/* Copy the name from the entry in fddep */
        	direntp = bptoep(pmp, bp, from_offset);
        	bcopy(direntp->deName, toname, 11);
        	buf_brelse(bp);
        }
        else
        {
			bcopy(fdep->de_Name, toname, 11);
		}
	}

	/*
	 * First write a new entry in the destination
	 * directory and mark the entry in the source directory
	 * as deleted.  Then move the denode to the correct hash
	 * chain for its new location in the filesystem.  And, if
	 * we moved a directory, then update its .. entry to point
	 * to the new parent directory.
	 *
	 * The name in the denode is updated for files.  For
	 * directories, the denode points at the "." entry in
	 * the directory, so temporarily change the name in
	 * the denode, and restore it; otherwise the "." entry
	 * may be overwritten.
	 */
	bcopy(fdep->de_Name, oldname, 11);
	bcopy(toname, fdep->de_Name, 11);	/* update denode */
	fdep->de_LowerCase = new_deLowerCase;
	error = createde(fdep, tddep, NULL, tcnp, context);
	if (error) {
		bcopy(oldname, fdep->de_Name, 11);
		/*¥ What if we already deleted the target? */
		goto exit;
	}
	/* For directories, restore the name to "." */
	if (doingdirectory)
		bcopy(oldname, fdep->de_Name, 11);	/* Change it back to "." */
	fdep->de_refcnt++;					/* Compensates for decrement in removede */
	fddep->de_fndoffset = from_offset;	/* removede() assumes de_fndoffset is item's offset in parent */
	error = removede(fddep, fdep, context);
	if (error) {
		/* XXX should really panic here, fs is corrupt */
		goto exit;
	}
	if (!doingdirectory) {
		/*
		 * Fix fdep's de_dirclust and de_diroffset to reflect
		 * its new location in the destination directory.
		 */
		error = pcbmap(tddep, de_cluster(pmp, to_diroffset), 1,
					NULL, &fdep->de_dirclust, NULL, context);
		if (error) {
			/* XXX should really panic here, fs is corrupt */
			goto exit;
		}
		fdep->de_diroffset = to_diroffset;
	}
	reinsert(fdep);

	/*
	 * If we moved a directory to a new parent directory, then we must
	 * fixup the ".." entry in the moved directory.
	 */
	if (doingdirectory && newparent) {
		struct direntry *dotdotp;

		/* Read in the first cluster of the directory and point to ".." entry */
		cn = fdep->de_StartCluster;
		bn = cntobn(pmp, cn);
		error = (int)buf_meta_bread(pmp->pm_devvp, bn, pmp->pm_bpcluster, vfs_context_ucred(context), &bp);
		if (error) {
			/* XXX should really panic here, fs is corrupt */
			buf_brelse(bp);
			goto exit;
		}
		dotdotp = (struct direntry *)buf_dataptr(bp) + 1;

		/* Update the starting cluster of ".." */
		putushort(dotdotp->deStartCluster, tddep->de_StartCluster);
		if (FAT32(pmp))
			putushort(dotdotp->deHighClust, tddep->de_StartCluster >> 16);

		if (pmp->pm_flags & MSDOSFSMNT_WAITONFAT)
			error = (int)buf_bwrite(bp);
		else
			buf_bdwrite(bp);	// error = 0 from above
		if (error) {
			/* XXX should really panic here, fs is corrupt */
			goto exit;
		}
	}

exit:
	return (error);

}

static struct {
	struct direntry dot;
	struct direntry dotdot;
} dosdirtemplate = {
	{	".       ", "   ",			/* the . entry */
		ATTR_DIRECTORY,				/* file attribute */
		0,	 				/* reserved */
		0, { 0, 0 }, { 0, 0 },			/* create time & date */
		{ 0, 0 },				/* access date */
		{ 0, 0 },				/* high bits of start cluster */
		{ 210, 4 }, { 210, 4 },			/* modify time & date */
		{ 0, 0 },				/* startcluster */
		{ 0, 0, 0, 0 } 				/* filesize */
	},
	{	"..      ", "   ",			/* the .. entry */
		ATTR_DIRECTORY,				/* file attribute */
		0,	 				/* reserved */
		0, { 0, 0 }, { 0, 0 },			/* create time & date */
		{ 0, 0 },				/* access date */
		{ 0, 0 },				/* high bits of start cluster */
		{ 210, 4 }, { 210, 4 },			/* modify time & date */
		{ 0, 0 },				/* startcluster */
		{ 0, 0, 0, 0 }				/* filesize */
	}
};

static int
msdosfs_mkdir(ap)
	struct vnop_mkdir_args /* {
		vnode_t a_dvp;
		vnode_t *a_vpp;
		struct componentname *a_cnp;
		struct vattr *a_vap;
		vfs_context_t a_context;
	} */ *ap;
{
	vnode_t dvp = ap->a_dvp;
	struct denode *pdep = VTODE(dvp);
	struct componentname *cnp = ap->a_cnp;
	vfs_context_t context = ap->a_context;
	struct denode *dep;
	struct direntry *denp;
	struct msdosfsmount *pmp = pdep->de_pmp;
	struct buf *bp;
	u_long newcluster, pcl;
	daddr64_t bn;
	int error;
	struct denode ndirent;
	struct timespec ts;
	char *bdata;

	/*
	 * If this is the root directory and there is no space left we
	 * can't do anything.  This is because the root directory can not
	 * change size.
	 */
	if (pdep->de_StartCluster == MSDOSFSROOT
		&& pdep->de_fndoffset >= pdep->de_FileSize)
	{
		error = ENOSPC;
		goto exit;
	}

	/*
	 * Allocate a cluster to hold the about to be created directory.
	 */
	error = clusteralloc(pmp, 0, 1, CLUST_EOFE, &newcluster, NULL, context);
	if (error)
		goto exit;

	bzero(&ndirent, sizeof(ndirent));
	ndirent.de_pmp = pmp;
	ndirent.de_flag = DE_ACCESS | DE_CREATE | DE_UPDATE;
	getnanotime(&ts);
	DETIMES(&ndirent, &ts, &ts, &ts);

	/*
	 * Now fill the cluster with the "." and ".." entries. And write
	 * the cluster to disk.  This way it is there for the parent
	 * directory to be pointing at if there were a crash.
	 */
	bn = cntobn(pmp, newcluster);
	bp = buf_getblk(pmp->pm_devvp, bn, pmp->pm_bpcluster, 0, 0, BLK_META);
	bdata = (char *)buf_dataptr(bp);

	bzero(bdata, pmp->pm_bpcluster);
	bcopy(&dosdirtemplate, bdata, sizeof dosdirtemplate);
	denp = (struct direntry *)bdata;
	putushort(denp[0].deStartCluster, newcluster);
	putushort(denp[0].deCDate, ndirent.de_CDate);
	putushort(denp[0].deCTime, ndirent.de_CTime);
	denp[0].deCHundredth = ndirent.de_CHun;
	putushort(denp[0].deADate, ndirent.de_ADate);
	putushort(denp[0].deMDate, ndirent.de_MDate);
	putushort(denp[0].deMTime, ndirent.de_MTime);
	pcl = pdep->de_StartCluster;
	if (FAT32(pmp) && pcl == pmp->pm_rootdirblk)
		pcl = 0;
	putushort(denp[1].deStartCluster, pcl);
	putushort(denp[1].deCDate, ndirent.de_CDate);
	putushort(denp[1].deCTime, ndirent.de_CTime);
	denp[1].deCHundredth = ndirent.de_CHun;
	putushort(denp[1].deADate, ndirent.de_ADate);
	putushort(denp[1].deMDate, ndirent.de_MDate);
	putushort(denp[1].deMTime, ndirent.de_MTime);
	if (FAT32(pmp)) {
		putushort(denp[0].deHighClust, newcluster >> 16);
		putushort(denp[1].deHighClust, pdep->de_StartCluster >> 16);
	}

	if (pmp->pm_flags & MSDOSFSMNT_WAITONFAT)
		error = (int)buf_bwrite(bp);
	else
		buf_bdwrite(bp);	// error = 0 from above
	if (error)
		goto exit;

	/*
	 * Now build up a directory entry pointing to the newly allocated
	 * cluster.  This will be written to an empty slot in the parent
	 * directory.
	 */
	error = uniqdosname(pdep, cnp, ndirent.de_Name, &ndirent.de_LowerCase, context);
	if (error)
		goto exit;

	ndirent.de_Attributes = ATTR_DIRECTORY;
	ndirent.de_StartCluster = newcluster;
	ndirent.de_FileSize = 0;
	ndirent.de_dev = pdep->de_dev;
	ndirent.de_devvp = pdep->de_devvp;
	error = createde(&ndirent, pdep, &dep, cnp, context);
	if (error)
		clusterfree(pmp, newcluster, NULL, context);
	else
		*ap->a_vpp = DETOV(dep);

exit:
	return error;
}

/*
 * Remove a directory.
 *
 * On entry, vp has been suspended, so there are no pending
 * create or lookup operations happening using it as the
 * parent directory.
 *
 * VFS has already checked that dvp != vp.
 * 
 */
static int
msdosfs_rmdir(ap)
	struct vnop_rmdir_args /* {
		vnode_t a_dvp;
		vnode_t a_vp;
		struct componentname *a_cnp;
		vfs_context_t a_context;
	} */ *ap;
{
	vnode_t vp = ap->a_vp;
	vnode_t dvp = ap->a_dvp;
	vfs_context_t context = ap->a_context;
	struct denode *ip, *dp;
	int error;
	
	/* No need to lock vp since it has been suspended */
	
	ip = VTODE(vp);
	dp = VTODE(dvp);

	/*
	 * No rmdir "." please.
	 *
	 * VFS already checks this in rmdir(), so do we
	 * need to check again?  (It would only be useful if
	 * some other entity called our VNOP directly.)
	 */
	if (dp == ip) {
		return EINVAL;
	}

	/*
	 * Verify the directory is empty (and valid).
	 * (Rmdir ".." won't be valid since
	 *  ".." will contain a reference to
	 *  the current directory and thus be
	 *  non-empty.)
	 */
	error = 0;
	if (!dosdirempty(ip, context)) {
		error = ENOTEMPTY;
		goto exit;
	}

    /* Make sure the directory isn't read-only */
    if (ip->de_Attributes & ATTR_READONLY)
    {
        error = EPERM;
        goto exit;
    }
    
	/*
	 * Delete the entry from the directory.  For dos filesystems this
	 * gets rid of the directory entry on disk, the in memory copy
	 * still exists but the de_refcnt is <= 0.  This prevents it from
	 * being found by deget().  When the vput() on dep is done we give
	 * up access and eventually msdosfs_reclaim() will be called which
	 * will remove it from the denode cache.
	 */
	error = removede(dp, ip, context);
	if (error) goto exit;

	/*
	 * This is where we decrement the link count in the parent
	 * directory.  Since dos filesystems don't do this we just purge
	 * the name cache.
	 */
	cache_purge(dvp);

	/*
	 * Truncate the directory that is being deleted.
	 */
	error = detrunc(ip, (u_long)0, IO_SYNC, context);
	cache_purge(vp);

exit:
	return (error);
}

static int
msdosfs_readdir(ap)
	struct vnop_readdir_args /* {
		vnode_t a_vp;
		struct uio *a_uio;
		int *a_eofflag;
		int *a_ncookies;
		u_long **a_cookies;
		vfs_context_t a_context;
	} */ *ap;
{
	int error = 0;
	vnode_t vp = ap->a_vp;
	struct uio *uio = ap->a_uio;
	struct denode *dep = VTODE(vp);
	struct msdosfsmount *pmp = dep->de_pmp;
	vfs_context_t context = ap->a_context;
	int diff;
	long n;
	u_long blsize;
	long on;
	u_long cn;
	u_long fileno;
	u_long dirsperblk;
	long bias = 0;
	daddr64_t bn, lbn;
	struct buf *bp;
	struct direntry *dentp;
	struct dirent dirbuf;
	u_long *cookies = NULL;
	int ncookies = 0;
	off_t offset, off;
	int chksum = -1;
	u_int16_t ucfn[WIN_MAXLEN + 1];
	u_int16_t unichars;
	size_t outbytes;
	char *bdata;

	/*
	 * msdosfs_readdir() won't operate properly on regular files since
	 * it does i/o only with the the filesystem vnode, and hence can
	 * retrieve the wrong block from the buffer cache for a plain file.
	 * So, fail attempts to readdir() on a plain file.
	 */
	if (!vnode_isdir(vp))
		return ENOTDIR;

	/*
	 * To be safe, initialize dirbuf
	 */
	bzero(dirbuf.d_name, sizeof(dirbuf.d_name));

	/*
	 * If the user buffer is smaller than the size of one dos directory
	 * entry or the file offset is not a multiple of the size of a
	 * directory entry, then we fail the read.
	 */
	off = offset = uio->uio_offset;
	if (uio->uio_resid < sizeof(struct direntry) ||
	    (offset & (sizeof(struct direntry) - 1)))
		return EINVAL;

	if (ap->a_ncookies) {
		ncookies = uio->uio_resid / 16;
		MALLOC(cookies, u_long *, ncookies * sizeof(u_long), M_TEMP,
		       M_WAITOK);
		*ap->a_cookies = cookies;
		*ap->a_ncookies = ncookies;
	}

	dirsperblk = pmp->pm_BytesPerSec / sizeof(struct direntry);

	/*
	 * If they are reading from the root directory then, we simulate
	 * the . and .. entries since these don't exist in the root
	 * directory.  We also set the offset bias to make up for having to
	 * simulate these entries. By this I mean that at file offset 64 we
	 * read the first entry in the root directory that lives on disk.
	 */
	if (dep->de_StartCluster == MSDOSFSROOT
	    || (FAT32(pmp) && dep->de_StartCluster == pmp->pm_rootdirblk)) {
#ifdef DEBUG
		printf("msdosfs_readdir(): going after . or .. in root dir, offset %d\n",
		    offset);
#endif
		bias = 2 * sizeof(struct direntry);
		if (offset < bias) {
			for (n = (int)offset / sizeof(struct direntry);
			     n < 2; n++) {
                dirbuf.d_fileno = defileid(dep);
				dirbuf.d_type = DT_DIR;
				switch (n) {
				case 0:
					dirbuf.d_namlen = 1;
					strcpy(dirbuf.d_name, ".");
					break;
				case 1:
					dirbuf.d_namlen = 2;
					strcpy(dirbuf.d_name, "..");
					break;
				}
				dirbuf.d_reclen = GENERIC_DIRSIZ(&dirbuf);
				if (uio->uio_resid < dirbuf.d_reclen)
					goto out;
				error = uiomove((caddr_t) &dirbuf,
						dirbuf.d_reclen, uio);
				if (error)
					goto out;
				offset += sizeof(struct direntry);
				off = offset;
				if (cookies) {
					*cookies++ = offset;
					if (--ncookies <= 0)
						goto out;
				}
			}
		}
	}

	off = offset;
	while (uio->uio_resid > 0) {
		lbn = de_cluster(pmp, offset - bias);
		on = (offset - bias) & pmp->pm_crbomask;
		n = min(pmp->pm_bpcluster - on, uio->uio_resid);
		diff = dep->de_FileSize - (offset - bias);
		if (diff <= 0)
			break;
		n = min(n, diff);
		error = pcbmap(dep, lbn, 1, &bn, &cn, &blsize, context);
		if (error)
			break;
		error = (int)buf_meta_bread(pmp->pm_devvp, bn, blsize, vfs_context_ucred(context), &bp);
		if (error) {
			buf_brelse(bp);
			return (error);
		}
		n = min(n, blsize - buf_resid(bp));

		bdata = (char *)buf_dataptr(bp);
		/*
		 * Convert from dos directory entries to fs-independent
		 * directory entries.
		 */
		for (dentp = (struct direntry *)(bdata + on);
		     (char *)dentp < bdata + on + n;
		     dentp++, offset += sizeof(struct direntry)) {
#ifdef DEBUG
			printf("rd: dentp %08x prev %08x crnt %08x deName %02x attr %02x\n",
			    dentp, prev, crnt, dentp->deName[0], dentp->deAttributes);
#endif
			/*
			 * If this is an unused entry, we can stop.
			 */
			if (dentp->deName[0] == SLOT_EMPTY) {
				buf_brelse(bp);
				goto out;
			}
			/*
			 * Skip deleted entries.
			 */
			if (dentp->deName[0] == SLOT_DELETED) {
				chksum = -1;
				continue;
			}

			/*
			 * Handle Win95 long directory entries
			 */
			if (dentp->deAttributes == ATTR_WIN95) {
				chksum = getunicodefn((struct winentry *)dentp,
						ucfn, &unichars, chksum);
				continue;
			}

			/*
			 * Skip volume labels
			 */
			if (dentp->deAttributes & ATTR_VOLUME) {
				chksum = -1;
				continue;
			}
			/*
			 * This computation of d_fileno must match
			 * the computation in defileid().
             */
            fileno = getushort(dentp->deStartCluster);
            if (FAT32(pmp))
                fileno |= getushort(dentp->deHighClust) << 16;
			if (dentp->deAttributes & ATTR_DIRECTORY) {
            	if (fileno == MSDOSFSROOT) {
                    /* if this is the root directory */
                    if (FAT32(pmp))
                        fileno = pmp->pm_rootdirblk;
                    else
                        fileno = FILENO_ROOT;
                }
				dirbuf.d_type = DT_DIR;
			} else {
                if (fileno == 0)
                    fileno = FILENO_EMPTY;	/* constant for empty files */
				dirbuf.d_type = DT_REG;
			}
            dirbuf.d_fileno = fileno;
			if (chksum != winChksum(dentp->deName)) {
				unichars = dos2unicodefn(dentp->deName, ucfn,
				    dentp->deLowerCase);
			}
			
			/* translate the name in ucfn into UTF-8 */
			(void) utf8_encodestr(ucfn, unichars * 2,
					dirbuf.d_name, &outbytes,
					sizeof(dirbuf.d_name), 0, UTF_DECOMPOSED);
			dirbuf.d_namlen = outbytes;
			chksum = -1;
			dirbuf.d_reclen = GENERIC_DIRSIZ(&dirbuf);

			if (uio->uio_resid < dirbuf.d_reclen) {
				buf_brelse(bp);
				goto out;
			}
			error = uiomove((caddr_t) &dirbuf,
					dirbuf.d_reclen, uio);
			if (error) {
				buf_brelse(bp);
				goto out;
			}
			if (cookies) {
				*cookies++ = offset + sizeof(struct direntry);
				if (--ncookies <= 0) {
					buf_brelse(bp);
					goto out;
				}
			}
			off = offset + sizeof(struct direntry);
		}
		buf_brelse(bp);
	}
out:
	/* Subtract unused cookies */
	if (ap->a_ncookies)
		*ap->a_ncookies -= ncookies;

	uio->uio_offset = off;

	/*
	 * Set the eofflag (NFS uses it)
	 */
	if (ap->a_eofflag) {
		if (dep->de_FileSize - (offset - bias) <= 0)
			*ap->a_eofflag = 1;
		else
			*ap->a_eofflag = 0;
	}

	return error;
}

/* blktooff converts a logical block number to a file offset */
__private_extern__ int
msdosfs_blktooff(ap)
	struct vnop_blktooff_args /* {
		vnode_t a_vp;
		daddr64_t a_lblkno;
		off_t *a_offset;    
	} */ *ap;
{
	if (ap->a_vp == NULL)
		return EINVAL;

	*ap->a_offset = ap->a_lblkno * PAGE_SIZE_64;

	return 0;
}

/* offtoblk converts a file offset to a logical block number */
__private_extern__ int
msdosfs_offtoblk(ap)
	struct vnop_offtoblk_args /* {
		vnode_t a_vp;
		off_t a_offset;    
		daddr64_t *a_lblkno;
	} */ *ap;
{
	if (ap->a_vp == NULL)
		return EINVAL;
	
	*ap->a_lblkno = ap->a_offset / PAGE_SIZE_64;
	
	return 0;
}


__private_extern__ int
msdosfs_blockmap(ap)
	struct vnop_blockmap_args /* {
		vnode_t a_vp;
		off_t a_foffset;
		size_t a_size;
		daddr64_t *a_bpn;
		size_t *a_run;
		void *a_poff;
		int a_flags;
		vfs_context_t a_context;
	} */ *ap;
{
	int error;
	vnode_t vp = ap->a_vp;
	struct denode *dep = VTODE(vp);
    struct msdosfsmount *pmp = dep->de_pmp;
	vfs_context_t context = ap->a_context;
	u_long runsize;
    u_long		cn;
    u_long		numclusters;
    daddr64_t	bn;

	if (ap->a_bpn == NULL)
		return (0);

    if (ap->a_size == 0)
        panic("msdosfs_blockmap: a_size == 0");

    /* Find the cluster that contains the given file offset */
    cn = de_cluster(pmp, ap->a_foffset);
    
    /* Determine number of clusters occupied by the given range */
    numclusters = de_cluster(pmp, ap->a_foffset + ap->a_size - 1) - cn + 1;
    
    /* Find the physical (device) block where that cluster starts */
    error = pcbmap(dep, cn, numclusters, &bn, NULL, &runsize, context);
#ifdef DEBUG
    printf("msdosfs_blockmap: off 0x%lx bn1 0x%lx",
           (u_long)ap->a_foffset, bn);
#endif

    /* Add the offset in physical (device) blocks from the start of the cluster */
    bn += (((u_long)ap->a_foffset - de_cn2off(pmp, cn)) >> pmp->pm_bnshift);
    runsize -= ((u_long)ap->a_foffset - (de_cn2off(pmp, cn)));
    
    *ap->a_bpn = bn;
	if (error == 0 && ap->a_run) {
		if (runsize > ap->a_size)
			* ap->a_run = ap->a_size;
		else
			* ap->a_run = runsize;
	}
	if (ap->a_poff)
		*(int *)ap->a_poff = 0;

#ifdef DEBUG
    printf(" bn 2 0x%lx run 0x%lx\n", bn, *ap->a_run);
#endif
 return (error);
}

/*
 * prepare and issue the I/O
 * buf_strategy knows how to deal
 * with requests that require 
 * fragmented I/Os
 */
static int
msdosfs_strategy(ap)
	struct vnop_strategy_args /* {
		struct buf *a_bp;
	} */ *ap;
{
	buf_t	bp = ap->a_bp;
	vnode_t	vp = buf_vnode(bp);
	struct denode *dep = VTODE(vp);
	
	return buf_strategy(dep->de_devvp, ap);
}

static int
msdosfs_pathconf(ap)
	struct vnop_pathconf_args /* {
		vnode_t a_vp;
		int a_name;
		register_t *a_retval;
		vfs_context_t a_context;
	} */ *ap;
{
	switch (ap->a_name) {
	case _PC_LINK_MAX:
		*ap->a_retval = 1;
		return (0);
	case _PC_NAME_MAX:
		*ap->a_retval = WIN_MAXLEN;
		return (0);
	case _PC_PATH_MAX:
		*ap->a_retval = PATH_MAX;
		return (0);
	case _PC_CHOWN_RESTRICTED:
		*ap->a_retval = 1;
		return (0);
	case _PC_NO_TRUNC:
		*ap->a_retval = 0;
		return (0);
	default:
		return (EINVAL);
	}
	/* NOTREACHED */
}


static int msdosfs_advlock(struct vnop_advlock_args /* {
	vnode_t a_vp;
	caddr_t a_id;
	int a_op;
	struct flock *a_fl;
	int a_flags;
	vfs_context_t a_context;
	} */ *ap)
{
	vnode_t vp = ap->a_vp;
	struct flock *fl = ap->a_fl;
	struct msdosfs_lockf *lock;
	struct denode *dep;
	off_t start, end;
	int error;
    
	/* Only regular files can have locks */
	if ( !vnode_isreg(vp))
		return EISDIR;

    dep = VTODE(vp);
    
	/*
	 * Avoid the common case of unlocking when inode has no locks.
	 */
	if (dep->de_lockf == (struct msdosfs_lockf *)0) {
		if (ap->a_op != F_SETLK) {
			fl->l_type = F_UNLCK;
			error = 0;
			goto exit;
		}
	}

	/*
	 * Convert the flock structure into a start and end.
	 */
	start = 0;
	switch (fl->l_whence) {
	case SEEK_SET:
	case SEEK_CUR:
		/*
		 * Caller is responsible for adding any necessary offset
		 * when SEEK_CUR is used.
		 */
		start = fl->l_start;
		break;
	case SEEK_END:
		start = dep->de_FileSize + fl->l_start;
		break;
	default:
		error = EINVAL;
		goto exit;
	}

	if (start < 0)
	{
		error = EINVAL;
		goto exit;
	}
	
	if (fl->l_len == 0)
 		end = -1;
	else
		end = start + fl->l_len - 1;

	/*
	 * Create the lockf structure
	 */
	MALLOC(lock, struct msdosfs_lockf *, sizeof *lock, M_LOCKF, M_WAITOK);
	lock->lf_start = start;
	lock->lf_end = end;
	lock->lf_id = ap->a_id;
	lock->lf_denode = dep;
	lock->lf_type = fl->l_type;
	lock->lf_next = (struct msdosfs_lockf *)0;
	TAILQ_INIT(&lock->lf_blkhd);
	lock->lf_flags = ap->a_flags;

	/*
	 * Do the requested operation.
	 */
	switch(ap->a_op) {
	case F_SETLK:
		error = msdosfs_setlock(lock);
		break;
	case F_UNLCK:
		error = msdosfs_clearlock(lock);
		FREE(lock, M_LOCKF);
		break;
	case F_GETLK:
		error = msdosfs_getlock(lock, fl);
		FREE(lock, M_LOCKF);
		break;
	default:
		error = EINVAL;
		_FREE(lock, M_LOCKF);
            break;
	}

exit:
	return error;
}


/* Global vfs data structures for msdosfs */

typedef int     vnop_t __P((void *));

int (**msdosfs_vnodeop_p)(void *);
static struct vnodeopv_entry_desc msdosfs_vnodeop_entries[] = {
	{ &vnop_default_desc,		(vnop_t *) vn_default_error },
	{ &vnop_lookup_desc,		(vnop_t *) msdosfs_lookup },
	{ &vnop_create_desc,		(vnop_t *) msdosfs_create },
	{ &vnop_mknod_desc,			(vnop_t *) msdosfs_mknod },
	{ &vnop_open_desc,			(vnop_t *) msdosfs_open },
	{ &vnop_close_desc,			(vnop_t *) msdosfs_close },
	{ &vnop_access_desc,		(vnop_t *) msdosfs_access },
	{ &vnop_getattr_desc,		(vnop_t *) msdosfs_getattr },
	{ &vnop_setattr_desc,		(vnop_t *) msdosfs_setattr },
    { &vnop_getattrlist_desc,	(vnop_t *) msdosfs_getattrlist },
    { &vnop_setattrlist_desc,	(vnop_t *) msdosfs_setattrlist },
	{ &vnop_read_desc,			(vnop_t *) msdosfs_read },
	{ &vnop_write_desc,			(vnop_t *) msdosfs_write },
	{ &vnop_fsync_desc,			(vnop_t *) msdosfs_fsync },
	{ &vnop_remove_desc,		(vnop_t *) msdosfs_remove },
	{ &vnop_rename_desc,		(vnop_t *) msdosfs_rename },
	{ &vnop_mkdir_desc,			(vnop_t *) msdosfs_mkdir },
	{ &vnop_rmdir_desc,			(vnop_t *) msdosfs_rmdir },
	{ &vnop_readdir_desc,		(vnop_t *) msdosfs_readdir },
	{ &vnop_inactive_desc,		(vnop_t *) msdosfs_inactive },
	{ &vnop_reclaim_desc,		(vnop_t *) msdosfs_reclaim },
	{ &vnop_pathconf_desc,		(vnop_t *) msdosfs_pathconf },
	{ &vnop_advlock_desc,		(vnop_t *) msdosfs_advlock },
	{ &vnop_pagein_desc,		(vnop_t *) msdosfs_pagein },
	{ &vnop_pageout_desc,		(vnop_t *) msdosfs_pageout },
	{ &vnop_blktooff_desc,		(vnop_t *) msdosfs_blktooff },
	{ &vnop_offtoblk_desc,		(vnop_t *) msdosfs_offtoblk },
  	{ &vnop_blockmap_desc,		(vnop_t *) msdosfs_blockmap },
	{ &vnop_strategy_desc,		(vnop_t *) msdosfs_strategy },
	{ NULL, NULL }
};

struct vnodeopv_desc msdosfs_vnodeop_opv_desc =
	{ &msdosfs_vnodeop_p, msdosfs_vnodeop_entries };
