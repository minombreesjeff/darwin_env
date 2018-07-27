/*  asnio.c
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
* File Name:  asnio.c
*
* Author:  James Ostell
*
* Version Creation Date: 3/4/91
*
* $Revision: 6.10 $
*
* File Description:
*   Routines for AsnIo objects.  This code has some machine dependencies.
*   Currently it works for all ncbi supported platforms.
*   
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
* 3/4/91   Kans        Stricter typecasting for GNU C and C++
* 4/7/91   Ostell      Berkely Socket support, block I/O
* 04-20-93 Schuler     LIBCALL calling convention
* 01-31-94 Schuler     Changed ErrGetOpts/ErrSetOpts to ErrSaveOptions/ErrRestoreOptions
*
* $Log: asnio.c,v $
* Revision 6.10  2001/10/11 14:39:08  ostell
* added support for XMLModulePrefix
*
* Revision 6.9  2001/03/13 13:11:46  ostell
* made AsnIoBSOpen() and AsnIoMemOpen() XML aware
*
* Revision 6.8  2001/02/02 22:08:47  shavirin
* Fixed case of opening AsnIoPtr in case of XML output for Linux.
*
* Revision 6.7  2000/05/10 03:12:37  ostell
* added support for XML DTD and XML data output
*
* Revision 6.6  1999/07/30 20:34:25  vakatov
* AsnIoGets():  rewritten -- to handle an incremental read
*
* Revision 6.5  1998/02/27 17:22:21  vakatov
* [WIN32 DLL]  Declared some functions as NLM_EXTERN(DLL-exportable)
*
* Revision 6.4  1998/01/09 15:51:03  shavirin
* Hash function turned to be external
*
* Revision 6.3  1998/01/09 15:47:52  shavirin
* Added hash calculating functions.
*
* Revision 6.2  1997/12/04 21:41:14  shavirin
* Fixed bug with printing error message
*
* Revision 6.1  1997/10/29 02:41:13  vakatov
* Type castings to pass through the C++ compiler
*
* Revision 6.0  1997/08/25 18:10:00  madden
* Revision changed to 6.0
*
* Revision 5.4  1997/04/23 15:44:51  ostell
* commented out references to g_asnlib which caused unecessary error messages
* trying to open asnlib.msg file which does not exist.
*
 * Revision 5.3  1996/12/03  21:43:48  vakatov
 * Adopted for 32-bit MS-Windows DLLs
 *
 * Revision 5.2  1996/10/07  14:37:28  vakatov
 * AsnIoErrorMsg() rearranged to avoid memory corruption when posting
 * long (>2048 bytes) error messages
 *
 * Revision 5.1  1996/06/18  16:07:46  sad
 * AsnIoGets errorneously reported that line too long at the end of file, fixed
 *
 * Revision 5.0  1996/05/28  14:00:29  ostell
 * Set to revision 5.0
 *
 * Revision 4.6  1996/02/18  18:05:37  kans
 * severity is an ErrSev variable
 *
 * Revision 4.5  1996/02/18  16:45:36  ostell
 * changed fix_non_print behavior and added option 3
 *
 * Revision 4.4  1995/12/21  14:00:51  ostell
 * added AsnIoFree()
 *
 * Revision 4.3  1995/12/18  21:38:00  epstein
 * add diagnostic values in AsnIoWriteBlock error message
 *
 * Revision 4.2  1995/10/28  15:02:06  ostell
 * added casts to quiet DOS compile warnings
 *
 * Revision 4.1  1995/09/14  20:41:02  sirotkin
 * modification to AsnIoErrMsg to give meaningful error codes
 *
 * Revision 4.0  1995/07/26  13:47:38  ostell
 * force revision to 4.0
 *
 * Revision 2.24  1995/05/26  22:14:40  ostell
 * added spec_version saving to AsnIoReset
 *
 * Revision 2.23  1995/05/15  18:38:28  ostell
 * added Log line
 *
*
* ==========================================================================
*/

/*****************************************************************************
*
*   asnio.c
*   	routines for AsnIo objects
*   	AsnIoError()  uses varargs, so has some portability issues
*
*****************************************************************************/

#include "asnbuild.h"
#include <tsprintf.h>

/*
NLM_EXTERN char * g_asnlib = "AsnLib";
*/

static Int2 bufsize = ASNIO_BUFSIZE;  /* current default size of AsnIo buf */

/*****************************************************************************
*
*   AsnIoPtr AsnIoOpen(file_name, mode)
*
*****************************************************************************/
NLM_EXTERN AsnIoPtr LIBCALL  AsnIoOpen (CharPtr file_name, CharPtr mode)
{
	AsnIoPtr aip;
	Int1 type;
    FILE *fp;

	if (file_name == NULL)
	{
		AsnIoErrorMsg(NULL, 80);
		return NULL;
	}

	if (! StringCmp(mode, "r"))
		type = ASNIO_TEXT_IN;
	else if (! StringCmp(mode, "w"))
		type = ASNIO_TEXT_OUT;
	else if (! StringCmp(mode, "wx"))
	{
		type = ASNIO_TEXT_OUT;
		type |= ASNIO_XML;
	}
	else if (! StringCmp(mode, "rb"))
		type = ASNIO_BIN_IN;
	else if (! StringCmp(mode, "wb"))
		type = ASNIO_BIN_OUT;
	else
	{
		AsnIoErrorMsg(NULL, 81, mode);
		return NULL;
	}

        if (!StringCmp(mode, "wx")) {
            fp = FileOpen(file_name, "w");            
        } else {
            fp = FileOpen(file_name, mode);
        }

	if (fp == NULL)
	{
	   /*** Message posted by FileOpen() **
		AsnIoErrorMsg(NULL, 82, file_name);
		***********************************/
		return NULL;
	}

    aip = AsnIoNew(type, fp, NULL, NULL, NULL);
	if (aip != NULL)
		aip->fname = StringSave(file_name);

	return aip;
}

static CharPtr AsnXMLModuleDefaultPrefix = "\0";
static CharPtr AsnXMLModulePrefix = "\0";

NLM_EXTERN Boolean LIBCALL AsnSetXMLmodulePrefix (CharPtr prefix)
{
	static CharPtr zero = "\0";
	AsnXMLModulePrefix = prefix;
	if (prefix == NULL)
		AsnXMLModulePrefix = zero;
	return TRUE;
}
NLM_EXTERN Boolean LIBCALL AsnSetXMLmodulePrefixToDefault (void)
{
	AsnXMLModulePrefix = AsnXMLModuleDefaultPrefix;
	return TRUE;
}

NLM_EXTERN CharPtr LIBCALL AsnGetXMLmodulePrefix (void)
{
	return AsnXMLModulePrefix;
}

/*****************************************************************************
*
*   AsnIoPtr AsnIoNew(type, fp, iostruct, readfunc, writefunc)
*        function pointers are optional
*        if not given, uses file functions
*
*****************************************************************************/
NLM_EXTERN AsnIoPtr LIBCALL  AsnIoNew (Int1 type, FILE *fp, Pointer iostruct, IoFuncType readfunc, IoFuncType writefunc)
{
    AsnIoPtr aip;

    aip = (AsnIoPtr) MemNew(sizeof(AsnIo));
    if (aip == NULL)
        return NULL;

    aip->bufsize = bufsize;
	if (! (type & ASNIO_CARRIER))      /* not AsnIoNullOpen() */
	    aip->buf = (BytePtr) MemNew(bufsize);
	aip->type = type;
    aip->fp = fp;
    aip->iostruct = iostruct;
    aip->readfunc = readfunc;
    aip->writefunc = writefunc;
    
    if (type & ASNIO_OUT)
    	aip->bytes = aip->bufsize;   /* have a buffer ready to write */
    aip->linebuf = (CharPtr)aip->buf;
	aip->tabsize = 2;
	aip->linelength = 78;
	aip->max_indent = 10;    /* start with 10 indent levels */
    aip->first = (BoolPtr) MemNew((sizeof(Boolean) * 10));
    aip->first[0] = TRUE;
	aip->typestack = (PstackPtr) MemNew((sizeof(Pstack) * 10));
	aip->max_type = 10;
    return aip;
}

/*****************************************************************************
*
*   AsnIoPtr AsnIoClose(aip)
*
*****************************************************************************/
NLM_EXTERN AsnIoPtr LIBCALL  AsnIoClose (AsnIoPtr aip)
{
	return AsnIoFree (aip, TRUE);
}

/*****************************************************************************
*
*   AsnIoPtr AsnIoFree(aip, close_file)
*
*****************************************************************************/
NLM_EXTERN AsnIoPtr LIBCALL AsnIoFree (AsnIoPtr aip, Boolean close_file)
{
	FILE *fp;
	int type;

	if (aip == NULL)
		return aip;

	type = aip->type;
	fp = aip->fp;

	if (type & ASNIO_OUT)
	{
		aip->first[aip->indent_level] = TRUE;
		if (type & ASNIO_TEXT)
			AsnPrintNewLine(aip);
		AsnIoFlush(aip);
	}

	if (close_file)
		FileClose(fp);

    MemFree(aip->first);
	MemFree(aip->typestack);
    MemFree(aip->buf);
	MemFree(aip->fname);
	AsnIoOptionFree(aip, 0, 0);
	AsnExpOptFree(aip, NULL);
	MemFree(aip);
	return NULL;
}

/*****************************************************************************
*
*   AsnOptionPtr AsnIoOptionNew(aip, ao_class, type, data, freefunc)
*   	allocates and adds an option to aip
*       returns a pointer to it.
*
*****************************************************************************/
NLM_EXTERN AsnOptionPtr LIBCALL  AsnIoOptionNew (AsnIoPtr aip, Int2 ao_class, Int2 type, DataVal data, AsnOptFreeFunc freefunc)
{
	if (aip == NULL)
		return NULL;

    return AsnOptionNew(&aip->aop, ao_class, type, data, freefunc);
}

/*****************************************************************************
*
*   AsnIoOptionFree(aip, ao_class, type)
*   	clears options of ao_class
*       or if ao_class == 0 clears all options
*       if type == 0, clears all options of ao_class
*
*****************************************************************************/
NLM_EXTERN void LIBCALL  AsnIoOptionFree (AsnIoPtr aip, Int2 ao_class, Int2 type)
{
    AsnOptionFree(&aip->aop, ao_class, type);
    return;
}

/*****************************************************************************
*
*   AsnOptionPtr AsnIoOptionGet(aip, ao_class, type, last)
*   	gets next option after last
*   	start with last = NULL
*   	if ao_class = 0, match any ao_class
*       type = 0 match any type
*
*****************************************************************************/
NLM_EXTERN AsnOptionPtr LIBCALL  AsnIoOptionGet (AsnIoPtr aip, Int2 ao_class, Int2 type, AsnOptionPtr last)
{
	if (aip == NULL)
		return NULL;

    return AsnOptionGet(aip->aop, ao_class, type, last);
}

/*****************************************************************************
*
*   AsnOptionPtr AsnOptionNew(aopp, ao_class, type, data, freefunc)
*   	allocates and adds an option to aopp
*       returns a pointer to it.
*
*****************************************************************************/
NLM_EXTERN AsnOptionPtr LIBCALL  AsnOptionNew (AsnOptionPtr PNTR aopp, Int2 ao_class, Int2 type, DataVal data, AsnOptFreeFunc freefunc)
{
	AsnOptionPtr aop, curr;

	aop = (AsnOptionPtr) MemNew(sizeof(AsnOption));
	if (aop == NULL)
		return NULL;

	aop->ao_class = ao_class;
	aop->type = type;
	aop->data = data;
	aop->freefunc = freefunc;

    if (aopp != NULL)
    {
    	curr = *aopp;
	    if (curr == NULL)
		    *aopp = aop;
    	else
	    {
		    while (curr->next != NULL)
			    curr = curr->next;
    		curr->next = aop;
	    }
    }
	return aop;
}

/*****************************************************************************
*
*   AsnOptionFree(aopp, ao_class, type)
*   	clears options of ao_class
*       or if ao_class == 0 clears all options
*   	if type == 0, clears all options of ao_class
*
*****************************************************************************/
NLM_EXTERN void LIBCALL  AsnOptionFree (AsnOptionPtr PNTR aopp, Int2 ao_class, Int2 type)
{
	AsnOptionPtr curr, next, prev, first;

	if (aopp == NULL)
		return;

	curr = *aopp;
	prev = NULL;
	first = NULL;
	while (curr != NULL)
	{
		next = curr->next;
		if (AsnClassTypeMatch (ao_class, type, 
				curr->ao_class, curr->type))
		{
				if (curr->freefunc != NULL)
					(*curr->freefunc)(curr->data.ptrvalue);
				if (prev != NULL)
					prev->next = next;
				MemFree(curr);
		}
		else
		{
			if (first == NULL)
				first = curr;
			prev = curr;
		}
		curr = next;
	}

	*aopp = first;
	return;
}


/*****************************************************************************
*
*  Boolean AsnClassTypeMatch (ao_class, type, this_class, this_type)
*     checks if ao_class and type match in a way that makes for
*     clean logic
**********************/
NLM_EXTERN Boolean LIBCALL  AsnClassTypeMatch (Int2 ao_class, Int2 type, Int2 this_class, Int2 this_type)
{
	Boolean retval = FALSE;

		if ((! ao_class) || (ao_class == this_class))
		{
			if ((! type) || (type == this_type))
			{
				retval = TRUE;
			}
		}

	return retval;
}

/*****************************************************************************
*
*   AsnOptionPtr AsnOptionGet(head, ao_class, type, last)
*   	gets next option after last
*   	start with last = NULL
*   	if ao_class = 0, match any ao_class
*       type = 0 match any type
*
*****************************************************************************/
NLM_EXTERN AsnOptionPtr LIBCALL  AsnOptionGet (AsnOptionPtr head, Int2 ao_class, Int2 type, AsnOptionPtr last)

{
	AsnOptionPtr aop;

	if (head == NULL)
		return NULL;

	aop = head;

	if (last != NULL)
	{
		while ((aop != NULL) && (aop != last))
			aop = aop->next;
		if (aop != NULL)
			aop = aop->next;
	}

	while (aop != NULL)
	{
		if ((ao_class == 0) || (ao_class == aop->ao_class))
		{
			if ((type == 0) || (type == aop->type))
				return aop;
		}
		aop = aop->next;
	}

	return NULL;
}

/*****************************************************************************
*
*   Boolean AsnIoSetBufsize(aip, size)
*       replaces current buffer with one of new size
*          ASSUMES aip has NOT YET been USED
*       if (aip == NULL)
*           changes default size of buffer for all new AsnIo
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL  AsnIoSetBufsize (AsnIoPtr aip, Int2 size)
{
    BytePtr tmp;

    if (aip == NULL)
    {
        bufsize = size;    /* set default size */
        return TRUE;
    }
    else
    {
        tmp = (BytePtr) MemNew(size);
        if (tmp == NULL)
            return FALSE;
        MemFree(aip->buf);
        aip->buf = tmp;
        aip->bufsize = size;
        aip->linebuf = (CharPtr)aip->buf;
        if (aip->type & ASNIO_OUT)
        	aip->bytes = size;
                    /* have a new sized buffer ready to write */
        return TRUE;
    }
}

/*****************************************************************************
*
*   void AsnIoPuts(aip)
*   	put the linebuf
*
*****************************************************************************/
NLM_EXTERN void AsnIoPuts (AsnIoPtr aip)
{
	if (aip->linepos == 0)
		return;

	if (! aip->no_newline)
	{
		aip->linebuf[aip->linepos] = '\n';
		aip->offset++;
	}

	aip->linepos = 0;

    if ((aip->bytes - aip->offset) > (aip->linelength + 20))  /* room left */
        aip->linebuf = (CharPtr)aip->buf + aip->offset;
    else
    {
        AsnIoWriteBlock(aip);    /* write it out */
        aip->linebuf = (CharPtr)aip->buf;   /* reset line pointer */
    }

	if (aip->no_newline)
		aip->no_newline = FALSE;

    return;
}


/*****************************************************************************
*
*   CharPtr AsnIoGets(aip)
*   	equivalent of fgets()
*   	only works on TEXT_IN files
*   	increments aip->linenumber
*
*****************************************************************************/
NLM_EXTERN CharPtr AsnIoGets (AsnIoPtr aip)
{
  Int2    offset = aip->offset;                 /* current end of line */
  Int2    bytes  = aip->bytes - offset;         /* unread bytes in buffer */
  CharPtr str    = (CharPtr)aip->buf + offset;  /* start of next line */
  Int2    len    = 0;                           /* # of bytes read in line */

  for (;;) {
    /* no more data in the buffer? */
    if (bytes <= 0) {
      /* check if line is longer than the whole block */
      if (offset == 0  &&  len == aip->bufsize) {
        AsnIoErrorMsg(aip, 74, (int)aip->bufsize, aip->linenumber);
        return 0;
      }

      /* move data to the very beginning of the buffer, if necessary */
      if (offset  &&  aip->bytes == aip->bufsize) {
        if ( len )
          MemCopy(aip->buf, aip->buf + offset, (size_t)len);
        offset = 0;
        str = (CharPtr)aip->buf + len;
      }

      /* read to the buffer */
      aip->offset = offset + len;
      bytes = AsnIoReadBlock(aip);
      if ( !bytes ) {
        if ( !len ) {
          return 0;  /* cant read a thing */
        } else {
          *str = '\n';
          break;     /* assume EOF and no last newline */
        }
      }
      continue;
    }

    /* check for EOL, else forward to the next symbol */
    len++;  /* EOL included */
    if (*str == '\n'  ||  *str == '\r')
      break; /* EOL */
    str++;
    bytes--;
  }  /* for (;;) */

  aip->offset = offset + len;
  aip->linenumber++;
  aip->linebuf = (CharPtr)aip->buf + offset;
  return aip->linebuf;
}


/*****************************************************************************
*
*   Int4 AsnIoTell(aip)
*   	gives seek offset in current file or -1 if error
*
*****************************************************************************/
NLM_EXTERN Int4 LIBCALL  AsnIoTell (AsnIoPtr aip)
{
	Int4 offset, bytes;

    if (!(aip->type & ASNIO_FILE))               
    {
        AsnIoErrorMsg(aip, 75);
        return 0L;
    }
	offset = ftell(aip->fp);             /* position after last read */
	if (aip->type & ASNIO_IN)
	{
		bytes = aip->bytes - aip->offset;    /* bytes left in buffer */
		offset -= bytes;                     /* real position by byte */
		if (aip->tagsaved)            /* binary tags saved? */
			offset -= aip->used;      /* remove that too */
	}
	else
	{
		offset += aip->offset;       /* unflushed stuff in buffer */
	}
	return offset;
}


/*****************************************************************************
*
*   Int4 AsnIoSeek(aip, pos)
*   	seeks to pos
*
*****************************************************************************/
NLM_EXTERN Int4 LIBCALL  AsnIoSeek (AsnIoPtr aip, Int4 pos)
{
    if (! (aip->type & ASNIO_IN))
    {
        AsnIoErrorMsg(aip, 76);
        return 0L;
    }
    if (! (aip->type & ASNIO_FILE))
    {
        AsnIoErrorMsg(aip, 77);
        return 0L;
    }
	pos = fseek(aip->fp, pos, 0);   /* formatted file */
    AsnIoReset(aip);
    return pos;
}

/*****************************************************************************
*
*   Int2 AsnIoReadBlock(aip)
*   	returns bytes read
*
*****************************************************************************/
NLM_EXTERN Int2 AsnIoReadBlock (AsnIoPtr aip)
{
	Int2 bytes, offset, retval;

    if (aip->type & ASNIO_TEXT)
    {
        offset = aip->offset;
        bytes = aip->bufsize - offset;
    }
    else
    {
        bytes = aip->bufsize;
        offset = 0;
    }

    if (aip->readfunc == NULL)     /* use standard read */
    {
	    retval = (Int2) FileRead((VoidPtr)(aip->buf + offset), (size_t)1, (size_t) bytes, aip->fp);
    }
    else                    /* use alternative read function */
         retval = (* aip->readfunc)(aip->iostruct, (CharPtr)(aip->buf + offset), (Uint2)bytes);
	if (retval > 0)
	    aip->bytes = retval + offset;       /* space available to read */
	else
	    aip->bytes = offset;
	aip->offset = 0;		  /* bytes read */
	if (retval < 0)
	{
		AsnIoErrorMsg(aip, 78, retval);
		retval = 0; /* callers look for 0 as bad return value */
	}
	return retval;
}

/*****************************************************************************
*
*   Int2 AsnIoWriteBlock(aip)
*   	does NOT return bytes written
*   	return bytes available in buffer to write on
*
*****************************************************************************/
NLM_EXTERN Int2 AsnIoWriteBlock (AsnIoPtr aip)
{
	Int2 retval, bytes_to_write, offset;

	if (aip->io_failure)
		return 0;
	if (aip->used > 2) /* recursion stopper */
		return 0;
	aip->used++;       /* recursion counter */

	if (aip->type & ASNIO_TEXT)
	{
		offset = (Int2)aip->length;   /* amount sent on last AsnIoFlush */
		bytes_to_write = aip->offset - offset;
	}
	else
	{
		offset = 0;
		bytes_to_write = aip->offset;
	}
	
    if (aip->writefunc == NULL)     /* use standard write */
    {
	    retval = (Int2) FileWrite((VoidPtr)(aip->buf + offset), (size_t)1,
 			(size_t) bytes_to_write, aip->fp);
    }
    else                    /* use alternative write function */
        retval = (* aip->writefunc)(aip->iostruct, (CharPtr)(aip->buf + offset),
 			(Uint2) bytes_to_write);
	if (retval != bytes_to_write)
	{
		AsnIoErrorMsg(aip, 79, retval, bytes_to_write);
		aip->io_failure = TRUE;
	}
	if (aip->type & ASNIO_TEXT)   /* check for full line */
	{
		if ((aip->buf[aip->offset - 1] == '\n') || (aip->no_newline))
		{
			aip->length = 0;
			aip->offset = 0;					 /* write space used */
			aip->linebuf = (CharPtr)aip->buf;
			aip->bytes = aip->bufsize;    /* free write space */
		}
		else
		{
			aip->length = aip->offset;  /* store flushed section */
		}
	}
	else
	{
		aip->bytes = aip->bufsize;    /* free write space */
		aip->offset = 0;					 /* write space used */
	}
	aip->used--; /* warning! each invocation of this function must decrement */
	return aip->bytes;
}

static char * AsnIoErrStr[107] = {
#ifdef WIN16

"Sorry. Not supported yet",
"Cannot start AsnDeBinType() with unknown Type",
"tag mismatch on %Fs",
"Expected 00 after tag for %Fs",
"Tag mismatch on SET/SEQ OF %Fs",
"Unable to match element in %Fs.",
"element %Fs out of order in SEQUENCE %Fs",
"Skipped element %Fs in SEQUENCE %Fs",
"AsnDeBinDecr: lenstack[%d] < 0",
"type [%Fs]. Unresolved imported type [%Fs]",
"type [%Fs]. Unresolved base type [%Fs]",
"Unable to read value of %Fs.",
"%ld is an invalid value for %Fs.",
"Boolean %Fs must be one byte",
"Can't allocate %ld bytes for %Fs",
"AsnDeBinReal- not encoded as decimal",
"NULL value without 0 length for %Fs",
"Premature end of file",
"End Struct too late in value",
"Unable to write value of %Fs.",
"AsnEndStruct with nothing on stack.",
"AsnEndStruct of %Fs does not match %Fs",
"%Fs [%Fs] is not an element of %Fs",
"[%Fs] CHOICE may only have one value [%Fs]",
"Cannot duplicate element %Fs in SEQ or SET %Fs",
"Parent type not struct. %Fs isa=%d",
"Input value file must start with Defined Type. line %ld",
"Type Reference not found in module. line %ld",
"Expected ::= after Type Reference on input. line %ld",
"Expected to start with %Fs. line %ld",
"Expected comma or } in input value. line %ld",
"Expected identifier. line %ld",
"Unable to match element in %Fs. line %ld",
"element %Fs out of order in SEQUENCE %Fs. line number %ld",
"Skipped element %Fs in SEQUENCE %Fs. line %ld",
"type [%Fs]. Unresolved imported type [%Fs]. line %ld",
"type [%Fs]. Unresolved base type [%Fs]. line %ld",
"Expected { line %ld",
"Unable to read value of %Fs. line %ld",
"Integer value not found for %Fs. line %ld",
"Invalid value for %Fs. line %ld",
"General defined values not supported yet. %Fs on line %ld",
"Invalid value for BOOLEAN %Fs.  line %ld",
"No \" on string start for %Fs. line %ld",
"No \" on string end for %Fs. line %ld",
"No \' on OCTET start for %Fs. line %ld",
"AsnLexReadOctets on %Fs. line %ld",
"No \'H on OCTETs end for %Fs. line %ld",
"Uneven number in OCTETS for %Fs. line %ld",
"REAL %Fs must start with {. line %ld",
"mantissa error on %Fs. line %ld",
"comma missing after mantissa on %Fs. line %ld",
"base incorrect on %Fs. line %ld",
"only base 2 or 10 allowed. %Fs on line %ld",
"comma missing after base on %Fs. line %ld",
"exponent error on %Fs.  line %ld",
"no } on %Fs.  line %ld",
"Expected NULL for %Fs.  line %ld",
"Can't identify Bit/Hex string line %ld",
"Unrecognized token %c. line %ld",
"ReadModule: Ref or ident required. line %ld but got %Fs",
"No ] on tag. line %ld",
"AsnLexTReadType: defined number not supported. line %ld",
"No integer tag. line %ld",
"Sorry. Subtypes not supported yet. line %ld",
"Enumerated type MUST have values. line %ld",
"Expected comma around line %ld",
"%Fs is an ASN.1 reserved word. line %ld",
"object identifier not implemented",
"Module: missing %s. line %ld",
"Comma out of place. line %ld",
"IMPORTS: expected typeref before FROM on line %ld",
"IMPORTS: expected Module on line %ld",
"Object Indentifier not supported. line %ld",
"Input line longer than %d. line %ld",
"AsnIoTell on non-FILE",
"AsnIoSeek on non-INPUT file",
"AsnIoSeek on non-FILE",
"Read Error [%d] in AsnIoReadBlock",
"AsnIoWriteBlock: too few bytes written [retvalue %d, requested %d]",
"No file name given on AsnIoOpen",
"Unrecognized AsnIoOpen mode [%Fs]",
"Cannot open file [%Fs]",
"Cannot open parse tree load file [%Fs]",
"[%Fs] should be EXPORTED from module [%Fs]",
"Cannot find path to parse tree load files",  /* [85] */
"Expected identifier for named integer. line %ld",
"Missing ( in named integer. line %ld",
"Missing ) in named integer. line %ld",
"Expected OF on line %ld",
"Expected STRING. line %ld",
"Invalid token after OF. line %ld",
"Syntax error after OBJECT. line %ld",
"Sorry. COMPONENTS OF not supported yet. line %ld",
"Default for %Fs not supported",
"Sorry.  Cannot find type def for %Fs to set DEFAULT. line %ld",
"IMPORTS: expected Ref line %ld",
"EXPORTS line after IMPORTS. line %ld",
"%Fs is not a valid Type name. line %ld",
"Redefinition of %Fs. line %ld",
"Type %Fs was not resolved in module %Fs",
"IMPORTED Type %Fs was never referenced in module %Fs",
"No IMPORT source for type %Fs in module %Fs",
"Did not AsnReadId before AsnReadVal. line %ld" ,
"Did not AsnReadVal before AsnReadId. line %ld",
"Did not load parse trees for trying to explore them",
"Invalid value(s) [%d] in VisibleString [%Fs ...]"

#else

"Sorry. Not supported yet",
"Cannot start AsnDeBinType() with unknown Type",
"tag mismatch on %s",
"Expected 00 after tag for %s",
"Tag mismatch on SET/SEQ OF %s",
"Unable to match element in %s.",
"element %s out of order in SEQUENCE %s",
"Skipped element %s in SEQUENCE %s",
"AsnDeBinDecr: lenstack[%d] < 0",
"type [%s]. Unresolved imported type [%s]",
"type [%s]. Unresolved base type [%s]",
"Unable to read value of %s.",
"%ld is an invalid value for %s.",
"Boolean %s must be one byte",
"Can't allocate %ld bytes for %s",
"AsnDeBinReal- not encoded as decimal",
"NULL value without 0 length for %s",
"Premature end of file",
"End Struct too late in value",
"Unable to write value of %s.",
"AsnEndStruct with nothing on stack.",
"AsnEndStruct of %s does not match %s",
"%s [%s] is not an element of %s",
"[%s] CHOICE may only have one value [%s]",
"Cannot duplicate element %s in SEQ or SET %s",
"Parent type not struct. %s isa=%d",
"Input value file must start with Defined Type. line %ld",
"Type Reference not found in module. line %ld",
"Expected ::= after Type Reference on input. line %ld",
"Expected to start with %s. line %ld",
"Expected comma or } in input value. line %ld",
"Expected identifier. line %ld",
"Unable to match element in %s. line %ld",
"element %s out of order in SEQUENCE %s. line number %ld",
"Skipped element %s in SEQUENCE %s. line %ld",
"type [%s]. Unresolved imported type [%s]. line %ld",
"type [%s]. Unresolved base type [%s]. line %ld",
"Expected { line %ld",
"Unable to read value of %s. line %ld",
"Integer value not found for %s. line %ld",
"Invalid value for %s. line %ld",
"General defined values not supported yet. %s on line %ld",
"Invalid value for BOOLEAN %s.  line %ld",
"No \" on string start for %s. line %ld",
"No \" on string end for %s. line %ld",
"No \' on OCTET start for %s. line %ld",
"AsnLexReadOctets on %s. line %ld",
"No \'H on OCTETs end for %s. line %ld",
"Uneven number in OCTETS for %s. line %ld",
"REAL %s must start with {. line %ld",
"mantissa error on %s. line %ld",
"comma missing after mantissa on %s. line %ld",
"base incorrect on %s. line %ld",
"only base 2 or 10 allowed. %s on line %ld",
"comma missing after base on %s. line %ld",
"exponent error on %s.  line %ld",
"no } on %s.  line %ld",
"Expected NULL for %s.  line %ld",
"Can't identify Bit/Hex string line %ld",
"Unrecognized token %c. line %ld",
"ReadModule: Ref or ident required. line %ld but got %s",
"No ] on tag. line %ld",
"AsnLexTReadType: defined number not supported. line %ld",
"No integer tag. line %ld",
"Sorry. Subtypes not supported yet. line %ld",
"Enumerated type MUST have values. line %ld",
"Expected comma around line %ld",
"%s is an ASN.1 reserved word. line %ld",
"object identifier not implemented",
"Module: missing %s. line %ld",
"Comma out of place. line %ld",
"IMPORTS: expected typeref before FROM on line %ld",
"IMPORTS: expected Module on line %ld",
"Object Indentifier not supported. line %ld",
"Input line longer than %d. line %ld",
"AsnIoTell on non-FILE",
"AsnIoSeek on non-INPUT file",
"AsnIoSeek on non-FILE",
"Read Error [%d] in AsnIoReadBlock",
"AsnIoWriteBlock: too few bytes written [retvalue %d, requested %d]",
"No file name given on AsnIoOpen",
"Unrecognized AsnIoOpen mode [%s]",
"Cannot open file [%s]",
"Cannot open parse tree load file [%s]",
"[%s] should be EXPORTED from module [%s]",
"Cannot find path to parse tree load files",	  /* [85] */
"Expected identifier for named integer. line %ld",
"Missing ( in named integer. line %ld",
"Missing ) in named integer. line %ld",
"Expected OF on line %ld",
"Expected STRING. line %ld",
"Invalid token after OF. line %ld",
"Syntax error after OBJECT. line %ld",
"Sorry. COMPONENTS OF not supported yet. line %ld",
"Default for %s not supported",
"Sorry.  Cannot find type def for %s to set DEFAULT. line %ld",
"IMPORTS: expected Ref line %ld",
"EXPORTS line after IMPORTS. line %ld",
"%s is not a valid Type name. line %ld",
"Redefinition of %s. line %ld",
"Type %s was not resolved in module %s",
"IMPORTED Type %s was never referenced in module %s",
"No IMPORT source for type %s in module %s",
"Did not AsnReadId before AsnReadVal. line %ld" ,
"Did not AsnReadVal before AsnReadId. line %ld",
"Did not load parse trees for trying to explore them",
"Invalid value(s) [%d] in VisibleString [%s ...]"

#endif
}; 

/**************************************************************************
*
* AsnErrGetTypeName(name)
*
**************************************************************************/
NLM_EXTERN CharPtr LIBCALL AsnErrGetTypeName (CharPtr name)
{
    if (name == NULL)
       return ".E";
    else
       return name;
}

/*****************************************************************************
*
*    AsnIoErrorMsg(aip, errcode, args)
*
*****************************************************************************/
#ifdef VAR_ARGS
NLM_EXTERN void CDECL AsnIoErrorMsg(aip, errcode, va_alist)
	AsnIoPtr aip;
    int errcode;
    va_dcl
#else
NLM_EXTERN void CDECL AsnIoErrorMsg(AsnIoPtr aip, int errcode, ...)
#endif
{
  CharPtr buf, ptr;
  ErrSev  severity  = SEV_FATAL;
  size_t  length    = 1024;  /* estimated maximum of AsnTypeDumpStack() */
  CharPtr aip_fname_str = NULL;
  CharPtr aip_type_str  = NULL;
  CharPtr arg_str       = NULL;

  if (aip != NULL)      /* set the error flag for asnlib error */
    {
      /* non-printing chars in string may not be fatal */
      if (errcode == 106  &&  aip->fix_non_print == 0)
        severity = SEV_ERROR;
      else
        aip->io_failure = TRUE;

      if (aip->fname != NULL)
        aip_fname_str = aip->fname;

      if (aip->type & ASNIO_IN)
        aip_type_str = "Input\n";
      else
        {
          AsnIoFlush( aip );
          aip_type_str = "Output\n";
        }
    }
  length += StringLen(aip_fname_str) + StringLen(aip_type_str);

  {{
  va_list args;
#ifdef VAR_ARGS
  va_start(args);                       /* get the arguments */
#else
  va_start(args, errcode);
#endif
  arg_str = StringSave( TSPrintfArgs(AsnIoErrStr[errcode], args) );
  va_end(args);
  }}
  length += StringLen(arg_str);


  buf = (CharPtr) Malloc(length + 1);  *buf = '\0';
  StringCat(buf, aip_fname_str);
  StringCat(buf, aip_type_str);
  ptr = buf + StringLen( buf );

  if (aip != NULL)
    {
      AsnTypeDumpStack(ptr, aip);          /* put in the type stack */
      while (*ptr != '\0')
        ptr++;
      *ptr = '\n';
      ptr++;
      *ptr = '\0';
    }

  ASSERT ( StringLen(buf) + StringLen(arg_str) <=  length );
  StringCat(ptr, arg_str);
  MemFree( arg_str );

  if (aip != NULL  &&  aip->error_ret != NULL)
    (*aip->error_ret)((Int2)errcode, (CharPtr)buf);
  else
    ErrPostEx(severity, CTX_NCBIASN1, errcode, "%s", buf);

  Free( buf );
  return;
}


/*****************************************************************************
*
*   void AsnIoSetErrorMsg(aip, ErrorRetType)
*   	set user error handler
*
*****************************************************************************/
NLM_EXTERN void LIBCALL  AsnIoSetErrorMsg (AsnIoPtr aip, AsnErrorFunc error_ret)
{
	aip->error_ret = error_ret;
	return;
}

/*****************************************************************************
*
*   void AsnIoReset(aip)
*   	resets parse state of aip
*   	does not reset file or error handler
*
*****************************************************************************/
NLM_EXTERN void LIBCALL  AsnIoReset (AsnIoPtr aip)
{
	FILE * fp;
	Int1 type, max_type, max_indent, linelength, tabsize;
	PstackPtr typestack;
    BoolPtr first;
	ErrorRetType error_ret;
    Pointer iostruct;
    IoFuncType readfunc, writefunc;
    Int2 tbufsize;
    BytePtr buf;
	CharPtr fname;       /* name of file in use */
	AsnOptionPtr aop;    /* head of options chain */
	AsnExpOptPtr aeop;   /* exploration options chain */
	AsnExpOptStructPtr aeosp;
	Uint1 fix_non_print;
	Int2 spec_version;

	if (aip->type & ASNIO_OUT)
	{
		if (aip->type & ASNIO_TEXT)
			AsnPrintNewLine(aip);
        AsnIoFlush(aip);                /* clear it */
	}

	fp = aip->fp;                     /* things to save */
	linelength = aip->linelength;
	tabsize = aip->tabsize;
	type = aip->type;
	max_type = aip->max_type;
	max_indent = aip->max_indent;
    first = aip->first;
	typestack = aip->typestack;
	error_ret = aip->error_ret;
    iostruct = aip->iostruct;
    readfunc = aip->readfunc;
    writefunc = aip->writefunc;
    tbufsize = aip->bufsize;
    buf = aip->buf;
	fname = aip->fname;
	aop = aip->aop;
	aeop = aip->aeop;
	aeosp = aip->aeosp;
	fix_non_print = aip->fix_non_print;
	spec_version = aip->spec_version;

	MemFill(aip, '\0', sizeof(AsnIo));	 /* clear it */
    MemFill(buf, '\0', (size_t)tbufsize);

	aip->fp = fp;					  /* replace saved things */
	aip->tabsize = tabsize;
	aip->linelength = linelength;
	aip->type = type;
	aip->max_type = max_type;
	aip->max_indent = max_indent;
    aip->buf = buf;
	aip->linebuf = (CharPtr)aip->buf;
    aip->first = first;
	aip->typestack = typestack;
	aip->error_ret = error_ret;
    aip->iostruct = iostruct;
    aip->readfunc = readfunc;
    aip->writefunc = writefunc;
    aip->bufsize = tbufsize;
	aip->fname = fname;
	aip->aop = aop;
	aip->aeop = aeop;
	aip->aeosp = aeosp;
	aip->fix_non_print = fix_non_print;
	aip->spec_version = spec_version;
										/* clear stacks and buffers */
	MemFill(aip->typestack, '\0', (size_t)(sizeof(Pstack) * aip->max_type));
	MemFill(aip->first, '\0', (size_t)(sizeof(Boolean) * aip->max_indent));
	aip->first[0] = TRUE;

	if (aip->type & ASNIO_OUT)
	   	aip->bytes = aip->bufsize;   /* space to write */

	return;
}

/*****************************************************************************
*
*   void AsnIoFlush(aip)
*
*****************************************************************************/
NLM_EXTERN void LIBCALL  AsnIoFlush (AsnIoPtr aip)
{
	if (! (aip->type & ASNIO_OUT))
		return;

    if (aip->offset)
        AsnIoWriteBlock(aip);
    if (aip->type & ASNIO_FILE)    /* not a socket, clear buffers */
        fflush(aip->fp);
    return;
}

/*****************************************************************************
*
*	AsnIoMemOpen(mode, buf, size)   
*
*****************************************************************************/

NLM_EXTERN AsnIoMemPtr LIBCALL  AsnIoMemOpen (CharPtr mode, BytePtr buf, Int4 size)   /* size of buffer, or bytes_to_read */
{
	Int1 type;
	AsnIoMemPtr aimp;

	if (! StringCmp(mode, "r"))
		type = (ASNIO_IN | ASNIO_TEXT);
	else if (! StringCmp(mode, "rb"))
		type = (ASNIO_IN | ASNIO_BIN);
	else if (! StringCmp(mode, "w"))
		type = (ASNIO_OUT | ASNIO_TEXT);
	else if (! StringCmp(mode, "wb"))
		type = (ASNIO_OUT | ASNIO_BIN);
	else if (! StringCmp(mode, "wx"))
	{
		type = (ASNIO_OUT | ASNIO_TEXT);
		type |= ASNIO_XML;
	}
	else
	{
		AsnIoErrorMsg(NULL, 81, mode);
		return NULL;
	}

	aimp =(AsnIoMemPtr) MemNew(sizeof(AsnIoMem));
	aimp->size = size;
	aimp->buf = buf;
	aimp->aip = AsnIoNew(type, NULL, (Pointer)aimp, AsnIoMemRead, AsnIoMemWrite);
	return aimp;
}

/*****************************************************************************
*
*   AsnIoMemClose(aimp)
*
*****************************************************************************/
NLM_EXTERN AsnIoMemPtr LIBCALL  AsnIoMemClose (AsnIoMemPtr aimp)
{
	if (aimp == NULL)
		return NULL;
	AsnIoClose(aimp->aip);
	return (AsnIoMemPtr) MemFree(aimp);
}

/*****************************************************************************
*
*   AsnIoMemWrite(ptr, buf, count)
*
*****************************************************************************/
NLM_EXTERN Int2 LIBCALL  AsnIoMemWrite (Pointer ptr, CharPtr buf, Uint2 count)
{
	Int4 bytes;
	AsnIoMemPtr aimp;

	aimp = (AsnIoMemPtr)ptr;
	bytes = MIN((Int4)count, ((aimp->size) - (aimp->count)));
	if (! bytes)
		return (Int2)(-1);
	MemCopy((aimp->buf + aimp->count), buf, (size_t)bytes);
	aimp->count += bytes;
	return (Int2) bytes;
}

/*****************************************************************************
*
*   AsnIoMemRead(ptr, buf, count)
*
*****************************************************************************/
NLM_EXTERN Int2 LIBCALL  AsnIoMemRead (Pointer ptr, CharPtr buf, Uint2 count)
{
	Int4 bytes;
	AsnIoMemPtr aimp;

	aimp = (AsnIoMemPtr)ptr;
	bytes = MIN((Int4)count, ((aimp->size) - (aimp->count)));
	MemCopy(buf, (aimp->buf + aimp->count), (size_t) bytes);
	aimp->count += bytes;
	return (Int2) bytes;
}

/*****************************************************************************
*
*   AsnIoMemReset(aimp, bytes_to_read)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL  AsnIoMemReset (AsnIoMemPtr aimp, Int4 bytes_to_read)
{
	AsnIoReset(aimp->aip);
	aimp->count = 0;
	if (aimp->aip->type & ASNIO_IN)
		aimp->size = bytes_to_read;
	return TRUE;
}

/*****************************************************************************
*
*	AsnIoBSOpen(mode, buf, size)   
*
*****************************************************************************/
NLM_EXTERN AsnIoBSPtr LIBCALL  AsnIoBSOpen (CharPtr mode, ByteStorePtr bsp)
{
	Int1 type;
	AsnIoBSPtr aibp;

	if ((mode == NULL) || (bsp == NULL))
		return NULL;

	if (! StringCmp(mode, "r"))
		type = (ASNIO_IN | ASNIO_TEXT);
	else if (! StringCmp(mode, "rb"))
		type = (ASNIO_IN | ASNIO_BIN);
	else if (! StringCmp(mode, "w"))
		type = (ASNIO_OUT | ASNIO_TEXT);
	else if (! StringCmp(mode, "wb"))
		type = (ASNIO_OUT | ASNIO_BIN);
	else if (! StringCmp(mode, "wx"))
	{
		type = (ASNIO_OUT | ASNIO_TEXT);
		type |= ASNIO_XML;
	}
	else
	{
		AsnIoErrorMsg(NULL, 81, mode);
		return NULL;
	}

	aibp = (AsnIoBSPtr) MemNew(sizeof(AsnIoBS));
	aibp->bsp = bsp;
	BSSeek(bsp, 0, SEEK_SET);
	aibp->aip = AsnIoNew(type, NULL, (Pointer)aibp, AsnIoBSRead, AsnIoBSWrite);
	if (type & ASNIO_OUT)
		AsnIoSetBufsize(aibp->aip, 10000);  /* big internal buffer for */
		                                    /* big ByteStore blocks */
	return aibp;
}

/*****************************************************************************
*
*   AsnIoBSClose(aibp)
*
*****************************************************************************/
NLM_EXTERN AsnIoBSPtr LIBCALL  AsnIoBSClose (AsnIoBSPtr aibp)
{
	if (aibp == NULL)
		return NULL;
	AsnIoClose(aibp->aip);
	return (AsnIoBSPtr) MemFree(aibp);
}

/*****************************************************************************
*
*   AsnIoBSWrite(ptr, buf, count)
*
*****************************************************************************/
NLM_EXTERN Int2 LIBCALL  AsnIoBSWrite (Pointer ptr, CharPtr buf, Uint2 count)
{
	Int4 bytes;
	AsnIoBSPtr aibp;

	aibp = (AsnIoBSPtr)ptr;
	bytes = BSWrite(aibp->bsp, (Pointer) buf, (Int4) count);
	return (Int2) bytes;
}

/*****************************************************************************
*
*   AsnIoBSRead(ptr, buf, count)
*
*****************************************************************************/
NLM_EXTERN Int2 LIBCALL  AsnIoBSRead (Pointer ptr, CharPtr buf, Uint2 count)
{
	Int4 bytes;
	AsnIoBSPtr aibp;

	aibp = (AsnIoBSPtr)ptr;
	bytes = BSRead(aibp->bsp, (Pointer) buf, (Int4) count);
	return (Int2) bytes;
}

/*****************************************************************************
*
*   AsnIoCopy(from, fromfunc, tofunc)
*   	Uses the object loader functions to write "from" to a temporary
*   file then read a new object and return a pointer to it
*
*****************************************************************************/
NLM_EXTERN Pointer LIBCALL  AsnIoCopy (Pointer from, AsnReadFunc readfunc, AsnWriteFunc writefunc)
{
	Pointer res;
	AsnIoPtr aip;
	CharPtr fname;
	
	if ((from == NULL) || (readfunc == NULL) || (writefunc == NULL))
		return NULL;

	fname = TmpNam(NULL);
	aip = AsnIoOpen(fname, "wb");
	if (aip == NULL)
		return NULL;

    if (! (*writefunc)(from, aip, NULL))
	{
		AsnIoClose(aip);
		return NULL;
	}

	AsnIoClose(aip);

	aip = AsnIoOpen(fname, "rb");
	res = (*readfunc)(aip, NULL);

	AsnIoClose(aip);
	FileRemove(fname);

	return res;
}

/*****************************************************************************
*
*   AsnIoMemCopy(from, fromfunc, tofunc)
*   	Uses the object loader functions to write "from" to a ByteStore
*   then read a new object and return a pointer to it
*
*****************************************************************************/
NLM_EXTERN Pointer LIBCALL  AsnIoMemCopy (Pointer from, AsnReadFunc readfunc, AsnWriteFunc writefunc)
{
	Pointer res;
	AsnIoBSPtr aibp;
	ByteStorePtr bsp;
	
	if ((from == NULL) || (readfunc == NULL) || (writefunc == NULL))
		return NULL;

	bsp = BSNew(5000);
	aibp = AsnIoBSOpen("wb", bsp);
	if (aibp == NULL)
		return NULL;

    if (! (*writefunc)(from, aibp->aip, NULL))
	{
		AsnIoBSClose(aibp);
		BSFree(bsp);
		return NULL;
	}

	AsnIoBSClose(aibp);

	aibp = AsnIoBSOpen("rb", bsp);
	res = (*readfunc)(aibp->aip, NULL);

	AsnIoBSClose(aibp);
	BSFree(bsp);

	return res;
}

static Int2 LIBCALLBACK AsnIoCompWrite PROTO((Pointer, CharPtr, Uint2));

/*****************************************************************************
*
*   AsnIoMemComp(a, b, writefunc)
*   	Uses the object loader functions to write "a" to a ByteStore
*   then does a byte by byte compare with the AsnIo stream from "b"
*   Returns TRUE if identical.
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL  AsnIoMemComp (Pointer a, Pointer b, AsnWriteFunc writefunc)
{
	AsnIoBSPtr aibp;
	ByteStorePtr bsp;
	Boolean retval = FALSE;
	ErrOpts opts;
	
	if ((a == NULL) || (b == NULL) || (writefunc == NULL))
		return retval;

	bsp = BSNew(5000);
	aibp = AsnIoBSOpen("wb", bsp);
	if (aibp == NULL)
		return retval;

    retval = (*writefunc)(a, aibp->aip, NULL);

	AsnIoBSClose(aibp);

	if (! retval)
	{
		BSFree(bsp);
		return FALSE;
	}

	aibp = (AsnIoBSPtr) MemNew(sizeof(AsnIoBS));
	aibp->bsp = bsp;
	BSSeek(bsp, 0, SEEK_SET);
	aibp->aip = AsnIoNew((Int1)(ASNIO_OUT | ASNIO_BIN), NULL, (Pointer)aibp, NULL, AsnIoCompWrite);

	ErrSaveOptions(&opts);          /* save state */
	ErrSetOpts(ERR_IGNORE, 0);   /* set to ignore an error */

    retval = (*writefunc)(b, aibp->aip, NULL);

	AsnIoFlush(aibp->aip);
	if (aibp->aip->io_failure == TRUE)
		retval = FALSE;

	AsnIoBSClose(aibp);
	BSFree(bsp);

	ErrRestoreOptions(&opts);   /* reset state */

	return retval;
}

/*****************************************************************************
*
*   AsnIoCompWrite(ptr, buf, count)
*
*****************************************************************************/
static Int2 LIBCALLBACK AsnIoCompWrite (Pointer ptr, CharPtr buf, Uint2 count)
{
	Uint2 bytes;
	Int2 the_byte;
	AsnIoBSPtr aibp;

	aibp = (AsnIoBSPtr)ptr;
	if (aibp->aip->io_failure)
		return (Int2) count;
		
	for (bytes = 0; bytes < count; bytes++, buf++)
	{
		the_byte = BSGetByte(aibp->bsp);
		if ((the_byte == EOF) ||
		     ((Uint1)(*buf) != (Uint1)the_byte))
		{
			aibp->aip->io_failure = TRUE;
			return (Int2) count;
		}
	}
	return (Int2) bytes;
}
/*****************************************************************************
*
*   AsnIoHashWrite(ptr, buf, count)
*
*****************************************************************************/
static Int2 LIBCALL  AsnIoHashWrite (Pointer ptr, CharPtr buf, Uint2 count)
{
    Uint4Ptr hashPt =  (Uint4Ptr) ptr;
    Uint4 hash = * hashPt;
    Uint2 i;
    
    for (i=0; i < count; i ++){
        hash *= (Uint4) 1103515245;
        hash += (Uint4) (buf[i]) + (Uint4) 12345;
    }
    *hashPt = hash; 
    
    return (Int2) count;
}
/*****************************************************************************
*
*   AsnIoHash(from, tofunc)
*   	Uses the object loader functions to calculate a hash value
*
*****************************************************************************/
NLM_EXTERN Uint4 LIBCALL  AsnIoHash (Pointer from, AsnWriteFunc writefunc)
{
    Uint4 retval = 0;
    AsnIoPtr aip;
    
    if ((from == NULL) || (writefunc == NULL))
        return retval;
    
    
    aip = AsnIoNew(ASNIO_OUT | ASNIO_BIN, NULL, (Pointer)&retval, 
                   NULL, AsnIoHashWrite);

    if (! (*writefunc)(from, aip, NULL))
        AsnIoFlush(aip);
    
    AsnIoClose(aip);        
    return retval;
}
