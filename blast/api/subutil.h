/*  subutil.h
* >> Set tabs to 4 spaces for a nice printout
*   
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
* File Name:  subutil.h
*
* Author:  James Ostell
*   
* Version Creation Date: 11/3/93
*
* $Revision: 6.39 $
*
* File Description: Utilities for creating ASN.1 submissions
*
* Modifications:  
* --------------------------------------------------------------------------
* Date	   Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* $Log: subutil.h,v $
* Revision 6.39  2001/12/12 16:01:47  kans
* added MOLECULE_TYPE_TRANSCRIBED_RNA, SUBSRC_transgenic, SUBSRC_environmental_sample, and SUBSRC_isolation_source
*
* Revision 6.38  2001/12/11 16:50:41  kans
* added TpaAssembly user object creation functions
*
* Revision 6.37  2001/12/10 13:47:44  kans
* added MOLECULE_TYPE defines for CRNA and SNORNA
*
* Revision 6.36  2001/11/09 15:12:21  kans
* moved collaborator to separate function per user object
*
* Revision 6.35  2001/11/09 13:21:08  kans
* added collaborator field to reftrack user object
*
* Revision 6.34  2001/11/09 12:54:47  kans
* added model evidence user object functions
*
* Revision 6.33  2001/07/29 16:23:58  kans
* added AddOrganismToEntryEx to allow entry of taxonID
*
* Revision 6.32  2001/01/25 19:55:14  kans
* added AddGenBankSetToSubmission for patents and other bulk submissions of unrelated sequences
*
* Revision 6.31  2000/11/15 23:18:00  kans
* gene ontology user object functions
*
* Revision 6.30  2000/10/10 15:06:01  kans
* added SUBSRC_endogenous_virus_name
*
* Revision 6.29  2000/09/28 11:41:33  ostell
* added GENOME_endogenous_virus
*
* Revision 6.28  2000/09/20 12:32:54  kans
* removed AddMethodToRefGeneTrackUserObject and AddSupportToRefGeneTrackUserObject
*
* Revision 6.27  2000/08/29 22:27:02  kans
* reftrack support now has organism field
*
* Revision 6.26  2000/07/21 10:54:14  kans
* added functions for adding Method and Support to RefGeneTrack user object
*
* Revision 6.25  2000/07/12 22:45:15  kans
* added ORGMOD_old_lineage
*
* Revision 6.24  2000/06/20 17:17:28  kans
* added orgmod.subtype authority through breed
*
* Revision 6.23  2000/06/14 16:41:20  kans
* added SUBSRC_segment
*
* Revision 6.22  2000/03/10 18:35:46  kans
* added CreateContigCloneUserObject for genome contig RefSeq records
*
* Revision 6.21  2000/02/26 23:55:34  kans
* added AddPhrapGraphToSeqLit, using internal extended SeqLit to track, readjust graphs to current position of seqlit in delta seq - may need to move fields to public SeqLit if problems arise
*
* Revision 6.20  2000/02/24 23:15:00  kans
* added AddSecondaryAccnToEntry
*
* Revision 6.19  2000/02/23 19:25:54  kans
* AddFakeGapToDeltaSeq returns SeqLitPtr so program can set fuzz after SpreadGapsInDeltaSeq is no longer being called
*
* Revision 6.18  2000/02/23 18:28:39  kans
* added AddFakeGapToDeltaSeq to set fuzz even if nonzero length is passed in
*
* Revision 6.17  2000/02/07 16:48:08  ostell
* added AddDeltaSeqToNucProtEntry()
*
* Revision 6.16  1999/12/20 19:57:45  kans
* added AddStatusToRefGeneTrackUserObject
*
* Revision 6.15  1999/12/14 17:32:45  ostell
* changed protoplast to proplastid
*
* Revision 6.14  1999/10/05 20:36:01  kans
* added more GENOME_ types (JO)
*
* Revision 6.13  1999/08/05 20:43:44  kans
* added CreateSubmissionUserObject (JP)
*
* Revision 6.12  1999/03/17 22:49:38  kans
* changed SUBSOURCE_ to SUBSRC_
*
* Revision 6.11  1999/03/17 20:04:40  kans
* added SUBSOURCE_ and ORGMOD_ defines
*
* Revision 6.10  1998/10/02 17:48:12  kans
* new parameters to AddAccessionToRefGeneTrackUserObject, and added CreateMrnaProteinLinkUserObject
*
* Revision 6.9  1998/09/29 15:34:20  kans
* added comment for new choices in molinfo.completeness
*
* Revision 6.8  1998/09/23 16:41:49  kans
* removed from and to from AddAccessionToRefGeneTrackUserObject
*
* Revision 6.7  1998/08/20 20:37:00  kans
* add comment parameter to reftrack user object function
*
* Revision 6.6  1998/08/11 22:27:26  kans
* added CreateRefGeneTrackUserObject and AddAccessionToRefGeneTrackUserObject
*
* Revision 6.5  1998/02/05 19:19:30  kans
* added AddPhrapGraph
*
* Revision 6.4  1998/01/06 03:29:49  ostell
* added AddGeneticCodetoEntry()
*
* Revision 6.3  1997/12/15 17:31:48  kans
* backed out accidental checkin
*
* Revision 6.1  1997/11/25 20:56:04  ostell
* GENOME_genomic changed from 2 to 1
*
* Revision 6.0  1997/08/25 18:07:39  madden
* Revision changed to 6.0
*
* Revision 5.12  1997/07/11 18:23:29  kans
* added Add[Pop/Phy/Mut]SetToSubmission
*
* Revision 5.11  1997/06/19 18:39:13  vakatov
* [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
*
* Revision 5.10  1997/05/16 19:14:03  ostell
* moved prototype of NewDescrOnSeqEntry from subutil.c to subutil.h to make
* it external.
*
 * Revision 5.9  1997/03/17  17:18:46  tatiana
 * added defines for BioSource.genome
 *
 * Revision 5.8  1996/10/16  18:10:56  epstein
 * change C++ right paren to brace per Eric Hackborn
 *
 * Revision 5.7  1996/09/05  20:20:28  ostell
 * added CitSubUpdateBuild()
 *
 * Revision 5.6  1996/09/04  22:33:06  ostell
 * added MakeCommentFeature()
 *
 * Revision 5.5  1996/08/20  15:32:53  tatiana
 * prototypes added for AddAAsToByteStore and AddBasesToByteStore
 *
 * Revision 5.4  1996/06/14  19:00:06  ostell
 * fixed line wraps
 *
 * Revision 5.3  1996/06/14  18:07:38  ostell
 * added AddOrgModToEntry() and AddSubSourceToEntry()
 *
 * Revision 5.2  1996/06/14  17:27:18  ostell
 * added comments to subutil on BioSource and MolInfo attributes
 *
 * Revision 5.1  1996/06/13  03:34:53  ostell
 * added support for delta seqs and submittor_key
 * fixed memory leak in AddAminoAcidsToBioseq()
 *
 * Revision 5.0  1996/05/28  13:23:23  ostell
 * Set to revision 5.0
 *
 * Revision 4.2  1996/03/05  17:06:25  tatiana
 * remove static from GetDescrOnSeqEntry,
 * taxonomy added to AddOrganismToEntryNew arguments
 *
 * Revision 4.1  1996/01/31  02:35:00  ostell
 * added AddCodeBreakToCdRegion() and AddAntiCodonTotRNA()
 *
 * Revision 4.0  1995/07/26  13:49:01  ostell
 * force revision to 4.0
 *
 * Revision 1.8  1995/07/25  17:49:32  kans
 * added prototype for AddOrganismToEntryNew
 *
 * Revision 1.7  1995/07/25  14:48:17  tatiana
 * add new functions for Biosource and Molinfo
 *
 * Revision 1.6  1995/06/03  13:45:47  ostell
 * changes made in valid to use gather functions and ErrPostItem instead
 * of previous custom functions
 *
 * Revision 1.5  1995/05/15  21:46:05  ostell
 * added Log line
 *
*
*
* ==========================================================================
*/

#ifndef _NCBI_SubUtil_
#define _NCBI_SubUtil_

#ifndef _NCBI_Submit_
#include <objsub.h>
#endif

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif


/*****************************************************************************
*
*   Create a GenBank direct submission
*   	This supports a basic set of datatypes for making a new direct
*   submission to GenBank in ASN.1. It is designed for folks wanting to
*   read their own data storage format, then make a valid direct submission
*   without going through an intermediate tool.
*
*   	You may have many "entries" in a single submission. A single entry
*   may contain:
*   	One protein sequence       (called a "Bioseq")
*   	One nucleic acid sequence  (called a "Bioseq")
*   	One nucleic acid sequence for which you only have a series of
*          sequence pieces (e.g. you sequenced around the exons of a
*          genomic sequence, but not the introns) (called a "segmented
*          set")
*   	One nucleic acid sequence and the protein sequences it codes for.
*          (nucleic acid may a a single Bioseq or a segmented set)
*          (this entry called a "nuc-prot set")
*
*   NCBI considers the protein sequences part
*   of the submission, and they are created as proteins in their own right
*   by the routines below. You can either supply the protein sequence from
*   your own software (best case), in which we check that the coding region
*   you supply translates to it. If you do not supply a protein sequence,
*   then all we can do is check that it translates without stops.
*
*   	NCBI also considers "gene" to refer to a region of nucleic acid
*   within which are found elements (such as promoters, coding regions,etc)
*   leading to a phenomenon recognized as a gene (note this also accomodates
*   anonymous markers as well as expressed products). This is in contrast to
*   so other notions that a gene is simply a qualifier on other features of
*   the DNA. A separate function to produce a gene feature is supplied. The
*   intervals given for it should include the intervals for the other
*   features it contains.
*
*   	The process of building the direct submission is roughly:
*
*   Create the submission
*   	Add the submission citation
*   	Create an entry  (can be 1 or more sequences)
*   		Add the organism information
*   		Add any publication citations
*   		Add the sequences
*             Fill in the residues
*   		Add the features
*   		Validate the entry
*   Write the entry
*   Free the memory used
*
*   Each element may have subfunctions:
*
*   Create a citation
*   	Add author names
*   	Add author affiliation
*
*   Create a sequence
*   	Add modifiers
*
*   Create a feature
*   	Add information specific to type of feature
*   	Add intervals on the sequence
*   
*****************************************************************************/
typedef Boolean (* SubErrorFunc) (CharPtr msg);

typedef struct ncbisub {
	SeqSubmitPtr ssp;               /* the submission */
	SubErrorFunc err_func;          /* the error handler */
	Int2 gap_count;                 /* for unique gap names in segs */
	CharPtr submittor_key;          /* used for turning local SeqId to General */
} NCBISub, PNTR NCBISubPtr;

#define PubPtr ValNodePtr           /* should really be typedeffed */

/*****************************************************************************
*
*   Prototypes for building a direct submission
*
*****************************************************************************/

						 /* default error handler */

NLM_EXTERN Boolean DefaultSubErrorFunc (CharPtr msg);

/*****************************************************************************
*
*   Create/Free the NCBISub
*
*****************************************************************************/

NLM_EXTERN NCBISubPtr NCBISubCreate (
	CharPtr last_name,
	CharPtr first_name,
	CharPtr middle_name,
	CharPtr initials,  /* separated by periods, no initial for last name */
	CharPtr suffix,    /* Jr. Sr. III */
	CharPtr affil,        /* e.g. "Xyz University" */
	CharPtr div,          /* e.g. "Dept of Biology" */
	CharPtr street,       /* e.g. "123 Academic Road" */
	CharPtr city,         /* e.g. "Metropolis" */
	CharPtr sub,          /* e.g. "Massachusetts" */
	CharPtr country ,     /* e.g. "USA" */
	CharPtr postal_code,  /* e.g."02133" */
	CharPtr phone ,
	CharPtr fax ,
	CharPtr email,
	Boolean hold_until_publish ,
	Int2 release_month ,
	Int2 release_day ,
	Int2 release_year );

NLM_EXTERN Boolean DefineSubmittorKey(
	NCBISubPtr nsp,
	CharPtr submittor_key );  /* submitting large scale lab, for regular submissions */

/**** WARNING: NCBISubBuild() is the old style submission         ***/
/**** It has been replaced by  NCBISubCreate()                    ***/
/**** NCBISubBuild will be discontinued                           ***/

NLM_EXTERN NCBISubPtr NCBISubBuild (
	CharPtr name,
	CharPtr PNTR address ,
	CharPtr phone ,
	CharPtr fax ,
	CharPtr email,
	Boolean hold_until_publish ,
	Int2 release_month,
	Int2 release_day,
	Int2 release_year );



               /** every submission must have 1 submission citation **/
               /** see below to add authors and affiliation **********/

NLM_EXTERN Boolean CitSubForSubmission (
	NCBISubPtr submission,
	PubPtr cit_sub );

NLM_EXTERN Boolean AddToolToSub (
	NCBISubPtr nsp,
	CharPtr tool );

NLM_EXTERN Boolean AddCommentToSub (
	NCBISubPtr nsp,
	CharPtr comment );

NLM_EXTERN Boolean AddTypeToSub (
	NCBISubPtr nsp,
	Uint1 type );

NLM_EXTERN Boolean NCBISubWrite (
	NCBISubPtr ssp,
	CharPtr filename );

NLM_EXTERN NCBISubPtr NCBISubFree (
	NCBISubPtr ssp );

/*****************************************************************************
*
*   You can (should) run the ncbi validator routines on your final submission.
*   It returns a count of all errors or questions found.
*
*   The errfile parameter is no longer supported. Errors are directed
*   based on the ErrLog functions in the toolkit. If you have done none
*   of this, then errors will appear on stderr.
*
*****************************************************************************/

NLM_EXTERN Int2 NCBISubValidate (NCBISubPtr nsp, FILE * errfile);

/*****************************************************************************
*
*   Add Entries to the Submission
*   Add Sequences to the Entries
*
*****************************************************************************/

/*****************************************************************************
*
*   About Sequence Identifiers:
*
*   Note that in all functions below where you create a Bioseq in your entry,
*   you can supply a number of different pieces of information to make a
*   sequence id.
*
*   local_name: This is a string for whatever name you call this sequence
*               locally. Could be a clone name or whatever. There are no
*               limits on this other than it should be unique in the
*               submission. It is REQUIRED.
*
*   SeqEntryPtr: Returned by the function, this is a pointer to the Bioseq.
*
*   In later functions, such as adding feature locations, you can refer to
*   the Bioseq you created either with the local_name or directly with the
*   SeqEntryPtr. Whatever is more convenient for you is fine.
*
*   The other ids only apply to updates. These allow you to update your
*   entry in GenBank simply by sending a new entry with the same accession
*   number you were issued on the last one. In this case you should also
*   be sure to add the create_date, which will be returned to you in the
*   ASN.1 of your direct submission after processing. This is not absolutely
*   required, but does let us check that it is the right entry (errors
*   could occur when you enter your old accession number).
*
*   genbank_locus:  OPTIONAL on update. The name appearing on the LOCUS line.
*   genbank_accession: REQUIRED on update.
*   gi_number: OPTIONAL on update for now. The unique ID assigned by NCBI
*      to a particular sequence (DNA or protein) in your entry.
*
*   If you update your entry, whether you change the sequence or not, the
*   accession number and locus will remain the same, so people can retrieve
*   your new data with the old id. However, the gi_number is explicitly keyed
*   to the sequence, and will change if there are any changes/additions to
*   the sequence. In addition, a history will be created indicating the old
*   gi_number and the date the new entry replaced it. Both old and new
*   entries will be available from NCBI for retrieval on gi_number. Only the
*   new entry will appear in the next GenBank or Entrez release.
*
*****************************************************************************/


            /*** Entry contains only 1 raw Bioseq ***/

NLM_EXTERN SeqEntryPtr AddSeqOnlyToSubmission (
	NCBISubPtr submission ,
	CharPtr local_name ,
	CharPtr genbank_locus ,
	CharPtr genbank_accession ,
	Int4 gi_number ,
	Int2 molecule_class,
	Int2 molecule_type ,
	Int4 length ,
	Int2 topology ,
	Int2 strandedness );

            /*** Entry contains a segmented set of Bioseqs ***/

NLM_EXTERN SeqEntryPtr AddSegmentedSeqToSubmission (
	NCBISubPtr submission ,
	CharPtr local_name ,
	CharPtr genbank_locus ,
	CharPtr genbank_accession ,
	Int4 gi_number ,
	Int2 molecule_class,
	Int2 molecule_type ,
	Int4 length ,
	Int2 topology ,
	Int2 strandedness );

NLM_EXTERN SeqEntryPtr AddSeqToSegmentedEntry (
	NCBISubPtr submission,
	SeqEntryPtr segmented_seq_entry,
	CharPtr local_name ,
	CharPtr genbank_locus ,
	CharPtr genbank_accession ,
	Int4 gi_number ,
	Int2 molecule_class,
	Int2 molecule_type ,
	Int4 length ,
	Int2 topology ,
	Int2 strandedness );

NLM_EXTERN Boolean AddGapToSegmentedEntry (
	NCBISubPtr submission,
	SeqEntryPtr segmented_seq_entry,
	Int4 length_of_gap );    /** 0 if not known */

NLM_EXTERN Boolean AddReferenceToSegmentedEntry (
	NCBISubPtr submission ,
	SeqEntryPtr segmented_seq_entry,
	CharPtr genbank_accession ,
	Int4 gi_number ,
	Int4 from ,
	Int4 to ,
	Boolean on_plus_strand );

			/*** Entry contains sets of similar sequences ***/

NLM_EXTERN SeqEntryPtr AddPopSetToSubmission (
	NCBISubPtr submission );

NLM_EXTERN SeqEntryPtr AddPhySetToSubmission (
	NCBISubPtr submission );

NLM_EXTERN SeqEntryPtr AddMutSetToSubmission (
	NCBISubPtr submission );

NLM_EXTERN SeqEntryPtr AddGenBankSetToSubmission (
	NCBISubPtr submission );

			/*** Entry contains nucleotide and translated proteins ***/

NLM_EXTERN SeqEntryPtr AddNucProtToSubmission (
	NCBISubPtr submission );

NLM_EXTERN SeqEntryPtr AddSeqToNucProtEntry (   /** add unsegmented nuc or prot bioseq */
	NCBISubPtr submission,
	SeqEntryPtr nuc_prot_entry,
	CharPtr local_name ,
	CharPtr genbank_locus ,
	CharPtr genbank_accession ,
	Int4 gi_number ,
	Int2 molecule_class,
	Int2 molecule_type ,
	Int4 length ,
	Int2 topology ,
	Int2 strandedness );
								  /** add segmented nuc or prot bioseq set */

NLM_EXTERN SeqEntryPtr AddSegmentedSeqToNucProtEntry (
	NCBISubPtr submission,
	SeqEntryPtr nuc_prot_entry ,
	CharPtr local_name ,
	CharPtr genbank_locus ,
	CharPtr genbank_accession ,
	Int4 gi_number ,
	Int2 molecule_class,
	Int2 molecule_type ,
	Int4 length ,
	Int2 topology ,
	Int2 strandedness );

NLM_EXTERN SeqEntryPtr AddDeltaSeqToNucProtEntry (
	NCBISubPtr submission,
	SeqEntryPtr nuc_prot_entry ,
	CharPtr local_name ,
	CharPtr genbank_locus ,
	CharPtr genbank_accession ,
	Int4 gi_number ,
	Int2 molecule_class,
	Int2 molecule_type ,
	Int4 length ,
	Int2 topology ,
	Int2 strandedness );

				 /**** Entry contains one delta sequence ****/

NLM_EXTERN SeqEntryPtr AddDeltaSeqOnlyToSubmission (
	NCBISubPtr submission,
	CharPtr local_name ,
	CharPtr genbank_locus ,
	CharPtr genbank_accession ,
	Int4 gi_number ,
	Int2 molecule_class,
	Int2 molecule_type ,
	Int4 length ,
	Int2 topology ,
	Int2 strandedness );

NLM_EXTERN Boolean AddGapToDeltaSeq (
	NCBISubPtr submission,
	SeqEntryPtr delta_seq_entry,
	Int4 length_of_gap );    /** 0 if not known */

NLM_EXTERN SeqLitPtr AddFakeGapToDeltaSeq (
	NCBISubPtr submission,
	SeqEntryPtr delta_seq_entry,
	Int4 length_of_gap );    /** returns slp so program can set lim - unk fuzz after empty gaps are spread */

NLM_EXTERN SeqLitPtr AddLiteralToDeltaSeq (
	NCBISubPtr submission,
	SeqEntryPtr delta_seq_entry,
	Int4 length_of_sequence );


#define MOLECULE_CLASS_DNA 1
#define MOLECULE_CLASS_RNA 2
#define MOLECULE_CLASS_NUC 4
#define MOLECULE_CLASS_PROTEIN 3

#define MOLECULE_TYPE_GENOMIC 1
#define MOLECULE_TYPE_PRE_MRNA 2
#define MOLECULE_TYPE_MRNA 3
#define MOLECULE_TYPE_RRNA 4
#define MOLECULE_TYPE_TRNA 5
#define MOLECULE_TYPE_SNRNA 6
#define MOLECULE_TYPE_SCRNA 7
#define MOLECULE_TYPE_PEPTIDE 8
#define MOLECULE_TYPE_OTHER_GENETIC_MATERIAL 9
#define MOLECULE_TYPE_GENOMIC_MRNA_MIX 10
#define MOLECULE_TYPE_CRNA 11
#define MOLECULE_TYPE_SNORNA 12
#define MOLECULE_TYPE_TRANSCRIBED_RNA 13

#define TOPOLOGY_LINEAR 1
#define TOPOLOGY_CIRCULAR 2
#define TOPOLOGY_TANDEM 3

#define STRANDEDNESS_SINGLE 1
#define STRANDEDNESS_DOUBLE 2
           
/******************************************************************
*
*   Fill in Bases or Amino Acids
*   	 1) You may call functions as often per bioseq as you like
*   		  up to the length of the Bioseq
*   	 2) All codes are iupac and defined in /ncbi/data/seqcode.prt
*   		  as an ASN.1 file used by this code. Excerpts at the
*   		  end of this file. Even though it's ASN.1 you will find
*   		  you can read it with no trouble.
*   	 3) IUPAC codes are UPPER CASE. These functions will upper
*   		  case for you.
*   	 4) In nucleic acids 'U' will be changed to 'T'
*   	 5) In both cases, non-letters will be stripped from the
*   		  the input strings to facilate input from external
*   		  formatted files with numbers and internal spaces and
*          such.
*     
******************************************************************/

NLM_EXTERN Boolean AddBasesToBioseq (
	NCBISubPtr submission ,
	SeqEntryPtr the_seq ,
	CharPtr the_bases );

NLM_EXTERN Boolean AddAminoAcidsToBioseq (
	NCBISubPtr submission ,
	SeqEntryPtr the_seq ,
	CharPtr the_aas );

           /** variant functions for Delta sequences ***/

NLM_EXTERN Boolean AddBasesToLiteral (
	NCBISubPtr submission ,
	SeqLitPtr the_literal ,
	CharPtr the_bases );

NLM_EXTERN Boolean AddAminoAcidsToLiteral (
	NCBISubPtr submission ,
	SeqLitPtr the_literal ,
	CharPtr the_aas );


/*****************************************************************************
*
*   Add Annotations to Entries
*
*****************************************************************************/

NLM_EXTERN Boolean AddTitleToEntry (
	NCBISubPtr submission,
	SeqEntryPtr entry ,
	CharPtr title );

NLM_EXTERN Boolean AddSecondaryAccnToEntry (
NCBISubPtr submission,
	SeqEntryPtr entry ,
	CharPtr accn );

/*****************************************************************
*
*   rules for long comments
*     1) include no non-ascii characters (e.g. \t \r \n)
*     2) you may force a line feed on display by using tilde '~'
*     3) you format a table by adding leading spaces after a '~'
*     4) non-ascii chars will be converted on input (also for
*         title) \n='~', all others='#'
*
*****************************************************************/

NLM_EXTERN Boolean AddCommentToEntry (
	NCBISubPtr submission,
	SeqEntryPtr entry ,
	CharPtr comment );

NLM_EXTERN Boolean AddOrganismToEntryNew (
	NCBISubPtr submission,
	SeqEntryPtr entry ,
	CharPtr scientific_name ,
	CharPtr common_name ,
	CharPtr virus_name ,
	CharPtr strain ,
	CharPtr synonym1,
	CharPtr synonym2,
	CharPtr synonym3,
	CharPtr taxonomy );

           /** AddOrganismToEntryNew() defaults to universal code (0)
           ** for both cytoplasmic and mitochondiral ribosomes. You
           ** also supply the code when you create a CdRegion. If the
           ** CdRegion code does not match the organism code, the
           ** validator will warn, but will translate by CdRegion code.
           ** if you need an alternate code, SetGeneticCodeForEntry()
           ** can be used to eliminate the conflict. See table of genetic
           ** codes at end of this file. You should call
           ** AddOrganismToEntryNew() before calling
           ** SetGeneticCodeForEntry() **/

NLM_EXTERN Boolean AddOrganismToEntryEx (
	NCBISubPtr submission,
	SeqEntryPtr entry ,
	CharPtr scientific_name ,
	CharPtr common_name ,
	CharPtr virus_name ,
	CharPtr strain ,
	CharPtr synonym1,
	CharPtr synonym2,
	CharPtr synonym3,
	CharPtr taxonomy,
	Int4 taxid );

           /** AddOrganismToEntryEx() allows taxonID to be entered **/

NLM_EXTERN Boolean SetGeneticCodeForEntry (
	NCBISubPtr submission,
        SeqEntryPtr entry,
        Int2 genetic_code,  /* for cytoplasm */
        Int2 mito_code );   /* for mitochondria */

        

/**************************************************
*  OBSOLETE!!! do not use. Use AddOrganismToEntryNew
*
**************************************************/
NLM_EXTERN Boolean AddOrganismToEntry (
	NCBISubPtr submission,
	SeqEntryPtr entry ,
	CharPtr scientific_name ,
	CharPtr common_name ,
	CharPtr virus_name ,
	CharPtr strain ,
	CharPtr synonym1,
	CharPtr synonym2,
	CharPtr synonym3);

NLM_EXTERN Boolean AddGenBankBlockToEntry (
	NCBISubPtr submission,
	SeqEntryPtr entry ,
	CharPtr taxonomy ,
	CharPtr division ,
	CharPtr keyword1 ,
	CharPtr keyword2 ,
	CharPtr keyword3 );

#define GENOME_unknown 0
#define GENOME_genomic 1
#define GENOME_chloroplast 2
#define GENOME_chromoplast 3
#define GENOME_kinetoplast 4
#define GENOME_mitochondrion 5
#define GENOME_plastid 6
#define GENOME_macronuclear 7
#define GENOME_extrachrom 8
#define GENOME_plasmid 9
#define GENOME_transposon 10
#define GENOME_insertion_seq 11
#define GENOME_cyanelle 12
#define GENOME_proviral 13
#define GENOME_virion 14
#define GENOME_nucleomorph 15
#define GENOME_apicoplast 16
#define GENOME_leucoplast 17
#define GENOME_proplastid 18
#define GENOME_endogenous_virus 19

/********************************************
*  Genome describes the type of genome from which the DNA or gene for
*   a protein is located. Values are:
    genome INTEGER {             -- biological context
        unknown (0) ,
        genomic (1) ,
        chloroplast (2) ,
        chromoplast (3) ,
        kinetoplast (4) ,
        mitochondrion (5) ,
        plastid (6) ,
        macronuclear (7) ,
        extrachrom (8) ,
        plasmid (9) ,
        transposon (10) ,
        insertion-seq (11) ,
        cyanelle (12) ,
        proviral (13) ,
        virion (14) } DEFAULT unknown ,
         more types added, see GENOME_.. above
**********************************************/
 
NLM_EXTERN Boolean AddGenomeToEntry (
	NCBISubPtr submission,
	SeqEntryPtr entry ,
	Int2	 type );

#define SUBSRC_chromosome 1
#define SUBSRC_map 2
#define SUBSRC_clone 3
#define SUBSRC_subclone 4
#define SUBSRC_haplotype 5
#define SUBSRC_genotype 6
#define SUBSRC_sex 7
#define SUBSRC_cell_line 8
#define SUBSRC_cell_type 9
#define SUBSRC_tissue_type 10
#define SUBSRC_clone_lib 11
#define SUBSRC_dev_stage 12
#define SUBSRC_frequency 13
#define SUBSRC_germline 14
#define SUBSRC_rearranged 15
#define SUBSRC_lab_host 16
#define SUBSRC_pop_variant 17
#define SUBSRC_tissue_lib 18
#define SUBSRC_plasmid_name 19
#define SUBSRC_transposon_name 20
#define SUBSRC_insertion_seq_name 21
#define SUBSRC_plastid_name 22
#define SUBSRC_country 23
#define SUBSRC_segment 24
#define SUBSRC_endogenous_virus_name 25
#define SUBSRC_transgenic 26
#define SUBSRC_environmental_sample 27
#define SUBSRC_isolation_source 28
#define SUBSRC_other 255

/*********************************************
*  SubSource defines subclasses of source material
*    (also see OrgMod below for subclasses of organism names)
*
*  allowed values for type are:
        chromosome (1) ,
        map (2) ,
        clone (3) ,
        subclone (4) ,
        haplotype (5) ,
        genotype (6) ,
        sex (7) ,
        cell-line (8) ,
        cell-type (9) ,
        tissue-type (10) ,
        clone-lib (11) ,
        dev-stage (12) ,
        frequency (13) ,
        germline (14) ,
        rearranged (15) ,
        lab-host (16) ,
        pop-variant (17) ,
        tissue-lib (18) ,
        plasmid-name (19) ,
        transposon-name (20) ,
        insertion-seq-name (21) ,
        plastid-name (22) ,
        other (255) } ,

*   value is an optional string to give the name (eg. of the
*     clone)
******************************************/
NLM_EXTERN Boolean AddSubSourceToEntry (
	NCBISubPtr submission,
	SeqEntryPtr entry ,
	Int2	 type ,
	CharPtr value);

#define ORGMOD_strain 2
#define ORGMOD_substrain 3
#define ORGMOD_type 4
#define ORGMOD_subtype 5
#define ORGMOD_variety 6
#define ORGMOD_serotype 7
#define ORGMOD_serogroup 8
#define ORGMOD_serovar 9
#define ORGMOD_cultivar 10
#define ORGMOD_pathovar 11
#define ORGMOD_chemovar 12
#define ORGMOD_biovar 13
#define ORGMOD_biotype 14
#define ORGMOD_group 15
#define ORGMOD_subgroup 16
#define ORGMOD_isolate 17
#define ORGMOD_common 18
#define ORGMOD_acronym 19
#define ORGMOD_dosage 20
#define ORGMOD_nat_host 21
#define ORGMOD_sub_species 22
#define ORGMOD_specimen_voucher 23
#define ORGMOD_authority 24
#define ORGMOD_forma 25
#define ORGMOD_forma_specialis 26
#define ORGMOD_ecotype 27
#define ORGMOD_synonym 28
#define ORGMOD_anamorph 29
#define ORGMOD_teleomorph 30
#define ORGMOD_breed 31
#define ORGMOD_old_lineage 253
#define ORGMOD_old_name 254
#define ORGMOD_other 255 

/*********************************************
*  OrgMod defines subclasses of organism names
*    (also see SubSource above for subclasses of source material)
*
*  allowed values for type are:
        strain (2) ,
        substrain (3) ,
        type (4) ,
        subtype (5) ,
        variety (6) ,
        serotype (7) ,
        serogroup (8) ,
        serovar (9) ,
        cultivar (10) ,
        pathovar (11) ,
        chemovar (12) ,
        biovar (13) ,
        biotype (14) ,
        group (15) ,
        subgroup (16) ,
        isolate (17) ,
        common (18) ,
        acronym (19) ,
        dosage (20) ,           -- chromosome dosage of hybrid
        nat-host (21) ,         -- natural host of this specimen
        sub-species (22) ,
  
*   value is an optional string to give the name (eg. of the
*     varient)
******************************************/
NLM_EXTERN Boolean AddOrgModToEntry (
	NCBISubPtr submission,
	SeqEntryPtr entry ,
	Int2	 type ,
	CharPtr value);

/********************************************
*  Biomol describes the biological type of the molecule
*   current values are:
    biomol INTEGER {
        unknown (0) ,
        genomic (1) ,
        pre-RNA (2) ,              -- precursor RNA of any sort really
        mRNA (3) ,
        rRNA (4) ,
        tRNA (5) ,
        snRNA (6) ,
        scRNA (7) ,
        peptide (8) ,
        other-genetic (9) ,      -- other genetic material
        genomic-mRNA (10) , -- reported a mix of genomic and cdna sequence
        other (255) } DEFAULT unknown ,
********************************************/
NLM_EXTERN Boolean AddBiomolToEntry (
	NCBISubPtr submission,
	SeqEntryPtr entry ,
	Int2	 type );

/********************************************
*
*  What technique was used to get this sequence ?
*    There are a set of defines in objpubd.h for this:
*    Current list is:
#define MI_TECH_unknown 0
#define MI_TECH_standard 1
#define MI_TECH_est 2         EST division 
#define MI_TECH_sts 3         STS division 
#define MI_TECH_survey 4      GSS division 
#define MI_TECH_genemap 5     Bioseq is a genetic map 
#define MI_TECH_physmap 6     Bioseq is physical map 
#define MI_TECH_derived 7     Bioseq is a computed inference 
#define MI_TECH_concept_trans 8   conceptual translation 
#define MI_TECH_seq_pept 9        peptide sequencing used 
#define MI_TECH_both 10           combination of 8 and 9 used 
#define MI_TECH_seq_pept_overlap 11  peptides ordered by overlap 
#define MI_TECH_seq_pept_homol 12    peptides ordered by homology 
#define MI_TECH_concept_trans_a 13   concept trans supplied by author 
#define MI_TECH_other 255            doesnt' fit anything 
***************************************
* The following are not explicitly in the ASN.1 spec yet
* but can still be legally used as numbers.
* These are for High Throughput Genome Sequences
* htgs_1  - preliminary data. sequence is made of multiple
*             contigs with gaps between them. The order of
*             the contigs is not known, although for
*             convenience they are in an arbitrary order
* htgs_2  - preliminary data. like htgs_1 except the
*             order of the contigs is known and the sequence
*             reflects the correct order
* htgs_3  - finished data. All annotations are machine
*             generated in bulk. Usually this has been placed
*             on a map
*
******************************************
#define MI_TECH_htgs_1 14
#define MI_TECH_htgs_2 15
#define MI_TECH_htgs_3 16
**********************************************************/
NLM_EXTERN Boolean AddTechToEntry (
	NCBISubPtr submission,
	SeqEntryPtr entry ,
	Int2	 tech );

/********************************************
*  How complete is the molecule?
*   here are the allowed values:
*
    completeness INTEGER {
      unknown (0) ,
      complete (1) ,                   -- complete biological entity
      partial (2) ,                    -- partial but no details given
      no-left (3),                     -- KNOWN missing 5' or NH3 end
      no-right (4) ,                   -- KNOWN missing 3' or COOH end
      no-ends (5) ,                    -- KNOWN missing both ends
      has-left (6) ,                   -- KNOWN has complete 5' or NH3 end
      has-right (7) ,                  -- KNOWN has complete 3' or COOH end
      other (255) } DEFAULT unknown }

*******************************************/
NLM_EXTERN Boolean AddCompleteToEntry (
	NCBISubPtr submission,
	SeqEntryPtr entry ,
	Int2	 complete );

NLM_EXTERN void AddCompleteness(NCBISubPtr submission, SeqEntryPtr sep, SeqFeatPtr sfp);

/**** OBSOLETE!!!! ***********************************************
*     DO NOT USE GIBBmethod 
*     this is subsumed into AddTechToEntry, above
*
****************************************************************/

NLM_EXTERN Boolean AddGIBBmethodToEntry (
	NCBISubPtr submission,
	SeqEntryPtr entry ,
	Int2	 method );

#define METHOD_concept_transl 1
#define METHOD_seq_pept 2
#define METHOD_both 3
#define METHOD_seq_pept_overlap 4
#define METHOD_seq_pept_homol 5
#define METHOD_concept_transl_a 6
#define METHOD_other 255

NLM_EXTERN Boolean AddCreateDateToEntry (
	NCBISubPtr submission,
	SeqEntryPtr entry ,
	Int2 month ,
	Int2 day ,
	Int2 year );

/*************************************************************************
*
*   Modifiers modify the meaning of all entries in the set or sequence
*   to which they are applied. This is particularly important for
*   indicating organelle sequences, RNA genomes, or mutants.
*
*   Less obvious is indicating completness.
*
*   A genomic sequence is assumed to be partial unless the "complete"
*      modifier is used.
*   A peptide sequence is assumed to be complete unless the "partial"
*      modifier is used.
*   A cDNA is assumed to be complete (as well as one can tell) unless
*      "partial" is used.
*   
*   A genomic sequence is assumed to be nuclear unless the "mitochondrial"
*      (or other organelle) modifier is used.
*   All sequences are assumed to be natural unless "synthetic",
*      "recombinant", or "mutagen" are used.
*
*************************************************************************/

/***************************************
*  Adds a ValNode of the appropriate type
*    to the SeqEntry
*    Note that caller must still create the
*     specific descriptor structure and attach it to
*     the returned ValNode
*
****************************************/

NLM_EXTERN ValNodePtr NewDescrOnSeqEntry (SeqEntryPtr entry, Int2 type);

NLM_EXTERN ValNodePtr GetDescrOnSeqEntry (
	SeqEntryPtr entry, 
	Int2 type);

NLM_EXTERN Boolean AddModifierToEntry (
	NCBISubPtr submission,
	SeqEntryPtr entry ,
	Int2 modifier );

#define MODIF_dna  0
#define MODIF_rna  1
#define MODIF_extrachrom  2
#define MODIF_plasmid  3
#define MODIF_mitochondrial  4
#define MODIF_chloroplast  5
#define MODIF_kinetoplast  6
#define MODIF_cyanelle  7
#define MODIF_synthetic  8		/* synthetic sequence */
#define MODIF_recombinant  9	/* recombinant construct */
#define MODIF_partial  10
#define MODIF_complete  11
#define MODIF_mutagen  12 /* subject of mutagenesis ? */
#define MODIF_natmut  13  /* natural mutant ? */
#define MODIF_transposon  14
#define MODIF_insertion_seq  15
#define MODIF_no_left  16 /* missing left end (5' for na, NH2 for aa) */
#define MODIF_no_right  17   /* missing right end (3' or COOH) */
#define MODIF_macronuclear  18
#define MODIF_proviral  19
#define MODIF_est  20    /* expressed sequence tag */


	                           /*** add/build publications ***/
NLM_EXTERN Boolean AddPubToEntry (
	NCBISubPtr submission,
	SeqEntryPtr entry ,
	PubPtr pub );

NLM_EXTERN PubPtr CitSubBuild (               /* for first data submission **/
	NCBISubPtr submission,
	Int2 month,
	Int2 day,
	Int2 year,
	Int2 medium );


NLM_EXTERN PubPtr CitSubUpdateBuild (   /* for updates to existing record */
	NCBISubPtr submission,
	Int2 month,
	Int2 day,
	Int2 year ,
	Int2 medium ,
	CharPtr descr );  /* description of update, make it short */

#define MEDIUM_NOT_SET 0
#define MEDIUM_PAPER 1
#define MEDIUM_TAPE 2
#define MEDIUM_FLOPPY 3
#define MEDIUM_EMAIL 4
#define MEDIUM_OTHER 255

NLM_EXTERN PubPtr CitArtBuild (
	NCBISubPtr submission,
	CharPtr title ,
	CharPtr journal ,
	CharPtr volume ,
	CharPtr issue ,
	CharPtr pages ,
	Int2 month ,
	Int2 day ,
	Int2 year ,
	Int2 status );

#define PUB_STATUS_PUBLISHED 0
#define PUB_STATUS_SUBMITTED 1
#define PUB_STATUS_IN_PRESS  2
#define PUB_STATUS_UNPUBLISHED 3

/*************************************************************************
*
*   Author names can be given in various forms
*   	You MUST give at least a last name
*   	You should give at least first name or initials.
*       Initials are just for first and middle names, and are
*         separated by periods.
*
*   example: John Q. Public
*   last_name = "Public"
*   first_name = "John"
*   middle_name = NULL
*   initials = "J.Q."
*
*************************************************************************/


NLM_EXTERN Boolean AddAuthorToPub (    /* call once for each author, in order */
	NCBISubPtr submission,
	PubPtr the_pub,
	CharPtr last_name,
	CharPtr first_name,
	CharPtr middle_name,
	CharPtr initials,  /* separated by periods, no initial for last name */
	CharPtr suffix );  /* Jr. Sr. III */


/*************************************************************************
*
*   Author Affiliation
*      only one allowed per pub (one per author is also possible, but is
*      not supported by this interface )
*
*   affil = institutional affiliation
*   div   = division of institution
*   street = street address
*   city = city
*   sub = subdivision of country (e.g. state.. optional)
*   country = country
*   postal_code = zip code in the USA
*
*************************************************************************/


NLM_EXTERN Boolean AddAffiliationToPub (  /* call once per pub */
	NCBISubPtr submission,
	PubPtr the_pub,
	CharPtr affil,        /* e.g. "Xyz University" */
	CharPtr div,          /* e.g. "Dept of Biology" */
	CharPtr street,       /* e.g. "123 Academic Road" */
	CharPtr city,         /* e.g. "Metropolis" */
	CharPtr sub,          /* e.g. "Massachusetts" */
	CharPtr country ,     /* e.g. "USA" */
	CharPtr postal_code ); /* e.g."02133" */


/*****************************************************************************
*
*   Add Features to the entry
*   	Add location to feature
*   	Add info for specific types to feature
*
*****************************************************************************/
NLM_EXTERN SeqFeatPtr FeatureBuild (
	NCBISubPtr submission,
	SeqEntryPtr entry_to_put_feature,
	Boolean feature_is_partial,
	Uint1 evidence_is_experimental,
	Boolean biological_exception,
	CharPtr comment );

#define EVIDENCE_NOT_SET 0
#define EVIDENCE_EXPERIMENTAL 1
#define EVIDENCE_NOT_EXPERIMENTAL 2

/*************************************************************************
*
*   About feature locations:
*     Internally the NCBI software represents locations on sequence as
*	offsets from the start of the sequence (i.e. from 0 - (length -1)).
*   Also, the "from" position is always <= "to", even for locations on
*   the minus strand. Finally, no location can cross the origin of a
*   circular sequence.. it must be split in two. This makes routines
*   that access locations very consistent and easy to write.
*
*     However, most biologists number sequences starting with 1, not 0.
*   It is natural to think of a coding region on the minus strand going
*   from 5243 to 2993. And it is not unusual to think of the origin of
*   replication being from 5344 to 10 on the plus strand of a circular
*   sequence.
*
*     AddIntervalToFeature and AddPointToFeature were written to support
*   the biological notion. They convert to the internal format
*   automatically. So, for these two functions:
*
*	1) numbers are in the range 1 - length
*   2) from <= to on plus strand
*      to <= from on minus strand
*   3) numbers not conforming to (2) are assumed to go around the origin
*      of a circular sequence. It is an error on a linear sequence.
*   4) Intervals should be added in biological order (e.g. exon1, exon2,
*      exon3...) no matter which strand the feature is on.
*   5) You must always indicate explicitly the Bioseq the interval is
*      on. You may either pass in the SeqEntryPtr or the local_name you
*      used when you created the sequence. The sequence must have
*      been previously created with AddSeqTo...	 If you give both the
*      SeqEntryPtr and the local_name, they must agree.
*   6) -1 (minus one) is a short hand for "end of sequence". To indicate
*      the whole sequence you can give from = 1, to = -1
*
*************************************************************************/

NLM_EXTERN Boolean AddIntervalToFeature (
	NCBISubPtr submission,
	SeqFeatPtr sfp,
	SeqEntryPtr the_seq ,
	CharPtr local_name ,
	Int4 from ,
	Int4 to ,
	Boolean on_plus_strand ,
	Boolean start_before_from ,
	Boolean stop_after_to );

NLM_EXTERN Boolean AddIntToSeqFeat (
	SeqFeatPtr sfp,
	Int4 from,
	Int4 to,
	BioseqPtr bsp,
	Int2 fuzz_from,
	Int2 fuzz_to,
	Int2 strand);

NLM_EXTERN Boolean AddPointToFeature (
	NCBISubPtr submission,
	SeqFeatPtr sfp,
	SeqEntryPtr the_seq ,
	CharPtr local_name ,
	Int4 location ,
	Boolean on_plus_strand ,
	Boolean is_after_location ,
	Boolean is_before_location );

/*************************************************************************
*
*   Having made a generalized feature, now add type specific info to it.
*
*************************************************************************/

/************************************************************
*
*  A comment is the simplest feature. It is required that you
*  supplied a "comment" argument to FeatureBuild. In GenBank format
*  it will appear as misc_feat, with the comment appearing as the
*  \note.
***************************************************************/

NLM_EXTERN Boolean MakeCommentFeature (
	NCBISubPtr submission,
	SeqFeatPtr feature );

/*****************************************************************
*
*   This connects a protein sequence with the nucleic acid
*   region which codes for it. So the protein is given as an
*   argument, as well as adding intervals on the nucleic acid.
*   A complete coding region includes the initial Met codon and
*   the final termination codon.
*
*****************************************************************/


NLM_EXTERN Boolean MakeCdRegionFeature (
	NCBISubPtr submission,
	SeqFeatPtr feature,
	Int2 frame ,
	Int2 genetic_code ,	   /* see end of this file for genetic codes */
	SeqEntryPtr protein_product,	/* give id of protein. if NULL, call */
	CharPtr local_id_for_protein); /* function below to create by transl */

           
/******************************************************************
*
*   A Code-break allows an exception to be made in the translation
*    of a particular codon. You must give positions of the first
*    and last bases of the codon in the DNA sequence and the amino
*    acid to place there, instead of the normal translation. This
*    should be used sparingly, and a comment on the feature should
*    explain why it was done.
*
*   The location is specified the same as in AddIntervalToFeature.
*   AA_for_protein points the amino acid to use, in ncbieaa code.
*
******************************************************************/

NLM_EXTERN Boolean AddCodeBreakToCdRegion (
	NCBISubPtr submission,
	SeqFeatPtr sfp,
	SeqEntryPtr the_seq ,
	CharPtr local_name ,
	Int4 from ,
	Int4 to ,
	Boolean on_plus_strand ,
	CharPtr AA_for_protein );

           
/******************************************************************
*
*   Special function to make protein from CdRegion feature
*
******************************************************************/

NLM_EXTERN SeqEntryPtr TranslateCdRegion (
	NCBISubPtr submission ,
	SeqFeatPtr cdregion_feature ,
	SeqEntryPtr nuc_prot_entry_to_put_sequence ,
	CharPtr local_name ,             /* for protein sequence */
	CharPtr genbank_locus ,
	CharPtr genbank_accession ,
	Int4 gi_number );

NLM_EXTERN Boolean MakeRNAFeature (
	NCBISubPtr submission,
	SeqFeatPtr feature,
	Int2 rna_type ,
	Boolean is_pseudo_gene,
	CharPtr rna_name ,
	CharPtr AA_for_tRNA ,
	CharPtr codon_for_tRNA );

#define RNA_TYPE_premsg 1
#define RNA_TYPE_mRNA   2
#define RNA_TYPE_tRNA   3
#define RNA_TYPE_rRNA   4
#define RNA_TYPE_snRNA  5
#define RNA_TYPE_scRNA  6
#define RNA_TYPE_other  255
           
/******************************************************************
*
*  Once you have made a tRNA feature, you may optionally add the
*   the location of the anticodon if you know it. This should be
*   within the range of the tRNA feature already created, obviously.
*
*   the location is specified on the DNA the same as for
*     AddIntervalToFeature
*
******************************************************************/

NLM_EXTERN Boolean AddAntiCodonTotRNA (
	NCBISubPtr submission,
	SeqFeatPtr sfp,
	SeqEntryPtr the_seq ,
	CharPtr local_name ,
	Int4 from ,
	Int4 to ,
	Boolean on_plus_strand );

NLM_EXTERN Boolean MakeGeneFeature (
	NCBISubPtr submission,
	SeqFeatPtr feature,
	CharPtr gene_symbol_for_locus ,
	CharPtr allele ,
	CharPtr descriptive_name ,
	CharPtr map_location ,
	Boolean is_pseudo_gene ,
	CharPtr genetic_database ,
	CharPtr gene_id_in_genetic_database ,
	CharPtr synonym1 ,
	CharPtr synonym2 ,
	CharPtr synonym3 );

NLM_EXTERN Boolean MakeProteinFeature (
	NCBISubPtr submission,
	SeqFeatPtr feature ,
	CharPtr protein_name1,
	CharPtr protein_name2,
	CharPtr protein_name3,
	CharPtr descriptive_name,
	CharPtr ECnum1,
	CharPtr ECnum2,
	CharPtr activity1,
	CharPtr activity2,
	CharPtr protein_database,
	CharPtr id_in_protein_database);

NLM_EXTERN Boolean MakeRegionFeature (
	NCBISubPtr submission,
	SeqFeatPtr feature ,
	CharPtr region_name );

NLM_EXTERN Boolean MakeSiteFeature (
	NCBISubPtr submission,
	SeqFeatPtr feature ,
	Int2 site_type );

NLM_EXTERN Boolean MakeImpFeature (
	NCBISubPtr submission,
	SeqFeatPtr feature ,
	CharPtr key );

NLM_EXTERN Boolean AddQualToImpFeature (
	NCBISubPtr submission,
	SeqFeatPtr imp_feature ,
	CharPtr qualifier ,
	CharPtr value );

NLM_EXTERN Boolean MakePubFeature (
	NCBISubPtr submission,
	SeqFeatPtr feature,
	PubPtr pub );

NLM_EXTERN Boolean AddBasesToByteStore (ByteStorePtr bsp, CharPtr the_bases);

NLM_EXTERN Boolean AddAAsToByteStore (ByteStorePtr bsp, CharPtr the_aas);

/*****************************************************************************
*
*   AddPhrapGraph (submission, the_seq, local_name, phrap_values)
*   	Converts phrap byte array to SeqGraph, wraps in SeqAnnot, adds to Bioseq.
*       The length of data in the array must be equal to the length of the Bioseq.
*
*****************************************************************************/

NLM_EXTERN Boolean AddPhrapGraph (
	NCBISubPtr submission,
	SeqEntryPtr the_seq ,
	CharPtr local_name ,
	BytePtr phrap_values );

NLM_EXTERN Boolean AddPhrapGraphToSeqLit (
	NCBISubPtr submission,
	SeqLitPtr slp ,
	BytePtr phrap_values );

/* internal functions for reference gene project */
NLM_EXTERN UserObjectPtr CreateRefGeneTrackUserObject (void);
NLM_EXTERN void AddStatusToRefGeneTrackUserObject (UserObjectPtr uop, CharPtr status);
NLM_EXTERN void AddCuratorToRefGeneTrackUserObject (UserObjectPtr uop, CharPtr collaborator);
NLM_EXTERN void AddAccessionToRefGeneTrackUserObject (UserObjectPtr uop, CharPtr field,
                                                      CharPtr accn, Int4 gi,
                                                      Boolean sequenceChange,
                                                      Boolean annotationChange,
                                                      CharPtr comment);

/* experimental function to associate mRNA with protein product in cases of alt splicing */
NLM_EXTERN UserObjectPtr CreateMrnaProteinLinkUserObject (BioseqPtr protbsp);

/* vector screen, validator count, general submission comment user object (JP) */
NLM_EXTERN UserObjectPtr CreateSubmissionUserObject (CharPtr univecComment,
                                                     CharPtr additionalComment,
                                                     Int4 validatorErrorCount,
                                                     Int4 validatorHashCode,
                                                     Boolean isCloningVector);

/* clone name and ID for genomic contig RefSeq records */
NLM_EXTERN UserObjectPtr CreateContigCloneUserObject (CharPtr name, Int4 ID);

/* gene ontology process, component, and function user object */
NLM_EXTERN UserObjectPtr CreateGeneOntologyUserObject (
  void
);
NLM_EXTERN void AddToGeneOntologyUserObject (
  UserObjectPtr uop,
  CharPtr type,
  CharPtr text,
  Int4 goid,
  Int4 pmid,
  CharPtr evidence
);

/* model evidence user object */
NLM_EXTERN UserObjectPtr CreateModelEvidenceUserObject (
  CharPtr method,
  CharPtr contigParent
);
NLM_EXTERN void AddMrnaOrESTtoModelEvidence (
  UserObjectPtr uop,
  CharPtr type,
  CharPtr accn,
  Int4 length,
  Int4 gaplen
);
NLM_EXTERN UserFieldPtr FindModelEvidenceField (
  UserObjectPtr uop,
  CharPtr type
);

/* third party accession list user object manipulation */
NLM_EXTERN UserObjectPtr CreateTpaAssemblyUserObject (
  void
);
NLM_EXTERN void AddAccessionToTpaAssemblyUserObject (
  UserObjectPtr uop,
  CharPtr accn
);


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


/*****************************************************************************
*
*   Allowed IUPAC nucleic acid codes from /ncbi/data/seqcode.prt
*   
                ( symbol "A", name "Adenine" ),
                ( symbol "B" , name "G or T or C" ),
                ( symbol "C", name "Cytosine" ),
                ( symbol "D", name "G or A or T" ),
                ( symbol "G", name "Guanine" ),
                ( symbol "H", name "A or C or T" ) ,
                ( symbol "K", name "G or T" ),
                ( symbol "M", name "A or C" ),
                ( symbol "N", name "A or G or C or T" ) ,
                ( symbol "R", name "G or A"),
                ( symbol "S", name "G or C"),
                ( symbol "T", name "Thymine"),
                ( symbol "V", name "G or C or A"),
                ( symbol "W", name "A or T" ),
                ( symbol "Y", name "T or C")
*   
*
*****************************************************************************/

/*****************************************************************************
*
*   Allowed IUPAC amino acid codes from /ncbi/data/seqcode.prt

                ( symbol "A", name "Alanine" ),
                ( symbol "B" , name "Asp or Asn" ),
                ( symbol "C", name "Cysteine" ),
                ( symbol "D", name "Aspartic Acid" ),
                ( symbol "E", name "Glutamic Acid" ),
                ( symbol "F", name "Phenylalanine" ),
                ( symbol "G", name "Glycine" ),
                ( symbol "H", name "Histidine" ) ,
                ( symbol "I", name "Isoleucine" ),
                ( symbol "K", name "Lysine" ),
                ( symbol "L", name "Leucine" ),
                ( symbol "M", name "Methionine" ),
                ( symbol "N", name "Asparagine" ) ,
                ( symbol "P", name "Proline" ),
                ( symbol "Q", name "Glutamine"),
                ( symbol "R", name "Arginine"),
                ( symbol "S", name "Serine"),
                ( symbol "T", name "Threoine"),
                ( symbol "V", name "Valine"),
                ( symbol "W", name "Tryptophan" ),
                ( symbol "X", name "Undetermined or atypical"),
                ( symbol "Y", name "Tyrosine"),
                ( symbol "Z", name "Glu or Gln" )
*   
*
*****************************************************************************/

/*****************************************************************************
*
*   Genetic Code id's and names from /ncbi/data/gc.prt
*      gc.prt lists the legal start codons and genetic codes fully
*   
		name "Standard" ,
		id 1 ,

		name "Vertebrate Mitochondrial" ,
		id 2 ,

		name "Yeast Mitochondrial" ,
		id 3 ,

		name "Mold Mitochondrial and Mycoplasma" ,
		id 4 ,

		name "Invertebrate Mitochondrial" ,
		id 5 ,

		name "Ciliate Macronuclear and Daycladacean" ,
		id 6 ,

		name "Protozoan Mitochondrial (and Kinetoplast)" ,
		id 7 ,

		name "Plant Mitochondrial" ,
		id 8 ,

		name "Echinoderm Mitochondrial" ,
		id 9 ,

		name "Euplotid Macronuclear" ,
		id 10 ,

		name "Eubacterial" ,
		id 11 ,

*   
*
*****************************************************************************/




