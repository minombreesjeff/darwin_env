/*  tofasta.h
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
* File Name:  tofasta.h
*
* Author:  James Ostell
*   
* Version Creation Date: 7/12/91
*
* $Revision: 6.27 $
*
* File Description:  various sequence objects to fasta output
*
* Modifications:  
* --------------------------------------------------------------------------
* Date	   Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
* $Log: tofasta.h,v $
* Revision 6.27  2005/03/21 22:15:09  kans
* added SeqLocFastaStream
*
* Revision 6.26  2004/10/14 19:36:34  kans
* CreateDefLineExEx has extProtTitle argument, normally only use first protein name in defline
*
* Revision 6.25  2004/07/23 20:55:52  kans
* added BioseqFastaMemStream that takes byte store instead of file pointer
*
* Revision 6.24  2004/07/16 19:37:37  kans
* SeqPortStream and FastaStream functions return Int4, negative count if any fetch failures
*
* Revision 6.23  2004/04/21 14:42:36  kans
* added blocklen and grouplen parameters to BioseqFastaStream and SeqEntryFastaStream, former also has do_defline parameter
*
* Revision 6.22  2004/03/15 19:56:09  kans
* BioseqFastaStream takes StreamFlgType flags parameter, added SeqEntryFastaStream for exploration
*
* Revision 6.21  2003/11/18 19:04:29  kans
* added BioseqFastaStream, rapid generator based on SeqPortStream
*
* Revision 6.20  2002/07/18 19:28:35  kans
*  working implementation of BioseqRawToFastaExtraEx, always zero out all of MyFsa struct, no need for fake seqloc
*
* Revision 6.18  2002/07/14 21:04:32  camacho
* Added option to print a subsequence of a raw bioseq to fasta format
*
* Revision 6.17  2002/01/16 17:02:04  camacho
* Changed buflen and seqlen in MyFsa struct. to use Uint4, as well as some function prototypes
*
* Revision 6.16  2001/08/31 15:29:54  kans
* added CreateDefLineEx to ignore existing title descriptor, force generation of protein title computationally
*
* Revision 6.15  2001/08/17 13:27:26  kans
* ClearProteinTitlesInNucProts, do not do it for outside protein databases
*
* Revision 6.14  2001/06/25 23:47:48  kans
* added InstantiateProteinTitles and ClearProteinTitles
*
* Revision 6.13  2001/02/27 21:52:37  madden
* Added BioseqToFastaDump and FastaDumpFileFunc for dumping BLAST db in FASTA
*
* Revision 6.12  2000/10/12 16:03:58  kans
* added SeqEntryToFastaEx, printid_general field to MyFsa structure, to support processing software that requires gnl ID in FASTA defline even in the presence of higher-priority ref ID
*
* Revision 6.11  2000/07/20 17:48:28  dondosha
* Added function FastaToSeqBuffForDb analogous to FastaToSeqEntryForDbtofasta.c
*
* Revision 6.10  2000/05/30 19:44:45  ostell
* added FastaSeqLineEx() with another parameter, do_virtual
*
* Revision 6.9  2000/04/03 22:09:26  kans
* added ClearGenBankKeywords for RefSeq processing
*
* Revision 6.8  2000/03/29 22:00:23  kans
* added NC_Cleanup function used internally for genome RefSeq processing
*
* Revision 6.7  2000/02/17 17:20:58  sicotte
* Add Reading of Lowercase Characters as SeqLoc for inputting masking informatioin. Use FastaToSeqEntryForDb or FastaToSeqEntryInternalEx
*
* Revision 6.6  2000/02/04 16:38:29  kans
* added FastaToSeqEntryForDb and FastaToSeqEntryInternalEx, giving control over generation of unique SeqID
*
* Revision 6.5  1999/09/20 18:37:55  shavirin
* Added definition of the function Int4 GetOrderBySeqId().
*
* Revision 6.4  1998/02/23 16:51:27  egorov
* Changes to make the tofasta.c independent on readdb.h
*
* Revision 6.2  1998/01/27 20:28:10  madden
* Added BioseqRawToFastaExtra with line_length arg
*
* Revision 6.1  1997/10/22 16:44:07  shavirin
* Added definitions for functions: FastaReadSequence() and FastaReadSequenceMem()
*
* Revision 6.0  1997/08/25 18:07:48  madden
* Revision changed to 6.0
*
* Revision 5.6  1997/06/19 18:39:21  vakatov
* [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
*
* Revision 5.5  1996/10/22 16:00:50  shavirin
* Added new Boolean no_sequence in MyFsa structure to disable
* sequence printing
*
 * Revision 5.4  1996/10/21  21:37:24  shavirin
 * Added definition for function SeqEntrysToDefline()
 *
 * Revision 5.3  1996/10/08  22:27:05  shavirin
 * Moved definition of functions FastaToSeqEntryEx and FastaToSeqBuffEx
 * into include file
 *
 * Revision 5.2  1996/08/15  18:15:23  tatiana
 * CreateDefLine() added
 *
 * Revision 5.1  1996/06/15  17:29:44  ostell
 * changed MyFsa structure by adding do_virtual and tech fields
 * added value of 3 for group_segs
 * addes support of tech to FastaDefLine()
 *
 * Revision 5.0  1996/05/28  13:23:23  ostell
 * Set to revision 5.0
 *
 * Revision 4.1  1996/03/13  19:50:23  shavirin
 * Added definition for new external function FastaToSeqBuff()
 *
 * Revision 4.0  1995/07/26  13:49:01  ostell
 * force revision to 4.0
 *
 * Revision 2.7  1995/05/09  18:43:09  ostell
 * added support for (accession) on GenPept deflines and [organism] on
 * GenPept and PRF deflines
 *
*
* ==========================================================================
*/

#ifndef _NCBI_Tofasta_
#define _NCBI_Tofasta_

#include <seqport.h>

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

	                            /* keys returned by FastaWriteFunc */

#define FASTA_ID ((Int2)1)       /* the SeqId string */
#define FASTA_DEFLINE ((Int2)2)	 /* the definition line */
#define FASTA_SEQLINE ((Int2)3)	 /* a line of sequence */
#define FASTA_EOS ((Int2)4)		 /* all the sequence has been printed */
#define FASTA_FORMATDB_AMB ((Int2)5) /* make conversion to standard form for nucleotides */

typedef Boolean (* FastaWriteFunc) PROTO ((BioseqPtr bsp, Int2 key,
                                           CharPtr buf, Uint4 buflen, Pointer data));

typedef struct iteminfo {   /* struct used for defline */
	Uint2	entityID,	   
		    itemID,			   
			itemtype;
} ItemInfo, PNTR ItemInfoPtr;

typedef struct descrinfo {   /* struct used for defline */
	BioseqPtr bsp;
	ValNodePtr vnp;
	ItemInfoPtr iip;
	Uint1 choice;
} DescrInfo, PNTR DescrInfoPtr;

typedef struct myfsa {   /* struct used for fasta searches */
    CharPtr buf;         /* buffer for strings (suggest 255 minimum) */
    Uint4 buflen,         /* length of buf */
          seqlen;          /* length of sequence lines (must be buflen-1 or less) */
    Pointer mydata;      /* pointer to your own data */
    FastaWriteFunc myfunc;  /* callback to process parts of fasta file */
    BioseqPtr bsp;       /* Bioseq being processed */
    Boolean bad_asn1; /* set if error in input object like mol not set */
    CharPtr accession;   /* used internally for GenPept def lines */
    CharPtr organism;    /* used internally for GenPept/PRF def lines */
    Uint1 order;          /* used to order def lines for BLAST */
    Boolean do_virtual;   /* if TRUE, instantiate virtual sequences */
    Uint1 tech;           /* for MolInfo.tech */
    Boolean no_sequence;  /* used to disable sequence printing */
    Uint1	code;	/* coding of sequence */
    Boolean	formatdb; /* TRUE, if is used in formatdb */
    Boolean printid_general; /* show gi and gnl for SeqId */
    SeqLocPtr seqloc;  /* sub-sequence of interest */
} MyFsa, PNTR MyFsaPtr;

typedef struct tofasta {
    Boolean is_na;
    Boolean got_one;
    MyFsaPtr mfp;
	Uint1 group_segs;
	Int2 last_indent,
		parts, seg;
} FastaDat, PNTR FastaPtr;
    

NLM_EXTERN Boolean BioseqRawToFastaExtra PROTO((BioseqPtr bsp, FILE *fp, Int2 line_length));
NLM_EXTERN Boolean BioseqRawToFastaExtraEx PROTO((BioseqPtr bsp, FILE *fp, Int2 line_length, SeqLocPtr slp));
NLM_EXTERN Boolean BioseqRawToFasta PROTO((BioseqPtr bsp, FILE * fp, Boolean is_na));
NLM_EXTERN Boolean SeqEntryToFasta PROTO((SeqEntryPtr sep, FILE * fp, Boolean is_na));
NLM_EXTERN Boolean SeqEntryToFastaEx PROTO((SeqEntryPtr sep, FILE * fp, Boolean is_na, Boolean printid_general));
NLM_EXTERN Boolean BioseqToFasta PROTO((BioseqPtr bsp, FILE *fp, Boolean is_na));
NLM_EXTERN Boolean BioseqToFastaDump PROTO((BioseqPtr bsp, FILE *fp, Boolean is_na));



NLM_EXTERN void	SeqEntryFasta PROTO ((SeqEntryPtr sep, Pointer data,
                                     Int4 index, Int2 indent));
   

/*****************************************************************************
*
*   SeqEntrysToFasta(sep, fp, is_na, group_segs)
*
*   	group_segs = 0 ... take only raw Bioseqs
*       group_segs = 1 ... group segmented seqs into single entry.. no parts
*       group_segs = 2 ... show only parts of segmented seqs
*       group_segs = 3 ... like 1, but also instantiate virtual sequences
*   
*****************************************************************************/
NLM_EXTERN Boolean SeqEntrysToFasta PROTO((SeqEntryPtr sep, FILE *fp, Boolean is_na, Uint1 group_segs));

NLM_EXTERN Boolean SeqEntrysToFastaX PROTO((SeqEntryPtr sep, MyFsaPtr mfp, Boolean is_na, Uint1 group_segs));
NLM_EXTERN Boolean SeqEntrysToDefline PROTO((SeqEntryPtr sep, 
                                  FILE *fp, Boolean is_na, Uint1 group_segs));
NLM_EXTERN Boolean BioseqRawToFastaX PROTO((BioseqPtr bsp, MyFsaPtr mfp, Boolean is_na));
NLM_EXTERN Boolean BioseqToFastaX PROTO((BioseqPtr bsp, MyFsaPtr mfp, Boolean is_na));

/*****************************************************************************
*
*   BioseqFastaStream (bsp, fp, flags, linelen, blocklen, grouplen, do_defline)
*   BioseqFastaMemStream (bsp, bs, flags, linelen, blocklen, grouplen, do_defline)
*   SeqLocFastaStream (slp, bs, flags, linelen, blocklen, grouplen)
*   SeqEntryFastaStream (sep, fp, flags, linelen, blocklen, grouplen,
*                        do_na, do_aa, master_style)
*
*   	Rapid FASTA generators using SeqPortStream
*
*****************************************************************************/
NLM_EXTERN Int4 BioseqFastaStream (
  BioseqPtr bsp,
  FILE *fp,
  StreamFlgType flags,
  Int2 linelen,
  Int2 blocklen,
  Int2 grouplen,
  Boolean do_defline
);

NLM_EXTERN Int4 BioseqFastaMemStream (
  BioseqPtr bsp,
  ByteStorePtr bs,
  StreamFlgType flags,
  Int2 linelen,
  Int2 blocklen,
  Int2 grouplen,
  Boolean do_defline
);

NLM_EXTERN Int4 SeqLocFastaStream (
  SeqLocPtr slp,
  FILE *fp,
  StreamFlgType flags,
  Int2 linelen,
  Int2 blocklen,
  Int2 grouplen
);

NLM_EXTERN Int4 SeqEntryFastaStream (
  SeqEntryPtr sep,
  FILE *fp,
  StreamFlgType flags,
  Int2 linelen,
  Int2 blocklen,
  Int2 grouplen,
  Boolean do_na,
  Boolean do_aa,
  Boolean master_style
);

/*****************************************************************************
*
*   FastaFileFunc(key, buf, data)
*   	standard "write to file" callback
*
*****************************************************************************/
NLM_EXTERN Boolean FastaFileFunc PROTO((BioseqPtr bsp, Int2 key,
                                        CharPtr buf, Uint4 buflen, Pointer data));


/*****************************************************************************
*
*   FastaDumpFileFunc(key, buf, data)
*       standard "write to file" callback
*
*       Used for BLAST (FASTA) databases.  If the defline is
*       longer than buflen, then check that an ID is not
*       truncated in the middle.
*
*****************************************************************************/
NLM_EXTERN Boolean FastaDumpFileFunc PROTO((BioseqPtr bsp, Int2 key, CharPtr buf, Uint4 buflen, Pointer data));

/*****************************************************************************
*
*   Reads a Fasta File into a SeqEntry structure
*   Conventions:
*   >name def
*   agaggagagagagag
*   agagagagagagagag
*
*   "name" = string is considered SEQID_LOCAL until first white space
*   "def"  = after first white space, and before first newline will be "title"
*   "agaga.." = sequence follows until EOF. can be upper or lower case IUPAC
*
*****************************************************************************/
NLM_EXTERN SeqEntryPtr FastaToSeqEntry PROTO((FILE *fp, Boolean is_na));

/*****************************************************************************
*
*   Reads a Fasta Buffer into a SeqEntry structure
*   Conventions:
*   >name def
*   agaggagagagagag
*   agagagagagagagag
*
*   "name" = string is considered SEQID_LOCAL until first white space
*   "def"  = after first white space, and before first newline will be "title"
*   "agaga.." = sequence follows until EOF. can be upper or lower case IUPAC
*
*****************************************************************************/
NLM_EXTERN SeqEntryPtr FastaToSeqBuff PROTO((CharPtr buffer, 
                                  CharPtr PNTR last_char,Boolean is_na));

NLM_EXTERN SeqEntryPtr FastaToSeqBuffEx(CharPtr buffer, CharPtr PNTR last_char, 
                             Boolean is_na, CharPtr PNTR errormsg,
                             Boolean parseSeqId);
NLM_EXTERN SeqEntryPtr FastaToSeqBuffForDb(CharPtr buffer, CharPtr PNTR last_char, 
                             Boolean is_na, CharPtr PNTR errormsg,
                             Boolean parseSeqId, CharPtr prefix, Int2Ptr ctrptr,
			     SeqLocPtr PNTR mask_ptr);
NLM_EXTERN SeqEntryPtr FastaToSeqEntryEx (FILE *fp, Boolean is_na, 
                               CharPtr PNTR errormsg,
                               Boolean parseSeqId); 
NLM_EXTERN SeqEntryPtr FastaToSeqEntryForDb ( FILE *fp, Boolean is_na,
                               CharPtr PNTR errormsg,
                               Boolean parseSeqId,
                               CharPtr prefix, Int2Ptr ctrptr, 
                               SeqLocPtr PNTR mask_ptr);

/********* DEFINES for input type *********/

#define FASTA_MEM_IO  1   /* type of reading from buffer in memory */
#define FASTA_FILE_IO 2   /* type of reading from file */

NLM_EXTERN SeqEntryPtr FastaToSeqEntryInternalEx ( VoidPtr input,
                               Int4 type, CharPtr PNTR next_char,
                               Boolean is_na, CharPtr PNTR errormsg,
                               Boolean parseSeqId, CharPtr special_symbol,
                               CharPtr prefix, Int2Ptr ctrptr,
                               SeqLocPtr PNTR mask_ptr);

/*****************************************************************************
*
*   Boolean FastaReadSequence() - read sequence from file
*
*****************************************************************************/

Boolean FastaReadSequence
(
 FILE *fd,                 /* input file pointer) */
 Boolean is_na,            /* type of sequence */
 Int4Ptr seq_length,       /* Returned length of sequence in residues */
 ByteStorePtr PNTR bs_out, /* Returned pointer to sequence ByteStore */
 CharPtr PNTR errormsg     /* error message for debugging */
 );

/*****************************************************************************
*
*   Boolean FastaReadSequenceMem() - read sequence from buffer
*
*****************************************************************************/

Boolean FastaReadSequenceMem
(
 CharPtr buffer,           /* input buffer with sequence */
 CharPtr PNTR next_char,   /* returned pointer to next FASTA sequence */
 Boolean is_na,            /* type of sequence */
 Int4Ptr seq_length,       /* Returned length of sequence in residues */
 ByteStorePtr PNTR bs_out, /* Returned pointer to sequence ByteStore */
 CharPtr PNTR errormsg     /* error message for debugging */
);

/*****************************************************************************
*
*   This routines get parts needed to make FASTA format from ASN.1
*
*****************************************************************************/
/*****************************************************************************
*
*   FastaId(bsp, buf, buflen)
*      Makes the string for the id part of fasta format.
*      buf should be at least 40 bytes
*
*****************************************************************************/
NLM_EXTERN Boolean FastaId PROTO((BioseqPtr bsp, CharPtr buf, Uint4 buflen));

/*****************************************************************************
*
*   FastaDefLine(bsp, buf, buflen, accession, organism)
*   	Finds or makes a FASTA format defline (just locates the string)
*       buf should be very long if possible
*       function truncates if buf not long enough
*       a few deflines are longer than 255
*       if (accession != NULL) prefixes defline with (accession)
*          used for translated GenBank records
*       if (organism != NULL) adds [organism] to end
*       if (tech == MI_TECH_phase1 or phase2, adds order comment to defline)
*
*****************************************************************************/
NLM_EXTERN Boolean FastaDefLine PROTO((BioseqPtr bsp, CharPtr buf, Uint4 buflen, CharPtr accession, CharPtr organism, Uint1 tech));

NLM_EXTERN Boolean CreateDefLine PROTO((ItemInfoPtr dip, BioseqPtr bsp, CharPtr buf, Uint4 buflen,
                                        Uint1 tech, CharPtr accession, CharPtr organism));
NLM_EXTERN Boolean CreateDefLineEx (ItemInfoPtr iip, BioseqPtr bsp, CharPtr buf, Uint4 buflen, Uint1 tech,
                                    CharPtr accession, CharPtr organism, Boolean ignoreTitle);
NLM_EXTERN Boolean CreateDefLineExEx (ItemInfoPtr iip, BioseqPtr bsp, CharPtr buf, Uint4 buflen, Uint1 tech,
                                      CharPtr accession, CharPtr organism, Boolean ignoreTitle, Boolean extProtTitle);
/*****************************************************************************
*
*   FastaSeqPort(bsp, is_na, do_virtual)
*   	opens a SeqPort for a fasta output of bsp
*
*****************************************************************************/
NLM_EXTERN SeqPortPtr FastaSeqPort PROTO((BioseqPtr bsp, Boolean is_na,
                                          Boolean do_virtual, Uint1 code));
NLM_EXTERN SeqPortPtr FastaSeqPortEx PROTO((BioseqPtr bsp, Boolean is_na, 
        Boolean do_virtual, Uint1 code, SeqLocPtr slp));

/*****************************************************************************
*
*   FastaSeqLine(spp, buf, linelen)
*     an open seqport is passed in.
*     fills buf with linelen bases
*     assumes buf[linelen] = '\0'
*     returns FALSE when no more residues to print
*     here for backward compatibility. Is the same as calling
*      FastaSeqLineEx() with do_virtual = FALSE;
*
*****************************************************************************/
NLM_EXTERN Boolean FastaSeqLine PROTO((SeqPortPtr spp, CharPtr buf, Int2 linelen, Boolean is_na));

/****************************************************************************
*
*  FastaSeqLineEx(spp, buf, linelen, is_na, do_virtual)
*
*    adds a parameter to indicate if virtual sequences should be shown
*     this is necessary in order to avoid showing SEQPORT_VIRT return
*     (gap of 0 length) as '-', when do_virtual is TRUE. 
*
*****************************************************************************/
NLM_EXTERN Boolean FastaSeqLineEx(SeqPortPtr spp, CharPtr buf, Int2 linelen, Boolean is_na, Boolean
do_virtual);

Int4 GetOrderBySeqId(Int4 choice, Boolean is_prot);

/*****************************************************************************
*
*   NC_Cleanup (entityID, ptr) and ClearGenBankKeywords (entityID, ptr)
*     internal functions for genome RefSeq processing
*
*****************************************************************************/
NLM_EXTERN void NC_Cleanup (Uint2 entityID, Pointer ptr);
NLM_EXTERN void ClearGenBankKeywords (Uint2 entityID, Pointer ptr);

/*****************************************************************************
*
*   InstantiateProteinTitles (entityID, ptr) and ClearProteinTitles (entityID, ptr)
*     allows proteins titles to be kept as Seq_descr_title rather than always
*     being generated on the fly
*
*****************************************************************************/
NLM_EXTERN void InstantiateProteinTitles (Uint2 entityID, Pointer ptr);
NLM_EXTERN void ClearProteinTitlesInNucProts (Uint2 entityID, Pointer ptr);


#ifdef __cplusplus
}
#endif

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

#endif
