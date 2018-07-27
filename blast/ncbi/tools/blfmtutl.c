static char const rcsid[] = "$Id: blfmtutl.c,v 1.4 2004/10/19 15:28:59 coulouri Exp $";

/* ===========================================================================
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
* ===========================================================================*/

/*****************************************************************************

File name: blfmtutl.c

Author: Tom Madden

Contents: Utilities for BLAST formatting

******************************************************************************/
/*
* $Revision: 
* $Log: blfmtutl.c,v $
* Revision 1.4  2004/10/19 15:28:59  coulouri
* bump version and date
*
* Revision 1.3  2004/10/04 17:54:14  madden
* BlastPrintVersionInfo[Ex] now takes const char* as arg for program
*
* Revision 1.2  2004/07/22 15:18:45  jianye
* correct blast paper url
*
* Revision 1.1  2004/06/30 12:31:15  madden
* Structures and prototypes for blast formatting utilities
*
*/

#include <ncbi.h>
#include <objcode.h>
#include <objseq.h>
#include <sequtil.h>
#include <readdb.h>
#include <ncbithr.h>
#include <txalign.h>
#include <blfmtutl.h>


/* the version of BLAST. */
#define BLAST_ENGINE_VERSION "2.2.10"
#define BLAST_RELEASE_DATE "Oct-19-2004"

#define BUFFER_LENGTH 255

/*
	adds the new string to the buffer, separating by a tilde.
	Checks the size of the buffer for FormatBlastParameters and
	allocates longer replacement if needed.
*/

Boolean LIBCALL
add_string_to_bufferEx(CharPtr buffer, CharPtr *old, Int2Ptr old_length, Boolean add_tilde)

{
	CharPtr new, ptr;
	Int2 length, new_length;

	length = (StringLen(*old));

	if((StringLen(buffer)+length+3) > *old_length)
	{
		new_length = *old_length + 255;
		new = MemNew(new_length*sizeof(Char));
		if (*old_length > 0 && *old != NULL)
		{
			MemCpy(new, *old, *old_length);
			*old = MemFree(*old);
		}
		*old = new;
		*old_length = new_length;
	}

	ptr = *old;
	ptr += length;
	if (add_tilde)
	{
		*ptr = '~';
		ptr++;
	}

	while (*buffer != NULLB)
	{
		*ptr = *buffer;
		buffer++; ptr++;
	}

	return TRUE;
}

Boolean LIBCALL
add_string_to_buffer(CharPtr buffer, CharPtr *old, Int2Ptr old_length)

{
	return add_string_to_bufferEx(buffer, old, old_length, TRUE);
}

/*
	Print the buffer, adding newlines where tildes are found.
*/

Boolean LIBCALL
PrintTildeSepLines(CharPtr buffer, Int4 line_length, FILE *outfp)

{
	if (outfp == NULL || buffer == NULL)
		return FALSE;

	asn2ff_set_output(outfp, NULL);

	ff_StartPrint(0, 0, line_length, NULL);
	while (*buffer != NULLB)
	{
		if (*buffer != '~')
			ff_AddChar(*buffer);
		else
			NewContLine();
		buffer++;
	}
	ff_EndPrint();

	return TRUE;
}

/*
	Print the Karlin-Altschul parameters.

	if gapped is TRUE, then slightly different formatting is used.
*/

Boolean LIBCALL
PrintKAParameters(Nlm_FloatHi Lambda, Nlm_FloatHi K, Nlm_FloatHi H, Int4 line_length, FILE *outfp, Boolean gapped)

{
	return PrintKAParametersExtra(Lambda, K, H, 0.0, line_length, outfp, gapped);
}

Boolean LIBCALL
PrintKAParametersExtra(Nlm_FloatHi Lambda, Nlm_FloatHi K, Nlm_FloatHi H, Nlm_FloatHi C, Int4 line_length, FILE *outfp, Boolean gapped)

{
	Char buffer[BUFFER_LENGTH];

	if (outfp == NULL)
		return FALSE;

	asn2ff_set_output(outfp, NULL);

	ff_StartPrint(0, 0, line_length, NULL);
	if (gapped)
	{
		ff_AddString("Gapped");
		NewContLine();
	}
	
	if (C == 0.0)
		ff_AddString("Lambda     K      H");
	else
		ff_AddString("Lambda     K      H      C");
	NewContLine();
	sprintf(buffer, "%#8.3g ", Lambda);
	ff_AddString(buffer);
	sprintf(buffer, "%#8.3g ", K);
	ff_AddString(buffer);
	sprintf(buffer, "%#8.3g ", H);
	ff_AddString(buffer);
	if (C != 0.0)
	{
		sprintf(buffer, "%#8.3g ", C);
		ff_AddString(buffer);
	}
	NewContLine();
	ff_EndPrint();

	return TRUE;

}


TxDfDbInfoPtr LIBCALL 
TxDfDbInfoNew (TxDfDbInfoPtr old)

{
	TxDfDbInfoPtr dbinfo;
	dbinfo = MemNew(sizeof(TxDfDbInfo));
	if (old)
		old->next = dbinfo;
	return dbinfo;
}

TxDfDbInfoPtr LIBCALL 
TxDfDbInfoDestruct (TxDfDbInfoPtr dbinfo)

{
	TxDfDbInfoPtr next;

	if (dbinfo == NULL)
		return NULL;

	while (dbinfo)
	{
		dbinfo->name = MemFree(dbinfo->name);
		dbinfo->definition = MemFree(dbinfo->definition);
		dbinfo->date = MemFree(dbinfo->date);
		next = dbinfo->next;
		dbinfo = MemFree(dbinfo);
		dbinfo = next;
	}

	return dbinfo;
}

Boolean LIBCALL
PrintDbReport(TxDfDbInfoPtr dbinfo, Int4 line_length, FILE *outfp)

{

	if (dbinfo == NULL || outfp == NULL)
		return FALSE;

	asn2ff_set_output(outfp, NULL);

	ff_StartPrint(2, 2, line_length, NULL);

	if (dbinfo->subset == FALSE)
	{
		ff_AddString("Database: ");
		ff_AddString(dbinfo->definition);
		NewContLine();
		ff_AddString("  Posted date:  ");
		ff_AddString(dbinfo->date);
		NewContLine();
		ff_AddString("Number of letters in database: "); 
		ff_AddString(Nlm_Int8tostr((Int8) dbinfo->total_length, 1));
		NewContLine();
		ff_AddString("Number of sequences in database:  ");
		ff_AddString(Ltostr((long) dbinfo->number_seqs, 1));
		NewContLine();
	}
	else
	{
		ff_AddString("Subset of the database(s) listed below");
		NewContLine();
		ff_AddString("   Number of letters searched: "); 
		ff_AddString(Nlm_Int8tostr((Int8) dbinfo->total_length, 1));
		NewContLine();
		ff_AddString("   Number of sequences searched:  ");
		ff_AddString(Ltostr((long) dbinfo->number_seqs, 1));
		NewContLine();
	}
	ff_EndPrint();

	return TRUE;
}

/*
	Prints an acknowledgement of the Blast Query, in the standard
	BLAST format.
*/


Boolean LIBCALL
AcknowledgeBlastQuery(BioseqPtr bsp, Int4 line_length, FILE *outfp, Boolean believe_query, Boolean html)

{
	Char buffer[BUFFER_LENGTH];

	if (bsp == NULL || outfp == NULL)
		return FALSE;
	
	asn2ff_set_output(outfp, NULL);

	ff_StartPrint(0, 0, line_length, NULL);
	if (html)
		ff_AddString("<b>Query=</b> ");
	else
		ff_AddString("Query= ");
	if (bsp->id && (bsp->id->choice != SEQID_LOCAL || believe_query))
	{
		SeqIdWrite(bsp->id, buffer, PRINTID_FASTA_LONG, BUFFER_LENGTH);
		if (StringNCmp(buffer, "lcl|", 4) == 0)
			ff_AddString(buffer+4);
		else
			ff_AddString(buffer);
		ff_AddChar(' ');
	}
	ff_AddString(BioseqGetTitle(bsp));
	NewContLine();
	TabToColumn(10);
	ff_AddChar('(');
	ff_AddString(Ltostr((long) BioseqGetLen(bsp), 1));
	ff_AddString(" letters)");
	NewContLine();
        ff_EndPrint();

        return TRUE;
}

/*
	return the version of BLAST as a char. string.
*/
CharPtr LIBCALL
BlastGetReleaseDate (void)

{
	return BLAST_RELEASE_DATE;
}


/*
	return the version of BLAST as a char. string.
*/
CharPtr LIBCALL
BlastGetVersionNumber (void)

{
	return BLAST_ENGINE_VERSION;
}

Boolean BlastPrintVersionInfo (const char* program, Boolean html, FILE *outfp)

{
	return BlastPrintVersionInfoEx(program, html, BlastGetVersionNumber(), BlastGetReleaseDate(), outfp);
}

Boolean BlastPrintVersionInfoEx (const char* program, Boolean html, CharPtr version, CharPtr date, FILE *outfp)

{
	CharPtr ret_buffer;


	if (outfp == NULL)
		return FALSE;

	ret_buffer = StringSave(program);
	Nlm_StrUpper(ret_buffer);
	if (html)
		fprintf(outfp, "<b>%s %s [%s]</b>\n", ret_buffer, version, date);
	else
		fprintf(outfp, "%s %s [%s]\n", ret_buffer, version, date);
	ret_buffer = MemFree(ret_buffer);

	return TRUE;
}

/* 
	Returns a reference for the header.
	The newlines are represented by tildes, use PrintTildeSepLines
	to print this.
*/

CharPtr LIBCALL
BlastGetReference(Boolean html)

{
	CharPtr ret_buffer;
	Int2 ret_buffer_length;

	ret_buffer = NULL;
	ret_buffer_length = 0;

	
	if (html) {
		add_string_to_bufferEx("<b><a href=\"http://www.ncbi.nlm.nih.gov/entrez/query.fcgi?db=PubMed&cmd=Retrieve&list_uids=9254694&dopt=Citation\">Reference</a>:</b>", &ret_buffer, &ret_buffer_length, TRUE);
		add_string_to_bufferEx("Altschul, Stephen F., Thomas L. Madden, Alejandro A. Sch&auml;ffer, ", &ret_buffer, &ret_buffer_length, TRUE);
	} else
		add_string_to_bufferEx("Reference: Altschul, Stephen F., Thomas L. Madden, Alejandro A. Schaffer, ", &ret_buffer, &ret_buffer_length, TRUE);
	add_string_to_bufferEx("Jinghui Zhang, Zheng Zhang, Webb Miller, and David J. Lipman (1997), ", &ret_buffer, &ret_buffer_length, TRUE);
	add_string_to_bufferEx("\"Gapped BLAST and PSI-BLAST: a new generation of protein database search", &ret_buffer, &ret_buffer_length, TRUE);
	add_string_to_bufferEx("programs\",  Nucleic Acids Res. 25:3389-3402.", &ret_buffer, &ret_buffer_length, TRUE);
	
	return ret_buffer;
}

Boolean LIBCALL
MegaBlastPrintReference(Boolean html, Int4 line_length, FILE *outfp)

{
	CharPtr ret_buffer;
	Int2 ret_buffer_length;

	ret_buffer = NULL;
	ret_buffer_length = 0;

	if (outfp == NULL)
		return FALSE;
	
	if (html) {
           add_string_to_bufferEx("<b><a href=\"http://www.ncbi.nlm.nih.gov/entrez/query.fcgi?db=PubMed&cmd=Retrieve&list_uids=10890397&dopt=Citation\">Reference</a>:</b>", &ret_buffer, &ret_buffer_length, TRUE);
           add_string_to_bufferEx("Zheng Zhang, Scott Schwartz, Lukas Wagner, and Webb Miller (2000),", &ret_buffer, &ret_buffer_length, TRUE);
	} else
           add_string_to_bufferEx("Reference: Zheng Zhang, Scott Schwartz, Lukas Wagner, and Webb Miller (2000), ", &ret_buffer, &ret_buffer_length, TRUE);
	add_string_to_bufferEx("\"A greedy algorithm for aligning DNA sequences\", ", 
                               &ret_buffer, &ret_buffer_length, TRUE);
	add_string_to_bufferEx("J Comput Biol 2000; 7(1-2):203-14.", 
                               &ret_buffer, &ret_buffer_length, TRUE);
	
        PrintTildeSepLines(ret_buffer, line_length, outfp);
        ret_buffer = MemFree(ret_buffer);
	return TRUE;
}

Boolean LIBCALL
BlastPrintReference(Boolean html, Int4 line_length, FILE *outfp)

{
	CharPtr ret_buffer;
	
	if (outfp == NULL)
		return FALSE;
	
        ret_buffer = BlastGetReference(html);
        PrintTildeSepLines(ret_buffer, line_length, outfp);
        ret_buffer = MemFree(ret_buffer);

	return TRUE;
}

/* 
	Returns a reference for the header.
	The newlines are represented by tildes, use PrintTildeSepLines
	to print this.
*/

CharPtr LIBCALL
BlastGetPhiReference(Boolean html)

{
	CharPtr ret_buffer;
	Int2 ret_buffer_length;

	ret_buffer = NULL;
	ret_buffer_length = 0;

	
	if (html) {
		add_string_to_bufferEx("<b><a http://www.ncbi.nlm.nih.gov/entrez/query.fcgi?db=PubMed&cmd=Retrieve&list_uids=9705509&dopt=Citation\">Reference</a>:</b>", &ret_buffer, &ret_buffer_length, TRUE);
		add_string_to_bufferEx("Zhang, Zheng, Alejandro A. Sch&auml;ffer, Webb Miller, Thomas L. Madden, ", &ret_buffer, &ret_buffer_length, TRUE);
	} else
		add_string_to_bufferEx("Reference: Zhang, Zheng, Alejandro A. Schaffer, Webb Miller, Thomas L. Madden, ", &ret_buffer, &ret_buffer_length, TRUE);
	add_string_to_bufferEx("David J. Lipman, Eugene V. Koonin, and Stephen F. Altschul (1998), ", &ret_buffer, &ret_buffer_length, TRUE);
	add_string_to_bufferEx("\"Protein sequence similarity searches using patterns as seeds\", ", &ret_buffer, &ret_buffer_length, TRUE);
	add_string_to_bufferEx("Nucleic Acids Res. 26:3986-3990.", &ret_buffer, &ret_buffer_length, TRUE);
	
	return ret_buffer;
}

Boolean LIBCALL
BlastPrintPhiReference(Boolean html, Int4 line_length, FILE *outfp)

{
	CharPtr ret_buffer;
	
	if (outfp == NULL)
		return FALSE;
	
        ret_buffer = BlastGetPhiReference(html);
        PrintTildeSepLines(ret_buffer, line_length, outfp);
        ret_buffer = MemFree(ret_buffer);

	return TRUE;
}

/*
        Counts the number of SeqAligns present.
*/

static Int4
GetSeqAlignCount(SeqAlignPtr sap)

{
        Int4 count = 0;
        SeqIdPtr last_id=NULL, id;

        while (sap)
        {
                id = TxGetSubjectIdFromSeqAlign(sap);
                if (last_id)
                {
                        if(SeqIdComp(id, last_id) != SIC_YES)
                                count++;
                }
                else
                {
                        count = 1;
                }
                last_id = id;
                sap = sap->next;
        }

        return count;

}

/*
        Duplicates a SeqAlignPtr, up to the number of unique
        records specified.
*/

static SeqAlignPtr
GetPrivateSeqAlign(SeqAlignPtr sap, Int4 number, Int4Ptr number_returned)

{
        Int4 count=0;
        SeqIdPtr last_id=NULL, id;
        SeqAlignPtr new_head=NULL, var;

        last_id = TxGetSubjectIdFromSeqAlign(sap);

        while (count<number && sap)
        {
                count++;
                while (sap)
                {
                        id = TxGetSubjectIdFromSeqAlign(sap);
                        if(SeqIdComp(id, last_id) != SIC_YES)
                        {
                                last_id = id;
                                break;
                        }
                        if (new_head == NULL)
                        {
                                new_head = AsnIoMemCopy(sap, (AsnReadFunc) SeqAlignAsnRead, (AsnWriteFunc) SeqAlignAsnWrite);
                                var = new_head;
                        }
                        else
                        {
                                var->next = AsnIoMemCopy(sap, (AsnReadFunc) SeqAlignAsnRead, (AsnWriteFunc) SeqAlignAsnWrite);
                                var = var->next;
                        }
                        last_id = id;
                        sap = sap->next;
                }
        }

        *number_returned = count;

        return new_head;
}

/*
        Duplicate a SeqAlignPtr, keeping on the number of unique db
        hits specified.
*/

BlastPruneSapStructPtr LIBCALL
BlastPruneHitsFromSeqAlign(SeqAlignPtr sap, Int4 number, BlastPruneSapStructPtr prune)

{
        if (prune == NULL)
        {
                prune = MemNew(sizeof(BlastPruneSapStruct));
        }
        else
        {
                if (prune->number == number)
                        return prune;
                if (prune->allocated)
                        prune->sap = SeqAlignSetFree(prune->sap);
                prune->sap = NULL;
                prune->allocated = FALSE;
                prune->original_number = 0;
                prune->number = 0;
        }

        prune->original_number = GetSeqAlignCount(sap);

        if (prune->original_number < number)
        {
                prune->number = prune->original_number;
                prune->sap = sap;
                prune->allocated = FALSE;
        }
        else
        {
                prune->sap = GetPrivateSeqAlign(sap, number, &(prune->number));
                prune->allocated = TRUE;
        }

        return prune;
}

BlastPruneSapStructPtr LIBCALL
BlastPruneSapStructDestruct(BlastPruneSapStructPtr prune)

{
        if (prune == NULL)
                return NULL;

        if (prune->allocated)
        {
                prune->sap = SeqAlignSetFree(prune->sap);
        }
        prune = MemFree(prune);

        return prune;
}

