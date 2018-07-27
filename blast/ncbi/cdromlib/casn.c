/* casn.c
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
 * RCS $Id: casn.c,v 6.1 2001/04/27 18:00:30 juran Exp $
 *
 * Author:  Greg Schuler
 *
 * Version Creation Date: 9/23/92
 *
 * File Description:  functions to decompress a compressed ASN,1 (CASN) file.
 *
 * Modifications:  
 * --------------------------------------------------------------------------
 * Date     Name        Description of modification
 * -------  ----------  -----------------------------------------------------
 * 04-21-93 Schuler     CASN_ReadBuff declared as LIBCALLBACK
 * 06-28-93 Schuler     New function:  CASN_Seek().
 * 06-17-94 Schuler     Modified to support new file format that is to debut 
 *                      in Entrez release 13.0
 * 07-20-94 Schuler     Fixed bug in CASN_Open (incorrect doc_type)
 * 09-07-94 Schuler     Changed implementation of rd_string (one byte length)
 * 10-05-94 Schuler     Added CASN_NextBiostruc
 *
 * 05-19-95 Schuler     Added rcs Log directive for automatic insertion of
 *                      modification comments.
 *
 * Revision $Log: casn.c,v $
 * Revision Revision 6.1  2001/04/27 18:00:30  juran
 * Revision Warnings.
 * Revision
 * Revision Revision 6.0  1997/08/25 18:12:41  madden
 * Revision Revision changed to 6.0
 * Revision
 * Revision Revision 5.2  1997/06/26 21:55:21  vakatov
 * Revision [PC] DLL'd "ncbicdr.lib", "ncbiacc.lib", "ncbinacc.lib" and "ncbicacc.lib"
 * Revision
 * Revision Revision 5.1  1997/05/29 18:17:11  savchuk
 * Revision CASN_NextSeqEntry() function is now seeking to the end of compressed ASN
 * Revision
 * Revision 5.0  1996/05/28  13:55:34  ostell
 * Set to revision 5.0
 *
 * Revision 4.0  1995/07/26  13:50:32  ostell
 * force revision to 4.0
 *
 * Revision 2.11  1995/06/23  16:02:43  kans
 * support for accmmdbs.c stub to resolve symbols without MMDB link
 *
 * Revision 2.10  1995/06/23  13:22:25  kans
 * Biostruc_CD_supported symbol needed for local MMDB access
 *
 * Revision 2.9  1995/05/16  14:36:20  schuler
 * Automatic comment insertion enabled
 *
 *
 * ==========================================================================
 */

#define REVISION_STR "$Revision: 6.1 $"


#include <asn.h>
#include <casn.h>

struct casn_ioblock
{
	short      rel_major;
	short      rel_minor;
	int        magic;
	int        format;
	int        compr;
	long	   bytes;
	CASN_Type  doc_type;
	long       doc_count;
	long       uid_min;
	long       uid_max;
	int        huff_count;
	short     *huff_left;
	short     *huff_right;
	unsigned   byte;
	unsigned   mask;
	FILE      *fd;
	AsnIo     *aio;
	AsnModule *amp;
	AsnType   *atp;
};


#define CURRENT_FILEFORMAT	2
#define MAGIC_FILEFORMAT	4541

#define MAGIC_IOBLOCK		3958
#define Handle_IsValid(x)  ((x) && ((x)->magic == MAGIC_IOBLOCK))

enum CASN_Compr { CASN_ComprNone, CASN_ComprHuff };

static char * _asn_type[] = { "", "Medline-entry", "Seq-entry" };
static char * file_emsg = "Unrecognized compressed file format [%s]\n";

static int compr_none_read (CASN_Handle handle, char *buff, int count);
static int compr_huff_read (CASN_Handle handle, char *buff, int count);


static Int2 LIBCALLBACK CASN_ReadBuff(Pointer param, CharPtr buffer, Uint2 count);

static char * rd_string (FILE *fd);
static unsigned long rd_integer (FILE *fd, int bytes);
#define RD_SHORT(f)   (short)rd_integer(fd,2)
#define RD_USHORT(f)  (unsigned short)rd_integer(fd,2)
#define RD_INT(f)     (int)rd_integer(fd,2)
#define RD_UINT(f)    (unsigned int)rd_integer(fd,2)
#define RD_LONG(f)    (long)rd_integer(fd,4)
#define RD_ULONG(f)   rd_integer(fd,4)



/* --------------- High-Level Functions --------------- */

NLM_EXTERN CASN_Handle LIBCALL CASN_Open (char *fname)
{
	int	i, j;
	CASN_Handle handle;
	FILE *fd =NULL;
	int	doc_type;
	long	l1, l2, l3;
	int     huff_count;
	short rel_major =0, rel_minor =0;

	if (!(fd = FileOpen(fname,"rb")))
	{
		ErrPostEx(SEV_ERROR,CASN_ErrFileOpen,0,"Unable to open file %s\n",fname);
		return NULL;
	}

	/* check to see that the file is recognizable */
	i = RD_SHORT(fd);
	j = RD_SHORT(fd);
	if (i != MAGIC_FILEFORMAT  ||  j > CURRENT_FILEFORMAT)
	{
		FileClose(fd);
		ErrPostEx(SEV_ERROR,CASN_ErrFileFormat,0,file_emsg,fname);
		return NULL;
	}
	if (j == CURRENT_FILEFORMAT)
	{
		rel_major = RD_SHORT(fd);
		rel_minor = RD_SHORT(fd);
	}

	l1 = RD_LONG(fd);
	l2 = RD_LONG(fd);
	l3 = RD_LONG(fd);

	doc_type = RD_SHORT(fd);
	if (j<CURRENT_FILEFORMAT)
		doc_type = -doc_type;
	huff_count = RD_SHORT(fd);

	if (!(handle = CASN_New((CASN_Type)doc_type,huff_count)))
	{
		FileClose(fd);
		return NULL;
	}
	handle->format = j;
	handle->huff_count = huff_count;
	for (i=0; i<huff_count; ++i)
		handle->huff_left[i] = RD_SHORT(fd);
	for (i=0; i<huff_count; ++i)
		handle->huff_right[i] = RD_SHORT(fd);

	if (!(handle->aio = AsnIoNew(ASNIO_BIN_IN,fd,handle,CASN_ReadBuff,NULL)))
	{
		FileClose(fd);
		CASN_Free(handle);
		return NULL;
	}
	handle->aio->fname = StrSave(fname);
	handle->fd = fd;
	handle->amp = AsnAllModPtr();
	handle->doc_count = l1;
	handle->uid_min = l2;
	handle->uid_max = l3;
	handle->rel_major = rel_major;
	handle->rel_minor = rel_minor;
	
	if (handle->format == CURRENT_FILEFORMAT)
	{
		char *asntype;
		/* skip over some things */
		MemFree((void*)rd_string(fd));
		rd_integer(fd,2);
		MemFree((void*)rd_string(fd));
		MemFree((void*)rd_string(fd));
		asntype = rd_string(fd);
		handle->atp = AsnTypeFind(handle->amp,asntype);
		MemFree((void*)asntype);
	}
	else
	{
		handle->atp = AsnTypeFind(handle->amp,_asn_type[-doc_type]);
	}
	return handle;
}

NLM_EXTERN void LIBCALL CASN_Close (CASN_Handle handle)
{
	ASSERT(Handle_IsValid(handle));
	AsnIoClose(handle->aio);
	CASN_Free(handle);
}

NLM_EXTERN AsnIo* LIBCALL CASN_GetAsnIoPtr (CASN_Handle handle)
{
	ASSERT(Handle_IsValid(handle));
	return handle->aio;
}

NLM_EXTERN CASN_Type LIBCALL CASN_DocType (CASN_Handle handle)
{
	ASSERT(Handle_IsValid(handle));
	return handle->doc_type;
}

NLM_EXTERN long LIBCALL CASN_DocCount (CASN_Handle handle)
{
	ASSERT(Handle_IsValid(handle));
	return handle->doc_count;
}

NLM_EXTERN MedlineEntry* LIBCALL CASN_NextMedlineEntry (CASN_Handle handle)
{
	AsnTypePtr atp;

	ASSERT(Handle_IsValid(handle));
	atp = AsnReadId(handle->aio,handle->amp,handle->atp);
	return atp ? MedlineEntryAsnRead(handle->aio,atp) : NULL;
}


NLM_EXTERN SeqEntry* LIBCALL CASN_NextSeqEntry (CASN_Handle handle)
{
	AsnTypePtr atp;

	ASSERT(Handle_IsValid(handle));
	if ((atp = AsnReadId(handle->aio, handle->amp, handle->atp))) {
	  SeqEntryPtr sep = SeqEntryAsnRead(handle->aio, atp);
	  while(handle->compr != -1) {
	    char buf[4];
	    compr_huff_read(handle, buf, 1);
	  }
	  return sep;
	}
	return NULL;
}


#ifdef Biostruc_supported
NLM_EXTERN Biostruc* LIBCALL CASN_NextBiostruc (CASN_Handle handle)
{
	AsnTypePtr atp;

	if (! BiostrucAvail ()) return NULL;
	ASSERT(Handle_IsValid(handle));
	atp = AsnReadId(handle->aio,handle->amp,handle->atp);
	return atp ? BiostrucAsnRead(handle->aio,atp) : NULL;
}
#endif

NLM_EXTERN int LIBCALL CASN_Seek (CASN_Handle handle, long offset, int origin)
{
	ASSERT(Handle_IsValid(handle));
	handle->compr = -1;          /* to reset the Huffman state */
	AsnIoReset(handle->aio);     /* to reset the ASN state */
	return fseek(handle->fd,offset,origin);
}


/* --------------- Low-Level Functions --------------- */

NLM_EXTERN CASN_Handle  LIBCALL CASN_New (CASN_Type doc_type, int huff_count)
{
	CASN_Handle handle;
	short *left;
	short *right;

	if (!(handle = (CASN_Handle) MemNew(sizeof(struct casn_ioblock))))
		return NULL;
	if (!(left = (short*) MemNew(huff_count*sizeof(short))))
		return NULL;
	if (!(right = (short*) MemNew(huff_count*sizeof(short))))
		return NULL;

	handle->magic = MAGIC_IOBLOCK;
	handle->doc_type = doc_type;
	handle->compr = -1;
	handle->huff_left = left;
	handle->huff_right = right;
	return handle;
}


NLM_EXTERN void LIBCALL CASN_Free (CASN_Handle handle)
{
	ASSERT(Handle_IsValid(handle));
	MemFree(handle->huff_left);
	MemFree(handle->huff_right);
	MemFree(handle);
}


static Int2 LIBCALLBACK CASN_ReadBuff(Pointer param, CharPtr buff, Uint2 count)
{
	CASN_Handle handle = (CASN_Handle) param;
	Int2 retval = 0;

	ASSERT(Handle_IsValid(handle));

	while (! retval)   /* has to allow for 0 bytes from compressed read */
	{
		if (handle->compr < 0)
		{
			Int2 c;
	
			/* read the "decompression protocol identifier" */
			if ((c = fgetc(handle->fd)) == EOF)
				return 0;
	
			if (c == CASN_ComprNone)
			{
				handle->bytes = rd_integer(handle->fd,3);
			}
			else if (c == CASN_ComprHuff)
			{
				if (handle->format ==2)
					rd_integer(handle->fd,3);   /* justskip over it for now */
				handle->byte = 0;
				handle->mask = 0;
			}
			else
			{
				ErrPostEx(SEV_ERROR,CASN_ErrFileFormat,0,file_emsg,"ReadBuff");
				return 0;
			}
			handle->compr = c;
		}
	
		switch(handle->compr)
		{
			case CASN_ComprNone:
				return compr_none_read(handle,buff,count);
	
			case CASN_ComprHuff:
				if ((retval = compr_huff_read(handle,buff,count)) !=0)
					return retval;
				break;
	
			default:
				ErrPostEx(SEV_ERROR,CASN_ErrFileFormat,0,file_emsg,"ReadBuff");
				return 0;
		}
	}

	return 0;
}


static int compr_none_read (CASN_Handle handle, char *buff, int count)
{
	size_t bytes;
	
	ASSERT(Handle_IsValid(handle));
	bytes = (size_t) MIN(handle->bytes,(Int4)count);
	bytes = FileRead(buff,1,bytes,handle->fd);
	handle->bytes -= bytes;
	if (handle->bytes <= 0)
	{
		/* reset for stream read of next entry */
		handle->compr = -1;
	}
	return (int)bytes;
}


static int compr_huff_read (CASN_Handle handle, char *buff, int count)
{
	register unsigned mask, byte;
	FILE *fd;
	char *p = buff;
	int i, cnt = 0;
	int c;
	int k;

	ASSERT(Handle_IsValid(handle));

	fd = handle->fd;
	mask = handle->mask;
	byte = handle->byte;

	while (cnt < count)
	{
		for (i=0; i>=0; )
		{
			if (mask == 0)
			{
				if ((c = fgetc(fd)) == EOF)
				{
					/* should never reach this point */
					ErrPostEx(SEV_INFO,0,0,
						"Unexpected EOF reading Huffman-compressed ASN.1");
					i = handle->huff_count - 257;
					break;
				}
				else
				{
					byte = (Uint2) c;
					mask = 0x80;
				}
			}

			if (byte & mask)
				i = handle->huff_left[i];
			else
				i = handle->huff_right[i];

			mask >>= 1;
		}

		if ((k = i + 257) == handle->huff_count)
		{
			handle->compr = -1; /* reset for next record */
			break;
		}

		*p++ = (char) k;
		cnt++;
	}

	handle->mask = mask;
	handle->byte = byte;
	return cnt;
}

static unsigned long rd_integer (FILE *fd, int bytes)
{
	int i, c;
	unsigned long value = 0;

	for (i=0; i<bytes; ++i)
	{
		if ((c = fgetc(fd)) ==EOF)  break;
		value <<= 8;
		value |= (unsigned long)c;
	}
	return value;
}

static char * rd_string (FILE *fd)
{
	size_t len = (size_t) fgetc(fd);
	if (len > 0)
	{
		char *str = MemGet(len+1,MGET_ERRPOST);
		if (fread((void*)str,1,len,fd) != len)
		{
			ErrPostEx(SEV_ERROR,CASN_ErrFileFormat,0,"File format error");
			MemFree((void*)str);
			return NULL;
		}
		*(str+len) = '\0';
		return str;
	}
	return NULL;
}

