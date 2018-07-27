/*  ncbibs.c
* ===========================================================================
*
*                            PUBLIC DOMAIN NOTICE                          
*               National Center for Biotechnology Information
*                                                                          
*  This software/database is a "United States Government Work" under the   
*  terms of the United States Copyright Act.  It was written as part of    
*  the author's official duties as a United States Government employee and 
*  thus cannot be copyrighted.  This software/database is freely available 
*  to the public for use. The National Library of Medicine and the U.S.    
*  Government have not placed any restriction on its use or reproduction.  
*                                                                          
*  Although all reasonable efforts have been taken to ensure the accuracy  
*  and reliability of the software and data, the NLM and the U.S.          
*  Government do not and cannot warrant the performance or results that    
*  may be obtained by using this software or data. The NLM and the U.S.    
*  Government disclaim all warranties, express or implied, including       
*  warranties of performance, merchantability or fitness for any particular
*  purpose.                                                                
*                                                                          
*  Please cite the author in any work or product based on this material.   
*
* ===========================================================================
*
* File Name:  ncbibs.c
*
* Author:  Jim Ostell
*
* Version Creation Date:  3/4/91
*
* $Revision: 6.7 $
*
* File Description:
*   ByteStore functions
*      NOTE: increasing BS_MAXALLOC will require changing len, and
* 		   len_avail to Int4 (ncbibs.h)
*
*   This version needs the following enhancements:
*   1) a BSCompact function to pack a fragmented ByteStore
*   2) use made of MIN_BSALLOC to make all allocated space at
*      least MIN_BSALLOC long, so it would be possible to have len_avail
*      bigger than len at internal BSUnits as well as at the end.  This
*      will make insertion and appending of small amounts much more
*      efficient.
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
* 3/4/91   Kans        Stricter typecasting for GNU C and C++.
* 09-19-91 Schuler     All sizes are expressed as Int4, not Uint4.
* 09-20-91 Schuler     All exported functions are LIBCALL.
* 09-20-91 Schuler     BSGetByte returns EOF on failure.
* 09-20-91 Schuler     BSPutByte(bsp,EOF) truncates bsp at curr. position.
* 04-15-93 Schuler     Changed _cdecl to LIBCALL
*
* $Log: ncbibs.c,v $
* Revision 6.7  2003/01/21 17:56:23  kans
* minor fix in BSAdd, comment that BSAdd does not change totlen
*
* Revision 6.6  2002/02/07 21:52:40  kans
* added cast in Nlm_SwapUint4Buff call
*
* Revision 6.5  2000/05/26 23:34:58  kans
* added BSDupAndSwapUint4 for copying and swapping of UID lists passed over network to BIG_ENDIAN server
*
* Revision 6.4  1999/11/05 19:59:09  beloslyu
* Fixed the error with size_t vs Int4 comparisons
*
* Revision 6.3  1999/08/25 17:43:04  madden
* Removed extra comment starts
*
* Revision 6.2  1999/01/21 20:08:37  ostell
* added SwitchUint2 and 4, added integer bytestores
*
* Revision 6.1  1998/06/11 18:59:58  shavirin
* Fixed some compiler warnings.
*
* Revision 6.0  1997/08/25 18:15:13  madden
* Revision changed to 6.0
*
* Revision 5.1  1996/12/03 21:48:33  vakatov
* Adopted for 32-bit MS-Windows DLLs
*
 * Revision 5.0  1996/05/28  13:18:57  ostell
 * Set to revision 5.0
 *
 * Revision 4.2  1995/12/21  02:33:19  ostell
 * added use_min_size to BSAll
 *
 * Revision 4.1  1995/12/20  22:56:58  ostell
 * started using MIN_BSALLOC in BSAdd()
 * added check for BSAdd failure in BSWrite
 *
 * Revision 4.0  1995/07/26  13:46:50  ostell
 * force revision to 4.0
 *
 * Revision 2.11  1995/05/15  18:45:58  ostell
 * added Log line
 *
*
*
* ==========================================================================
*/

#include <ncbi.h>
#include <ncbiwin.h>

         /* maximum size allocated for BSUnit.str */
#define MAX_BSALLOC 32700
         /* minimum size block to allocate except on new */
#define MIN_BSALLOC 1024

#ifdef MSC_VIRT
extern Nlm_Boolean wrote_to_handle;  /* needed by Microsoft DOS Virtual mem */
#undef MAX_BSALLOC					 /* bad with big blocks */
#define MAX_BSALLOC 10000
#endif

/*****************************************************************************
*
*   Pointer Nlm_BSMerge(bsp, dest)
*      if dest == NULL, allocates storage for merge and returns pointer to it
*
*****************************************************************************/
NLM_EXTERN Nlm_VoidPtr LIBCALL  Nlm_BSMerge (Nlm_ByteStorePtr bsp, Nlm_VoidPtr dest)

{
	Nlm_BytePtr tmp, from;
	Nlm_BSUnitPtr bsup;
    size_t size;

	if (bsp == NULL)
		return NULL;

    size = Nlm_BSLen(bsp) + 1;
    if (size > SIZE_MAX) {
		ErrPostEx(SEV_ERROR, 0, 0, "size[%u] is bigger then SIZE_MAX[%u]",
			size, SIZE_MAX);
        /* should post an error here */
        return NULL;
    }

	if (dest == NULL)         /* allocate storage with room for null at end */
	{
		dest = Nlm_MemNew(size);
		if (dest == NULL) return dest;
	}

	tmp = (Nlm_BytePtr) dest;
	bsup = bsp->chain;

	while (bsup != NULL)
	{
		from = (Nlm_BytePtr) Nlm_HandLock(bsup->str);
		Nlm_MemCopy(tmp, from, bsup->len);
		Nlm_HandUnlock(bsup->str);
		tmp += bsup->len;
		bsup = bsup->next;
	}

	return dest;
}

/*****************************************************************************
*
*   ByteStorePtr Nlm_BSNew(len)
*   	creates a big enough bytestore for len
*
*****************************************************************************/
NLM_EXTERN Nlm_ByteStorePtr LIBCALL Nlm_BSNew (Nlm_Int4 len)

{
	Nlm_ByteStorePtr bsp;
	Nlm_Int4 added;

	bsp = (Nlm_ByteStorePtr) Nlm_MemNew(sizeof(ByteStore));
	if (bsp == NULL)
		return NULL;

	added = Nlm_BSAdd(bsp, len, FALSE);
	if (added < len)
		bsp = Nlm_BSFree(bsp);
	return bsp;
}

/*****************************************************************************
*
*   Int4 BSAdd(bsp, len, use_min_size)
*   	adds len bytes BEFORE current bsp->seekptr
*       bsp->seekptr returned pointing at first added byte
*   	returns bytes added
*       if (use_min_size) then does not add anything smaller than MIN_BSALLOC
*
*****************************************************************************/
NLM_EXTERN Nlm_Int4 LIBCALL Nlm_BSAdd (Nlm_ByteStorePtr bsp, Nlm_Int4 len, Nlm_Boolean use_min_size)

{
	Nlm_BSUnitPtr bsup,      /* current bsunit */
		          ccbsup,    /* bsp->curchain */
				lastbsup,    /* bsunit after added section  */
				prevbsup;    /* bsunit before added section */
	Nlm_BytePtr to, from;
	Nlm_Int4 added = 0,
		      tlen;
	Nlm_Handle thand;

	if ((bsp == NULL) || (len == 0))
		return added;

	lastbsup = NULL;
	prevbsup = NULL;
	ccbsup = bsp->curchain;

	if (bsp->chain != NULL)        /* add or insert in exisiting chain */
	{
		if (bsp->seekptr == bsp->chain_offset)   /* before start of block */
		{
			lastbsup = ccbsup;       /* it comes after */
			if (bsp->seekptr != 0)          /* something before it */
			{
				prevbsup = bsp->chain;
				while (prevbsup->next != ccbsup)
					prevbsup = prevbsup->next;
			}
		}										 /* at very end of data */
		else if (bsp->seekptr == (bsp->chain_offset + ccbsup->len))
		{
			prevbsup = ccbsup; /* JK */
		}										 /* after all blocks */
		else if (bsp->seekptr >= (bsp->chain_offset + ccbsup->len_avail))
		{
			prevbsup = ccbsup;
		}
		else                /* split a bsunit */
		{
			bsup = (Nlm_BSUnitPtr) Nlm_MemNew(sizeof(Nlm_BSUnit));
			if (bsup == NULL) return added;
			if (bsp->chain_offset != 0)          /* not first BSUnit */
			{
				prevbsup = bsp->chain;
				while (prevbsup->next != ccbsup)
					prevbsup = prevbsup->next;
				prevbsup->next = bsup;      /* insert new bsunit */
			}
			else
			    bsp->chain = bsup;
			bsup->next = ccbsup;
			tlen = bsp->seekptr - bsp->chain_offset;  /* len of first half */
			bsup->str = Nlm_HandNew((size_t) tlen);
			if (bsup->str == NULL) return added;
			bsup->len = (Nlm_Int2) tlen;
			bsup->len_avail = (Nlm_Int2) tlen;
			to = (Nlm_BytePtr) Nlm_HandLock(bsup->str);
			thand = ccbsup->str;
			from = (Nlm_BytePtr) Nlm_HandLock(thand);
			Nlm_MemCopy(to, from, (size_t) tlen);
#ifdef MSC_VIRT
			wrote_to_handle = TRUE;
#endif
			Nlm_HandUnlock(bsup->str);
			from += tlen;      /* point past copied region */
			tlen = ccbsup->len - tlen;    /* the last half */
			ccbsup->len = (Nlm_Int2) tlen;
			ccbsup->len_avail = (Nlm_Int2) tlen;
			ccbsup->str = Nlm_HandNew((size_t) tlen);
			if (ccbsup->str == NULL) return added;
			to = (Nlm_BytePtr) Nlm_HandLock(ccbsup->str);
			Nlm_MemCopy(to, from, (size_t) tlen);
#ifdef MSC_VIRT
	wrote_to_handle = TRUE;
#endif
			Nlm_HandUnlock(ccbsup->str);
			Nlm_HandUnlock(thand);
			Nlm_HandFree(thand);
			prevbsup = bsup;
			lastbsup = ccbsup;
		}
	}

	ccbsup = NULL;
	bsup = NULL;
	while (len)
	{
		bsup = (Nlm_BSUnitPtr) Nlm_MemNew(sizeof(Nlm_BSUnit));
		if (bsup == NULL)
			return added;
		if (len < MAX_BSALLOC)

		{
			if ((use_min_size) && (len < MIN_BSALLOC))
				len = MIN_BSALLOC;
			tlen = len;
		}
		else
			tlen = MAX_BSALLOC;
		bsup->str = Nlm_HandNew((size_t)tlen);
		if (bsup->str == NULL)
		{
			Nlm_MemFree(bsup);
			return added;
		}
		bsup->len_avail = (Nlm_Int2) tlen;
		if (prevbsup == NULL)
			bsp->chain = bsup;
		else
			prevbsup->next = bsup;
		if (ccbsup == NULL)
			ccbsup = bsup;          /* current position */
		prevbsup = bsup;
		len -= tlen;
		added += tlen;
	}
	if (bsup != NULL)
		bsup->next = lastbsup;
	bsp->curchain = ccbsup;
	bsp->chain_offset = bsp->seekptr;   /* added block starts at seekptr */
	return added;
}

/*****************************************************************************
*
*   Int4 BSDelete(bsp, len)
*   	deletes len bytes starting at current bsp->seekptr
*   	returns bytes deleted
*
*****************************************************************************/
NLM_EXTERN Nlm_Int4 LIBCALL  Nlm_BSDelete (Nlm_ByteStorePtr bsp, Nlm_Int4 len)

{
	Nlm_BSUnitPtr bsup,      /* current bsunit */
		          ccbsup,    /* bsp->curchain */
				nextbsup,    /* bsunit after added section  */
				prevbsup;    /* bsunit before added section */
	Nlm_BytePtr to, from;
	Nlm_Int4 added = 0,
		      offset,
		      tlen,
			  start,
			  save;
	Nlm_Handle thand;

	if ((bsp == NULL) || (len == 0) || (bsp->chain == NULL) ||
		(bsp->seekptr >= bsp->totlen))
		return added;

	if ((bsp->seekptr + len) > bsp->totlen)   /* deleting too much */
		len = bsp->totlen - bsp->seekptr;

	nextbsup = NULL;
	ccbsup = bsp->curchain;
	start = bsp->chain_offset;
	offset = bsp->seekptr - bsp->chain_offset;
	if (offset)           /* will leave part of first bsunit */
		prevbsup = ccbsup;
	else if (bsp->chain_offset != 0)          /* something before this bsunit */
	{
		prevbsup = bsp->chain;
		while (prevbsup->next != ccbsup)
			prevbsup = prevbsup->next;
	}
	else
		prevbsup = NULL;       /* will remove beginning of first bsunit */

	bsup = ccbsup;
	while (len)
	{
		nextbsup = bsup->next;
		tlen = bsup->len - offset;
		if (tlen > len)
			tlen = len;			 /* will remove tlen bytes from this BSUnit*/
		save = bsup->len - tlen;
		if (save)    /* some bytes left after delete */
		{
			thand = bsup->str;
			bsup->str = Nlm_HandNew((size_t) save);
			if (bsup->str == NULL) return added;
			bsup->len = (Nlm_Int2) save;
			bsup->len_avail = (Nlm_Int2) save;
			to = (Nlm_BytePtr) Nlm_HandLock(bsup->str);
			from = (Nlm_BytePtr) Nlm_HandLock(thand);
			if (offset)         /* get the beginning */
				Nlm_MemCopy(to, from, (size_t) offset);
			save -= offset;
			if (save)           /* save end */
				Nlm_MemCopy((to + offset), (from + (offset + tlen)), (size_t)save);
			Nlm_HandUnlock(thand);
			Nlm_HandFree(thand);
#ifdef MSC_VIRT
	wrote_to_handle = TRUE;
#endif
			Nlm_HandUnlock(bsup->str);
			if (tlen < len)
				bsup = nextbsup;
		}
		else                    /* delete the whole thing */
		{
			Nlm_HandFree(bsup->str);
			Nlm_MemFree(bsup);
			bsup = nextbsup;
		}
		offset = 0;
		len -= tlen;
		added += tlen;
	}
	if ((bsup != prevbsup) ||   /* break in chain, rejoin it */
             ((bsup == NULL) && (prevbsup == NULL)))   /* chain gone */
	{
		if (prevbsup == NULL)
			bsp->chain = bsup;
		else
			prevbsup->next = bsup;
	}
	bsp->curchain = bsup;
	bsp->totlen -= added;
	if (bsp->totlen)
	{
		offset = bsp->seekptr;
		start = 0;
		bsup = bsp->chain;
		while (offset >= (start + bsup->len_avail))
		{
			if (bsup->next == NULL)    /* EOF */
				offset = 0;            /* stop loop */
			else
			{
				start += bsup->len;
				bsup = bsup->next;
			}
		}
		bsp->curchain = bsup;
		bsp->chain_offset = start;
	}
	else
	{
		bsp->chain_offset = 0;
		bsp->curchain = bsp->chain;
	}
	return added;
}

/*****************************************************************************
*
*   Nlm_Int4 Nlm_BSTell(bsp)
*
*****************************************************************************/
NLM_EXTERN Nlm_Int4 LIBCALL  Nlm_BSTell (Nlm_ByteStorePtr bsp)

{
	if (bsp == NULL)
		return 0L;

	return bsp->seekptr;
}

/*****************************************************************************
*
*   Int2 Nlm_BSSeek(bsp, offset, origin)
*   	seeks as fseek()
*   	trys to lock a BSUnit at that seek position
*
*****************************************************************************/
NLM_EXTERN Nlm_Int2 LIBCALL  Nlm_BSSeek (Nlm_ByteStorePtr bsp, Nlm_Int4 offset, Nlm_Int2 origin)

{
	Nlm_Int4 sp, start = 0;
	Nlm_BSUnitPtr bsup;
	Nlm_Boolean done;

	if (bsp == NULL)
		return 1;
	if (bsp->chain == NULL)
		return 1;

	sp = bsp->seekptr;

	switch (origin)
	{
		case SEEK_SET:
			if ((offset > bsp->totlen) || (offset < 0))
				return 1;
			sp = offset;
			break;
		case SEEK_CUR:
			if (((sp + offset) > bsp->totlen) ||
				((sp + offset) < 0 ))
				return 1;
			sp += offset;
			break;
		case SEEK_END:
			if ((ABS(offset) > bsp->totlen) || (offset > 0))
				return 1;
			sp = bsp->totlen + offset;
			break;
		default:
			return 1;
	}

    if (sp == bsp->seekptr)     /* already in right position */
        return 0;

	bsp->seekptr = sp;
	                             /* if a valid seek, lock the right BSUnit */
	bsup = bsp->curchain;

	if (sp == bsp->totlen)    /* seek to EOF */
	{
		if (sp > (bsp->chain_offset + bsup->len_avail))
		{
			start = bsp->chain_offset;
			while (sp > (start + bsup->len_avail))
			{
				start += bsup->len;
				bsup = bsup->next;
			}
		}
	}
	else if ((sp < bsp->chain_offset) ||
		(sp >= (bsp->chain_offset + bsup->len)))
	{
		if (sp < bsp->chain_offset)
		{
			bsup = bsp->chain;
			start = 0;
		}
		else
		{
			start = bsp->chain_offset + bsup->len;
			bsup = bsup->next;
		}
		done = FALSE;
		while (! done)
		{
			if ((start + bsup->len) > sp)
				done = TRUE;
			else
			{
				start += bsup->len;
				bsup = bsup->next;
			}
		}
	}
	if (bsup != bsp->curchain)
	{
		bsp->chain_offset = start;
		bsp->curchain = bsup;
	}
	return 0;
}
/*****************************************************************************
*
*   Int4 Nlm_BSLen(bsp)
*      gives cumulative length of strings in store (not counting \0)
*
*****************************************************************************/
NLM_EXTERN Nlm_Int4 LIBCALL  Nlm_BSLen (Nlm_ByteStorePtr bsp)

{
	if (bsp == NULL)
		return 0L;

	return bsp->totlen;
}

/*****************************************************************************
*
*   Int2 BSPutByte(bsp, value)
*       - returns value on success, EOF on failure
*       - BSPutByte(bsp, EOF) truncates at seekptr
*
*****************************************************************************/
NLM_EXTERN Nlm_Int2 LIBCALL  Nlm_BSPutByte (Nlm_ByteStorePtr bsp, Nlm_Int2 value)

{
	Nlm_Uint1 b;
    if (value == EOF)
        Nlm_BSDelete(bsp, INT4_MAX);
    else {
    	b = (Nlm_Uint1) value;
        if (Nlm_BSWrite(bsp, (Nlm_VoidPtr)&b, 1) == 0)
            return EOF;
    }
    return value;
}

/*****************************************************************************
*
*   Int4 BSWrite(bsp, ptr, len)
*   	returns bytes written from ptr to bsp
*       writes from current seekptr position
*       seekptr left pointing after last byte written
*       allocates storage if needed
*       writes over any data already there
*
*****************************************************************************/
NLM_EXTERN Nlm_Int4 LIBCALL  Nlm_BSWrite (Nlm_ByteStorePtr bsp, Nlm_VoidPtr ptr, Nlm_Int4 len)

{
	Nlm_BytePtr from, to;
	Nlm_BSUnitPtr bsup;
	Nlm_Int4 tlen, added = 0, offset, start, diff;

	if ((bsp == NULL) || (len <= 0))
		return added;

	bsup = bsp->curchain;
	from = (Nlm_BytePtr) ptr;
	offset = bsp->seekptr - bsp->chain_offset;
	start = bsp->chain_offset;



 	while (len)
	{

		if (bsup == NULL)
			tlen = 0;
		else
			tlen = bsup->len_avail - offset;
		if (len < tlen)
			tlen = len;
		if (! tlen)       /* out of space */
		{

			if (Nlm_BSAdd(bsp, len, TRUE) < len)
				return added;
			bsup = bsp->curchain;
			offset = bsp->seekptr - bsp->chain_offset;
			tlen = bsup->len_avail;

			if (len < tlen)

				tlen = len;
		}
		else
		{
			bsp->chain_offset = start;
			bsp->curchain = bsup;
		}
		to = (Nlm_BytePtr)Nlm_HandLock(bsup->str) + offset;
		Nlm_MemCopy(to, from, (size_t) tlen);
#ifdef MSC_VIRT
	wrote_to_handle = TRUE;
#endif
		Nlm_HandUnlock(bsup->str);
		if (bsup->len < (Nlm_Int2)(tlen + offset))   /* added more to this bsunit */
		{
			diff = (tlen + offset) - bsup->len;
			bsp->totlen += diff;
			bsup->len += (Nlm_Int2) diff;

		}
		offset = 0;        /* only offset on first one */
		len -= tlen;
		bsp->seekptr += tlen;
		start += bsup->len;
		added += tlen;
		from += tlen;
		if (len)
			bsup = bsup->next;
	}
	                 /* pointing past end of this BSunit */

	if ((bsp->seekptr - bsp->chain_offset) >= bsup->len_avail)
	{
		if (bsup->next != NULL)      /* not end of space */
		{
			bsp->curchain = bsup->next;
			bsp->chain_offset += bsup->len;  /* should = len_avail */
		}
	}
	return added;
}
/*****************************************************************************
*
*   Int4 BSRead(bsp, ptr, len)
*   	returns bytes read from bsp to ptr
*       reads from current seekptr position
*       seekptr left pointing after last byte read
*
*****************************************************************************/
NLM_EXTERN Nlm_Int4 LIBCALL  Nlm_BSRead (Nlm_ByteStorePtr bsp, Nlm_VoidPtr ptr, Nlm_Int4 len)

{
	Nlm_BytePtr from, to;
	Nlm_BSUnitPtr bsup;
	Nlm_Int4 tlen, added = 0, offset, start;

	if ((bsp == NULL) || (len <= 0))
		return added;

	bsup = bsp->curchain;
	to = (Nlm_BytePtr) ptr;
	offset = bsp->seekptr - bsp->chain_offset;
	start = bsp->chain_offset;

 	while (len)
	{
		if (bsup == NULL)
			return added;
		tlen = bsup->len - offset;
		if (len < tlen)
			tlen = len;
		if (! tlen)       /* out of data */
			return added;
		bsp->chain_offset = start;
		from = (Nlm_BytePtr)Nlm_HandLock(bsup->str) + offset;
		Nlm_MemCopy(to, from, (size_t) tlen);
		Nlm_HandUnlock(bsup->str);
		offset = 0;        /* only offset on first one */
		len -= tlen;
		bsp->seekptr += tlen;
		bsp->curchain = bsup;
		start += bsup->len;
		added += tlen;
		to += tlen;
		if (len)
			bsup = bsup->next;
	}                                    /*** end of this bsunit? **/
	if ((bsp->seekptr - bsp->chain_offset) == bsup->len)
	{
		if (bsup->next != NULL)    /* not EOF */
		{
			bsp->curchain = bsup->next;
			bsp->chain_offset += bsup->len;
		}
	}
	return added;
}

/*****************************************************************************
*
*   Int2 BSGetByte(bsp)
*      reads a byte and increments the seekptr
*      returns EOF on End-Of-BS or error
*
*****************************************************************************/
NLM_EXTERN Nlm_Int2 LIBCALL  Nlm_BSGetByte (Nlm_ByteStorePtr bsp)

{
	Nlm_Int2 retval;
	Nlm_BytePtr ptr;
	Nlm_Int4 diff;
	Nlm_BSUnitPtr bsup;

	if ((bsp == NULL) || (bsp->totlen == 0) || (bsp->seekptr == bsp->totlen))
		return EOF;      /* EOF */

	diff = bsp->seekptr - bsp->chain_offset;
	bsup = bsp->curchain;
	ptr = (Nlm_BytePtr) Nlm_HandLock(bsup->str);
	retval = (Nlm_Int2) *(ptr + diff);
	Nlm_HandUnlock(bsup->str);

	bsp->seekptr++;
	diff++;
	if (diff == bsup->len)
	{
		if (bsp->seekptr != bsp->totlen)    /* not EOF */
		{
			bsp->curchain = bsup->next;
			bsp->chain_offset += bsup->len;
		}
	}
	return retval;	
}

/*****************************************************************************
*
*   Int4 BSInsert(bsp, ptr, len)
*   	returns bytes written from ptr into bsp
*       writes from current seekptr position
*       seekptr left pointing after last byte written
*       allocates storage if needed
*       inserts data BEFORE seekptr
*
*****************************************************************************/
NLM_EXTERN Nlm_Int4 LIBCALL  Nlm_BSInsert (Nlm_ByteStorePtr bsp, Nlm_VoidPtr ptr, Nlm_Int4 len)

{
	Nlm_Int4 added;

	added = Nlm_BSAdd(bsp, len, FALSE);
	if (added != len)
	{
		Nlm_BSDelete(bsp, added);
		return 0;
	}
	added = Nlm_BSWrite(bsp, ptr, len);
	return added;
}

/*****************************************************************************
*
*   Int4 BSInsertFromBS(bsp, bsp2, len)
*   	returns bytes written from bsp2 into bsp
*       reads from bsp2 starting from current seek position
*       writes from current seekptr position
*       seekptr left pointing after last byte written
*       allocates storage if needed
*       inserts data BEFORE seekptr
*
*****************************************************************************/
NLM_EXTERN Nlm_Int4 LIBCALL  Nlm_BSInsertFromBS (Nlm_ByteStorePtr bsp, Nlm_ByteStorePtr bsp2, Nlm_Int4 len)
{
	Nlm_Int4 added;
	Nlm_Int2 x;

	added = Nlm_BSAdd(bsp, len, FALSE);
	if (added != len)
	{
		Nlm_BSDelete(bsp, added);
		return 0;
	}

	for (added = 0; added < len; added++)
	{
		x = Nlm_BSGetByte(bsp2);
		Nlm_BSPutByte(bsp, x);
	}
	return added;
}

/*****************************************************************************
*
*   ByteStorePtr Nlm_BSFree(bsp)
*
*****************************************************************************/
NLM_EXTERN Nlm_ByteStorePtr LIBCALL  Nlm_BSFree (Nlm_ByteStorePtr bsp)

{
	Nlm_BSUnitPtr bsup, tmp;

	if (bsp == NULL)
		return NULL;
		
	bsup = bsp->chain;
	while (bsup != NULL)
	{
		tmp = bsup;
		bsup = bsup->next;
		Nlm_HandFree(tmp->str);
		Nlm_MemFree(tmp);
	}
	return (Nlm_ByteStorePtr) Nlm_MemFree(bsp);
}

/*****************************************************************************
*
*   ByteStorePtr Nlm_BSDup(bsp)
*
*****************************************************************************/

#define BUFSIZE 256

NLM_EXTERN Nlm_ByteStorePtr LIBCALL Nlm_BSDup (Nlm_ByteStorePtr source)

{
  Nlm_Byte          buf [BUFSIZE + 4];
  Nlm_Int4          count;
  Nlm_ByteStorePtr  dest;
  Nlm_Int4          sourceLen;
  Nlm_Int4          sourceLoc;

  dest = NULL;
  if (source != NULL) {
    sourceLen = Nlm_BSLen (source);
    dest = Nlm_BSNew (sourceLen);
    /* read the original location */
    sourceLoc = Nlm_BSTell(source);
    Nlm_BSSeek(source, 0L, SEEK_SET);
    if (dest != NULL) {
      count = MIN (sourceLen, (Nlm_Int4) BUFSIZE);
      while (count > 0 && sourceLen > 0) {
        Nlm_BSRead (source, buf, count);
        Nlm_BSWrite (dest, buf, count);
        sourceLen -= count;
        count = MIN (sourceLen, (Nlm_Int4) BUFSIZE);
      }
      /* for neatness, make the duplicate point to the
	     same location as the old one */
      Nlm_BSSeek(dest, sourceLoc, SEEK_SET);
    }
    /* restore original location */
    Nlm_BSSeek(source, sourceLoc, SEEK_SET);
  }
  return dest;
}

/* for copying and swapping of UID lists passed over network to BIG_ENDIAN server */

NLM_EXTERN Nlm_ByteStorePtr LIBCALL Nlm_BSDupAndSwapUint4 (Nlm_ByteStorePtr source)

{
  Nlm_Byte          buf [BUFSIZE + 4];
  Nlm_Int4          count;
  Nlm_ByteStorePtr  dest;
  Nlm_Int4          sourceLen;
  Nlm_Int4          sourceLoc;
  Nlm_Int4          swapLen;

  dest = NULL;
  if (source != NULL) {
    sourceLen = Nlm_BSLen (source);
    dest = Nlm_BSNew (sourceLen);
    /* read the original location */
    sourceLoc = Nlm_BSTell(source);
    Nlm_BSSeek(source, 0L, SEEK_SET);
    if (dest != NULL) {
      count = MIN (sourceLen, (Nlm_Int4) BUFSIZE);
      while (count > 0 && sourceLen > 0) {
        swapLen = Nlm_BSRead (source, buf, count);
        swapLen /= 4;
        /* swap Uint4 if IS_LITTLE_ENDIAN */
        Nlm_SwapUint4Buff ((Nlm_Uint4Ptr) buf, swapLen);
        Nlm_BSWrite (dest, buf, count);
        sourceLen -= count;
        count = MIN (sourceLen, (Nlm_Int4) BUFSIZE);
      }
      /* for neatness, make the duplicate point to the
	     same location as the old one */
      Nlm_BSSeek(dest, sourceLoc, SEEK_SET);
    }
    /* restore original location */
    Nlm_BSSeek(source, sourceLoc, SEEK_SET);
  }
  return dest;
}

/****************************************************************************
*
*   Integer storage utilities
*      These assume integers are store in BIG_ENDIAN order in the ByteStore
*      They read and write Uint2 or Uint4 in the NATIVE endian order
*      All work with UNSIGNED 2 or 4 byte integers
*         (you should cast to make them signed)
*      These are just helper functions. They do no internal consistency
*         checking.
*      They are primarily to facilitate encoding SEQUENCE OF INTEGER as
*         OCTET STRING for ASN.1
*
****************************************************************************/

NLM_EXTERN Nlm_Uint2 LIBCALL Nlm_BSGetUint2 (Nlm_ByteStorePtr bsp)
{
	Nlm_Uint2 retval = 0;

	Nlm_BSRead(bsp, (Nlm_VoidPtr)(&retval), (Nlm_Int4)2);
	Nlm_SwapUint2(retval);

	return retval;
}

NLM_EXTERN Nlm_Uint4 LIBCALL Nlm_BSGetUint4 (Nlm_ByteStorePtr bsp)
{
	Nlm_Uint4 retval = 0;

	Nlm_BSRead(bsp, (Nlm_VoidPtr)(&retval), (Nlm_Int4)4);
	Nlm_SwapUint4(retval);

	return retval;
}

NLM_EXTERN Nlm_Int2 LIBCALL Nlm_BSPutUint2 (Nlm_ByteStorePtr bsp, Nlm_Uint2 value)
{
#ifdef IS_LITTLE_ENDIAN
	Nlm_SwitchUint2(value);
#endif
	if (Nlm_BSWrite(bsp, (Nlm_VoidPtr)(&value), (Nlm_Int4)2) == (Nlm_Int2)2)
		return (Nlm_Int2)1;
	else
		return (Nlm_Int2)0;
}

NLM_EXTERN Nlm_Int2 LIBCALL Nlm_BSPutUint4 (Nlm_ByteStorePtr bsp, Nlm_Uint4 value)
{
#ifdef IS_LITTLE_ENDIAN
	Nlm_SwitchUint4(value);
#endif
	if (Nlm_BSWrite(bsp, (Nlm_VoidPtr)(&value), (Nlm_Int4)4) == (Nlm_Int2)4)
		return (Nlm_Int2)1;
	else
		return (Nlm_Int2)0;
}

       /* In functions below, size is 2 or 4 */
       /* Integers are converted from ByteStore to native endian (BSUintXRead) */
	   /*   or from native endian to ByteStore (BSUintXWrite) */
	   /* len is number of INTEGERS, not number of bytes */
	   /* returns count of integers put in ptr */
	   /* WARNING: On LITTLE_ENDIAN machines the data in ptr is changed to BIG_ENDIAN in the */
	   /*   XXWrite functions and LEFT THAT WAY */
NLM_EXTERN Nlm_Int4 LIBCALL Nlm_BSUint4Read (Nlm_ByteStorePtr bsp, Nlm_Uint4Ptr ptr, Nlm_Int4 len)
{
	Nlm_Int4 len2;

	len2 = Nlm_BSRead(bsp, (Nlm_VoidPtr)ptr, (len * 4));
	len2 /= 4;
	Nlm_SwapUint4Buff(ptr, len2);
	return len2;
}

NLM_EXTERN Nlm_Int4 LIBCALL Nlm_BSUint4Write (Nlm_ByteStorePtr bsp, Nlm_Uint4Ptr ptr, Nlm_Int4 len)
{
	Nlm_Int4 len2;

#ifdef IS_LITTLE_ENDIAN
	Nlm_SwitchUint4Buff(ptr, len);
#endif
	len2 = Nlm_BSWrite(bsp, (Nlm_VoidPtr)ptr, (len * 4));
	return (len2 / 4);
}

NLM_EXTERN Nlm_Int4 LIBCALL Nlm_BSUint2Read (Nlm_ByteStorePtr bsp, Nlm_Uint2Ptr ptr, Nlm_Int4 len)
{
	Nlm_Int4 len2;

	len2 = Nlm_BSRead(bsp, (Nlm_VoidPtr)ptr, (len * 2));
	len2 /= 2;
	Nlm_SwapUint2Buff(ptr, len2);
	return len2;
}

NLM_EXTERN Nlm_Int4 LIBCALL Nlm_BSUint2Write (Nlm_ByteStorePtr bsp, Nlm_Uint2Ptr ptr, Nlm_Int4 len)
{
	Nlm_Int4 len2;

#ifdef IS_LITTLE_ENDIAN
	Nlm_SwitchUint2Buff(ptr, len);
#endif
	len2 = Nlm_BSWrite(bsp, (Nlm_VoidPtr)ptr, (len * 2));
	return (len2 / 2);
}

