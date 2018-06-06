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


// cssmdb.h
//
// classes for the DL related data structures
//

#ifndef _H_CDSA_UTILITIES_CSSMDB
#define _H_CDSA_UTILITIES_CSSMDB

#include <security_cdsa_utilities/cssmdata.h>
#include <security_cdsa_utilities/cssmpods.h>
#include <security_cdsa_utilities/cssmalloc.h>
#include <security_cdsa_utilities/walkers.h>
#include <security_cdsa_utilities/cssmdbname.h>


namespace Security {


//
// Template class to build and maintain external arrays.
// Feel free to add and vector<> member functions and behaviours as needed.
//
// This class differs from vector mainly because it does not construct or
// destruct any of the elements it contains.  Rather it zero fills the
// storage and returns references to elements.
// Also it does not implement insert(), erase() or assign().  It does implement
// which is equivalent to calling *insert(end()) on a vector.
//
template <class _Tp>
class ArrayBuilder {
public:
	typedef _Tp value_type;
	typedef value_type* pointer;
	typedef const value_type* const_pointer;
	typedef value_type* iterator;
	typedef const value_type* const_iterator;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef uint32 size_type;
	typedef ptrdiff_t difference_type;

	typedef reverse_iterator<const_iterator> const_reverse_iterator;
	typedef reverse_iterator<iterator> reverse_iterator;

protected:
  void insert_aux(iterator __position, const _Tp& __x);
  void insert_aux(iterator __position);

public:
	iterator begin() { return mArray; }
	const_iterator begin() const { return mArray; }
	iterator end() { return &mArray[mSize]; }
	const_iterator end() const { return &mArray[mSize]; }
	
	reverse_iterator rbegin()
	{ return reverse_iterator(end()); }
	const_reverse_iterator rbegin() const
	{ return const_reverse_iterator(end()); }
	reverse_iterator rend()
	{ return reverse_iterator(begin()); }
	const_reverse_iterator rend() const
	{ return const_reverse_iterator(begin()); }

	// Must be defined in base class.
	//size_type size() const
	//{ return mSize; }
	size_type max_size() const
	{ return size_type(-1) / sizeof(_Tp); }
	size_type capacity() const
	{ return mCapacity; }
	bool empty() const
	{ return begin() == end(); }

	ArrayBuilder(pointer &array, size_type &size, size_type capacity = 0, Allocator &allocator = Allocator::standard()) :
	mArray(array), mSize(size), mCapacity(capacity), mAllocator(allocator)
	{
#if BUG_GCC
		mArray = reinterpret_cast<pointer>(mAllocator.malloc(sizeof(value_type) * mCapacity));
#else
		mArray = reinterpret_cast<pointer>(mAllocator.malloc(sizeof(value_type) * mCapacity));
		//mArray = mAllocator.alloc(mCapacity);
#endif
		memset(mArray, 0, sizeof(value_type) * mCapacity);
		mSize = 0;
	}
	~ArrayBuilder() { mAllocator.free(mArray); }

	reference front() { return *begin(); }
	const_reference front() const { return *begin(); }
	reference back() { return *(end() - 1); }
	const_reference back() const { return *(end() - 1); }

	void reserve(size_type newCapacity)
	{
		if (newCapacity > mCapacity)
		{
#if BUG_GCC
			mArray = reinterpret_cast<pointer>(mAllocator.realloc(mArray, sizeof(value_type) * newCapacity));
#else
			mArray = reinterpret_cast<pointer>(mAllocator.realloc(mArray, sizeof(value_type) * newCapacity));
			//mArray = mAllocator.realloc<value_type>(mArray, newCapacity));
#endif
			memset(&mArray[mCapacity], 0, sizeof(value_type) * (newCapacity - mCapacity));
			mCapacity = newCapacity;
		}
	}

	// XXX Replace by push_back and insert.
	reference add()
	{
		if (mSize >= mCapacity)
			reserve(max(mSize + 1, mCapacity ? 2 * mCapacity : 1));

		return mArray[mSize++];
	}

	const_pointer get() const { return mArray; }
	pointer release() { const_pointer array = mArray; mArray = NULL; return array; }
	void clear() { if (mSize) { memset(mArray, 0, sizeof(value_type) * mSize); } mSize = 0; }

	// Must be defined in base class.
	//reference at(size_type ix) { return mArray[ix]; }
	//const_reference at(size_type ix) const { return mArray[ix]; }
	//reference operator[] (size_type ix) { assert(ix < size()); return at(ix); }
	//const_reference operator[] (size_type ix) const { assert(ix < size()); return at(ix); }
protected:
	Allocator &allocator() const { return mAllocator; }

private:

	pointer &mArray;
	size_type &mSize;
	size_type mCapacity;
	Allocator &mAllocator;
};


//
// CssmDLPolyData
//
class CssmDLPolyData
{
public:
	CssmDLPolyData(const CSSM_DATA &data, CSSM_DB_ATTRIBUTE_FORMAT format)
	: mData(CssmData::overlay(data)), mFormat(format) {}

	// @@@ Don't use assert, but throw an exception.
	// @@@ Do a size check on mData as well.

	// @@@ This method is dangerous since the returned string is not guaranteed to be zero terminated.
	operator const char *() const
	{
		assert(mFormat == CSSM_DB_ATTRIBUTE_FORMAT_STRING
               || mFormat == CSSM_DB_ATTRIBUTE_FORMAT_TIME_DATE);
		return reinterpret_cast<const char *>(mData.Data);
	}
	operator bool() const
	{
		assert(mFormat == CSSM_DB_ATTRIBUTE_FORMAT_UINT32 || mFormat == CSSM_DB_ATTRIBUTE_FORMAT_SINT32);
		return *reinterpret_cast<uint32 *>(mData.Data);
	}
	operator uint32() const
	{
		assert(mFormat == CSSM_DB_ATTRIBUTE_FORMAT_UINT32);
		return *reinterpret_cast<uint32 *>(mData.Data);
	}
	operator const uint32 *() const
	{
		assert(mFormat == CSSM_DB_ATTRIBUTE_FORMAT_MULTI_UINT32);
		return reinterpret_cast<const uint32 *>(mData.Data);
	}
	operator sint32() const
	{
		assert(mFormat == CSSM_DB_ATTRIBUTE_FORMAT_SINT32);
		return *reinterpret_cast<sint32 *>(mData.Data);
	}
	operator double() const
	{
		assert(mFormat == CSSM_DB_ATTRIBUTE_FORMAT_REAL);
		return *reinterpret_cast<double *>(mData.Data);
	}
	operator CSSM_DATE () const;
	operator Guid () const;
	operator const CssmData &() const
	{
		return mData;
	}

private:
	const CssmData &mData;
	CSSM_DB_ATTRIBUTE_FORMAT mFormat;
};


//
// CssmDbAttributeInfo pod wrapper for CSSM_DB_ATTRIBUTE_INFO
//
class CssmDbAttributeInfo : public PodWrapper<CssmDbAttributeInfo, CSSM_DB_ATTRIBUTE_INFO>
{
public:
	CssmDbAttributeInfo(const CSSM_DB_ATTRIBUTE_INFO &attr)
	{ (CSSM_DB_ATTRIBUTE_INFO &)*this = attr; }

	CSSM_DB_ATTRIBUTE_NAME_FORMAT nameFormat() const { return AttributeNameFormat; }
	void nameFormat(CSSM_DB_ATTRIBUTE_NAME_FORMAT nameFormat) { AttributeNameFormat = nameFormat; }

	CSSM_DB_ATTRIBUTE_FORMAT format() const { return AttributeFormat; }
	void format(CSSM_DB_ATTRIBUTE_FORMAT format) { AttributeFormat = format; }

	operator const char *() const
	{
		assert(nameFormat() == CSSM_DB_ATTRIBUTE_NAME_AS_STRING);
		return Label.AttributeName;
	}
	operator const CssmOid &() const
	{
		assert(nameFormat() == CSSM_DB_ATTRIBUTE_NAME_AS_OID);
		return CssmOid::overlay(Label.AttributeOID);
	}
	operator uint32() const
	{
		assert(nameFormat() == CSSM_DB_ATTRIBUTE_NAME_AS_INTEGER);
		return Label.AttributeID;
	}

	bool operator <(const CssmDbAttributeInfo& other) const;
	bool operator ==(const CssmDbAttributeInfo& other) const;
	bool operator !=(const CssmDbAttributeInfo& other) const
	{ return !(*this == other); }

	// XXX Add setting member functions.
};

//
// CssmDbRecordAttributeInfo pod wrapper for CSSM_DB_RECORD_ATTRIBUTE_INFO
//
class CssmDbRecordAttributeInfo : public PodWrapper<CssmDbRecordAttributeInfo, CSSM_DB_RECORD_ATTRIBUTE_INFO>
{
public:
	CssmDbRecordAttributeInfo()
	{ DataRecordType = CSSM_DL_DB_RECORD_ANY; }
	
	CssmDbRecordAttributeInfo(CSSM_DB_RECORDTYPE recordType, uint32 numberOfAttributes,
							  CSSM_DB_ATTRIBUTE_INFO_PTR attributeInfo)
	{
		DataRecordType = recordType;
		NumberOfAttributes = numberOfAttributes;
		AttributeInfo = attributeInfo;
	}

	CSSM_DB_RECORDTYPE recordType() const { return DataRecordType; }
	void recordType(CSSM_DB_RECORDTYPE recordType) { DataRecordType = recordType; }

	uint32 size() const { return NumberOfAttributes; }

	// Attributes by position
    CssmDbAttributeInfo &at(uint32 ix)
	{ return CssmDbAttributeInfo::overlay(AttributeInfo[ix]); }
    const CssmDbAttributeInfo &at(uint32 ix) const
	{ return CssmDbAttributeInfo::overlay(AttributeInfo[ix]); }

    CssmDbAttributeInfo &operator [](uint32 ix)
    { assert(ix < size()); return at(ix); }
    const CssmDbAttributeInfo &operator [](uint32 ix) const
    { assert(ix < size()); return at(ix); }
};

//
// CssmAutoDbRecordAttributeInfo pod wrapper for CSSM_DB_RECORD_ATTRIBUTE_INFO
//
class CssmAutoDbRecordAttributeInfo: public CssmDbRecordAttributeInfo, public ArrayBuilder<CssmDbAttributeInfo>
{
public:
	CssmAutoDbRecordAttributeInfo(uint32 capacity = 0, Allocator &allocator = Allocator::standard()) :
	CssmDbRecordAttributeInfo(),
	ArrayBuilder<CssmDbAttributeInfo>(CssmDbAttributeInfo::overlayVar(AttributeInfo),
									  NumberOfAttributes, capacity, allocator) {}
};


//
// CssmDbAttributeData pod wrapper for CSSM_DB_ATTRIBUTE_DATA
//
class CssmDbAttributeData : public PodWrapper<CssmDbAttributeData, CSSM_DB_ATTRIBUTE_DATA>
{
public:
	CssmDbAttributeData() { NumberOfValues = 0; Value = NULL; }
	CssmDbAttributeData(const CSSM_DB_ATTRIBUTE_DATA &attr)
	{ (CSSM_DB_ATTRIBUTE_DATA &)*this = attr; }
	CssmDbAttributeData(const CSSM_DB_ATTRIBUTE_INFO &info)
	{ Info = info; NumberOfValues = 0; Value = NULL; }

	CssmDbAttributeInfo &info() { return CssmDbAttributeInfo::overlay(Info); }
	const CssmDbAttributeInfo &info() const { return CssmDbAttributeInfo::overlay(Info); }
	void info (const CSSM_DB_ATTRIBUTE_INFO &inInfo) { Info = inInfo; }

	CSSM_DB_ATTRIBUTE_FORMAT format() const { return info().format(); }

	uint32 size() const { return NumberOfValues; }

	template <class T>
    T at(unsigned int ix) const { return CssmDLPolyData(Value[ix], format()); }

	template <class T>
    T operator [](unsigned int ix) const
	{ if (ix >= size()) CssmError::throwMe(CSSMERR_DL_MISSING_VALUE); return at(ix); }

	// this is intentionally unspecified since it could lead to bugs; the
	// data is not guaranteed to be NULL-terminated
	// operator const char *() const;

	// XXX Don't use assert, but throw an exception.
	operator string() const
	{
		if (size() < 1) CssmError::throwMe(CSSMERR_DL_MISSING_VALUE);
		assert(format() == CSSM_DB_ATTRIBUTE_FORMAT_STRING);
		return Value[0].Length ? string(reinterpret_cast<const char *>(Value[0].Data), Value[0].Length) : string();
	}		
	operator bool() const
	{
		if (size() < 1) CssmError::throwMe(CSSMERR_DL_MISSING_VALUE);
		assert(format() == CSSM_DB_ATTRIBUTE_FORMAT_UINT32 || format() == CSSM_DB_ATTRIBUTE_FORMAT_SINT32);
		return *reinterpret_cast<uint32 *>(Value[0].Data);
	}
	operator uint32() const
	{
		if (size() < 1) CssmError::throwMe(CSSMERR_DL_MISSING_VALUE);
		assert(format() == CSSM_DB_ATTRIBUTE_FORMAT_UINT32);
		return *reinterpret_cast<uint32 *>(Value[0].Data);
	}
	operator const uint32 *() const
	{
		if (size() < 1) CssmError::throwMe(CSSMERR_DL_MISSING_VALUE);
		assert(format() == CSSM_DB_ATTRIBUTE_FORMAT_MULTI_UINT32);
		return reinterpret_cast<const uint32 *>(Value[0].Data);
	}
	operator sint32() const
	{
		if (size() < 1) CssmError::throwMe(CSSMERR_DL_MISSING_VALUE);
		assert(format() == CSSM_DB_ATTRIBUTE_FORMAT_SINT32);
		return *reinterpret_cast<sint32 *>(Value[0].Data);
	}
	operator double() const
	{
		if (size() < 1) CssmError::throwMe(CSSMERR_DL_MISSING_VALUE);
		assert(format() == CSSM_DB_ATTRIBUTE_FORMAT_REAL);
		return *reinterpret_cast<double *>(Value[0].Data);
	}
	operator CssmData &() const
	{
		if (size() < 1) CssmError::throwMe(CSSMERR_DL_MISSING_VALUE);
		assert(format() == CSSM_DB_ATTRIBUTE_FORMAT_STRING
				|| format() == CSSM_DB_ATTRIBUTE_FORMAT_BIG_NUM
				|| format() == CSSM_DB_ATTRIBUTE_FORMAT_TIME_DATE
				|| format() == CSSM_DB_ATTRIBUTE_FORMAT_BLOB
				|| format() == CSSM_DB_ATTRIBUTE_FORMAT_MULTI_UINT32);
		return CssmData::overlay(Value[0]);
	}

	// Set the value of this Attr (assuming it was not set before).
	void set(const CSSM_DB_ATTRIBUTE_INFO &inInfo, const CssmPolyData &inValue,
			 Allocator &inAllocator)
	{
		info(inInfo);
		NumberOfValues = 0;
		Value = inAllocator.alloc<CSSM_DATA>();
		Value[0].Length = 0;
		Value[0].Data = inAllocator.alloc<uint8>(inValue.Length);
		Value[0].Length = inValue.Length;
		memcpy(Value[0].Data, inValue.Data, inValue.Length);
		NumberOfValues = 1;
	}

	// Set the value of this Attr (assuming it was not set before).
	void set(const CSSM_DB_ATTRIBUTE_DATA &other, Allocator &inAllocator)
	{
		info(other.Info);
		Value = inAllocator.alloc<CSSM_DATA>(other.NumberOfValues);
		NumberOfValues = other.NumberOfValues;
		for (NumberOfValues = 0; NumberOfValues < other.NumberOfValues; NumberOfValues++)
		{
			Value[NumberOfValues].Length = 0;
			Value[NumberOfValues].Data = inAllocator.alloc<uint8>(other.Value[NumberOfValues].Length);
			Value[NumberOfValues].Length = other.Value[NumberOfValues].Length;
			memcpy(Value[NumberOfValues].Data, other.Value[NumberOfValues].Data,
				   other.Value[NumberOfValues].Length);
		}
	}

	// Add a value to this attribute.
	void add(const CssmPolyData &inValue, Allocator &inAllocator)
	{
		Value = reinterpret_cast<CSSM_DATA *>(inAllocator.realloc(Value, sizeof(*Value) * (NumberOfValues + 1)));
		Value[NumberOfValues].Length = 0;
		Value[NumberOfValues].Data = inAllocator.alloc<uint8>(inValue.Length);
		Value[NumberOfValues].Length = inValue.Length;
		memcpy(Value[NumberOfValues++].Data, inValue.Data, inValue.Length);
	}

	void add(const CssmDbAttributeData &src, Allocator &inAllocator);

	// delete specific values if they are present in this attribute data
	bool deleteValue(const CssmData &src, Allocator &inAllocator);
	void deleteValues(const CssmDbAttributeData &src, Allocator &inAllocator);

	void deleteValues(Allocator &inAllocator);

	bool operator <(const CssmDbAttributeData& other) const;
};


//
// CssmDbRecordAttributeData pod wrapper for CSSM_DB_RECORD_ATTRIBUTE_DATA
//
class CssmDbRecordAttributeData : public PodWrapper<CssmDbRecordAttributeData, CSSM_DB_RECORD_ATTRIBUTE_DATA>
{
public:
	CssmDbRecordAttributeData()
	{ DataRecordType = CSSM_DL_DB_RECORD_ANY; SemanticInformation = 0; }

	CSSM_DB_RECORDTYPE recordType() const { return DataRecordType; }
	void recordType(CSSM_DB_RECORDTYPE recordType) { DataRecordType = recordType; }

	uint32 semanticInformation() const { return SemanticInformation; }
	void semanticInformation(uint32 semanticInformation) { SemanticInformation = semanticInformation; }

	uint32 size() const { return NumberOfAttributes; }

	// Attributes by position
    CssmDbAttributeData &at(unsigned int ix)
	{ return CssmDbAttributeData::overlay(AttributeData[ix]); }
    const CssmDbAttributeData &at(unsigned int ix) const
	{ return CssmDbAttributeData::overlay(AttributeData[ix]); }

    CssmDbAttributeData &operator [](unsigned int ix)
    { assert(ix < size()); return at(ix); }
    const CssmDbAttributeData &operator [](unsigned int ix) const
    { assert(ix < size()); return at(ix); }

    void deleteValues(Allocator &allocator)
	{ for (uint32 ix = 0; ix < size(); ++ix) at(ix).deleteValues(allocator); }

	CssmDbAttributeData *find(const CSSM_DB_ATTRIBUTE_INFO &inInfo);

	bool operator <(const CssmDbRecordAttributeData& other) const;
};


//
// CssmAutoDbRecordAttributeData
//
class CssmAutoDbRecordAttributeData : public CssmDbRecordAttributeData, public ArrayBuilder<CssmDbAttributeData>
{
public:
	CssmAutoDbRecordAttributeData(uint32 capacity = 0,
								  Allocator &valueAllocator = Allocator::standard(),
								  Allocator &dataAllocator = Allocator::standard()) :
	CssmDbRecordAttributeData(),
	ArrayBuilder<CssmDbAttributeData>(CssmDbAttributeData::overlayVar(AttributeData),
									  NumberOfAttributes, capacity, dataAllocator),
	mValueAllocator(valueAllocator) {}
	~CssmAutoDbRecordAttributeData();

	void clear();
    void deleteValues() { CssmDbRecordAttributeData::deleteValues(mValueAllocator); }

	CssmDbAttributeData &add() { return ArrayBuilder<CssmDbAttributeData>::add(); } // XXX using doesn't work here.
	CssmDbAttributeData &add(const CSSM_DB_ATTRIBUTE_INFO &info);
	CssmDbAttributeData &add(const CSSM_DB_ATTRIBUTE_INFO &info, const CssmPolyData &value);

	// So clients can pass this as the allocator argument to add()
	operator Allocator &() const { return mValueAllocator; }
private:
	Allocator &mValueAllocator;
	
	CssmDbAttributeData* findAttribute (const CSSM_DB_ATTRIBUTE_INFO &info);
	CssmDbAttributeData& getAttributeReference (const CSSM_DB_ATTRIBUTE_INFO &info);
};


//
// CssmSelectionPredicate a PodWrapper for CSSM_SELECTION_PREDICATE
//
class CssmSelectionPredicate : public PodWrapper<CssmSelectionPredicate, CSSM_SELECTION_PREDICATE> {
public:
	CssmSelectionPredicate() { /*IFDEBUG(*/ memset(this, 0, sizeof(*this)) /*)*/ ; }

	CSSM_DB_OPERATOR dbOperator() const { return DbOperator; }
	void dbOperator(CSSM_DB_OPERATOR dbOperator) { DbOperator = dbOperator; }

	CssmSelectionPredicate(CSSM_DB_OPERATOR inDbOperator)
	{ dbOperator(inDbOperator); Attribute.NumberOfValues = 0; Attribute.Value = NULL; }

	CssmDbAttributeData &attribute() { return CssmDbAttributeData::overlay(Attribute); }
	const CssmDbAttributeData &attribute() const { return CssmDbAttributeData::overlay(Attribute); }

	// Set the value of this CssmSelectionPredicate (assuming it was not set before).
	void set(const CSSM_DB_ATTRIBUTE_INFO &inInfo,
			 const CssmPolyData &inValue, Allocator &inAllocator)
	{ attribute().set(inInfo, inValue, inAllocator); }

	// Set the value of this CssmSelectionPredicate using another CssmSelectionPredicate's value.
	void set(const CSSM_SELECTION_PREDICATE &other, Allocator &inAllocator)
	{ DbOperator = other.DbOperator; attribute().set(other.Attribute, inAllocator); }

	// Add a value to the list of values for this CssmSelectionPredicate.
	void add(const CssmPolyData &inValue, Allocator &inAllocator)
	{ attribute().add(inValue, inAllocator); }

	void deleteValues(Allocator &inAllocator) { attribute().deleteValues(inAllocator); }
};

class CssmQuery : public PodWrapper<CssmQuery, CSSM_QUERY> {
public:
    CssmQuery()
    { memset(this, 0, sizeof(*this)) ; RecordType = CSSM_DL_DB_RECORD_ANY; }
    //CssmDLQuery(const CSSM_QUERY &q) { memcpy(this, &q, sizeof(*this)); }

    //CssmDLQuery &operator = (const CSSM_QUERY &q)
    //{ memcpy(this, &q, sizeof(*this)); return *this; }

	CSSM_DB_RECORDTYPE recordType() const { return RecordType; }
	void recordType(CSSM_DB_RECORDTYPE recordType)  { RecordType = recordType; }

	CSSM_DB_CONJUNCTIVE conjunctive() const { return Conjunctive; }
	void conjunctive(CSSM_DB_CONJUNCTIVE conjunctive)  { Conjunctive = conjunctive; }

	CSSM_QUERY_LIMITS queryLimits() const { return QueryLimits; }
	void queryLimits(CSSM_QUERY_LIMITS queryLimits)  { QueryLimits = queryLimits; }

	CSSM_QUERY_FLAGS queryFlags() const { return QueryFlags; }
	void queryFlags(CSSM_QUERY_FLAGS queryFlags)  { QueryFlags = queryFlags; }

	uint32 size() const { return NumSelectionPredicates; }

	CssmSelectionPredicate &at(uint32 ix)
	{ return CssmSelectionPredicate::overlay(SelectionPredicate[ix]); }
	const CssmSelectionPredicate &at(uint32 ix) const
	{ return CssmSelectionPredicate::overlay(SelectionPredicate[ix]); }

	CssmSelectionPredicate &operator[] (uint32 ix) { assert(ix < size()); return at(ix); }
	const CssmSelectionPredicate &operator[] (uint32 ix) const { assert(ix < size()); return at(ix); }

    void deleteValues(Allocator &allocator)
	{ for (uint32 ix = 0; ix < size(); ++ix) at(ix).deleteValues(allocator); }
};


class CssmAutoQuery : public CssmQuery, public ArrayBuilder<CssmSelectionPredicate> {
public:
	CssmAutoQuery(const CSSM_QUERY &query, Allocator &allocator = Allocator::standard());
	CssmAutoQuery(uint32 capacity = 0, Allocator &allocator = Allocator::standard()) :
	ArrayBuilder<CssmSelectionPredicate>(CssmSelectionPredicate::overlayVar(SelectionPredicate),
										 NumSelectionPredicates,
										 capacity, allocator) {}
	~CssmAutoQuery();
	void clear();
    void deleteValues() { CssmQuery::deleteValues(allocator()); }

	CssmSelectionPredicate &add() { return ArrayBuilder<CssmSelectionPredicate>::add(); }
	CssmSelectionPredicate &add(CSSM_DB_OPERATOR dbOperator, const CSSM_DB_ATTRIBUTE_INFO &info, const CssmPolyData &value);

	// So clients can pass this as the allocator argument to add()
	operator Allocator &() const { return allocator(); }
};


//
// DLDbIdentifier
//
class DLDbIdentifier
{
protected:
    class Impl : public RefCount
    {
        NOCOPY(Impl)
    public:
        Impl(const CSSM_SUBSERVICE_UID &ssuid,const char *DbName,const CSSM_NET_ADDRESS *DbLocation) :
            mCssmSubserviceUid(ssuid),mDbName(DbName,DbLocation) {}

        ~Impl() {} // Must be public since RefPointer uses it.

        // Accessors
        const CssmSubserviceUid &ssuid() const { return mCssmSubserviceUid; }
        const char *dbName() const { return mDbName.dbName().c_str(); }
        const CssmNetAddress *dbLocation() const { return mDbName.dbLocation(); }

        // comparison (simple lexicographic)
        bool operator < (const Impl &other) const
		{ return mCssmSubserviceUid < other.mCssmSubserviceUid ||
			(mCssmSubserviceUid == other.mCssmSubserviceUid && mDbName < other.mDbName); }

        bool operator == (const Impl &other) const
		{ return mCssmSubserviceUid == other.mCssmSubserviceUid && mDbName == other.mDbName; }

    private:
        // Private member variables
        CssmSubserviceUid mCssmSubserviceUid;
        DbName mDbName;
    };

public:
    // Constructors
    DLDbIdentifier() {}
    DLDbIdentifier(const CSSM_SUBSERVICE_UID &ssuid,const char *DbName,const CSSM_NET_ADDRESS *DbLocation)
        : mImpl(new Impl(ssuid, DbName, DbLocation)) {}

	// Conversion Operators
	bool operator !() const { return !mImpl; }
	operator bool() const { return mImpl; }

    // Operators
	bool operator <(const DLDbIdentifier &other) const
	{ return mImpl && other.mImpl ? *mImpl < *other.mImpl : mImpl.get() < other.mImpl.get(); }
	bool operator ==(const DLDbIdentifier &other) const
	{ return mImpl && other.mImpl ? *mImpl == *other.mImpl : mImpl.get() == other.mImpl.get(); }
	DLDbIdentifier &operator =(const DLDbIdentifier &other)
	{ mImpl = other.mImpl; return *this; }

    // Accessors
    const CssmSubserviceUid &ssuid() const { return mImpl->ssuid(); }
    const char *dbName() const { return mImpl->dbName(); }
    const CssmNetAddress *dbLocation() const { return mImpl->dbLocation(); }

    RefPointer<Impl> mImpl;
};

// Wrappers for index-related CSSM objects.

class CssmDbIndexInfo : public PodWrapper<CssmDbIndexInfo, CSSM_DB_INDEX_INFO>
{
public:
	CssmDbIndexInfo(const CSSM_DB_INDEX_INFO &attr)
	{ (CSSM_DB_INDEX_INFO &)*this = attr; }

	CSSM_DB_INDEX_TYPE indexType() const { return IndexType; }
	void indexType(CSSM_DB_INDEX_TYPE indexType) { IndexType = indexType; }

	CSSM_DB_INDEXED_DATA_LOCATION dataLocation() const { return IndexedDataLocation; }
	void dataLocation(CSSM_DB_INDEXED_DATA_LOCATION dataLocation)
	{
		IndexedDataLocation = dataLocation;
	}
	
	const CssmDbAttributeInfo &attributeInfo() const
	{
		return CssmDbAttributeInfo::overlay(Info);
	}
};


namespace DataWalkers
{

//
// DLDbIdentifiers don't walk directly because they have Impl structure and use strings.
// Happily, they are easily transcribed into a walkable form.
//
struct DLDbFlatIdentifier {
    CssmSubserviceUid *uid;		// module reference
    char *name;					// string name
    CssmNetAddress *address;	// optional network address

    DLDbFlatIdentifier(const DLDbIdentifier &ident) :
		uid(const_cast<CssmSubserviceUid *>(&ident.ssuid())),
		name(const_cast<char *>(ident.dbName())),
		address(const_cast<CssmNetAddress *>(ident.dbLocation()))
		{ }

    operator DLDbIdentifier ()	{ return DLDbIdentifier(*uid, name, address); }
};

template<class Action>
DLDbFlatIdentifier *walk(Action &operate, DLDbFlatIdentifier * &ident)
{
    operate(ident);
    if (ident->uid)
        walk(operate, ident->uid);
    walk(operate, ident->name);
    if (ident->address)
        walk(operate, ident->address);
    return ident;
}

template<class Action>
void walk(Action &operate, CSSM_DB_ATTRIBUTE_INFO &info)
{
	operate(info);
	switch (info.AttributeNameFormat)
	{
	case CSSM_DB_ATTRIBUTE_NAME_AS_STRING:
		walk(operate, info.Label.AttributeName);
		break;
	case CSSM_DB_ATTRIBUTE_NAME_AS_OID:
		walk(operate, info.Label.AttributeOID);
		break;
	case CSSM_DB_ATTRIBUTE_NAME_AS_INTEGER:
	default:
		break;
	}
}

template<class Action>
void walk(Action &operate, CSSM_DB_INDEX_INFO &info)
{
	operate(info);
	walk(operate, info.Info);
}

template<class Action>
void walk(Action &operate, CSSM_DB_PARSING_MODULE_INFO &info)
{
	operate(info);
}

template<class Action>
void walk(Action &operate, CSSM_DB_RECORD_ATTRIBUTE_INFO &info)
{
	operate(info);
	operate.blob(info.AttributeInfo, info.NumberOfAttributes * sizeof(*info.AttributeInfo));
	for (uint32 ix = 0; ix < info.NumberOfAttributes; ++ix)
		walk(operate, info.AttributeInfo[ix]);
}

template<class Action>
void walk(Action &operate, CSSM_DB_RECORD_INDEX_INFO &info)
{
	operate(info);
	operate.blob(info.IndexInfo, info.NumberOfIndexes * sizeof(*info.IndexInfo));
	for (uint32 ix = 0; ix < info.NumberOfIndexes; ++ix)
		walk(operate, info.IndexInfo[ix]);
}

template<class Action>
CSSM_DBINFO *walk(Action &operate, CSSM_DBINFO * &info)
{
    operate(info);
	operate.blob(info->DefaultParsingModules, info->DefaultParsingModules ?
		info->NumberOfRecordTypes * sizeof(*info->DefaultParsingModules) : 0);
	if (info->DefaultParsingModules)
	{
		for (uint32 ix = 0; ix < info->NumberOfRecordTypes; ++ix)
			walk(operate, info->DefaultParsingModules[ix]);
	}

	operate.blob(info->RecordAttributeNames, info->RecordAttributeNames ? 
		info->NumberOfRecordTypes * sizeof(*info->RecordAttributeNames) : 0);
	if (info->RecordAttributeNames)
	{
		for (uint32 ix = 0; ix < info->NumberOfRecordTypes; ++ix)
			walk(operate, info->RecordAttributeNames[ix]);
	}

	operate.blob(info->RecordIndexes, info->RecordIndexes ?
		info->NumberOfRecordTypes * sizeof(*info->RecordIndexes) : 0);
	if (info->RecordIndexes)
	{
		for (uint32 ix = 0; ix < info->NumberOfRecordTypes; ++ix)
			walk(operate, info->RecordIndexes[ix]);
	}

	walk(operate, info->AccessPath);
    return info;
}

template<class Action>
CSSM_NAME_LIST_PTR walk(Action &operate, CSSM_NAME_LIST_PTR &nameList)
{
	operate(nameList);
	operate.blob(nameList->String, nameList->NumStrings * sizeof(*nameList->String));
	for (uint32 ix = 0; ix < nameList->NumStrings; ++ix)
		walk(operate, nameList->String[ix]);
	return nameList;
}

template<class Action>
void walk(Action &operate, CSSM_DB_SCHEMA_ATTRIBUTE_INFO &info)
{
	operate(info);
	walk(operate, info.AttributeName);
	walk(operate, info.AttributeNameID);
}

template<class Action>
void walk(Action &operate, CSSM_DB_SCHEMA_INDEX_INFO &info)
{
	operate(info);
}

template<class Action>
void walk(Action &operate, CSSM_DB_ATTRIBUTE_DATA &data)
{
	operate(data);
	operate(data.Info);
	operate.blob(data.Value, data.Value ?
		data.NumberOfValues * sizeof(*data.Value) : 0);
	for (uint32 ix = 0; ix < data.NumberOfValues; ++ix)
		walk(operate, data.Value[ix]);
}

template<class Action>
CSSM_DB_RECORD_ATTRIBUTE_DATA *walk(Action &operate, CSSM_DB_RECORD_ATTRIBUTE_DATA *&data)
{
	operate(data);
	operate.blob(data->AttributeData, data->AttributeData ?
		data->NumberOfAttributes * sizeof(*data->AttributeData) : 0);
	for (uint32 ix = 0; ix < data->NumberOfAttributes; ++ix)
		walk(operate, data->AttributeData[ix]);
	return data;
}

template<class Action>
CssmDbRecordAttributeData *walk(Action &operate, CssmDbRecordAttributeData *&data)
{
	return CssmDbRecordAttributeData::overlay(walk(operate,
		reinterpret_cast<CSSM_DB_RECORD_ATTRIBUTE_DATA *&>(data)));
}

template<class Action>
void walk(Action &operate, CSSM_SELECTION_PREDICATE &predicate)
{
	operate(predicate);
	operate(predicate.Attribute);
}

template<class Action>
CSSM_QUERY_PTR walk(Action &operate, CSSM_QUERY_PTR &query)
{
	operate(query);
	operate.blob(query->SelectionPredicate, query->SelectionPredicate ?
		query->NumSelectionPredicates * sizeof(*query->SelectionPredicate) : 0);
	for (uint32 ix = 0; ix < query->NumSelectionPredicates; ++ix)
		walk(operate, query->SelectionPredicate[ix]);
	return query;
}

template<class Action>
CssmQuery *walk(Action &operate, CssmQuery *&query)
{
	return CssmQuery::overlay(walk(operate,
		reinterpret_cast<CSSM_QUERY *&>(query)));
}

} // end namespace DataWalkers

} // end namespace Security

#ifdef _CPP_UTILITIES
#pragma export off
#endif


#endif // _H_CDSA_UTILITIES_CSSMDB
