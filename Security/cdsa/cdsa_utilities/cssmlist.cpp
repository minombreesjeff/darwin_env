/*
 * Copyright (c) 2000-2001 Apple Computer, Inc. All Rights Reserved.
 * 
 * The contents of this file constitute Original Code as defined in and are
 * subject to the Apple Public Source License Version 1.2 (the 'License').
 * You may not use this file except in compliance with the License. Please obtain
 * a copy of the License at http://www.apple.com/publicsource and read it before
 * using this file.
 * 
 * This Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS
 * OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES, INCLUDING WITHOUT
 * LIMITATION, ANY WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT. Please see the License for the
 * specific language governing rights and limitations under the License.
 */


//
// cssmlist - CSSM_LIST operational utilities
//
#ifdef __MWERKS__
#define _CPP_CSSMLIST
#endif

#include <Security/cssmlist.h>


//
// Managing list elements
//
ListElement *ListElement::last()
{
	for (ListElement *p = this; ; p = p->next())
		if (p->next() == NULL)
			return p;
	// not reached
}


ListElement::ListElement(CSSM_WORDID_TYPE word)
{
	ElementType = CSSM_LIST_ELEMENT_WORDID;
	WordID = word;
}

ListElement::ListElement(const CssmData &data)
{
	ElementType = CSSM_LIST_ELEMENT_DATUM;
	WordID = 0;
	Element.Word = data;
}

ListElement::ListElement(CssmAllocator &alloc, string s)
{
    ElementType = CSSM_LIST_ELEMENT_DATUM;
    WordID = 0;
    size_t length = s.size();
    Element.Word = CssmData(memcpy(alloc.alloc<char>(length), s.data(), length), length);
}

ListElement::ListElement(const CssmList &list)
{
	ElementType = CSSM_LIST_ELEMENT_SUBLIST;
	WordID = 0;
	Element.Sublist = list;
}


CssmData &ListElement::data()
{
	assert(type() == CSSM_LIST_ELEMENT_DATUM);
	return CssmData::overlay(Element.Word);
}

const CssmData &ListElement::data() const
{
	assert(type() == CSSM_LIST_ELEMENT_DATUM);
	return CssmData::overlay(Element.Word);
}

CssmList &ListElement::list()
{
	assert(type() == CSSM_LIST_ELEMENT_SUBLIST);
	return CssmList::overlay(Element.Sublist);
}

const CssmList &ListElement::list() const
{
	assert(type() == CSSM_LIST_ELEMENT_SUBLIST);
	return CssmList::overlay(Element.Sublist);
}

CSSM_WORDID_TYPE ListElement::word() const
{
	assert(type() == CSSM_LIST_ELEMENT_WORDID);
	return WordID;
}


//
// List operations
//
ListElement &CssmList::operator [] (unsigned ix) const
{
	for (ListElement *elem = first(); elem; elem = elem->next(), ix--) {
		if (ix == 0)
			return *elem;
	}
	throw 999;	//@@@
}

unsigned int CssmList::length() const
{
	unsigned int len = 0;
	for (ListElement *elem = first(); elem; elem = elem->next())
		len++;
	return len;
}

CssmList &CssmList::append(ListElement *elem)
{
	if (Tail == NULL) {	// first element
		Head = Tail = elem;
	} else {
		Tail->NextElement = elem;
		Tail = elem;
	}
	elem->NextElement = NULL;
	return *this;
}

CssmList &CssmList::insert(ListElement *elem, ListElement *before)
{
	// null before -> append
	if (before == NULL)
		return append(elem);
	
	// we have a real position
	assert(!empty());
	if (Head == before) { // before first element
		elem->NextElement = before;
		Head = elem;
	} else { // before is not first
		for (CSSM_LIST_ELEMENT *p = Head; p; p = p->NextElement) {
			if (p->NextElement == before) {
				elem->NextElement = before;
				p->NextElement = elem;
				return *this;
			}
		}
		// end of list, before not in list
		throw 999;	//@@@
	}
	return *this;
}

CssmList &CssmList::remove(ListElement *elem)
{
	assert(elem);
	if (elem == Head) {	// remove first element
		Head = Head->NextElement;
	} else { // subsequent element
		for (CSSM_LIST_ELEMENT *p = Head; p; p = p->NextElement)
			if (p->NextElement == elem) {
				p->NextElement = elem->NextElement;
				if (elem->NextElement == NULL) // removing last element
					Tail = p;
				return *this;
			}
		// end of list, elem not found
		throw 999;	//@@@
	}
	return *this;
}

void CssmList::snip()
{
    assert(Head);	// can't be empty
    if (Head == Tail) {	// single element, empty when snipped
        Head = Tail = NULL;
    } else {		// more than one, bypass first
        Head = first()->next();
    }
}


//
// Deep-destruction of CssmLists and ListElements.
// The underlying assumption is that all components were allocated from a single
// CssmAllocator in canonical chunks.
//
void ListElement::clear(CssmAllocator &alloc)
{
	switch (type()) {
	case CSSM_LIST_ELEMENT_WORDID:
		break;	// no substructure
	case CSSM_LIST_ELEMENT_DATUM:
		alloc.free(data().data());
		break;
	case CSSM_LIST_ELEMENT_SUBLIST:
		list().clear(alloc);
		break;
	default:
		assert(false);
	}
}

void CssmList::clear(CssmAllocator &alloc)
{
	ListElement *elem = first();
	while (elem) {
		ListElement *next = elem->next();
		destroy(elem, alloc);
		elem = next;
	}
}


//
// Building TypedLists
//
TypedList::TypedList(CssmAllocator &alloc, CSSM_WORDID_TYPE type)
{
	append(new(alloc) ListElement(type));
}

TypedList::TypedList(CssmAllocator &alloc, CSSM_WORDID_TYPE type, ListElement *elem1)
{
	append(new(alloc) ListElement(type));
	append(elem1);
}

TypedList::TypedList(CssmAllocator &alloc, CSSM_WORDID_TYPE type, ListElement *elem1, ListElement *elem2)
{
	append(new(alloc) ListElement(type));
	append(elem1);
	append(elem2);
}


//
// Verify that a TypedList is "proper", i.e. has a first element of WORDID form
//
bool TypedList::isProper() const
{
	return first() && first()->type() == CSSM_LIST_ELEMENT_WORDID;
}
