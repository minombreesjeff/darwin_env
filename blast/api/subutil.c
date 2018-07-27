/*  subutil.c
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
* File Name:  subutil.c
*
* Author:  James Ostell
*   
* Version Creation Date: 11/3/93
*
* $Revision: 6.45 $
*
* File Description: Utilities for creating ASN.1 submissions
*
* Modifications:  
* --------------------------------------------------------------------------
* Date	   Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* $Log: subutil.c,v $
* Revision 6.45  2001/12/11 16:50:41  kans
* added TpaAssembly user object creation functions
*
* Revision 6.44  2001/11/09 19:51:42  kans
* fix in FindModelEvidenceField
*
* Revision 6.43  2001/11/09 15:12:21  kans
* moved collaborator to separate function per user object
*
* Revision 6.42  2001/11/09 13:21:08  kans
* added collaborator field to reftrack user object
*
* Revision 6.41  2001/11/09 12:54:47  kans
* added model evidence user object functions
*
* Revision 6.40  2001/10/12 21:55:20  kans
* convert nucleotide X to N
*
* Revision 6.39  2001/07/30 19:54:14  kans
* forgot to set dbt->db to taxon
*
* Revision 6.38  2001/07/29 16:23:58  kans
* added AddOrganismToEntryEx to allow entry of taxonID
*
* Revision 6.37  2001/02/14 23:58:42  kans
* handle BioseqseqSet_class_eco_set
*
* Revision 6.36  2001/01/25 19:55:13  kans
* added AddGenBankSetToSubmission for patents and other bulk submissions of unrelated sequences
*
* Revision 6.35  2000/11/15 23:17:59  kans
* gene ontology user object functions
*
* Revision 6.34  2000/09/25 23:19:52  kans
* mrna prot link user object now uses fasta long on the single best id, preferring refseq
*
* Revision 6.33  2000/09/20 12:32:53  kans
* removed AddMethodToRefGeneTrackUserObject and AddSupportToRefGeneTrackUserObject
*
* Revision 6.32  2000/08/29 22:27:01  kans
* reftrack support now has organism field
*
* Revision 6.31  2000/07/21 11:08:00  kans
* place method after status and above accession or support user object fields
*
* Revision 6.30  2000/07/21 10:54:13  kans
* added functions for adding Method and Support to RefGeneTrack user object
*
* Revision 6.29  2000/03/14 13:33:32  kans
* NCBISubValidate sets indexing, adds AppProperty to shut off specific messages to be decided later
*
* Revision 6.28  2000/03/10 18:35:45  kans
* added CreateContigCloneUserObject for genome contig RefSeq records
*
* Revision 6.27  2000/02/28 19:02:57  kans
* AddSecondaryAccnToEntry does not need submission parameter set
*
* Revision 6.26  2000/02/28 18:46:12  kans
* fixes for add phrap to seqlit
*
* Revision 6.25  2000/02/26 23:55:33  kans
* added AddPhrapGraphToSeqLit, using internal extended SeqLit to track, readjust graphs to current position of seqlit in delta seq - may need to move fields to public SeqLit if problems arise
*
* Revision 6.24  2000/02/24 23:15:00  kans
* added AddSecondaryAccnToEntry
*
* Revision 6.23  2000/02/23 19:25:53  kans
* AddFakeGapToDeltaSeq returns SeqLitPtr so program can set fuzz after SpreadGapsInDeltaSeq is no longer being called
*
* Revision 6.22  2000/02/23 18:28:38  kans
* added AddFakeGapToDeltaSeq to set fuzz even if nonzero length is passed in
*
* Revision 6.21  2000/02/07 16:48:07  ostell
* added AddDeltaSeqToNucProtEntry()
*
* Revision 6.20  1999/12/23 17:26:29  kans
* free old reftrack status string if replacing
*
* Revision 6.19  1999/12/20 20:35:12  kans
* fixed problem with add refseq status field
*
* Revision 6.18  1999/12/20 19:57:44  kans
* added AddStatusToRefGeneTrackUserObject
*
* Revision 6.17  1999/12/08 20:00:14  kans
* AddOrgModToEntry calls OrgNameNew if necessary
*
* Revision 6.16  1999/10/05 17:24:54  kans
* SeqDescrNew and SeqDescrAdd for new descriptors
*
* Revision 6.15  1999/08/05 20:43:44  kans
* added CreateSubmissionUserObject (JP)
*
* Revision 6.14  1998/12/09 20:38:25  kans
* changed compl to compr to avoid new c++ symbol collision
*
* Revision 6.13  1998/10/02 17:48:11  kans
* new parameters to AddAccessionToRefGeneTrackUserObject, and added CreateMrnaProteinLinkUserObject
*
* Revision 6.12  1998/09/23 16:41:48  kans
* removed from and to from AddAccessionToRefGeneTrackUserObject
*
* Revision 6.11  1998/08/20 20:36:58  kans
* add comment parameter to reftrack user object function
*
* Revision 6.10  1998/08/11 22:27:25  kans
* added CreateRefGeneTrackUserObject and AddAccessionToRefGeneTrackUserObject
*
* Revision 6.9  1998/05/27 20:42:05  kans
* default hup is one year from the end of current month
*
* Revision 6.8  1998/03/02 17:19:15  kans
* changed SixMonthsFromToday to DateAdvance
*
* Revision 6.7  1998/03/02 17:13:46  shavirin
* ... and in the case when hold_until_publish == TRUE
*
* Revision 6.6  1998/03/02 17:08:39  shavirin
* Added SixMonthsFromToday(NULL); in NCBISubCreate()
*
* Revision 6.5  1998/02/05 19:19:25  kans
* added AddPhrapGraph
*
* Revision 6.4  1998/01/09 20:16:47  shavirin
* Removed creation of Imprint in CitArtBuild () and CitSubUpdateBuild()
*
* Revision 6.3  1998/01/06 03:29:46  ostell
* added AddGeneticCodetoEntry()
*
* Revision 6.2  1997/12/15 17:31:43  kans
* backed out accidental checkin
*
* Revision 6.0  1997/08/25 18:07:35  madden
* Revision changed to 6.0
*
* Revision 5.19  1997/08/19 17:15:11  kans
* included utilpars.h
*
* Revision 5.18  1997/08/19 16:51:52  tatiana
* added ValidAminoAcid() to MakeRNAFeature
*
* Revision 5.17  1997/07/11 18:23:24  kans
* added Add[Pop/Phy/Mut]SetToSubmission
*
* Revision 5.16  1997/06/19 18:39:09  vakatov
* [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
*
* Revision 5.15  1997/05/16 19:13:57  ostell
* moved prototype of NewDescrOnSeqEntry from subutil.c to subutil.h to make
* it external.
*
 * Revision 5.14  1996/09/05  20:20:28  ostell
 * added CitSubUpdateBuild()
 *
 * Revision 5.13  1996/09/04  22:33:06  ostell
 * added MakeCommentFeature()
 *
 * Revision 5.12  1996/08/30  13:17:01  ostell
 * made accession check allow 2+6 form
 *
 * Revision 5.11  1996/08/20  15:32:10  tatiana
 * static removed from AddBasesToByteStore and AddAAsToByteStore
 *
 * Revision 5.10  1996/08/15  20:29:19  ostell
 * in MaketRNAFeature() added support for 'U' in codon and for 'R'and 'Y' as
 * wobble bases
 *
 * Revision 5.9  1996/07/19  22:13:13  ostell
 * added SpreadGapsInDeltaSeq()
 *
 * Revision 5.8  1996/06/18  01:21:56  ostell
 * removed the step of adding exp-ev qualifier as well as etting flag in feat
 *
 * Revision 5.7  1996/06/14  19:01:54  ostell
 * minor fixes
 *
 * Revision 5.6  1996/06/14  19:00:06  ostell
 * fixed line wraps
 *
 * Revision 5.5  1996/06/14  18:07:38  ostell
 * added AddOrgModToEntry() and AddSubSourceToEntry()
 *
 * Revision 5.4  1996/06/14  17:14:55  ostell
 * fixed make of Dbtag
 *
 * Revision 5.3  1996/06/14  14:11:46  kans
 * fixed proper uninitialized seqlit variable (JO)
 *
 * Revision 5.2  1996/06/14  00:13:35  ostell
 * fixed unitnitialized variable
 *
 * Revision 5.1  1996/06/13  03:34:53  ostell
 * added support for delta seqs and submittor_key
 * fixed memory leak in AddAminoAcidsToBioseq()
 *
 * Revision 5.0  1996/05/28  13:23:23  ostell
 * Set to revision 5.0
 *
 * Revision 4.9  1996/03/12  13:48:41  ostell
 * added a cast
 *
 * Revision 4.8  1996/03/05  17:05:25  tatiana
 * change in NCBISubNewBioseq, moltype goes to MolInfo
 * ,
 *
 * Revision 4.7  1996/02/01  18:12:42  ostell
 * added curley braces to make gcc happy
 *
 * Revision 4.6  1996/01/31  02:35:00  ostell
 * added AddCodeBreakToCdRegion() and AddAntiCodonTotRNA()
 *
 * Revision 4.5  1995/10/05  14:48:29  tatiana
 * error msg added "Illegal character in Bioseq"
 *
 * Revision 4.4  1995/09/28  17:30:41  kans
 * added static to GetDNAConv and GetProteinConv
 *
 * Revision 4.3  1995/09/28  16:28:54  tatiana
 * a bug fixed in AddBasesToBioseq()
 *
 * Revision 4.2  1995/09/27  19:50:39  tatiana
 * check for illegal residues added
 *
 * Revision 4.1  1995/08/31  20:49:57  tatiana
 * strain length is set to 70 not to exeed the buffer length
 *
 * Revision 4.0  1995/07/26  13:49:01  ostell
 * force revision to 4.0
 *
 * Revision 1.20  1995/07/25  14:47:09  tatiana
 * remove static
 *
 * Revision 1.19  1995/07/20  19:37:31  tatiana
 * change SeqIdprint to SeqIdWrite
 *
 * Revision 1.18  1995/07/20  17:48:27  kans
 * removed static on AddModifierToEntry and AddPubToEntry
 *
 * Revision 1.17  1995/07/14  18:33:34  kans
 * added statics to functions, removed unused variables
 *
 * Revision 1.16  1995/07/14  18:08:43  tatiana
 * BioSource added
 *
 * Revision 1.11  1995/05/15  21:46:05  ostell
 * added Log line
 *
*
*
* ==========================================================================
*/

/** for ErrPostEx() ****/

static char *this_module = "ncbiapi";
#define THIS_MODULE this_module
static char *this_file = __FILE__;
#define THIS_FILE this_file

/**********************/

#include <subutil.h>
#include <valid.h>
#include <seqport.h>
#include <utilpars.h>
#include <sqnutils.h>

/*****************************************************************************
*
*   prototypes for internal functions
*
*****************************************************************************/
static BioseqPtr NCBISubNewBioseq (
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

static Boolean StripNonAsciiChars (CharPtr str);
static BioseqPtr GetBioseqFromChoice (NCBISubPtr nsp, SeqEntryPtr the_seq,
                                      CharPtr local_name, CharPtr the_routine);
static Boolean AddFeatureToEntry (
	NCBISubPtr submission,
	SeqEntryPtr entry ,
	SeqFeatPtr feature );


/*****************************************************************************
*
*   DefaultSubErrorFunc(msg)
*   	default error handler
*
*****************************************************************************/
NLM_EXTERN Boolean DefaultSubErrorFunc (CharPtr msg)
{
	Message(MSG_ERROR, msg);
	return TRUE;
}

/*****************************************************************************
*
*   Create/Free the SubmitBlock
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
	Int2 release_year )
{
	NCBISubPtr nsp;
	SeqSubmitPtr ssp;
	SubmitBlockPtr sbp;
	ContactInfoPtr cip;
	DatePtr dp;
	AuthorPtr aup;
	PersonIdPtr pip;
	AffilPtr ap;
	NameStdPtr nmstdp;

	ErrSetOpts(ERR_ADVISE, ERR_LOG_OFF);

	if (! SeqEntryLoad())     /* can't find ncbi files */
	{
		Message(MSG_ERROR, "Can't load NCBI data files");
		return NULL;
	}

	nsp = MemNew(sizeof(NCBISub));

	ssp = SeqSubmitNew();
	nsp->ssp = ssp;
	ssp->datatype = 1;    /* seqentrys */

	sbp = SubmitBlockNew();
	ssp->sub = sbp;
	sbp->hup = hold_until_publish;

	if (release_year != 0) {
            dp = DateNew();
            if (! DateWrite(dp, release_year, release_month, 
                            release_day, NULL)) {
                ErrShow();
                DateFree(dp);
            } else {
                sbp->reldate = dp;
            }
	} else if(hold_until_publish) {
			dp = DateCurr();
			if (dp != NULL) {
				dp->data [3] = 0; /* force to end of month */
				sbp->reldate = DateAdvance(dp, 12); /* one year from now */
            }
        }

	cip = ContactInfoNew();
	sbp->contact = cip;

	nmstdp = NameStdNew();
	nmstdp->names[0] = StringSaveNoNull(last_name);
	nmstdp->names[1] = StringSaveNoNull(first_name);
	nmstdp->names[2] = StringSaveNoNull(middle_name);
	nmstdp->names[4] = StringSaveNoNull(initials);
	nmstdp->names[5] = StringSaveNoNull(suffix);

	pip = PersonIdNew();
	pip->choice = 2;   /* name */
	pip->data = (Pointer)nmstdp;

	aup = AuthorNew();
	aup->name = pip;

	cip->contact = aup;

	ap = AffilNew();
    aup->affil = ap;
	ap->choice = 2;    /* std affil */
	ap->affil = StringSaveNoNull(affil);
	ap->div = StringSaveNoNull(div);
	ap->city = StringSaveNoNull(city);
	ap->sub = StringSaveNoNull(sub);
	ap->country = StringSaveNoNull(country);
	ap->street = StringSaveNoNull(street);
	ap->postal_code = StringSaveNoNull(postal_code);
	ap->phone = StringSaveNoNull(phone);
	ap->fax = StringSaveNoNull(fax);
	ap->email = StringSaveNoNull(email);

	return nsp;
}

/***********************************************************************
*
*   submittor key is used for the Dbtag.db field, if not NULL.
*      local_name of the sequence will be type general in this case, not
*      type local
*
************************************************************************/

NLM_EXTERN Boolean DefineSubmittorKey(
	NCBISubPtr nsp,
	CharPtr submittor_key )
{
	if (nsp == NULL)
		return FALSE;
	if (nsp->submittor_key != NULL)
		return FALSE;
	nsp->submittor_key = StringSaveNoNull(submittor_key);
	return TRUE;
}

/**** This function is obsolete.. replaced by NCBISubCreate() ****/

NLM_EXTERN NCBISubPtr NCBISubBuild (
	CharPtr name,
	CharPtr PNTR address ,
	CharPtr phone ,
	CharPtr fax ,
	CharPtr email,
	Boolean hold_until_publish ,
	Int2 release_month,			  /* all 0 if no hold date */
	Int2 release_day,
	Int2 release_year )
{
	NCBISubPtr nsp;
	SeqSubmitPtr ssp;
	SubmitBlockPtr sbp;
	ContactInfoPtr cip;
	DatePtr dp;
	ValNodePtr vnp;

	ErrSetOpts(ERR_ADVISE, ERR_LOG_OFF);

	if (! SeqEntryLoad())     /* can't find ncbi files */
	{
		Message(MSG_ERROR, "Can't load NCBI data files");
		return NULL;
	}

	nsp = MemNew(sizeof(NCBISub));

	ssp = SeqSubmitNew();
	nsp->ssp = ssp;
	ssp->datatype = 1;    /* seqentrys */

	sbp = SubmitBlockNew();
	ssp->sub = sbp;
	sbp->hup = hold_until_publish;

	if (release_year != 0)
	{
		dp = DateNew();
		if (! DateWrite(dp, release_year, release_month, release_day, NULL))
		{
			ErrShow();
			DateFree(dp);
		}
		else
			sbp->reldate = dp;
	}

	cip = ContactInfoNew();
	sbp->contact = cip;

	cip->name = StringSaveNoNull(name);
	while (* address != NULL)
	{
		vnp = ValNodeNew(cip->address);
		if (cip->address == NULL)
			cip->address = vnp;
		vnp->data.ptrvalue = (Pointer)(StringSaveNoNull(*address));
		address++;
	}

	cip->phone = StringSaveNoNull(phone);
	cip->fax = StringSaveNoNull(fax);
	cip->email = StringSaveNoNull(email);

	return nsp;
}

/*****************************************************************************
*
*   ADD tool to the Submission
*
*****************************************************************************/
NLM_EXTERN Boolean AddToolToSub (
	NCBISubPtr nsp,
	CharPtr tool )
{
	SeqSubmitPtr ssp;
	SubmitBlockPtr sbp;

	if (nsp == NULL) {
		return FALSE;
	}
	ssp = nsp->ssp;
	if (ssp == NULL) {
		return FALSE;
	}
	sbp = ssp->sub;
	sbp->tool = Nlm_StringSave(tool);

	return TRUE;
}

/*****************************************************************************
*
*   ADD comments to the Submission
*
*****************************************************************************/
NLM_EXTERN Boolean AddCommentToSub (
	NCBISubPtr nsp,
	CharPtr comment )
{
	SeqSubmitPtr ssp;
	SubmitBlockPtr sbp;

	if (nsp == NULL) {
		return FALSE;
	}
	ssp = nsp->ssp;
	if (ssp == NULL) {
		return FALSE;
	}
	sbp = ssp->sub;
	sbp->comment = Nlm_StringSave(comment);

	return TRUE;
}
/*****************************************************************************
*
*   ADD submission type to the Submission
*
*****************************************************************************/
NLM_EXTERN Boolean AddTypeToSub (
	NCBISubPtr nsp,
	Uint1 type )
{
	SeqSubmitPtr ssp;
	SubmitBlockPtr sbp;

	if (nsp == NULL) {
		return FALSE;
	}
	ssp = nsp->ssp;
	if (ssp == NULL) {
		return FALSE;
	}
	sbp = ssp->sub;
	sbp->subtype = type;

	return TRUE;
}

/*****************************************************************************
*
*   Write the Submission to a disk file
*
*****************************************************************************/
NLM_EXTERN Boolean NCBISubWrite (
	NCBISubPtr ssp,
	CharPtr filename )
{
	AsnIoPtr aip;
	Boolean retval = FALSE;

	aip = AsnIoOpen(filename, "w");
	if (aip == NULL)
	{
		ErrShow();
		return retval;
	}

	if (! SeqSubmitAsnWrite(ssp->ssp, aip, NULL))
	{
		ErrShow();
	}
	else
		retval = TRUE;
	AsnIoClose(aip);
	return retval;
}

/*****************************************************************************
*
*   Free the NCBISub and associated data
*
*****************************************************************************/
NLM_EXTERN NCBISubPtr NCBISubFree (
	NCBISubPtr ssp )
{
	SeqSubmitFree(ssp->ssp);
	MemFree(ssp->submittor_key);
	return (NCBISubPtr)(MemFree(ssp));
}

/*****************************************************************************
*
*   add the Cit-sub to the submission
*
*****************************************************************************/
NLM_EXTERN Boolean CitSubForSubmission (
	NCBISubPtr submission,
	PubPtr cit_sub )
{
	if ((submission == NULL) || (cit_sub == NULL)) return FALSE;

	if ((cit_sub->choice != PUB_Sub) || (cit_sub->data.ptrvalue == NULL))
	{
		Message(MSG_ERROR, "You must give a Cit-sub to CitSubForSubmission()");
		return FALSE;
	}
	submission->ssp->sub->cit = (CitSubPtr)(cit_sub->data.ptrvalue);
	ValNodeFree(cit_sub);   /* free just the Pub part */
	return TRUE;
}

/*****************************************************************************
*
*   Add Entries to the Submission
*   Add Sequences to the Entries
*
*****************************************************************************/

/*****************************************************************************
*
*   utility function for making all new Bioseqs here
*
*****************************************************************************/
static BioseqPtr NCBISubNewBioseq (
	NCBISubPtr submission ,
	CharPtr local_name ,
	CharPtr genbank_locus ,
	CharPtr genbank_accession ,
	Int4 gi_number ,
	Int2 molecule_class,
	Int2 molecule_type ,
	Int4 length ,
	Int2 topology ,
	Int2 strandedness )
{
	BioseqPtr bsp;
	TextSeqIdPtr tsip;
	ValNodePtr vnp;
	ObjectIdPtr oip;
	DbtagPtr dbt;
	Int2 i;
	CharPtr tmp;
	Boolean fail;
	MolInfoPtr mip;

	bsp = BioseqNew();
	bsp->mol = (Uint1) molecule_class;
	bsp->repr = Seq_repr_raw;   /* default case */
/*	if (molecule_type != 0)
	{
		vnp = SeqDescrNew(NULL);
		bsp->descr = vnp;
		vnp->choice = Seq_descr_mol_type;
		vnp->data.intvalue = molecule_type;
	}
*/
	if (molecule_type != 0)
	{
		vnp = SeqDescrNew(NULL);
		bsp->descr = vnp;
		vnp->choice = Seq_descr_molinfo;
		mip = (MolInfoPtr) vnp->data.ptrvalue;
		if (mip == NULL) 
			mip = MolInfoNew();
		mip->biomol = (Uint1)molecule_type;
		vnp->data.ptrvalue =  (Pointer) mip;
	}
	bsp->length = length;
	bsp->topology = (Uint1)topology;
	bsp->strand = (Uint1)strandedness;

	vnp = NULL;

	if (local_name != NULL)
	{
		vnp = ValNodeNew(bsp->id);
		if (bsp->id == NULL)
			bsp->id = vnp;
		oip = ObjectIdNew();
		oip->str = StringSaveNoNull(local_name);
		if (submission->submittor_key == NULL)   /* make a local id */
		{
			vnp->choice = SEQID_LOCAL;
			vnp->data.ptrvalue = oip;
		}
		else
		{
			vnp->choice = SEQID_GENERAL;
			dbt = DbtagNew();
			vnp->data.ptrvalue = dbt;
			dbt->db = StringSave(submission->submittor_key);
			dbt->tag = oip;
		}
	}

	if (genbank_locus != NULL)
	{
		fail = FALSE;
		if (! IS_UPPER(*genbank_locus))
			fail = TRUE;

		i = 0;
		for (tmp = genbank_locus; *tmp != '\0'; tmp++, i++)
		{
			if (! ((IS_UPPER(*tmp)) || (IS_DIGIT(*tmp))))
			{
				fail = TRUE;
				break;
			}
		}
		if ((i < 1) || (i > 10))
			fail = TRUE;
		if (fail)
		{
			Message(MSG_ERROR,
				"A GenBank LOCUS is an upper case letter, then up to 9 upper case letters or digits [%s]",
				genbank_locus);
			genbank_locus = NULL;
		}
	}
	if (genbank_accession != NULL)
	{
		fail = FALSE;
		if (! IS_UPPER(*genbank_accession))
			fail = TRUE;
		tmp = genbank_accession + 1;
		for (i = 0; i < 5; i++, tmp++)
		{
			if (! IS_DIGIT(*tmp))
				fail = TRUE;
		}
		if (*tmp != '\0')
			fail = TRUE;
		if (fail)   /* not 1 + 5, could be 2 + 6 */
		{
			fail = FALSE;
			tmp = genbank_accession;
			for (i = 0; i < 2; i++, tmp++)
			{
				if (! IS_UPPER(*genbank_accession))
					fail = TRUE;
			}

			for (i = 0; i < 6; i++, tmp++)
			{
				if (! IS_DIGIT(*tmp))
					fail = TRUE;
			}
			if (*tmp != '\0')
				fail = TRUE;
		}

		if (fail)
		{
			Message(MSG_ERROR, "A GenBank accession is either an\
upper case letter + 5 digits, or 2 upper case letters + 6 digits [%s]",
				genbank_accession);
			genbank_accession = NULL;
		}
	}

	if ((genbank_locus != NULL) || (genbank_accession != NULL))
	{
		vnp = ValNodeNew(bsp->id);
		if (bsp->id == NULL)
			bsp->id = vnp;
		vnp->choice = SEQID_GENBANK;
		tsip = TextSeqIdNew();
		vnp->data.ptrvalue = tsip;
		if (genbank_locus != NULL)
			tsip->name = StringSaveNoNull(genbank_locus);
		if (genbank_accession != NULL)
			tsip->accession = StringSaveNoNull(genbank_accession);
	}

	if (gi_number > 0)
	{
		vnp = ValNodeNew(bsp->id);
		if (bsp->id == NULL)
			bsp->id = vnp;
		vnp->choice = SEQID_GI;
		vnp->data.intvalue = gi_number;
	}

	if (vnp == NULL)   /* no ids */
	{
		Message(MSG_ERROR, "You must have some type of ID to create a Bioseq");
		bsp = BioseqFree(bsp);
	}

	return bsp;
}

/*****************************************************************************
*
*   One raw Bioseq in the entry
*
*****************************************************************************/
NLM_EXTERN SeqEntryPtr AddSeqOnlyToSubmission (
	NCBISubPtr submission,
	CharPtr local_name ,
	CharPtr genbank_locus ,
	CharPtr genbank_accession ,
	Int4 gi_number ,
	Int2 molecule_class,
	Int2 molecule_type ,
	Int4 length ,
	Int2 topology ,
	Int2 strandedness )
{
	BioseqPtr bsp;
	BioseqSetPtr targetbssp;
	SeqEntryPtr sep, tmp;

	sep = SeqEntryNew();
	sep->choice = 1;    /* Bioseq */

	if (submission->ssp->data == NULL)
		submission->ssp->data = (Pointer)sep;
	else
	{
		tmp = (SeqEntryPtr) submission->ssp->data;
		if (tmp != NULL && tmp->choice == 2 && tmp->data.ptrvalue != NULL) {
			targetbssp = (BioseqSetPtr) tmp->data.ptrvalue;
			if (targetbssp->_class == 7 ||
			    (targetbssp->_class >= 13 && targetbssp->_class <= 16)) {
				tmp = targetbssp->seq_set;
			}
		}
		for (; tmp->next != NULL; tmp = tmp->next)
			continue;
		tmp->next = sep;
	}

	bsp = NCBISubNewBioseq (submission , local_name, genbank_locus,
		genbank_accession, gi_number, molecule_class, molecule_type,
		length, topology, strandedness);

	sep->data.ptrvalue = bsp;
	SeqMgrSeqEntry(SM_BIOSEQ, (Pointer)bsp, sep);
	
	return sep;
}

/*****************************************************************************
*
*   One delta Bioseq in the entry
*
*****************************************************************************/
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
	Int2 strandedness )
{
	BioseqPtr bsp;
	SeqEntryPtr sep;

	sep = AddSeqOnlyToSubmission(
		submission,
		local_name ,
		 genbank_locus ,
		 genbank_accession ,
		 gi_number ,
		 molecule_class,
		 molecule_type ,
		 length ,
		 topology ,
		 strandedness );

	if (sep == NULL) return sep;

	bsp = (BioseqPtr)(sep->data.ptrvalue);

	bsp->repr = Seq_repr_delta;
	bsp->seq_ext_type = 4;    /* delta extension */

	return sep;
}

/* extended SeqLit that adjusts attached SeqGraphs - internal for subutil only */

typedef struct extseqlit {
	SeqLit slp;
	BioseqPtr parentbsp;
	SeqGraphPtr graph;
} ExtSeqLit, PNTR ExtSeqLitPtr;

static void ReadjustSeqLitGraphs (BioseqPtr bsp)

{
  Int4          curroffset = 0;
  DeltaSeqPtr   dsp;
  SeqGraphPtr   sgp;
  SeqIntPtr     sintp;
  SeqLocPtr     slocp;
  SeqLitPtr     slp;
  ExtSeqLitPtr  xslp;

  if (bsp == NULL || bsp->repr != Seq_repr_delta) return;
  for (dsp = (DeltaSeqPtr) (bsp->seq_ext); dsp != NULL; dsp = dsp->next) {
    switch (dsp->choice) {
      case 1 :
        slocp = (SeqLocPtr) dsp->data.ptrvalue;
        if (slocp == NULL) break;
        if (slocp->choice != SEQLOC_NULL) {
          curroffset += SeqLocLen (slocp);
        }
        break;
      case 2 :
        slp = (SeqLitPtr) dsp->data.ptrvalue;
        if (slp == NULL) break;
        xslp = (ExtSeqLitPtr) slp;
        sgp = xslp->graph;
        if (sgp != NULL) {
          slocp = sgp->loc;
          if (slocp != NULL && slocp->choice == SEQLOC_INT) {
            sintp = (SeqIntPtr) slocp->data.ptrvalue;
            if (sintp != NULL) {
              sintp->from = curroffset;
              sintp->to = slp->length + curroffset - 1;
            }
          }
        }
        curroffset += slp->length;
        break;
      default :
        break;
    }
  }
}

static SeqLitPtr AddSeqLitToBioseq(
	NCBISubPtr submission,
	SeqEntryPtr delta_seq_entry,
	Int4 length ,
	Boolean isa_gap)
{
	BioseqPtr bsp;
	ValNodePtr vnp;
	SeqLitPtr slp;
	IntFuzzPtr ifp;
	ExtSeqLitPtr xslp;

	if (delta_seq_entry == NULL) return FALSE;
	if (IS_Bioseq_set(delta_seq_entry)) return FALSE;
	if (delta_seq_entry->data.ptrvalue == NULL) return FALSE;

	bsp = (BioseqPtr)(delta_seq_entry->data.ptrvalue);
	if (bsp->repr != Seq_repr_delta)
	{
		Message(MSG_ERROR, "AddToDeltaSeq: no delta seq found");
		return FALSE;
	}

	if ((length > 0) || ((length == 0) && (isa_gap)))
	{
		/* slp = SeqLitNew(); */
		slp = (SeqLitPtr) MemNew (sizeof (ExtSeqLit));
		slp->length = length;
		vnp = ValNodeAddPointer((ValNodePtr PNTR)&(bsp->seq_ext),(Int2) 2, (Pointer)slp);
		if ((length == 0) && (isa_gap)) /* gap of unknown length */
		{
			ifp = IntFuzzNew();
			ifp->choice = 4;    /* lim - unk*/
			slp->fuzz = ifp;
		}
		xslp = (ExtSeqLitPtr) slp;
		xslp->parentbsp = bsp;
		
		SpreadGapsInDeltaSeq(bsp);   /* distribute the unknown gap sizes */

		ReadjustSeqLitGraphs (bsp);  /* adjust seqlit graph positions */

		return slp;
	}

	return (SeqLitPtr)NULL;
}

NLM_EXTERN Boolean AddGapToDeltaSeq (
	NCBISubPtr submission,
	SeqEntryPtr delta_seq_entry,
	Int4 length_of_gap )    /** 0 if not known */
{
	if (AddSeqLitToBioseq(submission, delta_seq_entry, length_of_gap, TRUE) == NULL)
		return FALSE;
	else
		return TRUE;
}

NLM_EXTERN SeqLitPtr AddFakeGapToDeltaSeq (
	NCBISubPtr submission,
	SeqEntryPtr delta_seq_entry,
	Int4 length_of_gap )
{
	return AddSeqLitToBioseq(submission, delta_seq_entry, length_of_gap, TRUE);
}



NLM_EXTERN SeqLitPtr AddLiteralToDeltaSeq (
	NCBISubPtr submission,
	SeqEntryPtr delta_seq_entry,
	Int4 length_of_sequence )
{
	return AddSeqLitToBioseq(submission, delta_seq_entry, length_of_sequence, FALSE);
}


/*****************************************************************************
*
*   Segmented Set
*
*****************************************************************************/
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
	Int2 strandedness )
{
	BioseqPtr bsp;
	BioseqSetPtr bssp, targetbssp;
	SeqEntryPtr sep, the_set, tmp;

	bssp = BioseqSetNew();
	bssp->_class = 2;       /* segset */
	the_set = SeqEntryNew();
	the_set->choice = 2;    /* set */
	the_set->data.ptrvalue = bssp;
	SeqMgrSeqEntry(SM_BIOSEQSET, (Pointer)bssp, the_set);

	bsp = NCBISubNewBioseq (submission , local_name, genbank_locus,
		genbank_accession, gi_number, molecule_class, molecule_type,
		length, topology, strandedness);
	bsp->repr = Seq_repr_seg;
	bsp->seq_ext_type = 1;

	sep = SeqEntryNew();
	sep->choice = 1;   /* Bioseq */
	sep->data.ptrvalue = bsp;
	SeqMgrSeqEntry(SM_BIOSEQ, (Pointer)bsp, sep);

	bssp->seq_set = sep;
	SeqMgrConnect(SM_BIOSEQ, (Pointer)bsp, SM_BIOSEQSET, (Pointer)bssp);

	if (submission->ssp->data == NULL)
		submission->ssp->data = (Pointer)the_set;
	else
	{
		tmp = (SeqEntryPtr) submission->ssp->data;
		if (tmp != NULL && tmp->choice == 2 && tmp->data.ptrvalue != NULL) {
			targetbssp = (BioseqSetPtr) tmp->data.ptrvalue;
			if (targetbssp->_class == 7 ||
			    (targetbssp->_class >= 13 && targetbssp->_class <= 16)) {
				tmp = targetbssp->seq_set;
			}
		}
		for (; tmp->next != NULL; tmp = tmp->next)
			continue;
		tmp->next = the_set;
	}

	return the_set;
}

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
	Int2 strandedness )
{
	BioseqSetPtr segset, parts = NULL, tp;
	BioseqPtr bsp, segseq= NULL;
	SeqEntryPtr sep, tmp, prev;
	SeqLocPtr slp, curr;
	SeqIntPtr sip;

	if (segmented_seq_entry == NULL) return NULL;
	if (IS_Bioseq(segmented_seq_entry)) return NULL;
	if (segmented_seq_entry->data.ptrvalue == NULL) return NULL;

	segset = (BioseqSetPtr)(segmented_seq_entry->data.ptrvalue);
	if (segset->_class != 2) return NULL;

	prev = NULL;
	for (tmp = segset->seq_set; tmp != NULL; tmp = tmp->next)
	{
		if (IS_Bioseq(tmp))
		{
			bsp = (BioseqPtr)(tmp->data.ptrvalue);
			if (bsp->repr == Seq_repr_seg)
				segseq = bsp;
		}
		else
		{
			tp = (BioseqSetPtr)(tmp->data.ptrvalue);
			if (tp->_class == 4)
				parts = tp;
		}
		prev = tmp;
	}

	if (segseq == NULL)
	{
		Message(MSG_ERROR, "AddSeqToSegmentedEntry: no seg seq found");
		return NULL;
	}

	if (parts == NULL)   /* first member of segset */
	{
		parts = BioseqSetNew();
		parts->_class = 4;
		sep = SeqEntryNew();
		sep->choice = 2;
		sep->data.ptrvalue = (Pointer)parts;
		SeqMgrSeqEntry(SM_BIOSEQSET, (Pointer)parts, sep);
		SeqMgrConnect(SM_BIOSEQSET, (Pointer)parts, SM_BIOSEQSET, (Pointer)segset);
		prev->next = sep;
	}

	prev = NULL;
	for (tmp = parts->seq_set; tmp != NULL; tmp = tmp->next)
	{
		if (! IS_Bioseq(tmp))
		{
			Message(MSG_ERROR, "BioseqSet in Parts");
			return NULL;
		}

		prev = tmp;
	}

	sep = SeqEntryNew();
	if (prev == NULL)      /* first one */
		parts->seq_set = sep;
	else
		prev->next = sep;

	bsp = NCBISubNewBioseq (submission , local_name, genbank_locus,
		genbank_accession, gi_number, molecule_class, molecule_type,
		length, topology, strandedness);
	sep->choice = 1;
	sep->data.ptrvalue = (Pointer)bsp;

	SeqMgrSeqEntry(SM_BIOSEQ, (Pointer)bsp, sep);
	SeqMgrConnect(SM_BIOSEQ, (Pointer)bsp, SM_BIOSEQSET, (Pointer)parts);
						/* add pointer to segmented seq */
	if (segseq->seq_ext != NULL)
	{
		for (curr = (SeqLocPtr)segseq->seq_ext; curr->next != NULL; curr = curr->next)
			continue;
	}
	else
		curr = NULL;

	slp = ValNodeNew(NULL);
	if (curr == NULL)
		segseq->seq_ext = slp;
	else
		curr->next = slp;
	slp->choice = SEQLOC_INT;
	sip = SeqIntNew();
	slp->data.ptrvalue = sip;
	sip->id = SeqIdDup(bsp->id);
	sip->from = 0;
	sip->to = length - 1;

	segseq->length += length;   /* add the segment */

	return sep;
}

NLM_EXTERN Boolean AddGapToSegmentedEntry (
	NCBISubPtr submission,
	SeqEntryPtr segmented_seq_entry,
	Int4 length_of_gap )    /** 0 if not known */
{
	BioseqSetPtr segset, parts = NULL, tp;
	BioseqPtr bsp, segseq= NULL;
	SeqEntryPtr sep, tmp, prev;
	ValNodePtr vnp;
	SeqLocPtr slp, curr;
	SeqIntPtr sip;
	Char local_name[40];
	Int2 mol_type=0;

	if (segmented_seq_entry == NULL) return FALSE;
	if (IS_Bioseq(segmented_seq_entry)) return FALSE;
	if (segmented_seq_entry->data.ptrvalue == NULL) return FALSE;

	segset = (BioseqSetPtr)(segmented_seq_entry->data.ptrvalue);
	if (segset->_class != 2) return FALSE;

	prev = NULL;
	for (tmp = segset->seq_set; tmp != NULL; tmp = tmp->next)
	{
		if (IS_Bioseq(tmp))
		{
			bsp = (BioseqPtr)(tmp->data.ptrvalue);
			if (bsp->repr == Seq_repr_seg)
				segseq = bsp;
		}
		else
		{
			tp = (BioseqSetPtr)(tmp->data.ptrvalue);
			if (tp->_class == 4)
				parts = tp;
		}
		prev = tmp;
	}

	if (segseq == NULL)
	{
		Message(MSG_ERROR, "AddGapToSegmentedEntry: no seg seq found");
		return FALSE;
	}

	if ((parts == NULL) && (length_of_gap <= 0))
	{
		Message(MSG_ERROR, "Do not start or end a segmented set with unknown gaps");
		return FALSE;
	}

	if (length_of_gap > 0)     /* add a virtual sequence */
	{
		if (parts == NULL)   /* first member of segset */
		{
			parts = BioseqSetNew();
			parts->_class = 4;
			sep = SeqEntryNew();
			sep->choice = 2;
			sep->data.ptrvalue = (Pointer)parts;
			prev->next = sep;
		}

		prev = NULL;
		for (tmp = parts->seq_set; tmp != NULL; tmp = tmp->next)
		{
			if (! IS_Bioseq(tmp))
			{
				Message(MSG_ERROR, "BioseqSet in Parts");
				return FALSE;
			}

			prev = tmp;
		}

		sep = SeqEntryNew();
		if (prev == NULL)      /* first one */
			parts->seq_set = sep;
		else
			prev->next = sep;

		for (vnp = segseq->descr; vnp != NULL; vnp = vnp->next)
		{
			if (vnp->choice == Seq_descr_mol_type)
			{
				mol_type = (Int2)(vnp->data.intvalue);
				break;
			}
		}

		submission->gap_count++;
		sprintf(local_name, "Gap_%d", (int)(submission->gap_count));

		bsp = NCBISubNewBioseq (submission , local_name, NULL,
			NULL, 0, mol_type, (Int2)(segseq->mol), length_of_gap,
			(Int2)(segseq->topology), (Int2)(segseq->strand));
		bsp->repr = Seq_repr_virtual;
		sep->choice = 1;
		sep->data.ptrvalue = (Pointer)bsp;
		SeqMgrSeqEntry(SM_BIOSEQ, (Pointer)bsp, sep);

		SeqMgrConnect(SM_BIOSEQ, (Pointer)bsp, SM_BIOSEQSET, (Pointer)parts);
	}
					/* add pointer to segmented seq */
	if (segseq->seq_ext != NULL)
	{
		for (curr = (SeqLocPtr)segseq->seq_ext; curr->next != NULL; curr = curr->next)
			continue;
	}
	else
		curr = NULL;

	slp = ValNodeNew(NULL);
	if (curr == NULL)
		segseq->seq_ext = slp;
	else
		curr->next = slp;
	if (length_of_gap > 0)
	{
		slp->choice = SEQLOC_INT;
		sip = SeqIntNew();
		slp->data.ptrvalue = sip;
		sip->id = SeqIdDup(bsp->id);
		sip->from = 0;
		sip->to = length_of_gap - 1;
	}
	else
	{
		slp->choice = SEQLOC_NULL;
	}

	return TRUE;
}

NLM_EXTERN Boolean AddReferenceToSegmentedEntry (
	NCBISubPtr submission ,
	SeqEntryPtr segmented_seq_entry,
	CharPtr genbank_accession ,
	Int4 gi_number ,
	Int4 from ,
	Int4 to ,
	Boolean on_plus_strand )
{
	BioseqSetPtr segset;
	BioseqPtr bsp, segseq= NULL;
	SeqEntryPtr tmp, prev;
	SeqLocPtr slp, curr;
	SeqIntPtr sip;
	TextSeqIdPtr tsip;


	if (segmented_seq_entry == NULL) return FALSE;
	if (IS_Bioseq(segmented_seq_entry)) return FALSE;
	if (segmented_seq_entry->data.ptrvalue == NULL) return FALSE;

	segset = (BioseqSetPtr)(segmented_seq_entry->data.ptrvalue);
	if (segset->_class != 2) return FALSE;

	prev = NULL;
	for (tmp = segset->seq_set; tmp != NULL; tmp = tmp->next)
	{
		if (IS_Bioseq(tmp))
		{
			bsp = (BioseqPtr)(tmp->data.ptrvalue);
			if (bsp->repr == Seq_repr_seg)
				segseq = bsp;
		}
		prev = tmp;
	}

	if (segseq == NULL)
	{
		Message(MSG_ERROR, "AddRefToSegmentedEntry: no seg seq found");
		return FALSE;
	}

				/* add pointer to segmented seq */
	if (segseq->seq_ext != NULL)
	{
		for (curr = (SeqLocPtr)segseq->seq_ext; curr->next != NULL; curr = curr->next)
			continue;
	}
	else
		curr = NULL;

	slp = ValNodeNew(NULL);
	if (curr == NULL)
		segseq->seq_ext = slp;
	else
		curr->next = slp;

	slp->choice = SEQLOC_INT;
	sip = SeqIntNew();
	slp->data.ptrvalue = sip;
	sip->from = from;
	sip->to = to;
	if (on_plus_strand)
		sip->strand = Seq_strand_plus;
	else
		sip->strand = Seq_strand_minus;

	curr = ValNodeNew(NULL);
	sip->id = curr;
	if (gi_number > 0)    /* gis are better */
	{
		curr->choice = SEQID_GI;
		curr->data.intvalue = gi_number;
	}
	else
	{
		curr->choice = SEQID_GENBANK;
		tsip = TextSeqIdNew();
		tsip->accession = StringSaveNoNull(genbank_accession);
		curr->data.ptrvalue = tsip;
	}

	return TRUE;
}

/*****************************************************************************
*
*   Build sets of similar sequences
*
*****************************************************************************/
static SeqEntryPtr AddPopPhyMutSetToSubmission (
	NCBISubPtr submission, Uint1 choice )

{
	BioseqSetPtr bssp;
	SeqEntryPtr tmp, the_set;

	bssp = BioseqSetNew();
	bssp->_class = choice;       /* pop-set, phy-set, or mut-set */
	the_set = SeqEntryNew();
	the_set->choice = 2;    /* set */
	the_set->data.ptrvalue = bssp;
	SeqMgrSeqEntry(SM_BIOSEQSET, (Pointer)bssp, the_set);

	if (submission->ssp->data == NULL)
		submission->ssp->data = (Pointer)the_set;
	else
	{
		for (tmp = (SeqEntryPtr) submission->ssp->data; tmp->next != NULL; tmp = tmp->next)
			continue;
		tmp->next = the_set;
	}

	return the_set;
}

NLM_EXTERN SeqEntryPtr AddPopSetToSubmission (
	NCBISubPtr submission )

{
	return AddPopPhyMutSetToSubmission (submission, BioseqseqSet_class_pop_set);
}

NLM_EXTERN SeqEntryPtr AddPhySetToSubmission (
	NCBISubPtr submission )

{
	return AddPopPhyMutSetToSubmission (submission, BioseqseqSet_class_phy_set);
}

NLM_EXTERN SeqEntryPtr AddMutSetToSubmission (
	NCBISubPtr submission )

{
	return AddPopPhyMutSetToSubmission (submission, BioseqseqSet_class_mut_set);
}

NLM_EXTERN SeqEntryPtr AddGenBankSetToSubmission (
	NCBISubPtr submission )

{
	return AddPopPhyMutSetToSubmission (submission, BioseqseqSet_class_genbank);
}

/*****************************************************************************
*
*   Build nuc-prot sets
*
*****************************************************************************/
NLM_EXTERN SeqEntryPtr AddNucProtToSubmission (
	NCBISubPtr submission )
{
	BioseqSetPtr bssp, targetbssp;
	SeqEntryPtr tmp, the_set;

	bssp = BioseqSetNew();
	bssp->_class = 1;       /* nuc-prot */
	the_set = SeqEntryNew();
	the_set->choice = 2;    /* set */
	the_set->data.ptrvalue = bssp;
	SeqMgrSeqEntry(SM_BIOSEQSET, (Pointer)bssp, the_set);

	if (submission->ssp->data == NULL)
		submission->ssp->data = (Pointer)the_set;
	else
	{
		tmp = (SeqEntryPtr) submission->ssp->data;
		if (tmp != NULL && tmp->choice == 2 && tmp->data.ptrvalue != NULL) {
			targetbssp = (BioseqSetPtr) tmp->data.ptrvalue;
			if (targetbssp->_class == 7 ||
			    (targetbssp->_class >= 13 && targetbssp->_class <= 16)) {
				tmp = targetbssp->seq_set;
			}
		}
		for (; tmp->next != NULL; tmp = tmp->next)
			continue;
		tmp->next = the_set;
	}

	return the_set;
}

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
	Int2 strandedness )
{
	BioseqSetPtr nucprot, tp;
	BioseqPtr bsp, segseq= NULL;
	SeqEntryPtr sep, tmp, prev, tmp2;
	Boolean is_nuc;

	if (nuc_prot_entry == NULL) return NULL;
	if (IS_Bioseq(nuc_prot_entry)) return NULL;
	if (nuc_prot_entry->data.ptrvalue == NULL) return NULL;

	nucprot = (BioseqSetPtr)(nuc_prot_entry->data.ptrvalue);
	if (nucprot->_class != 1) return NULL;

	if (ISA_na(molecule_class))
		is_nuc = TRUE;
	else
		is_nuc = FALSE;

	prev = NULL;
	for (tmp = nucprot->seq_set; tmp != NULL; tmp = tmp->next)
	{
		if (is_nuc)   /* check for multiple nucleotides */
		{
			if (IS_Bioseq(tmp))
			{
				bsp = (BioseqPtr)(tmp->data.ptrvalue);
				if (ISA_na(bsp->mol))
				{
					Message(MSG_ERROR, "AddSeqToNucProt: adding more than one nucleotide seq");
					return NULL;
				}
			}
			else
			{
				tp = (BioseqSetPtr)(tmp->data.ptrvalue);
				if (tp->_class == 2)  /* seg-set */
				{
					for (tmp2 = tp->seq_set; tmp2 != NULL; tmp2 = tmp2->next)
					{
						if (IS_Bioseq(tmp2))
						{
							bsp = (BioseqPtr)(tmp2->data.ptrvalue);
							if (ISA_na(bsp->mol))
							{
								Message(MSG_ERROR, "AddSeqToNucProt: adding more than one nucleotide seq");
								return NULL;
							}
						}
					}
				}
			}
		}
		prev = tmp;
	}

	sep = SeqEntryNew();
	if (prev == NULL)      /* first one */
		nucprot->seq_set = sep;
	else
		prev->next = sep;

	bsp = NCBISubNewBioseq (submission , local_name, genbank_locus,
		genbank_accession, gi_number, molecule_class, molecule_type,
		length, topology, strandedness);
	sep->choice = 1;
	sep->data.ptrvalue = (Pointer)bsp;
	SeqMgrSeqEntry(SM_BIOSEQ, (Pointer)bsp, sep);
	SeqMgrConnect(SM_BIOSEQ, (Pointer)bsp, SM_BIOSEQSET, (Pointer)nucprot);

	return sep;

}
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
	Int2 strandedness )
{
	BioseqSetPtr nucprot, tp;
	BioseqPtr bsp;
	SeqEntryPtr sep, tmp, prev, tmp2, the_set;
	Boolean is_nuc;
	BioseqSetPtr bssp;

	if (nuc_prot_entry == NULL) return NULL;
	if (IS_Bioseq(nuc_prot_entry)) return NULL;
	if (nuc_prot_entry->data.ptrvalue == NULL) return NULL;

	nucprot = (BioseqSetPtr)(nuc_prot_entry->data.ptrvalue);
	if (nucprot->_class != 1) return NULL;

	if (ISA_na(molecule_class))
		is_nuc = TRUE;
	else
		is_nuc = FALSE;

	prev = NULL;
	for (tmp = nucprot->seq_set; tmp != NULL; tmp = tmp->next)
	{
		if (is_nuc)   /* check for multiple nucleotides */
		{
			if (IS_Bioseq(tmp))
			{
				bsp = (BioseqPtr)(tmp->data.ptrvalue);
				if (ISA_na(bsp->mol))
				{
					Message(MSG_ERROR, "AddSeqToNucProt: adding more than one nucleotide seq");
					return NULL;
				}
			}
			else
			{
				tp = (BioseqSetPtr)(tmp->data.ptrvalue);
				if (tp->_class == 2)  /* seg-set */
				{
					for (tmp2 = tp->seq_set; tmp2 != NULL; tmp2 = tmp2->next)
					{
						if (IS_Bioseq(tmp2))
						{
							bsp = (BioseqPtr)(tmp2->data.ptrvalue);
							if (ISA_na(bsp->mol))
							{
								Message(MSG_ERROR, "AddSeqToNucProt: adding more than one nucleotide seq");
								return NULL;
							}
						}
					}
				}
			}
		}
		prev = tmp;
	}

	bssp = BioseqSetNew();
	bssp->_class = 2;       /* segset */
	the_set = SeqEntryNew();
	the_set->choice = 2;    /* set */
	the_set->data.ptrvalue = bssp;
	if (prev == NULL)      /* first one */
		nucprot->seq_set = the_set;
	else
		prev->next = the_set;
	SeqMgrSeqEntry(SM_BIOSEQSET, (Pointer)bssp, the_set);
	SeqMgrConnect(SM_BIOSEQSET, (Pointer)bssp, SM_BIOSEQSET, (Pointer)nucprot);

	bsp = NCBISubNewBioseq (submission , local_name, genbank_locus,
		genbank_accession, gi_number, molecule_class, molecule_type,
		length, topology, strandedness);
	bsp->repr = Seq_repr_seg;
	bsp->seq_ext_type = 1;

	sep = SeqEntryNew();
	sep->choice = 1;   /* Bioseq */
	sep->data.ptrvalue = bsp;
	bssp->seq_set = sep;
	SeqMgrSeqEntry(SM_BIOSEQ, (Pointer)bsp, sep);
	SeqMgrConnect(SM_BIOSEQ, (Pointer)bsp, SM_BIOSEQSET, (Pointer)bssp);


	return the_set;
}

NLM_EXTERN SeqEntryPtr AddDeltaSeqToNucProtEntry (   /** add delta nuc bioseq */
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
	Int2 strandedness )
{
	BioseqSetPtr nucprot, tp;
	BioseqPtr bsp, segseq= NULL;
	SeqEntryPtr sep, tmp, prev, tmp2;
	Boolean is_nuc;

	if (nuc_prot_entry == NULL) return NULL;
	if (IS_Bioseq(nuc_prot_entry)) return NULL;
	if (nuc_prot_entry->data.ptrvalue == NULL) return NULL;

	nucprot = (BioseqSetPtr)(nuc_prot_entry->data.ptrvalue);
	if (nucprot->_class != 1) return NULL;

	if (ISA_na(molecule_class))
		is_nuc = TRUE;
	else
		is_nuc = FALSE;

	prev = NULL;
	for (tmp = nucprot->seq_set; tmp != NULL; tmp = tmp->next)
	{
		if (is_nuc)   /* check for multiple nucleotides */
		{
			if (IS_Bioseq(tmp))
			{
				bsp = (BioseqPtr)(tmp->data.ptrvalue);
				if (ISA_na(bsp->mol))
				{
					Message(MSG_ERROR, "AddSeqToNucProt: adding more than one nucleotide seq");
					return NULL;
				}
			}
			else
			{
				tp = (BioseqSetPtr)(tmp->data.ptrvalue);
				if (tp->_class == 2)  /* seg-set */
				{
					for (tmp2 = tp->seq_set; tmp2 != NULL; tmp2 = tmp2->next)
					{
						if (IS_Bioseq(tmp2))
						{
							bsp = (BioseqPtr)(tmp2->data.ptrvalue);
							if (ISA_na(bsp->mol))
							{
								Message(MSG_ERROR, "AddSeqToNucProt: adding more than one nucleotide seq");
								return NULL;
							}
						}
					}
				}
			}
		}
		prev = tmp;
	}

	sep = SeqEntryNew();
	if (prev == NULL)      /* first one */
		nucprot->seq_set = sep;
	else
		prev->next = sep;

	bsp = NCBISubNewBioseq (submission , local_name, genbank_locus,
		genbank_accession, gi_number, molecule_class, molecule_type,
		length, topology, strandedness);
	sep->choice = 1;
	sep->data.ptrvalue = (Pointer)bsp;
	SeqMgrSeqEntry(SM_BIOSEQ, (Pointer)bsp, sep);
	SeqMgrConnect(SM_BIOSEQ, (Pointer)bsp, SM_BIOSEQSET, (Pointer)nucprot);

	bsp->repr = Seq_repr_delta;
	bsp->seq_ext_type = 4;    /* delta extension */

	return sep;

}

/*************************************************************************
*  GetDNAConv:
*  -- DNA conversion table array
**************************************************************************/
static Uint1Ptr GetDNAConv(void)
{
   SeqCodeTablePtr sctp;
   Uint1Ptr        dnaconv;
   CharPtr         ptr;
   Int2            i;

   dnaconv = MemNew((size_t)255);		   /* DNA */
   MemSet((CharPtr)dnaconv, (Uint1)1, (size_t)255); /* everything an error */
   dnaconv[32] = 0;  /* blank */
   sctp = SeqCodeTableFind((Uint1)Seq_code_iupacna);
   ptr = sctp->letters;   /* one letter code */
   for (i = 0; i < (Int4)(sctp->num); i++, ptr++)
       dnaconv[*ptr] = *ptr;

   return (dnaconv);

}
/*************************************************************************
*  GetProteinConv:
*  -- Protein conversion table array
**************************************************************************/
static Uint1Ptr GetProteinConv(void)
{
   SeqCodeTablePtr sctp;
   Uint1Ptr        protconv;
   CharPtr         ptr;
   Int2            i;

   protconv = MemNew((size_t)255);		   /* proteins */
   MemSet((CharPtr)protconv, (Uint1)1, (size_t)255); /* everything an error */
   protconv[32] = 0;  /* blank */
   sctp = SeqCodeTableFind((Uint1)Seq_code_iupacaa);
   ptr = sctp->letters;   /* one letter code */
   for (i = 0; i < (Int4)(sctp->num); i++, ptr++)
       protconv[*ptr] = *ptr;

   return (protconv);

}

NLM_EXTERN Boolean AddBasesToByteStore (ByteStorePtr bsp, CharPtr the_bases)
{
	Uint1 PNTR buf;
	Uint1 PNTR bu;
	Uint1    residue;
	Uint1Ptr dnaconv;
	CharPtr tmp;

	dnaconv = GetDNAConv();
	buf = MemNew(StringLen(the_bases) + 1);
	bu = buf;
	for (tmp = the_bases; *tmp != '\0'; tmp++)
	{
		*tmp = TO_UPPER(*tmp);
		if (*tmp == 'U') *tmp = 'T';
		if (*tmp == 'X') *tmp = 'N';
		residue = dnaconv[*tmp];
		if (residue > 2) {
			*bu++ = residue;
		} else if (residue == 1 && IS_ALPHA(*tmp)) {
			*bu++ = 'N';
		} else {
			ErrPostEx(SEV_ERROR, 0,0, "Illegal character in Bioseq [%c]", *tmp);
		}
	}
	BSWrite(bsp, buf, (Int4) (bu - buf));
	MemFree(buf);
	MemFree(dnaconv);
	
	return TRUE;
}

NLM_EXTERN Boolean AddAAsToByteStore (ByteStorePtr bsp, CharPtr the_aas)
{
	Uint1 PNTR buf;
	Uint1 PNTR bu;
	Uint1    residue;
	Uint1Ptr aaconv;
	CharPtr tmp;

	aaconv = GetProteinConv();
	buf = MemNew(StringLen(the_aas) + 1);
	bu = buf;
	for (tmp = the_aas; *tmp != '\0'; tmp++)
	{
		*tmp = TO_UPPER(*tmp);
		residue = aaconv[*tmp];
		if (residue > 2) {
			*bu++ = residue;
		} else if (residue == 1 && IS_ALPHA(*tmp)) {
			*bu++ = 'X';
		} else {
			ErrPostEx(SEV_ERROR, 0,0, "Illegal character in Bioseq [%c]", *tmp);
		}
	}

	BSWrite(bsp, buf, (Int4) (bu - buf));
	MemFree(buf);
	MemFree(aaconv);

	return TRUE;
}

/*****************************************************************************
*
*   Add residues to raw Bioseqs
*
*****************************************************************************/
NLM_EXTERN Boolean AddBasesToBioseq (
	NCBISubPtr submission ,
	SeqEntryPtr the_seq ,
	CharPtr the_bases )
{
	BioseqPtr bsp;

	if ((the_seq == NULL) || (the_bases == NULL))
		return FALSE;

	if (! IS_Bioseq(the_seq))
	{
		Message(MSG_ERROR, "Adding bases to a Bioseq-set");
		return FALSE;
	}

	bsp = (BioseqPtr)(the_seq->data.ptrvalue);

	if (! ISA_na(bsp->mol))
	{
		Message(MSG_ERROR, "Adding bases to a protein");
		return FALSE;
	}

	if (bsp->repr != Seq_repr_raw)
	{
		Message(MSG_ERROR, "Adding residues to non-raw Bioseq");
		return FALSE;
	}

	if (bsp->seq_data == NULL)
	{
		bsp->seq_data = BSNew(bsp->length);
		bsp->seq_data_type = Seq_code_iupacna;
	}

	return AddBasesToByteStore(bsp->seq_data, the_bases);
}

NLM_EXTERN Boolean AddAminoAcidsToBioseq (
	NCBISubPtr submission ,
	SeqEntryPtr the_seq ,
	CharPtr the_aas )
{
	BioseqPtr bsp;

	if ((the_seq == NULL) || (the_aas == NULL))
		return FALSE;

	if (! IS_Bioseq(the_seq))
	{
		Message(MSG_ERROR, "Adding amino acids to a Bioseq-set");
		return FALSE;
	}

	bsp = (BioseqPtr)(the_seq->data.ptrvalue);

	if (ISA_na(bsp->mol))
	{
		Message(MSG_ERROR, "Adding amino acids to a nucleotide");
		return FALSE;
	}

	if (bsp->repr != Seq_repr_raw)
	{
		Message(MSG_ERROR, "Adding residues to non-raw Bioseq");
		return FALSE;
	}

	if (bsp->seq_data == NULL)
	{
		bsp->seq_data = BSNew(bsp->length);
		bsp->seq_data_type = Seq_code_ncbieaa;
	}

	return AddAAsToByteStore(bsp->seq_data, the_aas);
}


/*****************************************************************************
*
*   Add residues to Literals
*
*****************************************************************************/
NLM_EXTERN Boolean AddBasesToLiteral (
	NCBISubPtr submission ,
	SeqLitPtr the_literal ,
	CharPtr the_bases )
{
	if ((the_literal== NULL) || (the_bases == NULL))
		return FALSE;


	if (the_literal->seq_data == NULL)
	{
		the_literal->seq_data = BSNew(the_literal->length);
		the_literal->seq_data_type = Seq_code_iupacna;
	}

	return AddBasesToByteStore(the_literal->seq_data, the_bases);
}

NLM_EXTERN Boolean AddAminoAcidsToLiteral (
	NCBISubPtr submission ,
	SeqLitPtr the_literal ,
	CharPtr the_aas )
{

	if ((the_literal == NULL) || (the_aas == NULL))
		return FALSE;


	if (the_literal->seq_data == NULL)
	{
		the_literal->seq_data = BSNew(the_literal->length);
		the_literal->seq_data_type = Seq_code_ncbieaa;
	}

	return AddAAsToByteStore(the_literal->seq_data, the_aas);
}

/*****************************************************************************
*
*   Add Annotations to Entries
*
*****************************************************************************/

static Boolean StripNonAsciiChars (CharPtr str)
{
	Boolean retval = TRUE;

	while (*str != '\0')
	{
		if ((*str < ' ') || (*str > '~'))
		{
			retval = FALSE;
			if (*str == '\n' || *str == '\r')
				*str = '~';
			else
				*str = '#';
		}
		str++;
	}
	return retval;
}

NLM_EXTERN ValNodePtr NewDescrOnSeqEntry (SeqEntryPtr entry, Int2 type)
{
	ValNodePtr vnp;
	BioseqPtr bsp;
	BioseqSetPtr bssp;

	if (entry == NULL)
		return NULL;

	if (IS_Bioseq(entry))
	{
		bsp = (BioseqPtr)(entry->data.ptrvalue);
		vnp = SeqDescrAdd(&(bsp->descr));
	}
	else
	{
		bssp = (BioseqSetPtr)(entry->data.ptrvalue);
		vnp = SeqDescrAdd(&(bssp->descr));
	}
	if (vnp != NULL)
		vnp->choice = (Uint1)type;
	return vnp;
}

NLM_EXTERN ValNodePtr GetDescrOnSeqEntry (SeqEntryPtr entry, Int2 type)
{
	ValNodePtr vnp;
	BioseqPtr bsp;
	BioseqSetPtr bssp;

	if (entry == NULL)
		return NULL;

	if (IS_Bioseq(entry))
	{
		bsp = (BioseqPtr)(entry->data.ptrvalue);
		for (vnp = bsp->descr; vnp; vnp = vnp->next) {
			if (vnp->choice == (Uint1)type) {
				return vnp;
			}
		}
		vnp = SeqDescrAdd(&(bsp->descr));
	}
	else
	{
		bssp = (BioseqSetPtr)(entry->data.ptrvalue);
		for (vnp = bssp->descr; vnp; vnp = vnp->next) {
			if (vnp->choice == (Uint1)type) {
				return vnp;
			}
		}
		vnp = SeqDescrAdd(&(bssp->descr));
	}
	if (vnp != NULL)
		vnp->choice = (Uint1)type;
	return vnp;
}

static SeqIdPtr SubutilMakeAc2GBSeqId (CharPtr accession)
{
   TextSeqIdPtr tsip;
   SeqIdPtr sip;

   if (accession == NULL || *accession == '\0')
      return NULL;

   sip = ValNodeNew(NULL);
   sip->choice = SEQID_GENBANK;
   tsip = TextSeqIdNew();
   sip->data.ptrvalue = tsip;
   tsip->accession = StringSave(accession);

   return sip;

} /* MakeAc2GBSeqId */

NLM_EXTERN Boolean AddSecondaryAccnToEntry (
NCBISubPtr submission,
	SeqEntryPtr entry ,
	CharPtr accn )

{
	BioseqPtr bsp;
	ValNodePtr vnp;
	GBBlockPtr gbp;
	CharPtr p;
	Int4 i, j;
	SeqHistPtr shp;
	SeqIdPtr sip;

	if ((entry == NULL) || (accn == NULL))
		return FALSE;

	if (! IS_Bioseq(entry))
		return FALSE;

	bsp = (BioseqPtr)(entry->data.ptrvalue);
	if (bsp == NULL)
		return FALSE;

	vnp = GetDescrOnSeqEntry (entry, Seq_descr_genbank);
	if (vnp == NULL) {
		vnp = NewDescrOnSeqEntry (entry, Seq_descr_genbank);
		if (vnp != NULL) {
			vnp->data.ptrvalue = (Pointer) GBBlockNew ();
		}
	}
	if (vnp == NULL) return FALSE;
	gbp = (GBBlockPtr) vnp->data.ptrvalue;
	if (gbp == NULL) return FALSE;

	shp = bsp->hist; 

	p = accn;
	for (i = 0; isalnum(*p) && *p != '\0'; ++p, ++i) continue;

               /* check one_letter+5digits or two_letter+6digits */
       if (i == 6 || i == 8)
       {
          if (!isalpha(accn[0]) || (!(isdigit(accn[1]) && i == 6) &&
              !(isalpha(accn[1]) && i == 8)))
          {
             ErrPostEx(SEV_ERROR,0,0,
 "Invalid accession (one_letter+5digits or two_letter+6digits): %s",
                                                           accn);
             return FALSE;
          }

          for (j = 2; j < i; ++j)
          {
              if (!(isdigit(accn[j])))
              {
                 ErrPostEx(SEV_ERROR,0,0,
 "Invalid accession (one_letter+5digits or two_letter+6digits): %s",
                                                           accn);
                 return FALSE;
              }
          }

          ValNodeCopyStr (&gbp->extra_accessions, 0, accn);
          sip = SubutilMakeAc2GBSeqId (accn);
          if (shp == NULL)
          {
             shp = SeqHistNew();
             bsp->hist = shp;
          }
          ValNodeLink (&shp->replace_ids, sip);
       }
       else
       {
          ErrPostEx(SEV_ERROR,0,0,
 "Invalid accession (one_letter+5digits or two_letter+6digits): %s",
                                                           accn);
          return FALSE;
       }

	return TRUE;
}

NLM_EXTERN Boolean AddTitleToEntry (
	NCBISubPtr submission,
	SeqEntryPtr entry ,
	CharPtr title )
{
	ValNodePtr vnp;

	if ((submission == NULL) || (entry == NULL) || (title == NULL))
		return FALSE;

	vnp = NewDescrOnSeqEntry (entry, Seq_descr_title);

	vnp->data.ptrvalue = (Pointer)(StringSaveNoNull(title));
	StripNonAsciiChars((CharPtr)(vnp->data.ptrvalue));
	return TRUE;
}

NLM_EXTERN Boolean AddCommentToEntry (
	NCBISubPtr submission,
	SeqEntryPtr entry ,
	CharPtr comment )
{
	ValNodePtr vnp;

	if ((submission == NULL) || (entry == NULL) || (comment == NULL))
		return FALSE;

	vnp = NewDescrOnSeqEntry (entry, Seq_descr_comment);

	vnp->data.ptrvalue = (Pointer)(StringSaveNoNull(comment));
	StripNonAsciiChars((CharPtr)(vnp->data.ptrvalue));
	return TRUE;
}

NLM_EXTERN Boolean AddOrganismToEntry (
	NCBISubPtr submission,
	SeqEntryPtr entry ,
	CharPtr scientific_name ,
	CharPtr common_name ,
	CharPtr virus_name ,
	CharPtr strain ,
	CharPtr synonym1,
	CharPtr synonym2,
	CharPtr synonym3)
{
	ValNodePtr vnp;
	OrgRefPtr orp;
	Char buf[80];

	if ((submission == NULL) || (entry == NULL))
		return FALSE;

	vnp = NewDescrOnSeqEntry (entry, Seq_descr_org);

	orp = OrgRefNew();
	vnp->data.ptrvalue = (Pointer)orp;

	orp->taxname = StringSaveNoNull(scientific_name);
	orp->common = StringSaveNoNull(common_name);
	if (virus_name != NULL)    /* kludge for old Org-ref */
	{
		orp->taxname = StringSaveNoNull(virus_name);
		ValNodeCopyStr(&orp->syn, 0, scientific_name);
	}

	if (strain != NULL)
	{
		sprintf(buf, "strain=%.70s", strain);
		ValNodeCopyStr(&orp->mod, 0, buf);
	}

	ValNodeCopyStr(&orp->syn, 0, synonym1);
	ValNodeCopyStr(&orp->syn, 0, synonym2);
	ValNodeCopyStr(&orp->syn, 0, synonym3);

 	return TRUE;
}

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
	Int4 taxid)

{
	ValNodePtr vnp;
	BioSourcePtr bio;
	OrgRefPtr orp;
	OrgModPtr orm;
	OrgNamePtr onp = NULL;
	DbtagPtr dbt;
	ObjectIdPtr oip;
	
	if ((submission == NULL) || (entry == NULL))
		return FALSE;

	vnp = GetDescrOnSeqEntry (entry, Seq_descr_source);

	bio = vnp->data.ptrvalue;
	if (bio == NULL) {
		bio = BioSourceNew();
	}
	if ((orp = bio->org) == NULL)
		orp = OrgRefNew();

	orp->taxname = StringSaveNoNull(scientific_name);
	orp->common = StringSaveNoNull(common_name);
	if (virus_name != NULL)    /* kludge for old Org-ref */
	{
		orp->taxname = StringSaveNoNull(virus_name);
		ValNodeCopyStr(&orp->syn, 0, scientific_name);
	}

	if (taxonomy != NULL) {
		onp = OrgNameNew();
		onp->lineage = StringSave(taxonomy);
	}
	if (strain != NULL)
	{
		orm = OrgModNew();
		orm->subtype = 2;  /* strain */
		orm->subname = StringSave(strain);
		if (onp == NULL) {
			onp = OrgNameNew();
		}
		onp->mod = orm;
	}
	if (onp != NULL) {
		orp->orgname = onp;
	}
	ValNodeCopyStr(&orp->syn, 0, synonym1);
	ValNodeCopyStr(&orp->syn, 0, synonym2);
	ValNodeCopyStr(&orp->syn, 0, synonym3);
	bio->org = orp;
	vnp->data.ptrvalue = (Pointer)bio;

	if (taxid > 0) {
		dbt = DbtagNew ();
		if (dbt != NULL) {
			oip = ObjectIdNew ();
			if (oip != NULL) {
				dbt->db = StringSave ("taxon");
				dbt->tag = oip;
				oip->id = taxid;
				ValNodeAddPointer (&orp->db, 0, (Pointer) dbt);
			}
		}
	}

 	return TRUE;
}

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
	CharPtr taxonomy)
{
	return AddOrganismToEntryEx (submission, entry, scientific_name,
                                 common_name, virus_name, strain,
                                 synonym1, synonym2, synonym3,
                                 taxonomy, 0);
}

NLM_EXTERN Boolean SetGeneticCodeForEntry (
	NCBISubPtr submission,
        SeqEntryPtr entry,
        Int2 genetic_code,  /* for cytoplasm */
        Int2 mito_code )   /* for mitochondria */
{
	ValNodePtr vnp;
	BioSourcePtr bio;
	OrgRefPtr orp;
	OrgNamePtr onp = NULL;
	
	if ((submission == NULL) || (entry == NULL))
		return FALSE;

	vnp = GetDescrOnSeqEntry (entry, Seq_descr_source);

	bio = vnp->data.ptrvalue;
	if (bio == NULL) {
		bio = BioSourceNew();
		vnp->data.ptrvalue = bio;
	}
	if ((orp = bio->org) == NULL)
	{
		orp = OrgRefNew();
		bio->org = orp;
	}

	if ((onp = orp->orgname) == NULL)
	{
		onp = OrgNameNew();
		orp->orgname = onp;
	}

	onp->gcode = genetic_code;
	onp->mgcode = mito_code;

	return TRUE;
}

NLM_EXTERN Boolean AddGenBankBlockToEntry (
	NCBISubPtr submission,
	SeqEntryPtr entry ,
	CharPtr taxonomy ,
	CharPtr division ,
	CharPtr keyword1 ,
	CharPtr keyword2 ,
	CharPtr keyword3 )
{
	ValNodePtr vnp;
	GBBlockPtr gbp;

	if ((submission == NULL) || (entry == NULL))
		return FALSE;

	vnp = NewDescrOnSeqEntry (entry, Seq_descr_genbank);

	gbp = GBBlockNew();
	vnp->data.ptrvalue = (Pointer)gbp;

	gbp->taxonomy = StringSaveNoNull(taxonomy);
	gbp->div = StringSaveNoNull(division);

	ValNodeCopyStr(&gbp->keywords, 0, keyword1);
	ValNodeCopyStr(&gbp->keywords, 0, keyword2);
	ValNodeCopyStr(&gbp->keywords, 0, keyword3);

	return TRUE;
}

NLM_EXTERN Boolean AddGIBBmethodToEntry (
	NCBISubPtr submission,
	SeqEntryPtr entry ,
	Int2	 method )
{
	ValNodePtr vnp;

	if ((submission == NULL) || (entry == NULL))
		return FALSE;

	vnp = NewDescrOnSeqEntry (entry, Seq_descr_method);

	vnp->data.intvalue = method;

	return TRUE;
}

NLM_EXTERN Boolean AddGenomeToEntry (
	NCBISubPtr submission,
	SeqEntryPtr entry ,
	Int2	 type )
{
	ValNodePtr vnp;
	BioSourcePtr bio;

	if ((submission == NULL) || (entry == NULL))
		return FALSE;

	vnp = GetDescrOnSeqEntry (entry, Seq_descr_source);

	bio = (BioSourcePtr) vnp->data.ptrvalue;
	if (bio == NULL) {
		bio = BioSourceNew();
	}
	bio->genome = (Uint1)type;
	bio->origin = 0;               /* unknown */
	vnp->data.ptrvalue = (Pointer) bio;
	
	return TRUE;
}

NLM_EXTERN Boolean AddSubSourceToEntry (
	NCBISubPtr submission,
	SeqEntryPtr entry ,
	Int2	 type ,
	CharPtr value)
{
	BioSourcePtr bio;
	SubSourcePtr curr, tmp;
	ValNodePtr vnp;

	if ((submission == NULL) || (entry == NULL))
		return FALSE;

	vnp = GetDescrOnSeqEntry (entry, Seq_descr_source);

	bio = (BioSourcePtr) vnp->data.ptrvalue;
	if (bio == NULL) {
		return FALSE;
	}
	tmp = SubSourceNew();
        if (bio->subtype == NULL)
		bio->subtype = tmp;
	else
	{
		for (curr = bio->subtype; curr->next != NULL; curr = curr->next)
			continue;
		curr->next = tmp;
	}

	tmp->subtype = (Uint1)type;
	tmp->name = StringSaveNoNull(value);
	
	return TRUE;
}


NLM_EXTERN Boolean AddOrgModToEntry (
	NCBISubPtr submission,
	SeqEntryPtr entry ,
	Int2	 type ,
	CharPtr value)
{
	BioSourcePtr bio;
	OrgRefPtr orp;
	OrgNamePtr onp;
	OrgModPtr curr, tmp;
	ValNodePtr vnp;

	if ((submission == NULL) || (entry == NULL))
		return FALSE;

	vnp = GetDescrOnSeqEntry (entry, Seq_descr_source);

	bio = (BioSourcePtr) vnp->data.ptrvalue;
	if (bio == NULL) {
		return FALSE;
	}
	if (bio->org == NULL)
	{
		return FALSE;
	}
	orp = bio->org;
	if (orp->orgname == NULL) {
		orp->orgname = OrgNameNew ();
	}
	if (orp->orgname == NULL)
		return FALSE;
	onp = orp->orgname;
	tmp = OrgModNew();
        if (onp->mod == NULL)
		onp->mod = tmp;
	else
	{
		for (curr = onp->mod; curr->next != NULL; curr = curr->next)
			continue;
		curr->next = tmp;
	}

	tmp->subtype = (Uint1)type;
	tmp->subname = StringSaveNoNull(value);
	
	return TRUE;
}

NLM_EXTERN Boolean AddBiomolToEntry (
	NCBISubPtr submission,
	SeqEntryPtr entry ,
	Int2	 type )
{
	ValNodePtr vnp;
	MolInfoPtr mip;

	if ((submission == NULL) || (entry == NULL))
		return FALSE;

	vnp = GetDescrOnSeqEntry (entry, Seq_descr_molinfo);

	mip = (MolInfoPtr) vnp->data.ptrvalue;
	if (mip == NULL) 
		mip = MolInfoNew();
	mip->biomol = (Uint1)type;
	vnp->data.ptrvalue =  (Pointer) mip;
	
	return TRUE;
}

NLM_EXTERN Boolean AddTechToEntry (
	NCBISubPtr submission,
	SeqEntryPtr entry ,
	Int2	 tech )
{
	ValNodePtr vnp;
	MolInfoPtr mip;

	if ((submission == NULL) || (entry == NULL))
		return FALSE;

	vnp = GetDescrOnSeqEntry (entry, Seq_descr_molinfo);

	mip = (MolInfoPtr) vnp->data.ptrvalue;
	if (mip == NULL) 
		mip = MolInfoNew();
	mip->tech = (Uint1)tech;
	vnp->data.ptrvalue = (Pointer) mip;
	
	return TRUE;
}

NLM_EXTERN Boolean AddCompleteToEntry (
	NCBISubPtr submission,
	SeqEntryPtr entry ,
	Int2	 complete )
{
	ValNodePtr vnp;
	MolInfoPtr mip;

	if ((submission == NULL) || (entry == NULL))
		return FALSE;

	vnp = GetDescrOnSeqEntry (entry, Seq_descr_molinfo);

	mip = (MolInfoPtr) vnp->data.ptrvalue;
	if (mip == NULL) 
		mip = MolInfoNew();
	mip->completeness = (Uint1)complete;
	vnp->data.ptrvalue = (Pointer) mip;
	
	return TRUE;
}

NLM_EXTERN void AddCompleteness(NCBISubPtr submission, SeqEntryPtr sep, SeqFeatPtr sfp)
{
	Uint2	retval;
	Boolean	partial = FALSE;

	retval = SeqLocPartialCheck(sfp->location);
	if (retval & SLP_START) {
		AddCompleteToEntry(submission, sep, 3);   /* no_left */
		partial = TRUE;
	}
	if (retval & SLP_STOP) {
		AddCompleteToEntry(submission, sep, 4);  /* no_right */
		partial = TRUE;
	}
	if (retval & (SLP_OTHER | SLP_INTERNAL)) {
		AddCompleteToEntry(submission, sep, 2);  /* partial */
		partial = TRUE;
	}
	if (!partial && sfp->partial) {
		AddCompleteToEntry(submission, sep, 2);  /* partial */
	}
}

NLM_EXTERN Boolean AddCreateDateToEntry (
	NCBISubPtr submission,
	SeqEntryPtr entry ,
	Int2 month ,
	Int2 day ,
	Int2 year )
{
	ValNodePtr vnp;
	DatePtr dp;

	if ((submission == NULL) || (entry == NULL))
		return FALSE;

	vnp = NewDescrOnSeqEntry (entry, Seq_descr_create_date);

	dp = DateNew();
	DateWrite(dp, year, month, day, NULL);
	vnp->data.ptrvalue = (Pointer)dp;

	return TRUE;
}

NLM_EXTERN Boolean AddModifierToEntry (
	NCBISubPtr submission,
	SeqEntryPtr entry ,
	Int2 modifier )
{
	ValNodePtr vnp, tmp;
	BioseqPtr bsp;
	BioseqSetPtr bssp;

	if ((submission == NULL) || (entry == NULL))
		return FALSE;

	if (IS_Bioseq(entry))
	{
		bsp = (BioseqPtr)(entry->data.ptrvalue);
		tmp = bsp->descr;
	}
	else
	{
		bssp = (BioseqSetPtr)(entry->data.ptrvalue);
		tmp = bssp->descr;
	}

	vnp = NULL;
	while (tmp != NULL)
	{
		if (tmp->choice == Seq_descr_modif)  /* have one already */
		{
			vnp = SeqDescrNew((ValNodePtr)(tmp->data.ptrvalue));
			break;
		}
		tmp = tmp->next;
	}

	if (vnp == NULL)   /* first one */
	{
		tmp = NewDescrOnSeqEntry (entry, Seq_descr_modif);
		vnp = SeqDescrNew((ValNodePtr)(tmp->data.ptrvalue));
		tmp->data.ptrvalue = (Pointer)vnp;
	}

	vnp->data.intvalue = (Int4)modifier;

	return TRUE;
}

NLM_EXTERN Boolean AddPubToEntry (
	NCBISubPtr submission,
	SeqEntryPtr entry ,
	PubPtr pub )
{
	ValNodePtr vnp;
	PubdescPtr pdp;

	if ((submission == NULL) || (entry == NULL) || (pub == NULL))
		return FALSE;

	vnp = NewDescrOnSeqEntry (entry, Seq_descr_pub);
	pdp = PubdescNew();
	if (pub->choice == PUB_Equiv)   /* already a Pub-equiv */
	{
		pdp->pub = (ValNodePtr)(pub->data.ptrvalue);
		MemFree(pub);
	}
	else                     /* make a Pub-equiv of one member */
		pdp->pub = pub;
	vnp->data.ptrvalue = (Pointer)pdp;

	return TRUE;
}
NLM_EXTERN PubPtr CitSubBuild (NCBISubPtr submission, Int2 month,
                               Int2 day, Int2 year, Int2 medium)
{
    ValNodePtr vnp;
    CitSubPtr csp;
    
    vnp = ValNodeNew(NULL);
    vnp->choice = PUB_Sub;   /* Cit-sub */
    csp = CitSubNew();
    vnp->data.ptrvalue = (Pointer)csp;
    
    csp->medium = (Uint1)medium;
    csp->date = DateNew();
    DateWrite(csp->date, year, month, day, NULL);
    return (PubPtr) vnp;
}

NLM_EXTERN PubPtr CitSubUpdateBuild (NCBISubPtr submission, Int2 month,
                                     Int2 day, Int2 year,Int2 medium , 
                                     CharPtr descr )  
     /* description of update, make it short */
{
    ValNodePtr vnp;
    CitSubPtr csp;
    
    vnp = ValNodeNew(NULL);
    vnp->choice = PUB_Sub;   /* Cit-sub */
    csp = CitSubNew();
    vnp->data.ptrvalue = (Pointer)csp;
    
    csp->medium = (Uint1)medium;
    csp->descr = StringSave(descr);
    csp->date = DateNew();
    DateWrite(csp->date, year, month, day, NULL);
    return vnp;
}

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
                               Int2 status )
{
    ValNodePtr vnp, tmp;
    CitArtPtr cap;
    CitJourPtr cjp;
    CitGenPtr cgp;
    ImprintPtr  ip;

    vnp = ValNodeNew(NULL);
    
    if (status == PUB_STATUS_UNPUBLISHED) {  /* can't do a Cit-art */
        vnp->choice = PUB_Gen;   /* Cit-gen */
        cgp = CitGenNew();
        vnp->data.ptrvalue = cgp;
        cgp->cit = StringSaveNoNull("Unpublished");
        cgp->title = StringSaveNoNull(title);
        tmp = ValNodeCopyStr (&cgp->journal, Cit_title_jta, journal);
        cgp->volume = StringSaveNoNull(volume);
        cgp->issue = StringSaveNoNull(issue);
        cgp->pages = StringSaveNoNull(pages);
        if (year > 0) {
            cgp->date = DateNew();
            DateWrite(cgp->date, year, month, day, NULL);
        }
    } else {                             /* regular pub */
        
        vnp->choice = PUB_Article;   /* Cit-art */
        cap = CitArtNew();
        vnp->data.ptrvalue = cap;
        cap->from = 1;     /* from journal only */
        tmp = ValNodeCopyStr (&cap->title, Cit_title_name, title);
        cjp = CitJourNew();
        cap->fromptr = (Pointer)cjp;
        tmp = ValNodeCopyStr (&cjp->title, Cit_title_jta, journal);
        ip = ImprintNew();
        cjp->imp = ip;

        if (year > 0) {
            ip->date = DateNew();
            DateWrite(ip->date, year, month, day, NULL);
        } else
            ip->date = DateCurr();
        
        ip->volume = StringSaveNoNull(volume);
        ip->issue = StringSaveNoNull(issue);
        ip->pages = StringSaveNoNull(pages);

        if (status == PUB_STATUS_SUBMITTED)
            ip->prepub = (Uint1)1;
        else if (status == PUB_STATUS_IN_PRESS)
            ip->prepub = (Uint1)2;
    }
    return vnp;
}

NLM_EXTERN Boolean AddAuthorToPub (    /* call once for each author, in order */
	NCBISubPtr submission,
	PubPtr the_pub,
	CharPtr last_name,
	CharPtr first_name,
	CharPtr middle_name,
	CharPtr initials,  /* separated by periods, no initial for last name */
	CharPtr suffix )   /* Jr. Sr. III */
{
	CitGenPtr cgp;
	CitSubPtr csp;
	CitArtPtr cap;
	AuthListPtr alp;
	AuthorPtr ap;
	PersonIdPtr pip;
	NameStdPtr nsp;
	ValNodePtr vnp;

	if ((submission == NULL) || (the_pub == NULL) || (last_name == NULL))
		return FALSE;

	switch (the_pub->choice)
	{
		case PUB_Sub:
			csp = (CitSubPtr)(the_pub->data.ptrvalue);
			if (csp->authors == NULL)
			{
				csp->authors = AuthListNew();
				csp->authors->choice = 1;   /* std */
			}
			alp = csp->authors;
			break;
		case PUB_Gen:
			cgp = (CitGenPtr)(the_pub->data.ptrvalue);
			if (cgp->authors == NULL)
			{
				cgp->authors = AuthListNew();
				cgp->authors->choice = 1;   /* std */
			}
			alp = cgp->authors;
			break;
		case PUB_Article:
			cap = (CitArtPtr)(the_pub->data.ptrvalue);
			if (cap->authors == NULL)
			{
				cap->authors = AuthListNew();
				cap->authors->choice = 1;   /* std */
			}
			alp = cap->authors;
			break;
		default:
			Message(MSG_ERROR, "AddAuthorToPub: Unsupported Pub type [%d]",
				(int)the_pub->choice);
			return FALSE;
	}

	nsp = NameStdNew();
	nsp->names[0] = StringSaveNoNull(last_name);
	nsp->names[1] = StringSaveNoNull(first_name);
	nsp->names[2] = StringSaveNoNull(middle_name);
	nsp->names[4] = StringSaveNoNull(initials);
	nsp->names[5] = StringSaveNoNull(suffix);

	pip = PersonIdNew();
	pip->choice = 2;   /* name */
	pip->data = (Pointer)nsp;

	ap = AuthorNew();
	ap->name = pip;

	vnp = ValNodeAdd(&alp->names);
	vnp->data.ptrvalue = (Pointer)ap;

	return TRUE;
}

NLM_EXTERN Boolean AddAffiliationToPub (  /* call once per pub */
	NCBISubPtr submission,
	PubPtr the_pub,
	CharPtr affil,        /* e.g. "Xyz University" */
	CharPtr div,          /* e.g. "Dept of Biology" */
	CharPtr street,       /* e.g. "123 Academic Road" */
	CharPtr city,         /* e.g. "Metropolis" */
	CharPtr sub,          /* e.g. "Massachusetts" */
	CharPtr country,      /* e.g. "USA" */
	CharPtr postal_code ) /* e.g. "01234" */
{
	CitGenPtr cgp;
	CitSubPtr csp;
	CitArtPtr cap;
	AuthListPtr alp;
	AffilPtr ap;

	if ((submission == NULL) || (the_pub == NULL))
		return FALSE;

	switch (the_pub->choice)
	{
		case PUB_Sub:
			csp = (CitSubPtr)(the_pub->data.ptrvalue);
			if (csp->authors == NULL)
			{
				csp->authors = AuthListNew();
				csp->authors->choice = 1;   /* std */
			}
			alp = csp->authors;
			break;
		case PUB_Gen:
			cgp = (CitGenPtr)(the_pub->data.ptrvalue);
			if (cgp->authors == NULL)
			{
				cgp->authors = AuthListNew();
				cgp->authors->choice = 1;   /* std */
			}
			alp = cgp->authors;
			break;
		case PUB_Article:
			cap = (CitArtPtr)(the_pub->data.ptrvalue);
			if (cap->authors == NULL)
			{
				cap->authors = AuthListNew();
				cap->authors->choice = 1;   /* std */
			}
			alp = cap->authors;
			break;
		default:
			Message(MSG_ERROR, "AddAffilToPub: Unsupported Pub type [%d]",
				(int)the_pub->choice);
			return FALSE;
	}

	if (alp->affil != NULL)
	{
		Message(MSG_ERROR, "AddAffilToPub: Pub already has affil");
		return FALSE;
	}

	ap = AffilNew();
    alp->affil = ap;
    if (affil != NULL && div == NULL && city == NULL && sub == NULL
    	&& country == NULL && street == NULL && postal_code == NULL) {
		ap->choice = 1;    /* str affil */
		ap->affil = StringSaveNoNull(affil);
    } else {
		ap->choice = 2;    /* std affil */
		ap->affil = StringSaveNoNull(affil);
		ap->div = StringSaveNoNull(div);
		ap->city = StringSaveNoNull(city);
		ap->sub = StringSaveNoNull(sub);
		ap->country = StringSaveNoNull(country);
		ap->street = StringSaveNoNull(street);
		ap->postal_code = StringSaveNoNull(postal_code);
	}

	return TRUE;
}


/*****************************************************************************
*
*   Add Features to the entry
*
*****************************************************************************/
static Boolean AddFeatureToEntry (
	NCBISubPtr submission,
	SeqEntryPtr entry ,
	SeqFeatPtr feature )
{
	SeqFeatPtr sfp;
	SeqAnnotPtr sap;
	BioseqPtr bsp;
	BioseqSetPtr bssp;

	if ((submission == NULL) || (entry == NULL) || (feature == NULL))
		return FALSE;

	if (IS_Bioseq(entry))
	{
		bsp = (BioseqPtr)(entry->data.ptrvalue);
		for (sap = bsp->annot; sap != NULL; sap = sap->next)
		{
			if (sap->type == 1)   /* feature table */
				break;
		}
		if (sap == NULL)
		{
			sap = SeqAnnotNew();
			sap->type = 1;
			bsp->annot = sap;
		}
	}
	else
	{
		bssp = (BioseqSetPtr)(entry->data.ptrvalue);
		for (sap = bssp->annot; sap != NULL; sap = sap->next)
		{
			if (sap->type == 1)   /* feature table */
				break;
		}
		if (sap == NULL)
		{
			sap = SeqAnnotNew();
			sap->type = 1;
			bssp->annot = sap;
		}
	}

	sfp = (SeqFeatPtr)(sap->data);
	if (sfp == NULL)
		sap->data = (Pointer)feature;
	else
	{
		while (sfp->next != NULL)
			sfp = sfp->next;
		sfp->next = feature;
	}

	return TRUE;
}

NLM_EXTERN SeqFeatPtr FeatureBuild (
	NCBISubPtr submission,
	SeqEntryPtr entry_to_put_feature,
	Boolean feature_is_partial,
	Uint1 evidence_is_experimental,
	Boolean biological_exception,
	CharPtr comment )
{
	SeqFeatPtr sfp;
	static char * ev [2] = {
		"experimental", "not_experimental" };

	if (entry_to_put_feature == NULL) return NULL;

	sfp = SeqFeatNew();

	sfp->partial = feature_is_partial;
	if ((evidence_is_experimental == EVIDENCE_EXPERIMENTAL)
		|| (evidence_is_experimental == EVIDENCE_NOT_EXPERIMENTAL))
	{
		sfp->exp_ev = evidence_is_experimental;
	}
	sfp->excpt = biological_exception;
	if (comment != NULL)
	{
		if (*comment != '\0')
			sfp->comment = StringSaveNoNull(comment);
	}

	AddFeatureToEntry(submission, entry_to_put_feature, sfp);

	return sfp;
}


NLM_EXTERN Boolean AddIntervalToFeature (
	NCBISubPtr submission,
	SeqFeatPtr sfp,
	SeqEntryPtr the_seq ,
	CharPtr local_name ,
	Int4 from ,
	Int4 to ,
	Boolean on_plus_strand ,
	Boolean start_before_from ,
	Boolean stop_after_to )
{
	BioseqPtr bsp;
	Int4 tmp_from, tmp_to;
	Int2 tmp_fuzz_from, tmp_fuzz_to, fuzz_from, fuzz_to, strand;


	if (sfp == NULL) return FALSE;

	bsp = GetBioseqFromChoice(submission, the_seq, local_name, "AddIntervalToFeature");
	if (bsp == NULL) return FALSE;

	if (to == -1)
		to = BioseqGetLen(bsp);

	    /** allow to input numbering from 1 */

	to -= 1;
	from -= 1;
	fuzz_from = -1;     /* not-set */
	fuzz_to = -1;
	if (! on_plus_strand)
	{
		strand = Seq_strand_minus;
		tmp_from = from;
		from = to;
		to = tmp_from;
		if (start_before_from)
			fuzz_to = 1;    /* gt */
		if (stop_after_to)
			fuzz_from = 2;  /* lt */
	}
	else
	{
		strand = 0;
		if (start_before_from)
			fuzz_from = 2;    /* lt */
		if (stop_after_to)
			fuzz_to = 1;  /* gt */

	}

	if (to < from)    /* go around origin on circular sequence */
	{
		if ((bsp->topology != 2) && (bsp->topology != 3))
		{
			Message(MSG_ERROR, "Attempt to go around origin of non-circular sequence");
			return FALSE;
		}
		tmp_fuzz_from = -1;
		tmp_fuzz_to = -1;

		if (on_plus_strand)
		{
			tmp_from = from;
			tmp_to = (BioseqGetLen(bsp) - 1);
			from = 0;
			tmp_fuzz_from = fuzz_from;
			fuzz_from = -1;
		}
		else
		{
			tmp_from = 0;
			tmp_to = to;
			to = (BioseqGetLen(bsp) - 1);
			tmp_fuzz_to = fuzz_to;
			fuzz_to = -1;
		}

		if (! AddIntToSeqFeat(sfp, tmp_from, tmp_to, bsp, tmp_fuzz_from, tmp_fuzz_to, strand))
			return FALSE;
	}

	return AddIntToSeqFeat(sfp, from, to, bsp, fuzz_from, fuzz_to, strand);
}

NLM_EXTERN Boolean AddIntToSeqFeat (SeqFeatPtr sfp, Int4 from, Int4 to, BioseqPtr bsp,
							Int2 fuzz_from, Int2 
fuzz_to, Int2 strand)
{
	SeqLocPtr slp, tmp, tmp2;
	SeqIntPtr sintp;
	IntFuzzPtr ifp;

	sintp = SeqIntNew();
	sintp->from = from;
	sintp->to = to;
	sintp->id = SeqIdDup(SeqIdFindBest(bsp->id, 0));
	sintp->strand = (Uint1)strand;
	if (fuzz_from >= 0)
	{
		ifp = IntFuzzNew();
		ifp->choice = 4;   /* lim */
		ifp->a = (Int4)fuzz_from;
		sintp->if_from = ifp;
	}
	if (fuzz_to >= 0)
	{
		ifp = IntFuzzNew();
		ifp->choice = 4;   /* lim */
		ifp->a = (Int4)fuzz_to;
		sintp->if_to = ifp;
	}

	slp = ValNodeNew(NULL);
	slp->choice = SEQLOC_INT;
	slp->data.ptrvalue = (Pointer)sintp;

	if (sfp->location == NULL)
	{
		sfp->location = slp;
		return TRUE;
	}

	tmp = sfp->location;
	if (tmp->choice == SEQLOC_MIX)   /* second one already */
	{
		tmp2 = (ValNodePtr)(tmp->data.ptrvalue);
		while (tmp2->next != NULL)
			tmp2 = tmp2->next;
		tmp2->next = slp;
	}
	else                             /* create a chain */
	{
		tmp2 = ValNodeNew(NULL);
		tmp2->choice = SEQLOC_MIX;
		tmp2->data.ptrvalue = (Pointer)tmp;
		tmp->next = slp;
		sfp->location = tmp2;
	}

	return TRUE;
}

static BioseqPtr GetBioseqFromChoice (NCBISubPtr nsp, SeqEntryPtr the_seq, CharPtr local_name, CharPtr the_routine)
{
	BioseqPtr bsp;
	ValNode vn;
	ObjectId oid;
	Dbtag dbt;

	if (the_seq != NULL)
	{
		if (! IS_Bioseq(the_seq))
		{
			Message(MSG_ERROR, "%s: Gave Seq-entry which is not a Bioseq",
				the_routine);
			return NULL;
		}
		bsp = (BioseqPtr)(the_seq->data.ptrvalue);
	}
	else if (local_name != NULL)
	{
		vn.next = NULL;
		oid.str = local_name;
		if (nsp->submittor_key == NULL)
		{
			vn.choice = SEQID_LOCAL;
			vn.data.ptrvalue = (Pointer)(& oid);
		}
		else
		{
			vn.choice = SEQID_GENERAL;
			vn.data.ptrvalue = (Pointer)(& dbt);
			dbt.db = nsp->submittor_key;
			dbt.tag = & oid;
		}

		bsp = BioseqFind((SeqIdPtr)(& vn));
		if (bsp == NULL)
		{
			Message(MSG_ERROR, "%s: Can't find Bioseq [%s]",
				the_routine, local_name);
			return NULL;
		}

	}
	else
	{
		Message(MSG_ERROR, "%s: No the_seq or local_name given",
			the_routine);
		return NULL;
	}

	return bsp;
}

NLM_EXTERN Boolean AddPointToFeature (
	NCBISubPtr submission,
	SeqFeatPtr sfp,
	SeqEntryPtr the_seq ,
	CharPtr local_name ,
	Int4 location ,
	Boolean on_plus_strand ,
	Boolean is_after_location ,
	Boolean is_before_location )
{
	BioseqPtr bsp;
	SeqLocPtr slp, tmp, tmp2;
	SeqPntPtr spp;
	IntFuzzPtr ifp;


	if (sfp == NULL) return FALSE;

	bsp = GetBioseqFromChoice(submission, the_seq, local_name, "AddPointToFeature");
	if (bsp == NULL) return FALSE;

	spp = SeqPntNew();
	if (location == -1)
		location = BioseqGetLen(bsp);
	spp->point = location - 1;
	
	spp->id = SeqIdDup(SeqIdFindBest(bsp->id, 0));
	if (! on_plus_strand)
		spp->strand = Seq_strand_minus;
	if (is_before_location)
	{
		ifp = IntFuzzNew();
		ifp->choice = 4;   /* lim */
		ifp->a = 2;        /* lt */
		spp->fuzz = ifp;
	}
	else if (is_after_location)
	{
		ifp = IntFuzzNew();
		ifp->choice = 4;   /* lim */
		ifp->a = 1;        /* gt */
		spp->fuzz = ifp;
	}

	slp = ValNodeNew(NULL);
	slp->choice = SEQLOC_PNT;
	slp->data.ptrvalue = (Pointer)spp;

	if (sfp->location == NULL)
	{
		sfp->location = slp;
		return TRUE;
	}

	tmp = sfp->location;
	if (tmp->choice == SEQLOC_MIX)   /* second one already */
	{
		tmp2 = (ValNodePtr)(tmp->data.ptrvalue);
		while (tmp2->next != NULL)
			tmp2 = tmp2->next;
		tmp2->next = slp;
	}
	else                             /* create a chain */
	{
		tmp2 = ValNodeNew(NULL);
		tmp2->choice = SEQLOC_MIX;
		tmp2->data.ptrvalue = (Pointer)tmp;
		tmp->next = slp;
		sfp->location = tmp2;
	}
	return TRUE;
}

NLM_EXTERN Boolean MakeCommentFeature (
	NCBISubPtr submission,
	SeqFeatPtr feature )
{
	if (feature == NULL) return FALSE;

	if (feature->data.choice)
	{
		Message(MSG_ERROR, "MakeCommentFeature: feature already has data");
		return FALSE;
	}

	if (feature->comment == NULL)
	{
		Message(MSG_ERROR, "MakeCommentFeature: no comment provided to FeatureBuild");
		return FALSE;
	}

 	feature->data.choice = SEQFEAT_COMMENT;   /* comment */

	return TRUE;
}


NLM_EXTERN Boolean MakeCdRegionFeature (
	NCBISubPtr submission,
	SeqFeatPtr feature,
	Int2 frame ,
	Int2 genetic_code ,			
	SeqEntryPtr protein_product , 	 /* give id of protein. if NULL, call */
	CharPtr local_id_for_protein )  /* function below to create by transl */
{
	BioseqPtr bsp;
	CdRegionPtr crp;
	ValNodePtr vnp;
	Char buf[41];

	if (feature == NULL) return FALSE;

	if (feature->data.choice)
	{
		Message(MSG_ERROR, "MakeCdRegionFeature: feature already has data");
		return FALSE;
	}

	crp = CdRegionNew();
	crp->frame = (Uint1)frame;
	if (genetic_code)
	{
		crp->genetic_code = GeneticCodeNew();
		vnp = ValNodeAdd ((ValNodePtr PNTR)&(crp->genetic_code->data.ptrvalue));
		vnp->choice = 2;
		vnp->data.intvalue = (Int4)genetic_code;
	}

	feature->data.choice = 3;   /* cdregion */
	feature->data.value.ptrvalue = (Pointer)crp;

	if ((protein_product != NULL) || (local_id_for_protein != NULL))
	{
		bsp = GetBioseqFromChoice (submission, protein_product, local_id_for_protein,
			"MakeCdRegionFeature");
		if (bsp != NULL)
		{
			if (! ISA_aa(bsp->mol))
			{
				SeqIdWrite(bsp->id, buf, PRINTID_FASTA_LONG, 40);
				Message(MSG_ERROR, "Using non-protein [%s] for cdregion product",
					buf);
			}
			else
			{
				vnp = ValNodeNew(NULL);
				feature->product = vnp;
				vnp->choice = SEQLOC_WHOLE;
				vnp->data.ptrvalue = SeqIdDup(SeqIdFindBest(bsp->id, 0));
			}
		}
	}

	return TRUE;
}
           
		   
/******************************************************************
*
*   A Code-break allows an exception to be made in the translation
*    of a particular codon. You must give positions of the first
*    and last bases of the codon in the DNA sequence and the amino
*    acid to place there, instead of the normal translation. This
*    should be used sparingly, and a comment on the feature should
*    explain why it was done.
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
	CharPtr AA_for_protein )
{
	SeqFeat sf;
	CodeBreakPtr cbp, tmp;
	CdRegionPtr crp;


	if ((submission == NULL) || (sfp == NULL) || (AA_for_protein == NULL))
		return FALSE;
	if (sfp->data.choice != 3) return FALSE;

	if (on_plus_strand)
	{
		if ((to - from) != 2) return FALSE;
	}
	else
	{
		if ((from - to) != 2) return FALSE;
	}

	MemSet(&sf, 0, sizeof(SeqFeat));

	if (! AddIntervalToFeature(submission, &sf, the_seq, local_name, from, to, on_plus_strand, FALSE,FALSE))
		return FALSE;

	cbp = CodeBreakNew();

	cbp->loc = sf.location;
	cbp->aa.choice = 1;   /* ncbieaa */
	cbp->aa.value.intvalue = (Int4)TO_UPPER(*AA_for_protein);

	crp = (CdRegionPtr)(sfp->data.value.ptrvalue);
	if (crp->code_break == NULL)
		crp->code_break = cbp;
	else
	{
		for (tmp = crp->code_break; tmp->next != NULL; tmp = tmp->next)
			continue;
		tmp->next = cbp;
	}

	return TRUE;
}

           
		   
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
	Int4 gi_number )
{
	SeqEntryPtr sep;
	BioseqPtr bsp;
	ByteStorePtr bp;
	Int4 protlen;
	ValNodePtr vnp;
	Uint2	retval;
	Boolean	partial = FALSE;

	if ((cdregion_feature == NULL) || (nuc_prot_entry_to_put_sequence == NULL))
		return NULL;

	bp = ProteinFromCdRegion (cdregion_feature, FALSE);
	if (bp == NULL)
	{
		Message(MSG_ERROR, "TranslateCdRegion: Couldn't translate CdRegion");
		return NULL;
	}

	protlen = BSLen(bp);

	sep = AddSeqToNucProtEntry (submission , nuc_prot_entry_to_put_sequence,
		local_name, genbank_locus,
		genbank_accession, gi_number, MOLECULE_CLASS_PROTEIN,
		MOLECULE_TYPE_PEPTIDE ,	protlen, 0, 0);

	AddGIBBmethodToEntry(submission, sep, METHOD_concept_transl);

	retval = SeqLocPartialCheck(cdregion_feature->location);
	if (retval & SLP_START) {
		AddModifierToEntry(submission, sep, MODIF_no_left);
		partial = TRUE;
	}
	if (retval & SLP_STOP) {
		AddModifierToEntry(submission, sep, MODIF_no_right);
		partial = TRUE;
	}
	if (retval & (SLP_OTHER | SLP_INTERNAL)) {
		AddModifierToEntry(submission, sep, MODIF_partial);
		partial = TRUE;
	}
	if (!partial && cdregion_feature->partial) {
		AddModifierToEntry(submission, sep, MODIF_partial);
	}

	bsp = (BioseqPtr)(sep->data.ptrvalue);

	bsp->seq_data = bp;
	bsp->seq_data_type = Seq_code_iupacaa;

	vnp = ValNodeNew(NULL);
	cdregion_feature->product = vnp;
	vnp->choice = SEQLOC_WHOLE;
	vnp->data.ptrvalue = SeqIdDup(SeqIdFindBest(bsp->id, 0));

	return sep;
}


static SeqEntryPtr TranslateCdRegionNew (
	NCBISubPtr submission ,
	SeqFeatPtr cdregion_feature ,
	SeqEntryPtr nuc_prot_entry_to_put_sequence ,
	CharPtr local_name ,             /* for protein sequence */
	CharPtr genbank_locus ,
	CharPtr genbank_accession ,
	Int4 gi_number )
{
	SeqEntryPtr sep;
	BioseqPtr bsp;
	ByteStorePtr bp;
	Int4 protlen;
	ValNodePtr vnp;
	Boolean	partial = FALSE;

	if ((cdregion_feature == NULL) || (nuc_prot_entry_to_put_sequence == NULL))
		return NULL;

	bp = ProteinFromCdRegion (cdregion_feature, FALSE);
	if (bp == NULL)
	{
		Message(MSG_ERROR, "TranslateCdRegion: Couldn't translate CdRegion");
		return NULL;
	}

	protlen = BSLen(bp);

	sep = AddSeqToNucProtEntry (submission , nuc_prot_entry_to_put_sequence,
		local_name, genbank_locus,
		genbank_accession, gi_number, MOLECULE_CLASS_PROTEIN,
		MOLECULE_TYPE_PEPTIDE ,	protlen, 0, 0);

	AddBiomolToEntry(submission, sep, 8);  /* peptide */
	AddTechToEntry(submission, sep, 8); /* concept_transl */

	AddCompleteness(submission, sep, cdregion_feature);
	bsp = (BioseqPtr)(sep->data.ptrvalue);

	bsp->seq_data = bp;
	bsp->seq_data_type = Seq_code_iupacaa;

	vnp = ValNodeNew(NULL);
	cdregion_feature->product = vnp;
	vnp->choice = SEQLOC_WHOLE;
	vnp->data.ptrvalue = SeqIdDup(SeqIdFindBest(bsp->id, 0));

	return sep;
}


NLM_EXTERN Boolean MakeRNAFeature (
	NCBISubPtr submission,
	SeqFeatPtr feature,
	Int2 rna_type ,
	Boolean is_pseudo_gene,
	CharPtr rna_name ,
	CharPtr AA_for_tRNA ,
	CharPtr codon_for_tRNA )
{
	RnaRefPtr rrp;
	tRNAPtr trp;
	Int2 i, index, j, wobble;
	CharPtr tmp;
	Char base;

	if (feature == NULL) return FALSE;

	if (feature->data.choice)
	{
		Message(MSG_ERROR, "MakeRNAFeature: feature already has data");
		return FALSE;
	}

	rrp = RnaRefNew();
	feature->data.choice = 5;
	feature->data.value.ptrvalue = (Pointer)rrp;

	rrp->type = (Uint1) rna_type;
	rrp->pseudo = is_pseudo_gene;
	if (rna_name != NULL)
	{
		rrp->ext.choice = 1;
		rrp->ext.value.ptrvalue = (Pointer)StringSaveNoNull(rna_name);
	}
	else if (rna_type == RNA_TYPE_tRNA)
	{
		if ((AA_for_tRNA != NULL ) || (codon_for_tRNA != NULL))
		{
			trp = MemNew(sizeof(tRNA));
			rrp->ext.choice = 2;
			rrp->ext.value.ptrvalue = (Pointer)trp;
			for (i = 0; i < 6; i++)
				trp->codon[i] = 255;
			if (AA_for_tRNA != '\0')
			{
				trp->aatype = 1;    /* iupacaa */
				trp->aa = ValidAminoAcid(AA_for_tRNA);
		/*		trp->aa = (Uint1)TO_UPPER(*AA_for_tRNA);*/
			}
			if (codon_for_tRNA != NULL)
			{
				wobble = 1; /* assume no wobble */
				for (j = 0; j < wobble; j++)
				{
					tmp = codon_for_tRNA;
					index = 0;
					for (i = 16; i > 0; i /= 4, tmp++)
					{
						base = TO_UPPER(*tmp);
						switch (base)
						{
							case 'U':
								base = 'T';
								break;
							case 'R':
								if (j==0)
								 base = 'A';
								else
								 base = 'G';
								wobble = 2;
								break;
							case 'Y':
								if (j==0)
								 base = 'T';
								else
								 base = 'C';
								wobble = 2;
								break;
							default:
								break;
						}

						switch (base)
						{
							case 'T':
								break;    /* 0 */
							case 'C':
								index += (i * 1);
								break;
							case 'A':
								index += (i * 2);
								break;
							case 'G':
								index += (i * 3);
								break;
							default:
								Message(MSG_ERROR, "Invalid tRNA codon [%s]",
									codon_for_tRNA);
								return TRUE;
						}
					}
					trp->codon[j] = (Uint1)index;
				}
			}
		}
	}

	return TRUE;
}

           
		   
/******************************************************************
*
*  Once you have made a tRNA feature, you may optionally add 
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
	Boolean on_plus_strand )
{
	SeqFeat sf;
	RnaRefPtr rrp;
	tRNAPtr trp;

	if ((submission == NULL) || (sfp == NULL))
		return FALSE;
	if (sfp->data.choice != 5) return FALSE;

	rrp = (RnaRefPtr)(sfp->data.value.ptrvalue);
	if (rrp->type != RNA_TYPE_tRNA)
		return FALSE;

	MemSet(&sf, 0, sizeof(SeqFeat));

	if (! AddIntervalToFeature(submission, &sf, the_seq, local_name, from, to, on_plus_strand, FALSE,FALSE))
		return FALSE;

	if (rrp->ext.choice != 2)   /* not extension yet */
	{
		trp = MemNew(sizeof(tRNA));
		rrp->ext.choice = 2;
		rrp->ext.value.ptrvalue = (Pointer)trp;
	}
	else
		trp = (tRNAPtr)(rrp->ext.value.ptrvalue);

	trp->anticodon = sf.location;

	return TRUE;
}


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
	CharPtr synonym3 )
{
	GeneRefPtr grp;
	ValNodePtr vnp;
	DbtagPtr dbt;
	ObjectIdPtr oip;

	if (feature == NULL) return FALSE;

	if (feature->data.choice)
	{
		Message(MSG_ERROR, "MakeGeneFeature: feature already has data");
		return FALSE;
	}

	grp = GeneRefNew();
	feature->data.choice = 1;
	feature->data.value.ptrvalue = grp;

	grp->locus = StringSaveNoNull(gene_symbol_for_locus);
	grp->allele = StringSaveNoNull(allele);
	grp->desc = StringSaveNoNull(descriptive_name);
	grp->maploc = StringSaveNoNull(map_location);
	grp->pseudo = is_pseudo_gene;
	if ((genetic_database != NULL) && (gene_id_in_genetic_database != NULL))
	{
		vnp = ValNodeAdd(&grp->db);
		dbt = DbtagNew();
		vnp->data.ptrvalue = dbt;
		dbt->db = StringSaveNoNull(genetic_database);
		oip = ObjectIdNew();
		dbt->tag = oip;
		oip->str = StringSaveNoNull(gene_id_in_genetic_database);
	}

	ValNodeCopyStr(&grp->syn, 0, synonym1);
	ValNodeCopyStr(&grp->syn, 0, synonym2);
	ValNodeCopyStr(&grp->syn, 0, synonym3);

	return TRUE;
}

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
	CharPtr id_in_protein_database)
{
	ProtRefPtr prp;
	ValNodePtr vnp;
	DbtagPtr dbt;
	ObjectIdPtr oip;
	CharPtr tmp;
	Int2 i;

	if (feature == NULL) return FALSE;

	if (feature->data.choice)
	{
		Message(MSG_ERROR, "MakeProteinFeature: feature already has data");
		return FALSE;
	}

	prp = ProtRefNew();
	feature->data.choice = 4;
	feature->data.value.ptrvalue = prp;

	ValNodeCopyStr(&prp->name, 0, protein_name1);
	ValNodeCopyStr(&prp->name, 0, protein_name2);
	ValNodeCopyStr(&prp->name, 0, protein_name3);

	prp->desc = StringSaveNoNull(descriptive_name);

	tmp = ECnum1;
	for (i = 0; i < 2; i++)
	{
		if (tmp != NULL)
		{			/* skip any leading E.C. type stuff */
		 	while ((! IS_DIGIT(*tmp)))
			{
				if (*tmp == '\0') break;
				tmp++;
			}
			if (*tmp != '\0')
				ValNodeCopyStr(&prp->ec, 0, tmp);
		}
		tmp = ECnum2;
	}

	ValNodeCopyStr(&prp->activity, 0, activity1);
	ValNodeCopyStr(&prp->activity, 0, activity2);

	if ((protein_database != NULL) && (id_in_protein_database != NULL))
	{
		vnp = ValNodeAdd(&prp->db);
		dbt = DbtagNew();
		vnp->data.ptrvalue = dbt;
		dbt->db = StringSaveNoNull(protein_database);
		oip = ObjectIdNew();
		dbt->tag = oip;
		oip->str = StringSaveNoNull(id_in_protein_database);
	}

	return TRUE;
}

NLM_EXTERN Boolean MakeRegionFeature (
	NCBISubPtr submission,
	SeqFeatPtr feature ,
	CharPtr region_name )
{
	if (feature == NULL) return FALSE;

	if (feature->data.choice)
	{
		Message(MSG_ERROR, "MakeRegionFeature: feature already has data");
		return FALSE;
	}

	feature->data.choice = 9;
	feature->data.value.ptrvalue = (Pointer)StringSaveNoNull(region_name);
	return TRUE;
}

NLM_EXTERN Boolean MakeSiteFeature (
	NCBISubPtr submission,
	SeqFeatPtr feature ,
	Int2 site_type )
{
	if (feature == NULL) return FALSE;

	if (feature->data.choice)
	{
		Message(MSG_ERROR, "MakeSiteFeature: feature already has data");
		return FALSE;
	}

	feature->data.choice = 12;
	feature->data.value.intvalue = (Int4)site_type;
	return TRUE;
}

NLM_EXTERN Boolean MakeImpFeature (
	NCBISubPtr submission,
	SeqFeatPtr feature ,
	CharPtr key )
{
	ImpFeatPtr ifp;

	if (feature == NULL) return FALSE;

	if (feature->data.choice)
	{
		Message(MSG_ERROR, "MakeImpFeature: feature already has data");
		return FALSE;
	}

	ifp = ImpFeatNew();
	ifp->key = StringSaveNoNull(key);

	feature->data.choice = 8;
	feature->data.value.ptrvalue = (Pointer)ifp;
	return TRUE;
}

NLM_EXTERN Boolean AddQualToImpFeature (
	NCBISubPtr submission,
	SeqFeatPtr imp_feature ,
	CharPtr qualifier ,
	CharPtr value )
{
	GBQualPtr gbp, tmp;

	if ((imp_feature == NULL) || (qualifier == NULL))
		return FALSE;

	gbp = GBQualNew();
	gbp->qual = StringSaveNoNull(qualifier);
	gbp->val = StringSaveNoNull(value);

	if (imp_feature->qual == NULL)
		imp_feature->qual = gbp;
	else
	{
		for (tmp = imp_feature->qual; tmp->next != NULL; tmp = tmp->next)
			continue;
		tmp->next = gbp;
	}

	return TRUE;
}

NLM_EXTERN Boolean MakePubFeature (
	NCBISubPtr submission,
	SeqFeatPtr feature,
	PubPtr pub )
{

	PubdescPtr pdp;

	if (feature == NULL) return FALSE;

	if (feature->data.choice)
	{
		Message(MSG_ERROR, "MakePubFeature: feature already has data");
		return FALSE;
	}

	feature->data.choice = 6;
	pdp = PubdescNew();
	if (pub->choice == PUB_Equiv)   /* already a Pub-equiv */
	{
		pdp->pub = (ValNodePtr)(pub->data.ptrvalue);
		MemFree(pub);
	}
	else                     /* make a Pub-equiv of one member */
		pdp->pub = pub;
	feature->data.value.ptrvalue = (Pointer)pdp;

	return TRUE;
}

/*****************************************************************************
*
*   AddPhrapGraphInternal (submission, bsp, phrap_values, offset, num_values)
*   	Converts phrap byte array to a SeqGraph, adds to Bioseq.
*
*****************************************************************************/
static SeqAnnotPtr NewPhrapGraphSeqAnnot (CharPtr name, SeqGraphPtr sgp)

{
	SeqAnnotPtr  sap = NULL;

	if (sgp == NULL) return NULL;
	sap = SeqAnnotNew ();
	if (sap == NULL) return NULL;

	ValNodeAddPointer (&(sap->desc), Annot_descr_name, StringSave (name));
	sap->type = 3;
	sap->data = (Pointer) sgp;

  return sap;
}

static SeqGraphPtr AddPhrapGraphInternal (
	NCBISubPtr submission,
	BioseqPtr bsp ,
	BytePtr phrap_values ,
	Int4 offset ,
	Int4 num_values )
{
	SeqGraphPtr sgp = NULL;
	ByteStorePtr bs = NULL;
	Int2 max = INT2_MIN;
	Int2 min = INT2_MAX;
	BytePtr bp;
	Int4 i;
	Int2 val;
	SeqIntPtr sintp;
	SeqAnnotPtr sap;
	SeqGraphPtr lastsgp;

	if (bsp == NULL) return NULL;
	bs = BSNew (num_values);
	if (bs == NULL) return NULL;
	BSWrite (bs, (Pointer) phrap_values, num_values);

	sgp = SeqGraphNew ();
	if (sgp == NULL) {
		BSFree (bs);
		return FALSE;
	}
	sgp->numval = BSLen (bs);
	BSPutByte (bs, EOF);
	sgp->title = StringSave ("Phrap Quality");

	for (i = 0, bp = phrap_values; i < num_values; i++, bp++) {
		val = (Uint1) *bp;
		max = MAX (max, (Int2) val);
		min = MIN (min, (Int2) val);
	}
	sgp->flags [0] = 0;
	sgp->compr = 1;
	sgp->flags [1] = 0;
	sgp->flags [2] = 3;
	sgp->axis.intvalue = 0;
	sgp->min.intvalue = min;
	sgp->max.intvalue = max;
	sgp->a = 1.0;
	sgp->b = 0;
	sgp->values = (Pointer) bs;

	sintp = SeqIntNew ();
	sintp->from = offset;
	sintp->to = num_values - offset - 1;
	sintp->id = SeqIdDup (bsp->id);
	ValNodeAddPointer (&(sgp->loc), SEQLOC_INT, (Pointer) sintp);

	for (sap = bsp->annot; sap != NULL; sap = sap->next) {
		if (sap->type == 3) {
			for (lastsgp = sap->data; lastsgp->next != NULL; lastsgp = lastsgp->next) {
				continue;
			}
			lastsgp->next = sgp;
			break;
		}
	}
	if (sap == NULL) {
		if (bsp->annot != NULL) {
			for (sap = bsp->annot; sap->next != NULL; sap = sap->next) {
				continue;
			}
			sap->next = NewPhrapGraphSeqAnnot ("Graphs", sgp);
		} else {
			bsp->annot = NewPhrapGraphSeqAnnot ("Graphs", sgp);
		}
	}

	return sgp;
}

NLM_EXTERN Boolean AddPhrapGraph (
	NCBISubPtr submission,
	SeqEntryPtr the_seq ,
	CharPtr local_name ,
	BytePtr phrap_values )
{
	BioseqPtr    bsp;
	SeqGraphPtr  sgp;

	bsp = GetBioseqFromChoice(submission, the_seq, local_name, "AddPhrapGraph");
	sgp =  AddPhrapGraphInternal (submission, bsp, phrap_values, 0, bsp->length);
	return (Boolean) (sgp != NULL);
}

NLM_EXTERN Boolean AddPhrapGraphToSeqLit (
	NCBISubPtr submission,
	SeqLitPtr slp ,
	BytePtr phrap_values )
{
	BioseqPtr     bsp;
	SeqGraphPtr   sgp;
	ExtSeqLitPtr  xslp;

	if (slp == NULL) return FALSE;
	xslp = (ExtSeqLitPtr) slp;
	bsp = xslp->parentbsp;
	sgp = AddPhrapGraphInternal (submission, bsp, phrap_values, 0, slp->length);
	if (sgp != NULL) {
		xslp->graph = sgp;
		ReadjustSeqLitGraphs (bsp);  /* adjust seqlit graph positions */
		return TRUE;
	}
	return FALSE;
}

/*****************************************************************************
*
*   NCBISubValidate (nsp, errfile)
*   	Validate a submission
*
*****************************************************************************/
NLM_EXTERN Int2 NCBISubValidate (NCBISubPtr nsp, FILE * errfile)
{
	Int2 numerrors = 0, i;
	ValidStructPtr vsp;
	SeqEntryPtr sep;

	vsp = ValidStructNew();
	vsp->useSeqMgrIndexes = TRUE;
	SetAppProperty ("NcbiSubutilValidation", (void *) 1024);
	
	  /**** errfile is no longer supported ****
	vsp->errfile = errfile;
      ****************************************/
      
	for (sep = (SeqEntryPtr)(nsp->ssp->data); sep != NULL; sep = sep->next)
	{
	 	ValidateSeqEntry(sep, vsp);
		for (i = 0; i <=3; i++)
		{
			numerrors += vsp->errors[i];
		}
		ValidStructClear(vsp);
	}
	ValidStructFree(vsp);
	return numerrors;
}

/* reference gene project user object manipulation */

NLM_EXTERN UserObjectPtr CreateRefGeneTrackUserObject (void)

{
  ObjectIdPtr    oip;
  UserObjectPtr  uop;

  uop = UserObjectNew ();
  oip = ObjectIdNew ();
  oip->str = StringSave ("RefGeneTracking");
  uop->type = oip;

  return uop;
}

NLM_EXTERN void AddStatusToRefGeneTrackUserObject (UserObjectPtr uop, CharPtr status)

{
  UserFieldPtr  curr;
  ObjectIdPtr   oip;

  if (uop == NULL || status == NULL) return;
  oip = uop->type;
  if (oip == NULL || StringICmp (oip->str, "RefGeneTracking") != 0) return;

  for (curr = uop->data; curr != NULL; curr = curr->next) {
    oip = curr->label;
    if (oip != NULL && StringICmp (oip->str, "Status") == 0) {
      break;
    }
  }

  if (curr == NULL) {
    curr = UserFieldNew ();
    oip = ObjectIdNew ();
    oip->str = StringSave ("Status");
    curr->label = oip;
    curr->choice = 1; /* visible string */

    /* link status at beginning of list */

    curr->next = uop->data;
    uop->data = curr;
  }

  if (curr == NULL || curr->choice != 1) return;

  /* replace any existing status indication */

  curr->data.ptrvalue = MemFree (curr->data.ptrvalue);

  curr->data.ptrvalue = (Pointer) StringSave (status);
}

NLM_EXTERN void AddCuratorToRefGeneTrackUserObject (UserObjectPtr uop, CharPtr collaborator)

{
  UserFieldPtr  curr;
  ObjectIdPtr   oip;

  if (uop == NULL || collaborator == NULL) return;
  oip = uop->type;
  if (oip == NULL || StringICmp (oip->str, "RefGeneTracking") != 0) return;

  for (curr = uop->data; curr != NULL; curr = curr->next) {
    oip = curr->label;
    if (oip != NULL && StringICmp (oip->str, "Collaborator") == 0) {
      break;
    }
  }

  if (curr == NULL) {
    curr = UserFieldNew ();
    oip = ObjectIdNew ();
    oip->str = StringSave ("Collaborator");
    curr->label = oip;
    curr->choice = 1; /* visible string */

    /* link status at beginning of list */

    curr->next = uop->data;
    uop->data = curr;
  }

  if (curr == NULL || curr->choice != 1) return;

  /* replace any existing collaborator indication */

  curr->data.ptrvalue = MemFree (curr->data.ptrvalue);

  curr->data.ptrvalue = (Pointer) StringSave (collaborator);
}

NLM_EXTERN void AddAccessionToRefGeneTrackUserObject (UserObjectPtr uop, CharPtr field,
                                                      CharPtr accn, Int4 gi,
                                                      Boolean sequenceChange,
                                                      Boolean annotationChange,
                                                      CharPtr comment)

{
  UserFieldPtr  curr;
  UserFieldPtr  entry;
  UserFieldPtr  last;
  UserFieldPtr  prev = NULL;
  ObjectIdPtr   oip;
  UserFieldPtr  ufp;

  if (uop == NULL || field == NULL) return;
  oip = uop->type;
  if (oip == NULL || StringICmp (oip->str, "RefGeneTracking") != 0) return;

  for (curr = uop->data; curr != NULL; curr = curr->next) {
    oip = curr->label;
    if (oip != NULL && StringICmp (oip->str, field) == 0) {
      break;
    }
    prev = curr;
  }

  if (curr == NULL) {
    curr = UserFieldNew ();
    oip = ObjectIdNew ();
    oip->str = StringSave (field);
    curr->label = oip;
    curr->choice = 11; /* user fields */

    /* link new set at end of list */

    if (prev != NULL) {
      prev->next = curr;
    } else {
      uop->data = curr;
    }
  }

  if (curr == NULL || curr->choice != 11) return;

  /* curr is now the top level Assembly, Related, etc. */

  entry = UserFieldNew ();
  oip = ObjectIdNew ();
  oip->id = 0;
  entry->label = oip;
  entry->choice = 11;

  if (curr->data.ptrvalue == NULL) {
    curr->data.ptrvalue = (Pointer) entry;
  } else {
    for (prev = (UserFieldPtr) curr->data.ptrvalue; prev->next != NULL; prev = prev->next) continue;
    prev->next = entry;
  }

  /* entry is now in the appropriate list */

  ufp = UserFieldNew ();
  oip = ObjectIdNew ();
  oip->str = StringSave ("accession");
  ufp->label = oip;
  ufp->choice = 1; /* visible string */
  ufp->data.ptrvalue = (Pointer) StringSave (accn);

  entry->data.ptrvalue = (Pointer) ufp;
  last = ufp;

  if (gi > 0) {
    ufp = UserFieldNew ();
    oip = ObjectIdNew ();
    oip->str = StringSave ("gi");
    ufp->label = oip;
    ufp->choice = 2; /* integer */
    ufp->data.intvalue = gi;
    last->next = ufp;
    last = ufp;
  }

  if (sequenceChange) {
    ufp = UserFieldNew ();
    oip = ObjectIdNew ();
    oip->str = StringSave ("sequenceChange");
    ufp->label = oip;
    ufp->choice = 4; /* Boolean */
    ufp->data.boolvalue = sequenceChange;
    last->next = ufp;
    last = ufp;
  }

  if (annotationChange) {
    ufp = UserFieldNew ();
    oip = ObjectIdNew ();
    oip->str = StringSave ("annotationChange");
    ufp->label = oip;
    ufp->choice = 4; /* Boolean */
    ufp->data.boolvalue = annotationChange;
    last->next = ufp;
    last = ufp;
  }

  if (comment != NULL && *comment != '\0') {
    ufp = UserFieldNew ();
    oip = ObjectIdNew ();
    oip->str = StringSave ("comment");
    ufp->label = oip;
    ufp->choice = 1; /* visible string */
    ufp->data.ptrvalue = (Pointer) StringSave (comment);
    last->next = ufp;
    last = ufp;
  }
}

NLM_EXTERN UserObjectPtr CreateMrnaProteinLinkUserObject (BioseqPtr bsp)

{
  Char           buf [80];
  ObjectIdPtr    oip;
  SeqIdPtr       sip;
  UserFieldPtr   ufp;
  UserObjectPtr  uop;

  if (bsp == NULL) return NULL;

  uop = UserObjectNew ();
  oip = ObjectIdNew ();
  oip->str = StringSave ("MrnaProteinLink");
  uop->type = oip;

  sip = SeqIdDup (SeqIdFindBest (bsp->id, SEQID_OTHER));
  SeqIdWrite(sip, buf, PRINTID_FASTA_LONG, 79);
  SeqIdFree (sip);

  ufp = UserFieldNew ();
  oip = ObjectIdNew ();
  oip->str = StringSave ("protein seqID");
  ufp->label = oip;
  ufp->choice = 1; /* visible string */
  ufp->data.ptrvalue = (Pointer) StringSave (buf);

  uop->data = ufp;

  return uop;
}

NLM_EXTERN UserObjectPtr CreateSubmissionUserObject (CharPtr univecComment,
                                                     CharPtr additionalComment,
                                                     Int4 validatorErrorCount,
                                                     Int4 validatorHashCode,
                                                     Boolean isCloningVector)

{
  UserFieldPtr   last = NULL;
  ObjectIdPtr    oip;
  UserFieldPtr   ufp;
  UserObjectPtr  uop;

  uop = UserObjectNew ();
  oip = ObjectIdNew ();
  oip->str = StringSave ("Submission");
  uop->type = oip;

  ufp = UserFieldNew ();
  oip = ObjectIdNew ();
  oip->str = StringSave ("ValidatorErrorCount");
  ufp->label = oip;
  ufp->choice = 2; /* integer */
  ufp->data.intvalue = validatorErrorCount;

  uop->data = ufp; /* always making this ufp first */
  last = ufp;

  ufp = UserFieldNew ();
  oip = ObjectIdNew ();
  oip->str = StringSave ("ValidatorHash");
  ufp->label = oip;
  ufp->choice = 2; /* integer */
  ufp->data.intvalue = validatorHashCode;

  last->next = ufp;
  last = ufp;

  if (univecComment != NULL && *univecComment != '\0') {
    ufp = UserFieldNew ();
    oip = ObjectIdNew ();
    oip->str = StringSave ("UniVecComment");
    ufp->label = oip;
    ufp->choice = 1; /* visible string */
    ufp->data.ptrvalue = (Pointer) StringSave (univecComment);

    last->next = ufp;
    last = ufp;
  }

  if (additionalComment != NULL && *additionalComment != '\0') {
    ufp = UserFieldNew ();
    oip = ObjectIdNew ();
    oip->str = StringSave ("AdditionalComment");
    ufp->label = oip;
    ufp->choice = 1; /* visible string */
    ufp->data.ptrvalue = (Pointer) StringSave (additionalComment);

    last->next = ufp;
    last = ufp;
  }

  if (isCloningVector) {
    ufp = UserFieldNew ();
    oip = ObjectIdNew ();
    oip->str = StringSave ("IsCloningVector");
    ufp->label = oip;
    ufp->choice = 4; /* boolean */
    ufp->data.boolvalue = isCloningVector;

    last->next = ufp;
    last = ufp;
  }

  return uop;
}

NLM_EXTERN UserObjectPtr CreateContigCloneUserObject (CharPtr name, Int4 ID)

{
  UserFieldPtr   last = NULL;
  ObjectIdPtr    oip;
  UserFieldPtr   ufp;
  UserObjectPtr  uop;

  uop = UserObjectNew ();
  oip = ObjectIdNew ();
  oip->str = StringSave ("ContigClone");
  uop->type = oip;

  ufp = UserFieldNew ();
  oip = ObjectIdNew ();
  oip->str = StringSave ("CloneName");
  ufp->label = oip;
  ufp->choice = 1; /* visible string */
  ufp->data.ptrvalue = (Pointer) StringSave (name);

  uop->data = ufp;
  last = ufp;

  ufp = UserFieldNew ();
  oip = ObjectIdNew ();
  oip->str = StringSave ("CloneID");
  ufp->label = oip;
  ufp->choice = 2; /* integer */
  ufp->data.intvalue = ID;

  last->next = ufp;

  return uop;
}

/* gene ontology user object manipulation */

NLM_EXTERN UserObjectPtr CreateGeneOntologyUserObject (
  void
)

{
  ObjectIdPtr    oip;
  UserObjectPtr  uop;

  uop = UserObjectNew ();
  oip = ObjectIdNew ();
  oip->str = StringSave ("GeneOntology");
  uop->type = oip;

  return uop;
}

NLM_EXTERN void AddToGeneOntologyUserObject (
  UserObjectPtr uop,
  CharPtr type,
  CharPtr text,
  Int4 goid,
  Int4 pmid,
  CharPtr evidence
)

{
  UserFieldPtr  curr;
  UserFieldPtr  entry;
  UserFieldPtr  last;
  UserFieldPtr  prev = NULL;
  ObjectIdPtr   oip;
  UserFieldPtr  ufp;

  if (uop == NULL || type == NULL) return;
  oip = uop->type;
  if (oip == NULL || StringICmp (oip->str, "GeneOntology") != 0) return;

  for (curr = uop->data; curr != NULL; curr = curr->next) {
    oip = curr->label;
    if (oip != NULL && StringICmp (oip->str, type) == 0) {
      break;
    }
    prev = curr;
  }

  if (curr == NULL) {
    curr = UserFieldNew ();
    oip = ObjectIdNew ();
    oip->str = StringSave (type);
    curr->label = oip;
    curr->choice = 11; /* user fields */

    /* link new set at end of list */

    if (prev != NULL) {
      prev->next = curr;
    } else {
      uop->data = curr;
    }
  }

  if (curr == NULL || curr->choice != 11) return;

  /* curr is now the top level Process, Component, or Function */

  entry = UserFieldNew ();
  oip = ObjectIdNew ();
  oip->id = 0;
  entry->label = oip;
  entry->choice = 11;

  if (curr->data.ptrvalue == NULL) {
    curr->data.ptrvalue = (Pointer) entry;
  } else {
    for (prev = (UserFieldPtr) curr->data.ptrvalue; prev->next != NULL; prev = prev->next) continue;
    prev->next = entry;
  }

  /* entry is now in the appropriate list */

  ufp = UserFieldNew ();
  oip = ObjectIdNew ();
  oip->str = StringSave ("text string");
  ufp->label = oip;
  ufp->choice = 1; /* visible string */
  ufp->data.ptrvalue = (Pointer) StringSave (text);

  entry->data.ptrvalue = (Pointer) ufp;
  last = ufp;

  if (goid > 0) {
    ufp = UserFieldNew ();
    oip = ObjectIdNew ();
    oip->str = StringSave ("go id");
    ufp->label = oip;
    ufp->choice = 2; /* integer */
    ufp->data.intvalue = goid;
    last->next = ufp;
    last = ufp;
  }

  if (pmid > 0) {
    ufp = UserFieldNew ();
    oip = ObjectIdNew ();
    oip->str = StringSave ("pubmed id");
    ufp->label = oip;
    ufp->choice = 2; /* integer */
    ufp->data.intvalue = pmid;
    last->next = ufp;
    last = ufp;
  }

  if (evidence != NULL && *evidence != '\0') {
    ufp = UserFieldNew ();
    oip = ObjectIdNew ();
    oip->str = StringSave ("evidence");
    ufp->label = oip;
    ufp->choice = 1; /* visible string */
    ufp->data.ptrvalue = (Pointer) StringSave (evidence);
    last->next = ufp;
    last = ufp;
  }
}

/* model evidence user object */

NLM_EXTERN UserObjectPtr CreateModelEvidenceUserObject (
  CharPtr method,
  CharPtr contigParent
)

{
 UserFieldPtr    curr;
  ObjectIdPtr    oip;
  UserFieldPtr   prev = NULL;
  UserObjectPtr  uop;

  uop = UserObjectNew ();
  oip = ObjectIdNew ();
  oip->str = StringSave ("ModelEvidence");
  uop->type = oip;

  curr = UserFieldNew ();
  oip = ObjectIdNew ();
  oip->str = StringSave ("Method");
  curr->label = oip;
  curr->choice = 1; /* visible string */
  curr->data.ptrvalue = (Pointer) StringSave (method);

  uop->data = curr;
  prev = curr;

  curr = UserFieldNew ();
  oip = ObjectIdNew ();
  oip->str = StringSave ("Contig Name");
  curr->label = oip;
  curr->choice = 1; /* visible string */
  curr->data.ptrvalue = (Pointer) StringSave (contigParent);

  prev->next = curr;

  return uop;
}

static UserFieldPtr FindEvidenceField (
  UserObjectPtr uop,
  CharPtr type,
  Boolean create
)

{
  UserFieldPtr  curr;
  ObjectIdPtr   oip;
  UserFieldPtr  prev = NULL;

  if (uop == NULL || type == NULL) return NULL;
  oip = uop->type;
  if (oip == NULL || StringICmp (oip->str, "ModelEvidence") != 0) return NULL;

  /* search for mRNA or EST field */

  for (curr = uop->data; curr != NULL; curr = curr->next) {
    oip = curr->label;
    if (oip != NULL && StringICmp (oip->str, type) == 0) {
      break;
    }
    prev = curr;
  }

  if (curr == NULL && create) {

    /* create new top-level field */

    curr = UserFieldNew ();
    oip = ObjectIdNew ();
    oip->str = StringSave (type);
    curr->label = oip;
    curr->choice = 11; /* user fields */

    /* link new set at end of list */

    if (prev != NULL) {
      prev->next = curr;
    } else {
      uop->data = curr;
      
    }
  }

  if (curr == NULL) return NULL;
  if (curr->choice == 1 || curr->choice == 11) return curr;

  return NULL;
}

NLM_EXTERN UserFieldPtr FindModelEvidenceField (
  UserObjectPtr uop,
  CharPtr type
)

{
  return FindEvidenceField (uop, type, FALSE);
}

static UserFieldPtr FindAccnBlock (
  UserFieldPtr group,
  CharPtr accn
)

{
  UserFieldPtr  curr;
  ObjectIdPtr   oip;
  UserFieldPtr  prev = NULL;
  UserFieldPtr  ufp;

  if (group == NULL || group->choice != 11 || accn == NULL) return NULL;

  for (curr = (UserFieldPtr) group->data.ptrvalue; curr != NULL; curr = curr->next) {
    if (curr->choice == 11) {
      for (ufp = (UserFieldPtr) curr->data.ptrvalue; ufp != NULL; ufp = ufp->next) {
        oip = ufp->label;
        if (oip != NULL && StringICmp (oip->str, "accession") == 0) {
          if (StringICmp ((CharPtr) ufp->data.ptrvalue, accn) == 0) {
            return curr;
          }
        }
      }
    }
    prev = curr;
  }

  /* create new top-level field */

  curr = UserFieldNew ();
  oip = ObjectIdNew ();
  oip->id = 0;
  curr->label = oip;
  curr->choice = 11; /* user fields */

  /* link new set at end of list */

  if (prev != NULL) {
    prev->next = curr;
  } else {
    group->data.ptrvalue = curr;    
  }

  ufp = UserFieldNew ();
  oip = ObjectIdNew ();
  oip->str = StringSave ("accession");
  ufp->label = oip;
  ufp->choice = 1; /* visible string */
  ufp->data.ptrvalue = (Pointer) StringSave (accn);

  if (curr->data.ptrvalue == NULL) {
    curr->data.ptrvalue = (Pointer) ufp;
  } else {
    for (prev = (UserFieldPtr) curr->data.ptrvalue; prev->next != NULL; prev = prev->next) continue;
    prev->next = ufp;
  }

  return curr;
}

static UserFieldPtr FindIntParamBlock (
  UserFieldPtr curr,
  CharPtr type,
  Boolean create
)

{
  ObjectIdPtr   oip;
  UserFieldPtr  prev = NULL;
  UserFieldPtr  ufp;

  if (curr == NULL || type == NULL) return NULL;

  prev = NULL;
  for (ufp = (UserFieldPtr) curr->data.ptrvalue; ufp != NULL; ufp = ufp->next) {
    oip = ufp->label;
    if (oip != NULL && StringICmp (oip->str, type) == 0) {
      break;
    }
    prev = ufp;
  }

  if (ufp == NULL && create) {
    ufp = UserFieldNew ();
    oip = ObjectIdNew ();
    oip->str = StringSave (type);
    ufp->label = oip;
    ufp->choice = 2; /* int */

    /* link new set at end of list */

    if (prev != NULL) {
      prev->next = ufp;
    } else {
      curr->data.ptrvalue = ufp;      
    }
  }

  if (ufp == NULL || ufp->choice != 2) return NULL;

  return ufp;
}

NLM_EXTERN void AddMrnaOrESTtoModelEvidence (
  UserObjectPtr uop,
  CharPtr type,
  CharPtr accn,
  Int4 length,
  Int4 gaplen
)

{
  UserFieldPtr  curr;
  UserFieldPtr  group;
  ObjectIdPtr   oip;
  UserFieldPtr  ufp;

  if (uop == NULL || type == NULL || accn == NULL) return;
  oip = uop->type;
  if (oip == NULL || StringICmp (oip->str, "ModelEvidence") != 0) return;

  group = FindEvidenceField (uop, type, TRUE);
  if (group == NULL) return;

  curr = FindAccnBlock (group, accn);
  if (curr == NULL) return;

  if (length > 0) {
    ufp = FindIntParamBlock (curr, "exon count", TRUE);
    if (ufp == NULL) return;
    (ufp->data.intvalue)++;

    ufp = FindIntParamBlock (curr, "exon length", TRUE);
    if (ufp == NULL) return;
    ufp->data.intvalue += length;
  }

  if (gaplen > 0) {
    ufp = FindIntParamBlock (curr, "gap count", TRUE);
    if (ufp == NULL) return;
    (ufp->data.intvalue)++;

    ufp = FindIntParamBlock (curr, "gap length", TRUE);
    if (ufp == NULL) return;
    ufp->data.intvalue += gaplen;
  }
}

/* third party accession list user object manipulation */

NLM_EXTERN UserObjectPtr CreateTpaAssemblyUserObject (void)

{
  ObjectIdPtr    oip;
  UserObjectPtr  uop;

  uop = UserObjectNew ();
  oip = ObjectIdNew ();
  oip->str = StringSave ("TpaAssembly");
  uop->type = oip;

  return uop;
}

NLM_EXTERN void AddAccessionToTpaAssemblyUserObject (UserObjectPtr uop, CharPtr accn)

{
  UserFieldPtr  curr;
  UserFieldPtr  prev = NULL;
  ObjectIdPtr   oip;
  UserFieldPtr  ufp;

  if (uop == NULL || accn == NULL) return;
  oip = uop->type;
  if (oip == NULL || StringICmp (oip->str, "TpaAssembly") != 0) return;

  for (curr = uop->data; curr != NULL; curr = curr->next) {
    prev = curr;
  }

  curr = UserFieldNew ();
  oip = ObjectIdNew ();
  oip->id = 0;
  curr->label = oip;
  curr->choice = 11; /* user fields */

  /* link new set at end of list */

  if (prev != NULL) {
    prev->next = curr;
  } else {
    uop->data = curr;
  }

  if (curr == NULL || curr->choice != 11) return;

  ufp = UserFieldNew ();
  oip = ObjectIdNew ();
  oip->str = StringSave ("accession");
  ufp->label = oip;
  ufp->choice = 1; /* visible string */
  ufp->data.ptrvalue = (Pointer) StringSave (accn);

  curr->data.ptrvalue = (Pointer) ufp;
}

