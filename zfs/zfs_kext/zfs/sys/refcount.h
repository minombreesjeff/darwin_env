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
 * Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 * Portions Copyright 2007 Apple Inc. All rights reserved.
 * Use is subject to license terms.
 */

#ifndef	_SYS_REFCOUNT_H
#define	_SYS_REFCOUNT_H

#pragma ident	"%Z%%M%	%I%	%E% SMI"

#ifdef _KERNEL

#include <sys/zfs_context.h>
#include <kern/locks.h>

#ifdef	__cplusplus
extern "C" {
#endif

/*
 * If the reference is held only by the calling function and not any
 * particular object, use FTAG (which is a string) for the holder_tag.
 * Otherwise, use the object that holds the reference.
 */
#define	FTAG ((char *)__func__)

/*
 * In Mac OS X we use a spinlock to protect access to rc_count.
 *
 * This is primarily due to the fact that there is no native
 * atomic add 64 support on PPC.
 */
typedef struct refcount {
	uint64_t     rc_count;
	uint32_t     rc_spinlck[10];   /* opaque lck_spin_t data */
} refcount_t;

/* Note: refcount_t should be initialized to zero before use. */

void refcount_create(refcount_t *rc);
void refcount_destroy(refcount_t *rc);
void refcount_destroy_many(refcount_t *rc, uint64_t number);
int refcount_is_zero(refcount_t *rc);
int64_t refcount_count(refcount_t *rc);
int64_t refcount_add(refcount_t *rc, void *holder_tag);
int64_t refcount_remove(refcount_t *rc, void *holder_tag);
int64_t refcount_add_many(refcount_t *rc, uint64_t number, void *holder_tag);
int64_t refcount_remove_many(refcount_t *rc, uint64_t number, void *holder_tag);

#define	refcount_init()
#define	refcount_fini()

#ifdef	__cplusplus
}
#endif

#endif /* _KERNEL */

#endif /* _SYS_REFCOUNT_H */
