/*
 * CDDL HEADER START
 *
 * The contents of this file are subject to the terms of the
 * Common Development and Distribution License (the "License").
 * You may not use this file except in compliance with the License.
 *
 * You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
 * or http://www.opensolaris.org/os/licensing.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 *
 * When distributing Covered Code, include this CDDL HEADER in each
 * file and include the License file at usr/src/OPENSOLARIS.LICENSE.
 * If applicable, add the following below this CDDL HEADER, with the
 * fields enclosed by brackets "[]" replaced with your own identifying
 * information: Portions Copyright [yyyy] [name of copyright owner]
 *
 * CDDL HEADER END
 */

/*
 * Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 * Portions Copyright 2007 Apple Inc. All rights reserved.
 * Use is subject to license terms.
 */

/*
 * Portions Copyright 2007 Apple Inc.  All rights reserved.
 */

#pragma ident	"%Z%%M%	%I%	%E% SMI"

/*
 * Routines to manage ZFS mounts.  We separate all the nasty routines that have
 * to deal with the OS.  The following functions are the main entry points --
 * they are used by mount and unmount and when changing a filesystem's
 * mountpoint.
 *
 * 	zfs_is_mounted()
 * 	zfs_mount()
 * 	zfs_unmount()
 * 	zfs_unmountall()
 *
 * This file also contains the functions used to manage sharing filesystems via
 * NFS and iSCSI:
 *
 * 	zfs_is_shared()
 * 	zfs_share()
 * 	zfs_unshare()
 *
 * 	zfs_is_shared_nfs()
 * 	zfs_share_nfs()
 * 	zfs_unshare_nfs()
 * 	zfs_unshareall_nfs()
 * 	zfs_is_shared_iscsi()
 * 	zfs_share_iscsi()
 * 	zfs_unshare_iscsi()
 *
 * The following functions are available for pool consumers, and will
 * mount/unmount and share/unshare all datasets within pool:
 *
 * 	zpool_enable_datasets()
 * 	zpool_disable_datasets()
 */

#include <dirent.h>
#ifndef __APPLE__
#include <dlfcn.h>
#endif
#include <errno.h>
#include <libgen.h>
#include <libintl.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#ifdef __APPLE__
#include <sys/param.h>
#include <sys/xattr.h>
#else
#include <zone.h>
#endif /*__APPLE__*/
#include <sys/mntent.h>
#include <sys/mnttab.h>
#include <sys/mount.h>
#include <sys/stat.h>

#include <libzfs.h>

#include "libzfs_impl.h"

#ifndef __APPLE__
static int (*iscsitgt_zfs_share)(const char *);
static int (*iscsitgt_zfs_unshare)(const char *);
static int (*iscsitgt_zfs_is_shared)(const char *);

#pragma init(zfs_iscsi_init)
static void
zfs_iscsi_init(void)
{
	void *libiscsitgt;

	if ((libiscsitgt = dlopen("/lib/libiscsitgt.so.1",
	    RTLD_LAZY | RTLD_GLOBAL)) == NULL ||
	    (iscsitgt_zfs_share = (int (*)(const char *))dlsym(libiscsitgt,
	    "iscsitgt_zfs_share")) == NULL ||
	    (iscsitgt_zfs_unshare = (int (*)(const char *))dlsym(libiscsitgt,
	    "iscsitgt_zfs_unshare")) == NULL ||
	    (iscsitgt_zfs_is_shared = (int (*)(const char *))dlsym(libiscsitgt,
	    "iscsitgt_zfs_is_shared")) == NULL) {
		iscsitgt_zfs_share = NULL;
		iscsitgt_zfs_unshare = NULL;
		iscsitgt_zfs_is_shared = NULL;
	}
}
#endif /* !__APPLE__ */

/*
 * Search the sharetab for the given mountpoint, returning true if it is found.
 */
static boolean_t
is_shared(libzfs_handle_t *hdl, const char *mountpoint)
{
	char buf[MAXPATHLEN], *tab;

	if (hdl->libzfs_sharetab == NULL)
		return (0);

	(void) fseek(hdl->libzfs_sharetab, 0, SEEK_SET);

	while (fgets(buf, sizeof (buf), hdl->libzfs_sharetab) != NULL) {

		/* the mountpoint is the first entry on each line */
		if ((tab = strchr(buf, '\t')) != NULL) {
			*tab = '\0';
			if (strcmp(buf, mountpoint) == 0)
				return (B_TRUE);
		}
	}

	return (B_FALSE);
}

#ifndef __APPLE__
/*
 * Returns true if the specified directory is empty.  If we can't open the
 * directory at all, return true so that the mount can fail with a more
 * informative error message.
 */
static boolean_t
dir_is_empty(const char *dirname)
{
	DIR *dirp;
	struct dirent64 *dp;

	if ((dirp = opendir(dirname)) == NULL)
		return (B_TRUE);

	while ((dp = readdir64(dirp)) != NULL) {

		if (strcmp(dp->d_name, ".") == 0 ||
		    strcmp(dp->d_name, "..") == 0)
			continue;

		(void) closedir(dirp);
		return (B_FALSE);
	}

	(void) closedir(dirp);
	return (B_TRUE);
}
#endif /*!__APPLE__*/

/*
 * Checks to see if the mount is active.  If the filesystem is mounted, we fill
 * in 'where' with the current mountpoint, and return 1.  Otherwise, we return
 * 0.
 */
boolean_t
is_mounted(libzfs_handle_t *zfs_hdl, const char *special, char **where)
{
	struct mnttab search = { 0 }, entry;

	/*
	 * Search for the entry in /etc/mnttab.  We don't bother getting the
	 * mountpoint, as we can just search for the special device.  This will
	 * also let us find mounts when the mountpoint is 'legacy'.
	 */
	search.mnt_special = (char *)special;
	search.mnt_fstype = MNTTYPE_ZFS;

#ifndef __APPLE__
	rewind(zfs_hdl->libzfs_mnttab);
#endif
	if (getmntany(zfs_hdl->libzfs_mnttab, &entry, &search) != 0)
		return (B_FALSE);

	if (where != NULL)
		*where = zfs_strdup(zfs_hdl, entry.mnt_mountp);

	return (B_TRUE);
}

boolean_t
zfs_is_mounted(zfs_handle_t *zhp, char **where)
{
	return (is_mounted(zhp->zfs_hdl, zfs_get_name(zhp), where));
}

/*
 * Returns true if the given dataset is mountable, false otherwise.  Returns the
 * mountpoint in 'buf'.
 */
static boolean_t
zfs_is_mountable(zfs_handle_t *zhp, char *buf, size_t buflen,
    zfs_source_t *source)
{
	char sourceloc[ZFS_MAXNAMELEN];
	zfs_source_t sourcetype;

	if (!zfs_prop_valid_for_type(ZFS_PROP_MOUNTPOINT, zhp->zfs_type))
		return (B_FALSE);

	verify(zfs_prop_get(zhp, ZFS_PROP_MOUNTPOINT, buf, buflen,
	    &sourcetype, sourceloc, sizeof (sourceloc), B_FALSE) == 0);

	if (strcmp(buf, ZFS_MOUNTPOINT_NONE) == 0 ||
	    strcmp(buf, ZFS_MOUNTPOINT_LEGACY) == 0)
		return (B_FALSE);

	if (!zfs_prop_get_int(zhp, ZFS_PROP_CANMOUNT))
		return (B_FALSE);

#ifndef	__APPLE__
	if (zfs_prop_get_int(zhp, ZFS_PROP_ZONED) &&
	    getzoneid() == GLOBAL_ZONEID)
		return (B_FALSE);
#endif /*!__APPLE__*/

	if (source)
		*source = sourcetype;

	return (B_TRUE);
}


#ifdef __APPLE__

struct zfs_mount_args {
	const char	*fspec;		/* block special device to mount */
	int	flags; 
};

#define MOUNT_POINT_COOKIE		".autodiskmounted"
#define MOUNT_POINT_CUSTOM_ICON		".VolumeIcon.icns"
#define CUSTOM_ICON_PATH		"/System/Library/Filesystems/zfs.fs/Contents/Resources/VolumeIcon.icns"

#endif

/*
 * Mount the given filesystem.
 */
int
zfs_mount(zfs_handle_t *zhp, const char *options, int flags)
{
	struct stat buf;
	char mountpoint[ZFS_MAXPROPLEN];
	char mntopts[MNT_LINE_MAX];
	libzfs_handle_t *hdl = zhp->zfs_hdl;
#ifdef __APPLE__
	struct zfs_mount_args mnt_args;
	char  path[MAXPATHLEN];
	FILE *  file;
#endif

	if (options == NULL)
		mntopts[0] = '\0';
	else
		(void) strlcpy(mntopts, options, sizeof (mntopts));

	if (!zfs_is_mountable(zhp, mountpoint, sizeof (mountpoint), NULL))
		return (0);

	/* Create the directory if it doesn't already exist */
	if (lstat(mountpoint, &buf) != 0) {
		if (mkdirp(mountpoint, 0755) != 0) {
			zfs_error_aux(hdl, dgettext(TEXT_DOMAIN,
			    "failed to create mountpoint"));
			return (zfs_error_fmt(hdl, EZFS_MOUNTFAILED,
			    dgettext(TEXT_DOMAIN, "cannot mount '%s'"),
			    mountpoint));
		}
#ifdef __APPLE__
		/*
		 * Create the mount point cookie file.
		 */
		snprintf(path, MAXPATHLEN, "%s/%s", mountpoint, MOUNT_POINT_COOKIE);
		file = fopen( path, "w" );
		if ( file )
			fclose( file );
#endif
	}

#ifndef __APPLE__
	/*
	 * Determine if the mountpoint is not empty.  If so, refuse to perform the
	 * mount.  We don't perform this check if MS_OVERLAY is specified, which
	 * would defeat the point.  We also avoid this check if 'remount' is
	 * specified.
	 */
	if ((flags & MS_OVERLAY) == 0 &&
	    strstr(mntopts, MNTOPT_REMOUNT) == NULL &&
	    !dir_is_empty(mountpoint)) {
		zfs_error_aux(hdl, dgettext(TEXT_DOMAIN,
		    "directory is not empty"));
		return (zfs_error_fmt(hdl, EZFS_MOUNTFAILED,
		    dgettext(TEXT_DOMAIN, "cannot mount '%s'"), mountpoint));
	}
#endif /*!__APPLE__*/

	/* perform the mount */
#ifdef __APPLE__
	if (options)
		printf("zfs_mount: unused options: \"%s\"\n", mntopts);
	mnt_args.fspec = zfs_get_name(zhp);
	if (mount(MNTTYPE_ZFS, mountpoint, flags, &mnt_args) != 0) {
#else
	if (mount(zfs_get_name(zhp), mountpoint, MS_OPTIONSTR | flags,
	    MNTTYPE_ZFS, NULL, 0, mntopts, sizeof (mntopts)) != 0) {
#endif
		/*
		 * Generic errors are nasty, but there are just way too many
		 * from mount(), and they're well-understood.  We pick a few
		 * common ones to improve upon.
		 */
		if (errno == EBUSY)
			zfs_error_aux(hdl, dgettext(TEXT_DOMAIN,
			    "mountpoint or dataset is busy"));
		else
			zfs_error_aux(hdl, strerror(errno));

		return (zfs_error_fmt(hdl, EZFS_MOUNTFAILED,
		    dgettext(TEXT_DOMAIN, "cannot mount '%s'"),
		    zhp->zfs_name));
	}
#ifdef __APPLE__
	/*
	 * For a root file system, add a volume icon.
	 */
	if (strpbrk(mnt_args.fspec, "/") == NULL) {
		ssize_t  attrsize;
		u_int16_t finderinfo[16];
		struct stat sbuf;

		/* Tag the root directory as having a custom icon. */
		attrsize = getxattr(mountpoint, XATTR_FINDERINFO_NAME, &finderinfo,
		                    sizeof (finderinfo), 0, 0);
		if (attrsize != sizeof (finderinfo))
			(void) memset(&finderinfo, 0, sizeof (finderinfo));

		finderinfo[4] |= OSSwapHostToBigInt16(0x0400);

		(void) setxattr(mountpoint, XATTR_FINDERINFO_NAME, &finderinfo,
		                sizeof (finderinfo), 0, 0);

		snprintf(path, MAXPATHLEN, "%s/%s", mountpoint, MOUNT_POINT_CUSTOM_ICON);
		if ((stat(path, &sbuf) != 0 || sbuf.st_size == 0) &&
		    (stat(CUSTOM_ICON_PATH, &sbuf) == 0 && sbuf.st_size > 0)) {
			FILE *  srcfile;
			void * buf;

			srcfile = fopen(CUSTOM_ICON_PATH, "r");
			file = fopen(path, "w");

			if (srcfile && file) {
				/* Copy the custom icon to the root directory */
				buf = malloc(sbuf.st_size);
				if (fread(buf, 1, sbuf.st_size, srcfile) == sbuf.st_size)
					(void) fwrite(buf, 1, sbuf.st_size, file);
				free(buf);

				/* Init the custom icon's Finder Info. */
				(void) memset(&finderinfo, 0, sizeof (finderinfo));
				finderinfo[4] = OSSwapHostToBigInt16(0x4000);
				(void) setxattr(path, XATTR_FINDERINFO_NAME,
				                &finderinfo, sizeof (finderinfo), 0, 0);
			}
			if (srcfile)
				fclose(srcfile);
			if (file)
				fclose(file);
		}
	}
#endif

	return (0);
}

/*
 * Unmount a single filesystem.
 */
static int
unmount_one(libzfs_handle_t *hdl, const char *mountpoint, int flags)
{
	if (unmount(mountpoint, flags) != 0) {
		zfs_error_aux(hdl, strerror(errno));
		return (zfs_error_fmt(hdl, EZFS_UMOUNTFAILED,
		    dgettext(TEXT_DOMAIN, "cannot unmount '%s'"),
		    mountpoint));
	}

	return (0);
}

/*
 * Unmount the given filesystem.
 */
int
zfs_unmount(zfs_handle_t *zhp, const char *mountpoint, int flags)
{
	struct mnttab search = { 0 }, entry;

	/* check to see if need to unmount the filesystem */
	search.mnt_special = zhp->zfs_name;
	search.mnt_fstype = MNTTYPE_ZFS;
#ifndef __APPLE__
	rewind(zhp->zfs_hdl->libzfs_mnttab);
#endif /*!__APPLE__*/
	if (mountpoint != NULL || ((zfs_get_type(zhp) == ZFS_TYPE_FILESYSTEM) &&
	    getmntany(zhp->zfs_hdl->libzfs_mnttab, &entry, &search) == 0)) {

		if (mountpoint == NULL)
			mountpoint = entry.mnt_mountp;

		/*
		 * Unshare and unmount the filesystem
		 */
		if (zfs_unshare_nfs(zhp, mountpoint) != 0 ||
		    unmount_one(zhp->zfs_hdl, mountpoint, flags) != 0)
			return (-1);
	}

	return (0);
}

/*
 * Unmount this filesystem and any children inheriting the mountpoint property.
 * To do this, just act like we're changing the mountpoint property, but don't
 * remount the filesystems afterwards.
 */
int
zfs_unmountall(zfs_handle_t *zhp, int flags)
{
	prop_changelist_t *clp;
	int ret;

	clp = changelist_gather(zhp, ZFS_PROP_MOUNTPOINT, flags);
	if (clp == NULL)
		return (-1);

	ret = changelist_prefix(clp);
	changelist_free(clp);

	return (ret);
}

boolean_t
zfs_is_shared(zfs_handle_t *zhp)
{
	if (ZFS_IS_VOLUME(zhp))
		return (zfs_is_shared_iscsi(zhp));

	return (zfs_is_shared_nfs(zhp, NULL));
}

int
zfs_share(zfs_handle_t *zhp)
{
	if (ZFS_IS_VOLUME(zhp))
		return (zfs_share_iscsi(zhp));

	return (zfs_share_nfs(zhp));
}

int
zfs_unshare(zfs_handle_t *zhp)
{
	if (ZFS_IS_VOLUME(zhp))
		return (zfs_unshare_iscsi(zhp));

	return (zfs_unshare_nfs(zhp, NULL));
}

/*
 * Check to see if the filesystem is currently shared.
 */
boolean_t
zfs_is_shared_nfs(zfs_handle_t *zhp, char **where)
{
	char *mountpoint;

	if (!zfs_is_mounted(zhp, &mountpoint))
		return (B_FALSE);

	if (is_shared(zhp->zfs_hdl, mountpoint)) {
		if (where != NULL)
			*where = mountpoint;
		else
			free(mountpoint);
		return (B_TRUE);
	} else {
		free(mountpoint);
		return (B_FALSE);
	}
}

/*
 * Share the given filesystem according to the options in 'sharenfs'.  We rely
 * on share(1M) to the dirty work for us.
 */
int
zfs_share_nfs(zfs_handle_t *zhp)
{
	char mountpoint[ZFS_MAXPROPLEN];
	char shareopts[ZFS_MAXPROPLEN];
	char buf[MAXPATHLEN];
	FILE *fp;
	libzfs_handle_t *hdl = zhp->zfs_hdl;

	if (!zfs_is_mountable(zhp, mountpoint, sizeof (mountpoint), NULL))
		return (0);

	/*
	 * Return success if there are no share options.
	 */
	if (zfs_prop_get(zhp, ZFS_PROP_SHARENFS, shareopts, sizeof (shareopts),
	    NULL, NULL, 0, B_FALSE) != 0 ||
	    strcmp(shareopts, "off") == 0)
		return (0);

#ifndef	__APPLE__
	/*
	 * If the 'zoned' property is set, then zfs_is_mountable() will have
	 * already bailed out if we are in the global zone.  But local
	 * zones cannot be NFS servers, so we ignore it for local zones as well.
	 */
	if (zfs_prop_get_int(zhp, ZFS_PROP_ZONED))
		return (0);
#endif	/*!__APPLE__*/

	/*
	 * Invoke the share(1M) command.  We always do this, even if it's
	 * currently shared, as the options may have changed.
	 */
	if (strcmp(shareopts, "on") == 0)
		(void) snprintf(buf, sizeof (buf), "/usr/sbin/share "
		    "-F nfs \"%s\" 2>&1", mountpoint);
	else
		(void) snprintf(buf, sizeof (buf), "/usr/sbin/share "
		    "-F nfs -o \"%s\" \"%s\" 2>&1", shareopts,
		    mountpoint);

	if ((fp = popen(buf, "r")) == NULL)
		return (zfs_error_fmt(hdl, EZFS_SHARENFSFAILED,
		    dgettext(TEXT_DOMAIN, "cannot share '%s'"),
		    zfs_get_name(zhp)));

	/*
	 * share(1M) should only produce output if there is some kind
	 * of error.  All output begins with "share_nfs: ", so we trim
	 * this off to get to the real error.
	 */
	if (fgets(buf, sizeof (buf), fp) != NULL) {
		char *colon = strchr(buf, ':');

		while (buf[strlen(buf) - 1] == '\n')
			buf[strlen(buf) - 1] = '\0';

		if (colon != NULL)
			zfs_error_aux(hdl, colon + 2);

		(void) zfs_error_fmt(hdl, EZFS_SHARENFSFAILED,
		    dgettext(TEXT_DOMAIN, "cannot share '%s'"),
		    zfs_get_name(zhp));

		verify(pclose(fp) != 0);
		return (-1);
	}

	verify(pclose(fp) == 0);

	return (0);
}

/*
 * Unshare a filesystem by mountpoint.
 */
static int
unshare_one(libzfs_handle_t *hdl, const char *name, const char *mountpoint)
{
	char buf[MAXPATHLEN];
	FILE *fp;

	(void) snprintf(buf, sizeof (buf),
	    "/usr/sbin/unshare  \"%s\" 2>&1",
	    mountpoint);

	if ((fp = popen(buf, "r")) == NULL)
		return (zfs_error_fmt(hdl, EZFS_UNSHARENFSFAILED,
		    dgettext(TEXT_DOMAIN,
		    "cannot unshare '%s'"), name));

	/*
	 * unshare(1M) should only produce output if there is
	 * some kind of error.  All output begins with "unshare
	 * nfs: ", so we trim this off to get to the real error.
	 */
	if (fgets(buf, sizeof (buf), fp) != NULL) {
		char *colon = strchr(buf, ':');

		while (buf[strlen(buf) - 1] == '\n')
			buf[strlen(buf) - 1] = '\0';

		if (colon != NULL)
			zfs_error_aux(hdl, colon + 2);

		verify(pclose(fp) != 0);

		return (zfs_error_fmt(hdl, EZFS_UNSHARENFSFAILED,
		    dgettext(TEXT_DOMAIN,
		    "cannot unshare '%s'"), name));
	}

	verify(pclose(fp) == 0);

	return (0);
}

/*
 * Unshare the given filesystem.
 */
int
zfs_unshare_nfs(zfs_handle_t *zhp, const char *mountpoint)
{
	struct mnttab search = { 0 }, entry;

	/* check to see if need to unmount the filesystem */
	search.mnt_special = (char *)zfs_get_name(zhp);
	search.mnt_fstype = MNTTYPE_ZFS;

#ifndef __APPLE__
	rewind(zhp->zfs_hdl->libzfs_mnttab);
#endif /*!__APPLE__*/

	if (mountpoint != NULL || ((zfs_get_type(zhp) == ZFS_TYPE_FILESYSTEM) &&
	    getmntany(zhp->zfs_hdl->libzfs_mnttab, &entry, &search) == 0)) {

		if (mountpoint == NULL)
			mountpoint = entry.mnt_mountp;

		if (is_shared(zhp->zfs_hdl, mountpoint) &&
		    unshare_one(zhp->zfs_hdl, zhp->zfs_name, mountpoint) != 0)
			return (-1);
	}

	return (0);
}

/*
 * Same as zfs_unmountall(), but for NFS unshares.
 */
int
zfs_unshareall_nfs(zfs_handle_t *zhp)
{
	prop_changelist_t *clp;
	int ret;

	clp = changelist_gather(zhp, ZFS_PROP_SHARENFS, 0);
	if (clp == NULL)
		return (-1);

	ret = changelist_unshare(clp);
	changelist_free(clp);

	return (ret);
}

/*
 * Remove the mountpoint associated with the current dataset, if necessary.
 * We only remove the underlying directory if:
 *
 *	- The mountpoint is not 'none' or 'legacy'
 *	- The mountpoint is non-empty
 *	- The mountpoint is the default or inherited
 *	- The 'zoned' property is set, or we're in a local zone
 *
 * Any other directories we leave alone.
 */
void
remove_mountpoint(zfs_handle_t *zhp)
{
	char mountpoint[ZFS_MAXPROPLEN];
	zfs_source_t source;

	if (!zfs_is_mountable(zhp, mountpoint, sizeof (mountpoint),
	    &source))
		return;

	if (source == ZFS_SRC_DEFAULT ||
	    source == ZFS_SRC_INHERITED) {
		/*
		 * Try to remove the directory, silently ignoring any errors.
		 * The filesystem may have since been removed or moved around,
		 * and this error isn't really useful to the administrator in
		 * any way.
		 */
#ifdef __APPLE__
		{
			char  path[MAXPATHLEN];
			/*
			 * Remove the mount point cookie file.
			 */
			snprintf(path, MAXPATHLEN, "%s/%s", mountpoint, MOUNT_POINT_COOKIE);
			(void) unlink(path);
		}
#endif
		(void) rmdir(mountpoint);
	}
}

boolean_t
zfs_is_shared_iscsi(zfs_handle_t *zhp)
{
#ifdef __APPLE__
	return (0);
#else
	return (iscsitgt_zfs_is_shared != NULL &&
	    iscsitgt_zfs_is_shared(zhp->zfs_name) != 0);
#endif
}

int
zfs_share_iscsi(zfs_handle_t *zhp)
{
	char shareopts[ZFS_MAXPROPLEN];
//	const char *dataset = zhp->zfs_name;
//	libzfs_handle_t *hdl = zhp->zfs_hdl;

	/*
	 * Return success if there are no share options.
	 */
	if (zfs_prop_get(zhp, ZFS_PROP_SHAREISCSI, shareopts,
	    sizeof (shareopts), NULL, NULL, 0, B_FALSE) != 0 ||
	    strcmp(shareopts, "off") == 0)
		return (0);

#ifndef __APPLE__
	if (iscsitgt_zfs_share == NULL || iscsitgt_zfs_share(dataset) != 0)
		return (zfs_error_fmt(hdl, EZFS_SHAREISCSIFAILED,
		    dgettext(TEXT_DOMAIN, "cannot share '%s'"), dataset));
#endif /* !__APPLE__ */

	return (0);
}

int
zfs_unshare_iscsi(zfs_handle_t *zhp)
{
//	const char *dataset = zfs_get_name(zhp);
//	libzfs_handle_t *hdl = zhp->zfs_hdl;

	/*
	 * Return if the volume is not shared
	 */
	if (!zfs_is_shared_iscsi(zhp))
		return (0);

#ifndef __APPLE__
	/*
	 * If this fails with ENODEV it indicates that zvol wasn't shared so
	 * we should return success in that case.
	 */
	if (iscsitgt_zfs_unshare == NULL ||
	    (iscsitgt_zfs_unshare(dataset) != 0 && errno != ENODEV))
		return (zfs_error_fmt(hdl, EZFS_UNSHAREISCSIFAILED,
		    dgettext(TEXT_DOMAIN, "cannot unshare '%s'"), dataset));
#endif /* !__APPLE__ */

	return (0);
}

typedef struct mount_cbdata {
	zfs_handle_t	**cb_datasets;
	int 		cb_used;
	int		cb_alloc;
} mount_cbdata_t;

static int
mount_cb(zfs_handle_t *zhp, void *data)
{
	mount_cbdata_t *cbp = data;

	if (!(zfs_get_type(zhp) & (ZFS_TYPE_FILESYSTEM | ZFS_TYPE_VOLUME))) {
		zfs_close(zhp);
		return (0);
	}

	if (cbp->cb_alloc == cbp->cb_used) {
		void *ptr;

		if ((ptr = zfs_realloc(zhp->zfs_hdl,
		    cbp->cb_datasets, cbp->cb_alloc * sizeof (void *),
		    cbp->cb_alloc * 2 * sizeof (void *))) == NULL)
			return (-1);
		cbp->cb_datasets = ptr;

		cbp->cb_alloc *= 2;
	}

	cbp->cb_datasets[cbp->cb_used++] = zhp;

	return (zfs_iter_children(zhp, mount_cb, cbp));
}

static int
dataset_cmp(const void *a, const void *b)
{
	zfs_handle_t **za = (zfs_handle_t **)a;
	zfs_handle_t **zb = (zfs_handle_t **)b;
	char mounta[MAXPATHLEN];
	char mountb[MAXPATHLEN];
	boolean_t gota, gotb;

	if ((gota = (zfs_get_type(*za) == ZFS_TYPE_FILESYSTEM)) != 0)
		verify(zfs_prop_get(*za, ZFS_PROP_MOUNTPOINT, mounta,
		    sizeof (mounta), NULL, NULL, 0, B_FALSE) == 0);
	if ((gotb = (zfs_get_type(*zb) == ZFS_TYPE_FILESYSTEM)) != 0)
		verify(zfs_prop_get(*zb, ZFS_PROP_MOUNTPOINT, mountb,
		    sizeof (mountb), NULL, NULL, 0, B_FALSE) == 0);

	if (gota && gotb)
		return (strcmp(mounta, mountb));

	if (gota)
		return (-1);
	if (gotb)
		return (1);

	return (strcmp(zfs_get_name(a), zfs_get_name(b)));
}

/*
 * Mount and share all datasets within the given pool.  This assumes that no
 * datasets within the pool are currently mounted.  Because users can create
 * complicated nested hierarchies of mountpoints, we first gather all the
 * datasets and mountpoints within the pool, and sort them by mountpoint.  Once
 * we have the list of all filesystems, we iterate over them in order and mount
 * and/or share each one.
 */
//#pragma weak zpool_mount_datasets = zpool_enable_datasets
int
zpool_enable_datasets(zpool_handle_t *zhp, const char *mntopts, int flags)
{
	mount_cbdata_t cb = { 0 };
	libzfs_handle_t *hdl = zhp->zpool_hdl;
	zfs_handle_t *zfsp;
	int i, ret = -1;

	/*
	 * Gather all datasets within the pool.
	 */
	if ((cb.cb_datasets = zfs_alloc(hdl, 4 * sizeof (void *))) == NULL)
		return (-1);
	cb.cb_alloc = 4;

	if ((zfsp = zfs_open(hdl, zhp->zpool_name, ZFS_TYPE_ANY)) == NULL)
		goto out;

	cb.cb_datasets[0] = zfsp;
	cb.cb_used = 1;

	if (zfs_iter_children(zfsp, mount_cb, &cb) != 0)
		goto out;

	/*
	 * Sort the datasets by mountpoint.
	 */
	qsort(cb.cb_datasets, cb.cb_used, sizeof (void *), dataset_cmp);

	/*
	 * And mount all the datasets.
	 */
	ret = 0;
	for (i = 0; i < cb.cb_used; i++) {
		if (zfs_mount(cb.cb_datasets[i], mntopts, flags) != 0 ||
		    zfs_share(cb.cb_datasets[i]) != 0)
			ret = -1;
	}

out:
	for (i = 0; i < cb.cb_used; i++)
		zfs_close(cb.cb_datasets[i]);
	free(cb.cb_datasets);

	return (ret);
}


static int
zvol_cb(const char *dataset, void *data)
{
	libzfs_handle_t *hdl = data;
	zfs_handle_t *zhp;

	/*
	 * Ignore snapshots and ignore failures from non-existant datasets.
	 */
	if (strchr(dataset, '@') != NULL ||
	    (zhp = zfs_open(hdl, dataset, ZFS_TYPE_VOLUME)) == NULL)
		return (0);

	(void) zfs_unshare_iscsi(zhp);

	zfs_close(zhp);

	return (0);
}

static int
mountpoint_compare(const void *a, const void *b)
{
	const char *mounta = *((char **)a);
	const char *mountb = *((char **)b);

	return (strcmp(mountb, mounta));
}

/*
 * Unshare and unmount all datasets within the given pool.  We don't want to
 * rely on traversing the DSL to discover the filesystems within the pool,
 * because this may be expensive (if not all of them are mounted), and can fail
 * arbitrarily (on I/O error, for example).  Instead, we walk /etc/mnttab and
 * gather all the filesystems that are currently mounted.
 */
//#pragma weak zpool_unmount_datasets = zpool_disable_datasets
int
zpool_disable_datasets(zpool_handle_t *zhp, boolean_t force)
{
	int used, alloc;
#ifdef __APPLE__
	struct statfs *sfsp;
	int nitems;
#else
	struct mnttab entry;
#endif
	size_t namelen;
	char **mountpoints = NULL;
	zfs_handle_t **datasets = NULL;
	libzfs_handle_t *hdl = zhp->zpool_hdl;
	int i;
	int ret = -1;
	int flags = (force ? MNT_FORCE : 0);

	/*
	 * First unshare all zvols.
	 */
	if (zpool_iter_zvol(zhp, zvol_cb, hdl) != 0)
		return (-1);

	namelen = strlen(zhp->zpool_name);

#ifdef __APPLE__
	if ((nitems = getmntinfo(&sfsp, MNT_WAIT)) == 0) {
		return (ret);
	}
	used = alloc = 0;
	while (nitems--) {
		/*
		 * Ignore non-ZFS entries.
		 */
		if (strcmp(sfsp->f_fstypename, MNTTYPE_ZFS) != 0) {
			++sfsp;
			continue;
		}
		/*
		 * Ignore filesystems not within this pool.
		 */
		if (strncmp(sfsp->f_mntfromname, zhp->zpool_name, namelen) != 0 ||
		    (sfsp->f_mntfromname[namelen] != '/' &&
		     sfsp->f_mntfromname[namelen] != '\0')) {
			++sfsp;
			continue;
		}
		/*
		 * At this point we've found a filesystem within our pool.  Add
		 * it to our growing list.
		 */
		if (used == alloc) {
			if (alloc == 0) {
				if ((mountpoints = zfs_alloc(hdl,
				    8 * sizeof (void *))) == NULL)
					goto out;

				if ((datasets = zfs_alloc(hdl,
				    8 * sizeof (void *))) == NULL)
					goto out;

				alloc = 8;
			} else {
				void *ptr;

				if ((ptr = zfs_realloc(hdl, mountpoints,
				    alloc * sizeof (void *),
				    alloc * 2 * sizeof (void *))) == NULL)
					goto out;
				mountpoints = ptr;

				if ((ptr = zfs_realloc(hdl, datasets,
				    alloc * sizeof (void *),
				    alloc * 2 * sizeof (void *))) == NULL)
					goto out;
				datasets = ptr;

				alloc *= 2;
			}
		}

		if ((mountpoints[used] = zfs_strdup(hdl, sfsp->f_mntonname)) == NULL)
			goto out;

		/*
		 * This is allowed to fail, in case there is some I/O error.  It
		 * is only used to determine if we need to remove the underlying
		 * mountpoint, so failure is not fatal.
		 */
		datasets[used] = make_dataset_handle(hdl, sfsp->f_mntfromname);

		used++;
		++sfsp;
	}
#else
	rewind(hdl->libzfs_mnttab);
	used = alloc = 0;
	while (getmntent(hdl->libzfs_mnttab, &entry) == 0) {
		/*
		 * Ignore non-ZFS entries.
		 */
		if (entry.mnt_fstype == NULL ||
		    strcmp(entry.mnt_fstype, MNTTYPE_ZFS) != 0)
			continue;

		/*
		 * Ignore filesystems not within this pool.
		 */
		if (entry.mnt_mountp == NULL ||
		    strncmp(entry.mnt_special, zhp->zpool_name, namelen) != 0 ||
		    (entry.mnt_special[namelen] != '/' &&
		    entry.mnt_special[namelen] != '\0'))
			continue;

		/*
		 * At this point we've found a filesystem within our pool.  Add
		 * it to our growing list.
		 */
		if (used == alloc) {
			if (alloc == 0) {
				if ((mountpoints = zfs_alloc(hdl,
				    8 * sizeof (void *))) == NULL)
					goto out;

				if ((datasets = zfs_alloc(hdl,
				    8 * sizeof (void *))) == NULL)
					goto out;

				alloc = 8;
			} else {
				void *ptr;

				if ((ptr = zfs_realloc(hdl, mountpoints,
				    alloc * sizeof (void *),
				    alloc * 2 * sizeof (void *))) == NULL)
					goto out;
				mountpoints = ptr;

				if ((ptr = zfs_realloc(hdl, datasets,
				    alloc * sizeof (void *),
				    alloc * 2 * sizeof (void *))) == NULL)
					goto out;
				datasets = ptr;

				alloc *= 2;
			}
		}

		if ((mountpoints[used] = zfs_strdup(hdl,
		    entry.mnt_mountp)) == NULL)
			goto out;

		/*
		 * This is allowed to fail, in case there is some I/O error.  It
		 * is only used to determine if we need to remove the underlying
		 * mountpoint, so failure is not fatal.
		 */
		datasets[used] = make_dataset_handle(hdl, entry.mnt_special);

		used++;
	}
#endif

	/*
	 * At this point, we have the entire list of filesystems, so sort it by
	 * mountpoint.
	 */
	qsort(mountpoints, used, sizeof (char *), mountpoint_compare);

	/*
	 * Walk through and first unshare everything.
	 */
	for (i = 0; i < used; i++) {
		if (is_shared(hdl, mountpoints[i]) &&
		    unshare_one(hdl, mountpoints[i], mountpoints[i]) != 0)
			goto out;
	}

	/*
	 * Now unmount everything, removing the underlying directories as
	 * appropriate.
	 */
	for (i = 0; i < used; i++) {
		if (unmount_one(hdl, mountpoints[i], flags) != 0)
			goto out;
	}

	for (i = 0; i < used; i++) {
		if (datasets[i])
			remove_mountpoint(datasets[i]);
	}

	ret = 0;
out:
	for (i = 0; i < used; i++) {
		if (datasets[i])
			zfs_close(datasets[i]);
		free(mountpoints[i]);
	}
	free(datasets);
	free(mountpoints);

	return (ret);
}

#ifdef __APPLE__
int
getmntany(FILE *fp, struct mnttab *mgetp, struct mnttab *mrefp)
{
	struct statfs *sfsp;
	int nitems;

	nitems = getmntinfo(&sfsp, MNT_WAIT);
	
	while (nitems-- > 0) {
		if (strcmp(mrefp->mnt_fstype, sfsp->f_fstypename) == 0 &&
		    strcmp(mrefp->mnt_special, sfsp->f_mntfromname) == 0) {
			mgetp->mnt_special = sfsp->f_mntfromname;
			mgetp->mnt_mountp = sfsp->f_mntonname;
			mgetp->mnt_fstype = sfsp->f_fstypename;
			mgetp->mnt_mntopts = "";
			return (0);
		}
		++sfsp;
	}
	return (-1);
}

char *
mntopt(char **p)
{
	char *cp = *p;
	char *retstr;

	while (*cp && isspace(*cp))
		cp++;

	retstr = cp;
	while (*cp && *cp != ',')
		cp++;

	if (*cp) {
		*cp = '\0';
		cp++;
	}

	*p = cp;
	return (retstr);
}

char *
hasmntopt(struct mnttab *mnt, char *opt)
{
	char tmpopts[MNT_LINE_MAX];
	char *f, *opts = tmpopts;

	if (mnt->mnt_mntopts == NULL)
		return (NULL);
	(void) strcpy(opts, mnt->mnt_mntopts);
	f = mntopt(&opts);
	for (; *f; f = mntopt(&opts)) {
		if (strncmp(opt, f, strlen(opt)) == 0)
			return (f - tmpopts + mnt->mnt_mntopts);
	}
	return (NULL);
}
#endif

