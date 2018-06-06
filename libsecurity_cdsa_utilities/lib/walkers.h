/*
 * Copyright (c) 2000-2004 Apple Computer, Inc. All Rights Reserved.
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


//
// walkers - facilities for traversing and manipulating recursive data structures
//
// Very briefly, this facility allows for deep traversals of (potentially) recursive
// data structures through templated structure "walkers." Standard operations include
// deep copying to a contiguous memory buffer, size calculation, and reconstitution
// after relocation (e.g. via IPC). You can add other operations (e.g. scattered deep
// copy, debug dumping, etc.) by defining operations classes and applying them to the
// existing walkers. You can also extend the reach of the facility to new data structures
// by writing appropriate walker functions for them.
//
// For more detailed rules and regulations, see the accompanying documentation.
//
#ifndef _H_WALKERS
#define _H_WALKERS

#include <security_utilities/alloc.h>
#include <security_utilities/memstreams.h>
#include <security_cdsa_utilities/cssmdata.h>
#include <security_utilities/debugging.h>
#include <set>


namespace Security {
namespace DataWalkers {

#define WALKERDEBUG 1


#if defined(WALKERDEBUG)
# define DEBUGWALK(who)	secdebug("walkers", "walk " who " %s@%p (%ld)", \
									Debug::typeName(addr).c_str(), addr, size)
#else
# define DEBUGWALK(who)	/* nothing */
#endif


//
// Standard operators for sizing, copying, and reinflating
//
class SizeWalker : public LowLevelMemoryUtilities::Writer::Counter {
public:
    template <class T>
	void operator () (T &obj, size_t size = sizeof(T)) { }

    template <class T>
    void operator () (T *addr, size_t size = sizeof(T))
    { DEBUGWALK("size"); LowLevelMemoryUtilities::Writer::Counter::insert(size); }
    
	void blob(void *addr, size_t size)
	{ (*this)(addr, size); }

    void reserve(size_t space)
    { LowLevelMemoryUtilities::Writer::Counter::insert(space); }
    
    static const bool needsRelinking = false;
    static const bool needsSize = true;
};

class CopyWalker : public LowLevelMemoryUtilities::Writer {
public:
    CopyWalker() { }
    CopyWalker(void *base) : LowLevelMemoryUtilities::Writer(base) { }
    
public:
    template <class T>
	void operator () (T &obj, size_t size = sizeof(T))
	{ }

    template <class T>
    void operator () (T * &addr, size_t size = sizeof(T))
    {
		DEBUGWALK("copy");
        if (addr)
            addr = reinterpret_cast<T *>(LowLevelMemoryUtilities::Writer::operator () (addr, size));
    }
    
	template <class T>
	void blob(T * &addr, size_t size)
	{ (*this)(addr, size); }
    
    static const bool needsRelinking = true;
    static const bool needsSize = true;
};

class ReconstituteWalker {
public:
    ReconstituteWalker(off_t offset) : mOffset(offset) { }
    ReconstituteWalker(void *ptr, void *base)
    : mOffset(LowLevelMemoryUtilities::difference(ptr, base)) { }

    template <class T>
	void operator () (T &obj, size_t size = sizeof(T))
	{ }

    template <class T>
    void operator () (T * &addr, size_t size = 0)
    {
		DEBUGWALK("reconstitute");
        if (addr)
            addr = LowLevelMemoryUtilities::increment<T>(addr, mOffset);
    }
    
	template <class T>
	void blob(T * &addr, size_t size)
	{ (*this)(addr, size); }
	
    static const bool needsRelinking = true;
    static const bool needsSize = false;
    
private:
    off_t mOffset;
};

class ChunkCopyWalker {
public:
    ChunkCopyWalker(Allocator &alloc = Allocator::standard()) : allocator(alloc) { }
    
    Allocator &allocator;
    
    template <class T>
	void operator () (T &obj, size_t size = sizeof(T))
	{ }

    template <class T>
    void operator () (T * &addr, size_t size = sizeof(T))
    {
		DEBUGWALK("chunkcopy");
#if BUG_GCC
        T *copy = reinterpret_cast<T *>(allocator.malloc(size));
#else
        T *copy = allocator.malloc<T>(size);
#endif
        memcpy(copy, addr, size);
        addr = copy;
    }
    
	template <class T>
	void blob(T * &addr, size_t size)
	{ (*this)(addr, size); }
	
    static const bool needsRelinking = true;
    static const bool needsSize = true;
};

class ChunkFreeWalker {
public:
    ChunkFreeWalker(Allocator &alloc = Allocator::standard()) : allocator(alloc) { }
    
    Allocator &allocator;
    
    template <class T>
	void operator () (T &obj, size_t size = 0)
	{ }

	template <class T>
    void operator () (T *addr, size_t size = 0)
    {
		DEBUGWALK("chunkfree");
        freeSet.insert(addr);
    }
    
	void blob(void *addr, size_t size)
	{ (*this)(addr, 0); }

    void free();
    ~ChunkFreeWalker() { free(); }
    
    static const bool needsRelinking = false;
    static const bool needsSize = false;

private:
    std::set<void *> freeSet;
};


//
// Stand-alone operations for a single structure web
//
template <class T>
size_t size(T obj)
{
    SizeWalker w;
    walk(w, obj);
    return w;
}

// Special version for const pointer's
template <class T>
size_t size(const T *obj)
{ return size(const_cast<T *>(obj)); }


template <class T>
T *copy(const T *obj, void *addr)
{
    if (obj == NULL)
        return NULL;
    CopyWalker w(addr);
    walk(w, const_cast<T *>(obj));
    return reinterpret_cast<T *>(addr);
}

template <class T>
T *copy(const T *obj, Allocator &alloc = Allocator::standard())
{
    return obj ? copy(obj, alloc, size(obj)) : NULL;
}

template <class T>
T *copy(const T *obj, Allocator &alloc, size_t size)
{
    if (obj == NULL)
        return NULL;
    return copy(obj, alloc.malloc(size));
}

template <class T>
void copy(const T *obj, Allocator &alloc, CssmData &data)
{
    if (obj == NULL) {
        data.Length = 0;
        return;
    }
    if (data.data() == NULL) {
        size_t length = size(obj);
        data = CssmData(alloc.malloc(length), length);
    } else
        assert(size(obj) <= data.length());
    copy(obj, data.data());
}


template <class T>
void relocate(T *obj, T *base)
{
	if (obj) {
		ReconstituteWalker w(LowLevelMemoryUtilities::difference(obj, base));
		walk(w, base);
	}
}


template <class T>
T *chunkCopy(const T *obj, Allocator &alloc = Allocator::standard())
{
	if (obj) {
		ChunkCopyWalker w(alloc);
		return walk(w, const_cast<T *>(obj));
	} else
		return NULL;
}

template <class T>
void chunkFree(const T *obj, Allocator &alloc = Allocator::standard())
{
    if (obj) {
		ChunkFreeWalker w(alloc);
		walk(w, const_cast<T *>(obj));
	}
}

template <class T>
class Copier {
public:
    Copier(const T *obj, Allocator &alloc = Allocator::standard()) : allocator(alloc)
    {
        if (obj == NULL) {
            mValue = NULL;
            mLength = 0;
        } else {
            mLength = size(const_cast<T *>(obj));
#if BUG_GCC
            mValue = reinterpret_cast<T *>(alloc.malloc(mLength));
#else
            mValue = alloc.malloc<T>(mLength);
#endif
            mValue = copy(obj, mValue);
        }
    }
    
    Copier(const T *obj, uint32 count, Allocator &alloc = Allocator::standard())
		: allocator(alloc)
    {
        if (obj == NULL) {
            mValue = NULL;
            mLength = 0;
        } else {
            SizeWalker sizer;
            sizer.reserve(sizeof(T) * count);	// initial vector size
            for (uint32 n = 0; n < count; n++)
                walk(sizer, const_cast<T &>(obj[n]));	// dependent data sizes
            mLength = sizer;
#if BUG_GCC
            mValue = reinterpret_cast<T *>(alloc.malloc(mLength));
#else
            mValue = alloc.malloc<T>(mLength);
#endif
            CopyWalker copier(LowLevelMemoryUtilities::increment(mValue, sizeof(T) * count));
            for (uint32 n = 0; n < count; n++) {
                mValue[n] = obj[n];
                walk(copier, mValue[n]);
            }
        }
    }

    Allocator &allocator;

    ~Copier() { allocator.free(mValue); }
    
	T *value() const		{ return mValue; }
    operator T *() const	{ return value(); }
    size_t length() const	{ return mLength; }
    
    T *keep() { T *result = mValue; mValue = NULL; return result; }	
    
private:
    T *mValue;
    size_t mLength;
};


} // end namespace DataWalkers
} // end namespace Security

#endif //_H_WALKERS
