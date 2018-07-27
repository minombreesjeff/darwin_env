/*   ffprint.c
* ===========================================================================
*
*                            PUBLIC DOMAIN NOTICE
*            National Center for Biotechnology Information (NCBI)
*
*  This software/database is a "United States Government Work" under the
*  terms of the United States Copyright Act.  It was written as part of
*  the author's official duties as a United States Government employee and
*  thus cannot be copyrighted.  This software/database is freely available
*  to the public for use. The National Library of Medicine and the U.S.
*  Government do not place any restriction on its use or reproduction.
*  We would, however, appreciate having the NCBI and the author cited in
*  any work or product based on this material
*
*  Although all reasonable efforts have been taken to ensure the accuracy
*  and reliability of the software and data, the NLM and the U.S.
*  Government do not and cannot warrant the performance or results that
*  may be obtained by using this software or data. The NLM and the U.S.
*  Government disclaim all warranties, express or implied, including
*  warranties of performance, merchantability or fitness for any particular
*  purpose.
*
* ===========================================================================
*
* File Name:  ffprint.c
*
* Author:  Karl Sirotkin, Tom Madden, Tatiana Tatusov
*
* Version Creation Date:   7/15/95
*
* $Revision: 6.8 $
*
* File Description: 
*
* Modifications:  
* --------------------------------------------------------------------------
 * $Log: ffprint.c,v $
 * Revision 6.8  2003/07/15 14:35:56  dondosha
 * Added #defines for substitutes to fprintf and fflush, needed for gzip compression of Web BLAST results
 *
 * Revision 6.7  2002/08/26 22:06:57  kans
 * ff_RecalculateLinks (MS) to fix hotlink artifact
 *
 * Revision 6.6  1999/10/07 19:22:21  bazhin
 * Changed Int2 to Int4 for one variable. To prevent coredump.
 *
 * Revision 6.5  1999/10/06 20:25:28  bazhin
 * Removed memory leak.
 *
 * Revision 6.4  1999/08/31 14:36:06  tatiana
 * ff_print_string_mem() added
 *
 * Revision 6.3  1999/04/09 21:17:18  bazhin
 * Added functions "FFBSPrint()" and "BSAppend()" to parse ASN.1
 * data to ByteStore.
 *
 * Revision 6.2  1999/04/01 20:44:13  kans
 * Int2 lengths to Int4 to allow CountGapsInDeltaSeq with buffer > 32K
 *
 * Revision 6.1  1999/03/12 17:47:11  tatiana
 * file was lost
 *
 * Revision 6.7  1999/02/02 17:29:20  kans
 * added ff_MergeString
 *
 * Revision 6.6  1998/06/15 15:00:31  tatiana
 * UNIX compiler warnings fixed
 *
 * Revision 6.5  1998/02/26 22:43:46  madden
 * Used init_buff_ex in ff_StartPrint
 *
 * Revision 6.4  1998/02/19 01:54:51  ostell
 * added use of Thread Local Storage to make functions thread safe
 *
 * Revision 6.3  1997/10/23 20:21:31  madden
 * Fixed ByteStore leak
 *
 * Revision 6.2  1997/10/23 20:03:51  tatiana
 * allocates byte store in init_buf_ex()
 *
 * Revision 6.1  1997/09/16 16:56:04  tatiana
 * buffer reinit in ff_StartPrint
 *
 * Revision 6.0  1997/08/25 18:05:32  madden
 * Revision changed to 6.0
 *
 * Revision 5.8  1997/08/04 22:56:21  tatiana
 * init_buff_ex() added
 *
 * Revision 5.7  1997/07/18 20:11:16  vakatov
 * Restored NLM_EXTERNs which were lost in R5.5
 *
 * Revision 5.6  1997/07/16 21:24:12  tatiana
 * add AddPintLater
 *
Revision 5.3  1997/02/25  22:08:59  tatiana
ff_AddStringWithTildes(0 will print out "`~" as real tilde

Revision 5.2  1997/01/08  18:52:45  madden
Added LIBCALL's.

Revision 5.1  1996/09/10  14:26:56  tatiana
init_buff() added to ff_StartPrint

Revision 4.8  1996/04/15  18:44:41  tatiana
free_buff() added

Revision 4.4  1996/01/29  22:41:20  tatiana
ChangeStringWithTildes function added

Revision 4.3  1995/12/13  21:04:47  kans
descriptors now store entityID/itemID/itemtype in proper array

Revision 4.2  1995/12/13  16:34:06  tatiana
itemID added to FFPrintArray structure

Revision 4.1  1995/11/17  21:28:35  kans
asn2ff now uses gather (Tatiana)

Revision 1.5  1995/07/17  19:33:20  kans
parameters combined into Asn2ffJobPtr structure
*
* ==========================================================================
*/

#include <asn2ffp.h>
#include <ffprint.h>
#include <ncbithr.h>

int (*ff_fprintf)(FILE*, const char *, ...) = fprintf;
int (*ff_fflush)(FILE*) = fflush;

#define fprintf ff_fprintf
#define fflush ff_fflush

static TNlmTls ffprint_tls = NULL;

static void free_buff_contents(BuffStructPtr bfp)
{
	if (bfp == NULL) return;
	bfp->buffer = MemFree(bfp->buffer);
	bfp->line_prefix = MemFree(bfp->line_prefix);
	bfp->line_return = MemFree(bfp->line_return);
	if (bfp->byte_sp)
		bfp->byte_sp = BSFree(bfp->byte_sp);
	return;
}

static void BuffStructCleanup (TNlmTls tls, VoidPtr ptr)
{
	BuffStructPtr bfp;

	bfp = (BuffStructPtr)ptr;
	free_buff_contents(bfp);
	MemFree(bfp);
	return;
}


static BuffStruct PNTR NEAR GetBuffStruct ( void )
{
	BuffStructPtr bfp = NULL;

	if (NlmTlsGetValue( ffprint_tls, (VoidPtr *)(&bfp)))
	{
		if (bfp == NULL)
		{
			bfp = MemNew(sizeof(BuffStruct));
			NlmTlsSetValue(&ffprint_tls, bfp, BuffStructCleanup);
		}
	}
	return bfp;
}

/******************************************************************************
*
*	print utilities to handle the printing of the asn2ff system.
*	
*****************************************************************************/
NLM_EXTERN void LIBCALL init_buff(void)
{
	BuffStructPtr bfp;

	bfp = GetBuffStruct();
	bfp->buffer = (CharPtr) MemNew((MAX_BTP_BUF+2)*sizeof(Char));
	bfp->line_prefix = (CharPtr) MemNew(3*sizeof(Char));
	bfp->newline = NEWLINE;
}

NLM_EXTERN void LIBCALL init_buff_ex(Int2 init_size)
{
	BuffStructPtr bfp;

	bfp = GetBuffStruct();
	bfp->buffer = (CharPtr) MemNew((init_size+2)*sizeof(Char));
	bfp->line_prefix = (CharPtr) MemNew(3*sizeof(Char));
	bfp->newline = NEWLINE;
	bfp->byte_sp = BSNew(init_size+2);
}

NLM_EXTERN void LIBCALL free_buff(void)
{
	BuffStructPtr bfp;

	bfp = GetBuffStruct();
	free_buff_contents (bfp);
	return;
}

/***********************************************************************
*void asn2ff_set_output (FILE * fp, CharPtr line_return)
*
**************************************************************************/

NLM_EXTERN void LIBCALL asn2ff_set_output (FILE *fp, CharPtr line_return)
{
	BuffStructPtr bfp;

	bfp = GetBuffStruct();
	bfp->fp = fp;
	if (line_return)
	{
		if(bfp->line_return != NULL)
			MemFree(bfp->line_return);
		bfp->line_return = StringSave(line_return);
	}
}

NLM_EXTERN CharPtr LIBCALL ff_MergeString (void)

{
	ByteStorePtr byte_sp=NULL;
	CharPtr string=NULL;
	BuffStructPtr bfp;

	bfp = GetBuffStruct();

	if (! bfp->fp)
	{
		byte_sp = bfp->byte_sp;

		string = BSMerge(byte_sp, NULL);
		bfp->byte_sp = BSFree(bfp->byte_sp);
	}

	return string;
}

NLM_EXTERN CharPtr LIBCALL FFPrint (FFPrintArrayPtr pap, Int4 index, Int4 pap_size)
{
	GBEntryPtr gbp;
	Asn2ffJobPtr ajp;
	ByteStorePtr byte_sp=NULL;
	CharPtr string=NULL;
	FFPrintArray pa=pap[index];
	BuffStructPtr bfp;

	bfp = GetBuffStruct();

	gbp = pa.gbp;
	ajp = pa.ajp;
	if (ajp == NULL) {
		return NULL;
	}
	ajp->pap_index = pa.index;
	ajp->pap_last = pa.last;

	if (index == 0 || index == (pap_size-1))
	{
		flat2asn_delete_locus_user_string();
		flat2asn_install_locus_user_string(gbp->locus);
		flat2asn_delete_accession_user_string();
		flat2asn_install_accession_user_string(gbp->accession);
	}

	bfp->n_links = 0;
	pa.fct(ajp, gbp);
	if (pa.descr == NULL) {
		pap[index].descr = gbp->descr;
	}
	if (pa.printxx == PRINTXX) {
		PrintXX();
	}
	if (! bfp->fp) {
		byte_sp = bfp->byte_sp;
		string = BSMerge(byte_sp, NULL);
		bfp->byte_sp = BSFree(bfp->byte_sp);
	}

	return string;
}

/**********************************************************/
static ByteStorePtr BSAppend(ByteStorePtr to, ByteStorePtr from)
{
    Nlm_BSUnitPtr tobup;
    Nlm_BSUnitPtr frombup;

    if(from == NULL)
        return(to);

    if(to == NULL)
    {
        to = MemNew(sizeof(Nlm_ByteStore));
        to->totlen = 0;
        to->chain = NULL;
    }

    to->seekptr = 0;
    to->chain_offset = 0;

    if(to->chain != NULL)
        for(tobup = to->chain; tobup->next != NULL; tobup = tobup->next)
            continue;
    else
        tobup = NULL;

    for(frombup = from->chain; frombup != NULL; frombup = frombup->next)
    {
        to->totlen += frombup->len;
        if(tobup == NULL)
        {
            to->chain = MemNew(sizeof(Nlm_BSUnit));
            tobup = to->chain;
        }
        else
        {
            tobup->next = MemNew(sizeof(Nlm_BSUnit));
            tobup = tobup->next;
        }
        tobup->str = MemNew(frombup->len);
        MemCpy(tobup->str, frombup->str, frombup->len);
        tobup->len = frombup->len;
        tobup->len_avail = frombup->len_avail;
    }
    to->curchain = to->chain;
    return(to);
}

/**********************************************************/
NLM_EXTERN void LIBCALL FFBSPrint(FFPrintArrayPtr pap, Int4 index,
                                  Int4 pap_size)
{
    GBEntryPtr    gbp;
    Asn2ffJobPtr  ajp;
    ByteStorePtr  byte_sp = NULL;
    FFPrintArray  pa = pap[index];
    BuffStructPtr bfp;

    bfp = GetBuffStruct();

    gbp = pa.gbp;
    ajp = pa.ajp;
    if(ajp == NULL)
        return;

    ajp->pap_index = pa.index;
    ajp->pap_last = pa.last;

    if(index == 0 || index == pap_size - 1)
    {
        flat2asn_delete_locus_user_string();
        flat2asn_install_locus_user_string(gbp->locus);
        flat2asn_delete_accession_user_string();
        flat2asn_install_accession_user_string(gbp->accession);
    }

    bfp->n_links = 0;
    pa.fct(ajp, gbp);
    if(pa.descr == NULL)
        pap[index].descr = gbp->descr;

    if(pa.printxx == PRINTXX)
    {
        PrintXX();
    }

    ajp->byte_st = BSAppend(ajp->byte_st, bfp->byte_sp);
    BSFree(bfp->byte_sp);
    bfp->byte_sp = NULL;
}

NLM_EXTERN void LIBCALL ff_print_string (FILE *fp, CharPtr string, CharPtr line_return)

{
	CharPtr s;
	Int2	i, l;
	BuffStructPtr bfp;

	bfp = GetBuffStruct();

	if (! fp) {
		return;
	}
	if (! string || *string == *line_return) {
		ErrPostEx(SEV_WARNING, 1, 1, 
			"CAUTION: NULL String in ff_print_string\n\n");
		return;
	}
	for (s = string; (s = strchr(s, *line_return)) != NULL; s++) {
		*s = '\n';
	}
	for (i = 0, s = string; i < bfp->n_links; i++) {
		l = string - s + bfp->pos_links[i];
		if (s[l-1] == '\n') {
			for (; IS_WHITESP(s[l]); l++) ;
			fwrite(s, 1, l, fp);
		} else {
			fwrite(s, 1, l, fp);
		}
		fwrite(bfp->links[i], 1, StringLen(bfp->links[i]), fp);
		MemFree(bfp->links[i]);
		s += l;
	}
	fwrite(s, 1, StringLen(s), fp);
	BuffFree();
}

static CharPtr ff_print_insert(CharPtr str, CharPtr ins)
{
	CharPtr buff;
	
	buff = (CharPtr) MemNew(StringLen(str) + StringLen(ins) + 1);
	StringCpy(buff, ins);
	StringCat(buff, str);
	MemFree(str);
	return buff;
}

NLM_EXTERN CharPtr LIBCALL ff_print_string_mem (CharPtr string)

{
	CharPtr s, buff, bu;
	Int2	i, l;
	Int4	ll;
	BuffStructPtr bfp;

	bfp = GetBuffStruct();

	if (! string) {
		ErrPostEx(SEV_WARNING, 1, 1, 
			"CAUTION: NULL String in ff_print_string\n\n");
		return NULL;
	}
	ll = StringLen(string);
	for (i = 0, s = string; i < bfp->n_links; i++) {
		ll += StringLen(bfp->links[i]);
	}
	buff = (CharPtr) MemNew(ll+1);
	bu = buff;
	for (i = 0, s = string; i < bfp->n_links; i++) {
		l = string - s + bfp->pos_links[i];
		if (s[l-1] == '\n') {
			for (; IS_WHITESP(s[l]); l++) ;
		}
		MemCpy(bu, s, l);
		s += l;
		bu += l;
		StringCpy(bu, bfp->links[i]);
		bu += StringLen(bfp->links[i]);
		MemFree(bfp->links[i]);
	}
	StringCpy(bu, s);
	BuffFree();
	MemFree(string);
	return buff;
}

NLM_EXTERN Int2 LIBCALL ff_StartPrint (Int2 init_indent, Int2 cont_indent, Int2 line_max, CharPtr line_prefix)

{

	CharPtr buffer; 
	Int2 indent_space;
	BuffStructPtr bfp;

	bfp = GetBuffStruct();

	buffer = bfp->buffer;
	if (buffer == NULL) {
		if (line_max > MAX_BTP_BUF)
			init_buff_ex(line_max);
		else
			init_buff();
		buffer = bfp->buffer;
	}

	bfp->init_indent = init_indent;
	bfp->cont_indent = cont_indent;

	if (line_max > 0)
		bfp->line_max = line_max;
	else
		bfp->line_max = MAX_BTP_BUF;

	if (line_prefix) {
		StringCpy(bfp->line_prefix, line_prefix);
		*buffer = *line_prefix;
		buffer++;
		line_prefix++;
		*buffer = *line_prefix;
		buffer++;
		line_prefix++;
		indent_space = init_indent - StringLen(bfp->line_prefix);
	} else {
		bfp->line_prefix[0] = '\0'; 
		indent_space = init_indent;
	}

	if (indent_space > 0) {
		MemSet((VoidPtr) buffer, ' ', indent_space);
	}
	return init_indent;
}	/* ff_StartPrint */
		
NLM_EXTERN void LIBCALL ff_AddString (CharPtr string)

{
	Char newline;
	CharPtr buffer;
	Int2 increment_string=0, index, length_b/*, length_s -- UNUSED */, line_max;
	BuffStructPtr bfp;

	bfp = GetBuffStruct();
	newline = bfp->newline;
	line_max = bfp->line_max;

	if (string == NULL)
		return;

	buffer = CheckBufferState(&increment_string, string[0]);

	if (buffer == NULL)
		return;

	length_b = StringLen(bfp->buffer);

	string += increment_string;
	index = length_b;
	while ((*buffer = *string) != '\0')
	{	
		if (*string == newline)
		{
			*buffer = '\0';
			*string = ' ';
			NewContLine();
			string++;
			ff_AddString(string);
			break;
		}
		else
		{
			buffer++;
			string++;
			index++;
			if (index == line_max)
			{ /* return value for ff_AddString??????? */
				ff_AddString(string);
				break;
			}
		}
	}

/*	length_s = index - length_b; -- NO EFFECT */

	return;
}	/* ff_AddString */	

NLM_EXTERN void LIBCALL AddLink (CharPtr str)
{
	CharPtr 	buffer;
	Int2 		increment_string=0;
	Int4		l;
	CharPtr PNTR buf_links;
	Int4 PNTR	buf_pos;
	BuffStructPtr bfp;

	bfp = GetBuffStruct();
	
	buffer = CheckBufferState(&increment_string, str[0]);
	if (buffer == NULL) {
		return;
	}
	l = (bfp->byte_sp == NULL) ? 0 : BSLen(bfp->byte_sp);
	if (bfp->n_links >= bfp->buf_n_links) {
		buf_pos = MemNew((bfp->buf_n_links + LINKS)*sizeof(buf_pos[0]));
		buf_links = MemNew((bfp->buf_n_links + LINKS)*sizeof(buf_links[0]));
		if (bfp->buf_n_links > 0) {
			MemCpy(buf_pos, bfp->pos_links, bfp->buf_n_links*sizeof(buf_pos[0]));
			MemFree(bfp->pos_links);
			MemCpy(buf_links, bfp->links, bfp->buf_n_links*sizeof(buf_links[0]));
			MemFree(bfp->links);
		}
		bfp->pos_links = buf_pos;
		bfp->links = buf_links;
		bfp->buf_n_links += LINKS;
	}
	bfp->pos_links[bfp->n_links] = (buffer - bfp->buffer + l);
	bfp->links[bfp->n_links] = StringSave(str);
	bfp->n_links++;
	return;
}

NLM_EXTERN void LIBCALL AddLinkLater (CharPtr str, Int2 prevlen)
{
	CharPtr 	buffer;
	Int2 		increment_string=0;
	Int4		l;
	CharPtr PNTR buf_links;
	Int4 PNTR	buf_pos;
	BuffStructPtr bfp;

	bfp = GetBuffStruct();
	
	buffer = CheckBufferState(&increment_string, str[0]);
	if (buffer == NULL) {
		return;
	}
	l = (bfp->byte_sp == NULL) ? 0 : BSLen(bfp->byte_sp);
	if (bfp->n_links >= bfp->buf_n_links) {
		buf_pos = MemNew((bfp->buf_n_links + LINKS)*sizeof(buf_pos[0]));
		buf_links = MemNew((bfp->buf_n_links + LINKS)*sizeof(buf_links[0]));
		if (bfp->buf_n_links > 0) {
			MemCpy(buf_pos, bfp->pos_links, bfp->buf_n_links*sizeof(buf_pos[0]));
			MemFree(bfp->pos_links);
			MemCpy(buf_links, bfp->links, bfp->buf_n_links*sizeof(buf_links[0]));
			MemFree(bfp->links);
		}
		bfp->pos_links = buf_pos;
		bfp->links = buf_links;
		bfp->buf_n_links += LINKS;
	}
	bfp->pos_links[bfp->n_links] = buffer - bfp->buffer + l - prevlen;
	bfp->links[bfp->n_links] = StringSave(str);
	bfp->n_links++;
	return;
}

NLM_EXTERN void LIBCALL ff_AddChar (Char character)
{
	CharPtr buffer;
	Int2 increment_string=0;
	BuffStructPtr bfp;

	bfp = GetBuffStruct();

	if (character == NULLB)
		return;

	if (character == bfp->newline)
	{
		NewContLine();
	}
	else
	{
		buffer = CheckBufferState(&increment_string, character);
		*buffer = character;
	}

	return ;
}	/* ff_AddChar */

NLM_EXTERN void LIBCALL PrintXX(void)
{
	ff_StartPrint(0, 0, ASN2FF_EMBL_MAX, "XX");
	ff_EndPrint();
}

NLM_EXTERN void LIBCALL ff_AddInteger (CharPtr fmt, long integer)
{
	Char buffer[10];

	/* Only one integer read in	*/
	sprintf(buffer, fmt, integer);
	ff_AddString(buffer);
	return;
}	/* ff_AddInteger */
		
/***********************************************************************
*void ff_AddStringWithTildes (CharPtr string)
*
*	This function prints out a string and replaces the tiles ("~")
*	by new lines (by calling NewContLine).  Before this function is
*	called, printing must be initialized by calling ff_StartPrint;
*	afterwards ff_EndPrint must be called!
*	
*	Use accent grave (ASCII 96) "`~" to print out real tilde
*
* Can't this be rewritten to use ff_AddString????  That would be faster! 
************************************************************************/

NLM_EXTERN void LIBCALL ff_AddStringWithTildes (CharPtr string)
{
/* One "~" is a  new line, "~~" or "~~ ~~" means 2 returns */       

	while (*string != '\0') {
		if (*string == '`' && *(string+1) == '~') {
			ff_AddChar('~');
			string += 2;
		} else if (*string == '~') {
			NewContLine();
			string++;
			if (*string == '~') {
				NewContLine();
				string++;
				if (*string == ' ' && *(string+1) == '~' && *(string+2) == '~')
					string += 3;
             }
		} else if (*string == '\"') {   
			*string = '\'';
			ff_AddChar(*string);
			string++;
		} else {  
			ff_AddChar(*string);
			string++;
		}
	}

}	/* ff_AddStringWithTildes */
	
NLM_EXTERN void LIBCALL ChangeStringWithTildes (CharPtr string)
{
                while (*string != '\0') {
                /* One "~" is a line return, "~~" or "~~ ~~" means 2 returns */       
					if (*string == '~') {
						*string = '\n';
						string++;
						if (*string == '~') {
							*string = '\n';
							string++;
							if (*string == ' ' && *(string+1) == '~' &&
                                 		*(string+2) == '~') {
								string += 3;
							}
                        }
					} else if (*string == '\"') {
						*string = '\'';
						string++;
                    } else {  
						string++;
                    }
                }

}	/* ChangeStringWithTildes */
	
NLM_EXTERN void LIBCALL ff_RecalculateLinks(Int4 indent) {
    BuffStructPtr bfp = GetBuffStruct();
    Int4 len = (bfp->byte_sp == NULL) ? 0 : BSLen(bfp->byte_sp);
    Int2 i;

    for ( i = bfp->n_links - 1; i >= 0; --i ) {
        if ( bfp->pos_links[i] + 1 >= len ) { 
            bfp->pos_links[i] += indent;
        } else {
            break;
        }
    }
}

NLM_EXTERN CharPtr LIBCALL CheckBufferState(Int2Ptr increment_string, Char next_char)

{
	CharPtr buffer, line_prefix;
	CharPtr buf_ptr_comma=NULL, buf_ptr_dash=NULL, buf_ptr_space=NULL, 
	buf_ptr_start, ptr_index;
	CharPtr temp_ptr, temp_ptr_start;
	Char temp[MAX_TO_RT_SIDE+1];
	Int2 length, cont_indent, indent_space; 
	Int2 line_max, line_index;
	BuffStructPtr bfp;
    Int2 IndentSize = 0;

	bfp = GetBuffStruct();
	cont_indent = bfp->cont_indent;
	line_max = bfp->line_max;

	*increment_string = 0;
	temp_ptr = temp_ptr_start = temp;

	buffer = bfp->buffer;
	length=StringLen(buffer);
	
	if (length < bfp->line_max) {
		return buffer+length;
	} else if (length == bfp->line_max && next_char == '\0') {
		return buffer+length;
	} else {
		if (StringLen(bfp->line_prefix) > 0) {
			line_prefix = bfp->line_prefix;
			indent_space = cont_indent - StringLen(line_prefix);
		} else {
			line_prefix = NULL;
			indent_space = cont_indent;
		}

		if (next_char == ' ') {
			buf_ptr_space = buffer+line_max+1;
		} else {
			for (ptr_index=buffer+line_max, line_index=line_max-cont_indent; 
				ptr_index>buffer+line_max-MAX_TO_RT_SIDE &&
					line_index > 0;
						ptr_index--, line_index--) {
				if (ptr_index[0] == ' ') {
					buf_ptr_space = ptr_index;
					break;
				}
			}
		
			for (ptr_index=buffer+line_max, line_index=line_max-cont_indent; 
				ptr_index>buffer+line_max-MAX_TO_RT_SIDE &&
					line_index > 0;
						ptr_index--, line_index--) {
				if (ptr_index[0] == ',') {
					buf_ptr_comma = ptr_index;
/* Add one on the next line, otherwise the comma is lost, as space is. */
					buf_ptr_comma++;
					break;
				}
			}
			for (ptr_index=buffer+line_max, line_index=line_max-cont_indent; 
				ptr_index>buffer+line_max-MAX_TO_RT_SIDE &&
					line_index > 0;
						ptr_index--, line_index--) {
				if (ptr_index[0] == '-') {
				 /* Don't put "-" on next line! */
					buf_ptr_dash = ptr_index + 1;
					break;
				}
			}
		}

		if (next_char != ' ' &&
			(buf_ptr_space || buf_ptr_comma || buf_ptr_dash)) {
			if (buf_ptr_space) {
				buf_ptr_start = buf_ptr_space;
				buf_ptr_space++;
				while((*temp_ptr = *buf_ptr_space) != '\0') {
					temp_ptr++;
					buf_ptr_space++;
				}
				buf_ptr_start[0] = '\0';
				if (*(buf_ptr_start-1) == ' ') {
				/* If there are two spaces in a row */
					buf_ptr_start--;
					buf_ptr_start[0] = '\0';
				}
			} else if (buf_ptr_comma) {
				buf_ptr_start = buf_ptr_comma;
/* Check if a space follows the comma; if so, delete it; otherwise it shows
up on the next line and really looks stupid.			*/
				if (*buf_ptr_comma == ' ') {
					buf_ptr_comma++;
				}
				while((*temp_ptr = *buf_ptr_comma) != '\0') {
					temp_ptr++;
					buf_ptr_comma++;
				}
				buf_ptr_start[0] = '\0';
			} else if (buf_ptr_dash) {
				buf_ptr_start = buf_ptr_dash;
				while((*temp_ptr = *buf_ptr_dash) != '\0') {
					temp_ptr++;
					buf_ptr_dash++;
				}
				buf_ptr_start[0] = '\0';
			}
            
            
			FlushBuffer();
			if (line_prefix != NULL) {
				*buffer = *line_prefix;
				buffer++;
				line_prefix++;
				*buffer = *line_prefix;
				buffer++;
				line_prefix++;
			}
			if (indent_space > 0)
				MemSet((VoidPtr) buffer, ' ', indent_space);
			buffer += indent_space;
            IndentSize = buffer - bfp->buffer + 1;
			temp_ptr = temp_ptr_start;
			while((*buffer = *temp_ptr) != '\0') {
				temp_ptr++;
				buffer++;
			}
		    
            ff_RecalculateLinks(IndentSize);
			return buffer;
		} else if (next_char == ' ') {
			FlushBuffer();
			if (line_prefix) {
				*buffer = *line_prefix;
				buffer++;
				line_prefix++;
				*buffer = *line_prefix;
				buffer++;
				line_prefix++;
			}
			if (indent_space-1 > 0) {
				MemSet((VoidPtr) buffer, ' ', indent_space-1);
                ff_RecalculateLinks(buffer+indent_space-1 - bfp->buffer);
				return buffer+indent_space-1;
			} else if (indent_space-1 == 0) {
			/* if there is one space indentation! */
                ff_RecalculateLinks(buffer - bfp->buffer);
				return buffer;
			} else if (indent_space-1 < 0) {
			/* if there is zero space indentation! */
				*increment_string = 1;
                ff_RecalculateLinks(buffer - bfp->buffer);
				return buffer;
			}
		} else { 
			FlushBuffer();
			if (line_prefix) {
				*buffer = *line_prefix;
				buffer++;
				line_prefix++;
				*buffer = *line_prefix;
				buffer++;
				line_prefix++;
			}
			if (indent_space > 0) {
				MemSet((VoidPtr) buffer, ' ', indent_space);
			}
            ff_RecalculateLinks(buffer +indent_space - bfp->buffer);
			return buffer+indent_space;
		}
	}
	return buffer;	/* never used!  Only put in to make CodeWarrior happy*/
}	/* CheckBufferState */

NLM_EXTERN Int2 LIBCALL NewContLine (void)

{
	Int2 cont_indent, indent_space;
	CharPtr buffer, line_prefix=NULL;
	BuffStructPtr bfp;

	bfp = GetBuffStruct();
	cont_indent = bfp->cont_indent;

	FlushBuffer();
	buffer=bfp->buffer;

	if (StringLen(bfp->line_prefix) > 0)
	{
		line_prefix = bfp->line_prefix;
		indent_space = cont_indent - StringLen(line_prefix);
	}
	else
	{
		indent_space = cont_indent;
	}

	if (line_prefix)
	{
		*buffer = *line_prefix;
		buffer++;
		line_prefix++;
		*buffer = *line_prefix;
		buffer++;
		line_prefix++;
	}

	if (indent_space > 0)
		MemSet((VoidPtr) buffer, ' ', indent_space);
	return cont_indent;
}	/* NewContLine */


NLM_EXTERN Int2 LIBCALL TabToColumn (Int2 column)

{
	CharPtr start_buffer, buffer;
	Int2 increment_string=0, length;
	BuffStructPtr bfp;

	bfp = GetBuffStruct();
	start_buffer = bfp->buffer;

	if ((length=StringLen(start_buffer)) > column)
		return -1;

	buffer = CheckBufferState(&increment_string, '\0');

	if ((column-length-1) > 0)
		MemSet((VoidPtr) buffer, ' ', (column-length-1));
		
	return length;
}

NLM_EXTERN void LIBCALL ff_EndPrint (void)

{
	FlushBuffer();
}

NLM_EXTERN void LIBCALL FlushBuffer (void)

{
	ByteStorePtr byte_sp=NULL;
	CharPtr line_ret;
	BuffStructPtr bfp;

	bfp = GetBuffStruct();
	line_ret = bfp->line_return;

	if (bfp->fp)
	{
		fflush(bfp->fp);
		fprintf(bfp->fp, "%s\n", bfp->buffer);
	}
	else
	{
		byte_sp=bfp->byte_sp;
		if (! byte_sp) {
			bfp->byte_sp = byte_sp = BSNew(MAX_BTP_BUF);
		}
		BSWrite(byte_sp, bfp->buffer, StringLen(bfp->buffer));
		BSWrite(byte_sp, line_ret, StringLen(line_ret));
	}
	MemSet((VoidPtr) bfp->buffer, '\0', bfp->line_max);
}


/**************************************************************************
*CheckEndPunctuation
*
*	This code checks to ensure that the last character of a string
*	is the character passed.  Any extra spaces or tabs at the end of
*	the string are eliminated.
*
**************************************************************************/

NLM_EXTERN CharPtr LIBCALL CheckEndPunctuation (CharPtr string, Char end)

{
	CharPtr stringptr, newstring;
	Int4 length;

	if (string == NULL)
		return NULL;

	length = StringLen(string);
	newstring = (CharPtr) MemNew((length+2)*sizeof(Char));

	if (length > 0)
	{
		newstring = StringCpy(newstring, string);
		for (stringptr=newstring+length-1; stringptr > newstring; stringptr--)
		{
			if (*stringptr == ' ' || *stringptr == '\t' || *stringptr == '~')
			{
				*stringptr = '\0';
			}
			else
			{
				break;
			}
		}
	
		if (*stringptr != end)
		{
			stringptr++;
			*stringptr = end;
			stringptr++;
			*stringptr = '\0';
		}	
	} 
	else
	{
		newstring[0] = end;
		newstring[1] = '\0';
	}

	return newstring;
}

NLM_EXTERN void LIBCALL BuffFree(void)
{
	BuffStructPtr bfp;

	bfp = GetBuffStruct();
	bfp->buffer[0] = '\0';
	bfp->n_links = 0;

}

NLM_EXTERN CharPtr LIBCALL ReportPrint (FFPrintArrayPtr pap, Int4 index, Int4 pap_size)

{
	GBEntryPtr gbp;
	Asn2ffJobPtr ajp;
	ByteStorePtr byte_sp=NULL;
	CharPtr string=NULL;
	FFPrintArray pa=pap[index];
	BuffStructPtr bfp;

	bfp = GetBuffStruct();

	gbp = pa.gbp;
	ajp = pa.ajp;
	ajp->pap_index = pa.index;
	ajp->pap_last = pa.last;

	if (index == 0 || index == (pap_size-1))
	{
		flat2asn_delete_locus_user_string();
		flat2asn_install_locus_user_string(gbp->locus);
		flat2asn_delete_accession_user_string();
		flat2asn_install_accession_user_string(gbp->accession);
	}

	pa.fct(ajp, gbp);

	if (! bfp->fp)
	{
		byte_sp = bfp->byte_sp;

		string = BSMerge(byte_sp, NULL);
		bfp->byte_sp = BSFree(bfp->byte_sp);
	}

	return string;
}

/*****************************************************************************
*Boolean DoSpecialLineBreak (BiotablePtr btp, CharPtr string, Int2 indent)
*
*	Look for line breaks in special cases, such as KEYWORDS, when
*	line breaks are only allowed after semi-colons.
*
*	"length" is reset to zero if string is NULL, "indent" tells how
*	many spaces the line is indented from the left side.
*****************************************************************************/
 
NLM_EXTERN Boolean LIBCALL DoSpecialLineBreak (CharPtr string, Int2 indent)

{
	Boolean retval;
	static Int2 length=0;

	if (string == NULL)
	{
		length = 0;
		retval = FALSE;
	}
	else
	{
		length += StringLen(string);
		length += 2;
		if (length < (ASN2FF_GB_MAX-indent))
		{
			retval = FALSE;
		}
		else
		{
			length = StringLen(string);
			length += 2;
			retval = TRUE;
		}
	}
	return retval;

}	/* DoSpecialLineBreak */
