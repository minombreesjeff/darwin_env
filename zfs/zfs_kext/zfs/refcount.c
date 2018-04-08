/*
 * CDDL HEADER START
 *
 * The contents of this file are subject to the terms of the
 * Common Development and Distribution License, Version 1.0 only
 * (the "License").  You may not use this file except in compliance
 * with the License.
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



#pragma ident	"%Z%%M%	%I%	%E% SMI"

#ifdef _KERNEL

#include <sys/zfs_context.h>
#include <sys/refcount.h>


extern lck_attr_t *  zfs_lock_attr;
extern lck_grp_t *  zfs_spinlock_group;


void
refcount_create(refcount_t *rc)
{
	rc->rc_count = 0;
	lck_spin_init((lck_spin_t *)&rc->rc_spinlck[0],
	              zfs_spinlock_group, zfs_lock_attr);
}

void
refcount_destroy_many(refcount_t *rc, uint64_t number)
{
	ASSERT(rc->rc_count == number);
	rc->rc_count = 0;
	lck_spin_destroy((lck_spin_t *)&rc->rc_spinlck[0], zfs_spinlock_group);
}

void
refcount_destroy(refcount_t *rc)
{
	rc->rc_count = 0;
	lck_spin_destroy((lck_spin_t *)&rc->rc_spinlck[0], zfs_spinlock_group);
}

int
refcount_is_zero(refcount_t *rc)
{
	ASSERT(rc->rc_count >= 0);
	return (rc->rc_count == 0);
}

int64_t
refcount_count(refcount_t *rc)
{
	ASSERT(rc->rc_count >= 0);
	return (rc->rc_count);
}

int64_t
refcount_add_many(refcount_t *rc, uint64_t number, __unused void *holder)
{
	int64_t count;

	lck_spin_lock((lck_spin_t *)&rc->rc_spinlck[0]);
	ASSERT(rc->rc_count >= 0);
	rc->rc_count += number;
	count = rc->rc_count;
	lck_spin_unlock((lck_spin_t *)&rc->rc_spinlck[0]);

	return (count);
}

int64_t
refcount_add(refcount_t *rc, __unused void *holder)
{
	int64_t count;

	lck_spin_lock((lck_spin_t *)&rc->rc_spinlck[0]);
	ASSERT(rc->rc_count >= 0);
	count = ++rc->rc_count;
	lck_spin_unlock((lck_spin_t *)&rc->rc_spinlck[0]);

	return (count);
}

int64_t
refcount_remove_many(refcount_t *rc, uint64_t number, void *holder)
{
	int64_t count;

	lck_spin_lock((lck_spin_t *)&rc->rc_spinlck[0]);

	ASSERT(rc->rc_count >= number);
	rc->rc_count -= number;
	count = rc->rc_count;

	lck_spin_unlock((lck_spin_t *)&rc->rc_spinlck[0]);
	return (count);
}

int64_t
refcount_remove(refcount_t *rc, void *holder)
{
	int64_t count;

	lck_spin_lock((lck_spin_t *)&rc->rc_spinlck[0]);

	ASSERT(rc->rc_count >= 1);
	count = --rc->rc_count;

	lck_spin_unlock((lck_spin_t *)&rc->rc_spinlck[0]);
	return (count);
}

#endif
