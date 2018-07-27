/*   asn2ff4.c
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
* File Name:  asn2ff4.c
*
* Author:  Karl Sirotkin, Tom Madden, Tatiana Tatusov
*
* Version Creation Date:   7/15/95
*
* $Revision: 6.51 $
*
* File Description: 
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* ==========================================================================
*/


/*************************************
*
=======
* $Log: asn2ff4.c,v $
* Revision 6.51  2001/10/02 16:13:15  yaschenk
* GetSeqIdForGI returns SeqIdDup() - needs freeing
*
* Revision 6.50  2001/09/06 19:15:19  yaschenk
* removing memory leak - AsnIoMemCopy is done twice
*
* Revision 6.49  2001/09/05 23:31:34  tatiana
*  synonym is added to Genestruct with choice 1
*
* Revision 6.48  2001/08/07 16:49:41  kans
* use NUM_SEQID, added third party annotation SeqIDs to one more place
*
* Revision 6.47  2001/08/07 15:51:08  kans
* use NUM_SEQID, added third party annotation seqids
*
* Revision 6.46  2001/07/18 14:50:13  kans
* gather features with gsc.useSeqMgrIndexes if genpept, raw, indexing requested, and IndexedGetDescrForDiv to speed up finding division
*
* Revision 6.45  2001/06/26 20:41:16  kans
* FlatLocPoint as last resort prints gi|#####
*
* Revision 6.44  2001/06/25 19:18:13  kans
* get_feats SEQFEAT_CDREGION finds core without changing scope, if using indexes it indexes the entity if necessary, never goes to old gather code
*
* Revision 6.43  2001/04/12 22:48:52  yaschenk
* removing excessive ObjMgr calls
*
* Revision 6.42  2001/04/05 21:44:51  tatiana
* additional synonym in GeneRefInfoToGsp()
*
* Revision 6.41  2001/02/01 23:06:32  tatiana
* check for NULL added in MatchNAGeneToFeat
*
* Revision 6.40  2001/01/19 17:32:23  yaschenk
* Removed BioseqLockById when only GetSeqIdForGI is needed
*
* Revision 6.39  2000/12/05 22:24:34  tatiana
* bug fixed in FeatMatch
*
* Revision 6.38  2000/11/21 20:52:44  tatiana
* fixes in CreateImpFeatFromProt
*
* Revision 6.37  2000/11/02 01:53:07  tatiana
* static CompXref() added in FeatMatch
*
* Revision 6.36  2000/06/05 17:52:18  tatiana
* increase size of feature arrays to Int4
*
* Revision 6.35  2000/04/13 14:17:32  ostell
* fixed support for lim->tr. FlatLocHalfCaret alwasy assume lim->tl
*
* Revision 6.34  2000/04/03 23:28:19  tatiana
* added showSeqLoc for web feature view
*
* Revision 6.33  2000/01/21 17:17:52  kans
* MatchAAGeneToFeat now calls SeqMgrGetOverlappingGene on CDS first, avoids multiple targeted gathers, just like MatchNAGeneToFeat has done since feature indexing was first implemented
*
* Revision 6.32  1999/12/22 22:08:19  tatiana
* strand check fixed
*
* Revision 6.31  1999/12/13 19:51:47  tatiana
* Seq_strand_unknown added to strand check
*
* Revision 6.30  1999/12/09 14:33:07  tatiana
* check the starnd for mapping gene
*
* Revision 6.29  1999/11/05 14:55:53  tatiana
* check foe embl_feat added the get_feats in Gather
*
* Revision 6.28  1999/10/07 15:17:20  bazhin
* Bug fixed.
*
* Revision 6.27  1999/10/06 20:21:50  bazhin
* Removed memory leak in get_feats() function.
*
* Revision 6.26  1999/06/04 21:03:52  tatiana
* a bug fixed in MatchAAGeneToFeat()
*
* Revision 6.25  1999/04/29 22:49:20  tatiana
* added REFSEQ dbxrefs in GenPept format
*
* Revision 6.24  1999/03/30 22:23:33  kans
* pseudo can be on grp or sfp
*
* Revision 6.23  1999/03/30 19:47:40  tatiana
* use non-strict binding for REFSEQ
*
* Revision 6.22  1999/03/25 00:26:38  kans
* restored first sort in SortOrganizeFeat
*
* Revision 5.31  1997/06/19 18:37:07  vakatov
* [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
*
* Revision 5.30  1997/04/25 15:35:39  tatiana
* EMBL_PREFNUM added
*
 * Revision 5.29  1997/03/13  17:58:27  tatiana
 * *** empty log message ***
 *
 * Revision 5.28  1997/02/27  16:20:23  kans
 * check for sfp != NULL in UniqueGeneName
 *
 * Revision 5.27  1997/01/27  19:14:39  tatiana
 * *** empty log message ***
 *
 * Revision 5.25  1997/01/13  23:27:10  tatiana
 * added check for NULL in UniqueGeneNames()
 *
 * Revision 5.24  1997/01/13  22:32:48  tatiana
 * *** empty log message ***
 *
 * Revision 5.23  1997/01/13  21:44:50  tatiana
 * a bug fixed in CreateImpFeatFromProt()
 *
 * Revision 5.22  1997/01/06  19:55:22  tatiana
 * convert site and bond features to dna misc features
 *
 * Revision 5.21  1997/01/02  22:50:19  tatiana
 * *** empty log message ***
 *
 * Revision 5.20  1996/12/17  22:49:02  tatiana
 * StoreFeat() changed to StoreFeatFree for converted peptide feats
 *
 * Revision 5.19  1996/10/22  17:48:33  tatiana
 * check for right-truncated genes added in get_feats callback
 *
 * Revision 5.17  1996/09/27  22:07:05  tatiana
 * no gene binding to repeat_region feature
 *
 * Revision 5.16  1996/09/25  18:06:23  tatiana
 * SEQFEAT_COMMENT is stored in a generic feature list
 *
 * Revision 5.15  1996/09/17  14:59:40  tatiana
 * error msg for identical feats added
 *
 * Revision 5.14  1996/09/13  17:01:31  kans
 * feature field is now excpt, not except, and removed extraneous line
 *
 * Revision 5.13  1996/09/13  16:48:54  tatiana
 * except and exp_ev added in CreateImpFeatFromProt
 *
 * Revision 5.11  1996/09/13  16:26:55  kans
 * get_feats can NULL out sfp, so should test before dereferencing
 *
 * Revision 5.10  1996/09/12  17:52:53  tatiana
 * less peptide fets were missing some qualifiers
 *
 * Revision 5.9  1996/09/03  19:52:00  tatiana
 * extra_loc added in StoreFeat
 *
 * Revision 5.8  1996/08/06  20:30:46  kans
 * SeqIdFindBest called to handle local IDs and genbank IDs coexisting
 *
 * Revision 5.7  1996/07/30  19:20:44  tatiana
 * Don't bind gene to gene in MatchNAToGene()
 *
 * Revision 5.6  1996/07/30  16:37:16  tatiana
 * a bug fixed in UniqueFeat()
 *
 * Revision 5.5  1996/07/23  22:34:11  tatiana
 * prot feats in genpept (piptides)
 *
 * Revision 5.4  1996/07/16  15:45:24  tatiana
 * *** empty log message ***
 *
 * Revision 5.3  1996/07/02  18:10:50  tatiana
 * calculate hash in StoreFeat
 *
 * Revision 5.2  1996/06/11  17:05:59  tatiana
 * *** empty log message ***
 *
 * Revision 5.1  1996/06/11  15:44:00  tatiana
 * Support Prot-ref feature mapping
 *
 * Revision 4.12  1996/05/16  20:59:50  tatiana
 * RemoveRedundantFeats addded
 *
 * Revision 4.11  1996/03/25  15:21:24  tatiana
 * *** empty log message ***
 *
 * Revision 4.10  1996/03/08  15:03:19  tatiana
 * don't bind gene to boisource feature
 * a bug fixed in FlatLocPoint
 *
 * Revision 4.9  1996/03/04  17:11:20  ostell
 * added support for ignore_top features
 *
 * Revision 4.8  1996/02/28  04:53:06  ostell
 * changes to support segmented master seeuquences
 *
 * Revision 4.7  1996/02/15  15:53:43  tatiana
 * Gather for temp loaded items added
 *
 * Revision 4.6  1996/01/29  22:35:36  tatiana
 * *** empty log message ***
 *
 * Revision 4.5  1995/12/20  22:40:55  tatiana
 * GetDBXrefFromGene() added
 *
 * Revision 4.4  1995/11/22  19:14:03  tatiana
 * a bug fixed for GenPept
 *
 * Revision 4.3  1995/11/22  19:01:07  tatiana
 * a bug fixed in orphan genes printing
 *
 * Revision 4.2  1995/11/17  21:28:35  kans
 * asn2ff now uses gather (Tatiana)
 *
 * Revision 4.1  1995/08/01  14:52:03  tatiana
 * change SeqIdPrint to SeqIdWrite.
 *
 * Revision 1.15  1995/07/17  19:33:20  kans
 * parameters combined into Asn2ffJobPtr structure
 *
 * Revision 1.14  1995/06/19  21:40:02  kans
 * Tatiana's first major reorganization, moving printing, adding HTML
 *
 * Revision 1.13  1995/05/15  21:46:05  ostell
 * added Log line
 *
*
**************************************/

#include <asn2ffp.h>
#include <a2ferrdf.h>
#include <gather.h>
#include <asn2ff6.h>
#include <explore.h>

#define CTX_2GB_LOCATION_TROUBLE 0
#define CTX_2GB_NOT_IMPLEMENTED 1

/******************** Function Prototypes *********************************/

NLM_EXTERN CharPtr FlatLocHalf PROTO ((CharPtr buf, Int4 base, IntFuzzPtr fuzz));
NLM_EXTERN CharPtr FlatLocHalfCaret PROTO ((CharPtr buf, Int4 base, IntFuzzPtr fuzz));
NLM_EXTERN Boolean FlatLocPoint PROTO ((SeqIdPtr pointIdPtr, SeqIdPtr this_sidp, CharPtr piecebuf, Int4 point, IntFuzzPtr pointfuzzPtr));
NLM_EXTERN Boolean FlatLocCaret PROTO ((SeqIdPtr pointIdPtr, SeqIdPtr this_sidp, CharPtr piecebuf, Int4 point, IntFuzzPtr pointfuzzPtr));
NLM_EXTERN Boolean FlatVirtLoc PROTO ((BioseqPtr bsp, ValNodePtr location));
NLM_EXTERN Boolean FlatLocElement PROTO ((BioseqPtr bsp, ValNodePtr location, CharPtr buf));
NLM_EXTERN CharPtr complement_FlatLoc PROTO ((SeqIdPtr this_sidp, Boolean PNTR is_okPt, CharPtr total_buf, CharPtr temp, Int4Ptr lengthPt, BioseqPtr bsp, ValNodePtr location));
NLM_EXTERN CharPtr FlatSmartStringMove PROTO ((CharPtr total_buf, Int4Ptr lengthPt, CharPtr temp, CharPtr string));
NLM_EXTERN Boolean FlatNullAhead PROTO ((BioseqPtr bsp, ValNodePtr location));
NLM_EXTERN CharPtr FlatPackedPoint PROTO ((CharPtr total_buf, CharPtr temp, Int4Ptr lengthPt, PackSeqPntPtr pspp, SeqIdPtr this_sidp));
NLM_EXTERN CharPtr do_FlatLoc PROTO ((Boolean PNTR is_okPt, Boolean ok_to_complement, SeqIdPtr this_sidp, CharPtr total_buf, CharPtr temp, Int4Ptr lengthPt, BioseqPtr bsp, ValNodePtr location));
NLM_EXTERN CharPtr group_FlatLoc PROTO ((SeqIdPtr this_sidp, Boolean PNTR is_okPt, int which, CharPtr total_buf, CharPtr temp, Int4Ptr lengthPt, BioseqPtr bsp, ValNodePtr location));
NLM_EXTERN Boolean is_real_id PROTO ((SeqIdPtr pointIdPtr, SeqIdPtr this_sidp));
NLM_EXTERN void Bond PROTO ((SeqBondPtr bondp, SeqIdPtr this_sidp, CharPtr buf));
NLM_EXTERN Boolean LookForFuzz PROTO ((SeqLocPtr head));
/*************************************************************************/

/*--- the number of characters per location element is less
      than 130.  The maximum would be for a bond with both accessions
      and both with (n.m) locations.
----*/
#define MAX_CHAR_LOCATION 135

static CharPtr lim_str [5] = {"", ">","<", ">", "<"};

/*----------- FlatLocHalf ()------*/
NLM_EXTERN CharPtr FlatLocHalf 
(CharPtr buf, Int4 base, IntFuzzPtr fuzz)
{
	char localbuf [30];
	Uint1 index;

/*------
typedef struct intfuzz {
   Uint1 choice;        1=p-m, 2=range, 3=pct, 4=lim 
   Int4 a, b;           a=p-m,max,pct,orlim, b=min 
} IntFuzz, PNTR IntFuzzPtr;

Int-fuzz ::= CHOICE {
    p-m INTEGER ,                    -- plus or minus fixed amount
    range SEQUENCE {                 -- max to min
        max INTEGER ,
        min INTEGER } ,
    pct INTEGER ,                    -- % plus or minus (x10) 0-1000
    lim ENUMERATED {                 -- some limit value
        unk (0) ,                    -- unknown
        gt (1) ,                     -- greater than
        lt (2) ,                     -- less than
        tr (3) ,                     -- space to right of position
        tl (4) ,                     -- space to left of position
        other (255) } }              -- something else
-------*/

	localbuf[0] = '\0';
	buf[0] = '\0';

	if (fuzz){
		/* Fuzz_found = TRUE; */
		switch (fuzz -> choice){
			case 1:
				sprintf(localbuf,"(%ld.%ld)", (long) (base - fuzz -> a),
					(long) (base + fuzz -> a));
				break;
			case 2:
				sprintf(localbuf,"(%ld.%ld)", (long) (1+fuzz -> b),
					(long) (1+fuzz -> a));
				break;
			case 3:
				sprintf(localbuf,"(%ld.%ld)", 
					(long) (base - base* ((double) fuzz -> a/1000.0 )),
					(long) (base +base*( (double) fuzz -> a/1000.0 )));
				break;
			case 4:
				index = (Uint1) fuzz -> a;
				if (index > 4) index = 0;
				sprintf(localbuf,"%s%ld", lim_str[index], (long) base); 
				break;
			default:
			sprintf(localbuf,"%ld", (long) base);
		}
	} else {
		sprintf(localbuf,"%ld", (long) base);
	}

	StringMove(buf, localbuf);

	return buf;
}

/*----------- FlatLocHalfCaret ()------*/

NLM_EXTERN CharPtr FlatLocHalfCaret
(CharPtr buf, Int4 base, IntFuzzPtr fuzz)
{
	char localbuf [30];
	Uint1 index;

	localbuf[0] = '\0';
	buf[0] = '\0';

	if (fuzz){
		/* Fuzz_found = TRUE; */
		switch (fuzz -> choice){
			case 1:
				sprintf(localbuf,"(%ld.%ld)..(%ld.%ld)", 
					(long) (base - fuzz -> a), (long) base, (long) base,
													(long) (base + fuzz -> a));
				break;
			case 2:
				sprintf(localbuf,"%ld^%ld", (long) (1+fuzz -> b),
					(long) (1+fuzz -> a));
				break;
			case 3:
				sprintf(localbuf,"%ld^%ld", 
					(long) (base - base* ((double) fuzz -> a/1000.0 )),
					(long) (base +base*( (double) fuzz -> a/1000.0 )));
				break;
			case 4:
				if (fuzz->a == 3) /* space to right */
				{
					sprintf(localbuf, "%ld^%ld", (long)(base), (long)(base+1));
				}
				else if ((fuzz->a == 4) && (base > 1))   /* space to left */
				{
					sprintf(localbuf, "%ld^%ld", (long)(base-1), (long)(base));
				}
				else{
					index = (Uint1) fuzz -> a;
					if (index > 4) index = 0;
					sprintf(localbuf,"%s%ld", 
						lim_str[index], (long) base); 
				}
				break;
			default:
				sprintf(localbuf,"%ld", (long) base);
				break;
		}
	}else{
		sprintf(localbuf,"%ld", (long) base);
	}

	StringMove(buf, localbuf);

	return buf;
}

NLM_EXTERN Boolean FlatLocPoint (SeqIdPtr pointIdPtr, SeqIdPtr this_sidp, CharPtr piecebuf, Int4 point, IntFuzzPtr pointfuzzPtr)
/* FLATLOC_CONTEXT_LOC is removed 08.31.95 */
{
	SeqIdPtr use_id,free_seqid=NULL;
	Char buf_space[MAX_CHAR_LOCATION +1], halfbuf_space[MAX_CHAR_LOCATION +1];
	CharPtr buf, halfbuf, temp;
	static Boolean order_initialized = FALSE;
	static Uint1 order[NUM_SEQID];
	ObjectIdPtr ob;
	
if ( ! order_initialized){
	int dex;
	for (dex=0; dex < NUM_SEQID; dex ++)
		order[dex] = 255;
	order_initialized = TRUE;
		order[SEQID_GENBANK ] = 1;
		order[SEQID_EMBL ] = 2;
		order[SEQID_DDBJ ] = 3;
		order[SEQID_LOCAL ] =4;
		order[SEQID_OTHER ] =5;
		order[SEQID_TPG ] = 6;
		order[SEQID_TPE ] = 7;
		order[SEQID_TPD ] = 8;
		order[SEQID_GIBBSQ ] =9;
		order[SEQID_GIBBMT ] =10;
		order[SEQID_PRF ] =11;
		order[SEQID_PDB ] =12;
		order[SEQID_PIR ] =13;
		order[SEQID_SWISSPROT ] =14;
		order[SEQID_PATENT ] =15;
		order[SEQID_GI ] =16;
		order[SEQID_GENERAL ] =17;
		order[SEQID_GIIM ] =18;
}

	buf = buf_space;
	halfbuf = halfbuf_space;
	piecebuf[0] = '\0';
	buf[0] = '\0';
	temp = buf;

	if (pointIdPtr) {
		if ( ! SeqIdIn ( pointIdPtr, this_sidp)){
			if (pointIdPtr->choice == SEQID_GI) {
				free_seqid = use_id = GetSeqIdForGI(pointIdPtr->data.intvalue); /** returns SeqIdDup **/
			} else {
				use_id = pointIdPtr;
			}
			
			SeqIdWrite( use_id, buf, PRINTID_TEXTID_ACC_VER, MAX_CHAR_LOCATION);
			if(*buf == '\0') {
				SeqIdWrite(use_id, buf,PRINTID_FASTA_LONG, MAX_CHAR_LOCATION);
			}
			if (*buf == '\0' && use_id == NULL && pointIdPtr->choice == SEQID_GI) {
				SeqIdWrite (pointIdPtr, buf, PRINTID_FASTA_LONG, MAX_CHAR_LOCATION);
			}
			if (*buf == '\0') {
				StringCpy(buf,"?00000");
				if (use_id && use_id -> choice == SEQID_LOCAL){
					ob = (ObjectIdPtr) use_id -> data.ptrvalue;
					if (ob ->str) {
						if (*ob -> str) {
							StringNCpy(buf, ob ->str, 12);
						}
					}
				}
			}
			temp = StringMove (temp, buf);
			temp = StringMove(temp,":");
		}
	}
	FlatLocHalf(halfbuf, point+1, pointfuzzPtr);
	temp = StringMove(temp, halfbuf);
	StringMove(piecebuf, buf);
	if(free_seqid) SeqIdFree(free_seqid);
	return TRUE;
}

NLM_EXTERN Boolean FlatLocCaret 
(SeqIdPtr pointIdPtr, SeqIdPtr this_sidp, CharPtr piecebuf, Int4 point, IntFuzzPtr pointfuzzPtr)
{
	BioseqPtr bs;
	Char buf_space[MAX_CHAR_LOCATION +1], halfbuf_space[MAX_CHAR_LOCATION +1];
	CharPtr buf, halfbuf, temp;
	SeqIdPtr use_id;
	static Boolean order_initialized = FALSE;
	static Uint1 order[NUM_SEQID];
	
if ( ! order_initialized){
	int dex;
	for (dex=0; dex < NUM_SEQID; dex ++)
		order[dex] = 255;
	order_initialized = TRUE;
		order[SEQID_GENBANK ] = 1;
		order[SEQID_EMBL ] = 2;
		order[SEQID_DDBJ ] = 3;
		order[SEQID_LOCAL ] =4;
		order[SEQID_OTHER ] =5;
		order[SEQID_TPG ] = 6;
		order[SEQID_TPE ] = 7;
		order[SEQID_TPD ] = 8;
		order[SEQID_GIBBSQ ] =9;
		order[SEQID_GIBBMT ] =10;
		order[SEQID_PRF ] =11;
		order[SEQID_PDB ] =12;
		order[SEQID_PIR ] =13;
		order[SEQID_SWISSPROT ] =14;
		order[SEQID_PATENT ] =15;
		order[SEQID_GI ] =16;
		order[SEQID_GENERAL ] =17;
		order[SEQID_GIIM ] =18;
}

	buf = &(buf_space[0]);
	halfbuf = &(halfbuf_space[0]);
	piecebuf[0] = '\0';
	buf[0] = '\0';
	temp = buf;

		if (pointIdPtr)
		if ( ! SeqIdIn ( pointIdPtr, this_sidp)){
			use_id = pointIdPtr;
                        bs = BioseqFind(use_id);
			if ( bs ){
				use_id = SeqIdSelect ( bs -> id, order,NUM_SEQID);
			}
			SeqIdWrite( use_id, buf, PRINTID_TEXTID_ACC_VER, MAX_CHAR_LOCATION);
			temp = StringMove (temp, buf);
			temp = StringMove(temp,":");
		}
		FlatLocHalfCaret(halfbuf, point+1, pointfuzzPtr);
    temp = StringMove(temp, halfbuf);
		StringMove(piecebuf, buf);

	return TRUE;
}

NLM_EXTERN Boolean FlatVirtLoc(BioseqPtr bsp, ValNodePtr location)
{
	Boolean retval = FALSE;
	SeqIntPtr sintp;
	BioseqPtr this_bsp=NULL;
	SeqIdPtr this_sidp=NULL, sidp = NULL;
	SeqPntPtr spp;

	this_bsp = bsp;
	this_sidp = this_bsp -> id;


	switch ( location -> choice){
		case SEQLOC_MIX:
		case SEQLOC_EQUIV:
		case SEQLOC_PACKED_INT:
		case  SEQLOC_PACKED_PNT:
		case  SEQLOC_NULL:
		break;
		case  SEQLOC_EMPTY:
			break;
		case  SEQLOC_WHOLE:
			      sidp = (SeqIdPtr) location -> data.ptrvalue;
			if (! sidp){
				if (ASN2FF_SHOW_ERROR_MSG == TRUE)
				ErrPostEx(SEV_INFO, CTX_NCBI2GB,CTX_2GB_LOCATION_TROUBLE,
							"FlatLocElement: whole location without ID:");
            	retval = TRUE;
            	break;
			}
/*--- no break on purpose ---*/
		case  SEQLOC_INT:
			if ( location -> choice == SEQLOC_INT){
				sintp = (SeqIntPtr) location -> data.ptrvalue;
				sidp = sintp-> id;
			}
		
			if ( ! is_real_id(sidp, this_sidp)){
				retval = TRUE;
			}

			break;
		case  SEQLOC_PNT:
			spp = (SeqPntPtr) ( location -> data.ptrvalue);
			if ( ! is_real_id(spp-> id, this_sidp)){
				retval = TRUE;
			}
			break;
		case SEQLOC_BOND:
			break;
		case SEQLOC_FEAT:
		/*if ( !  (Flat_Be_quiet&1))*/
		if (ASN2FF_SHOW_ERROR_MSG == TRUE)
				ErrPostEx(SEV_INFO, CTX_NCBI2GB, CTX_2GB_NOT_IMPLEMENTED,
					"FlatVirtLoc:SEQLOC_FEAT not implemented");
			break;
	}


	return retval;
}

NLM_EXTERN Boolean FlatLocElement (BioseqPtr bsp, ValNodePtr location, CharPtr buf)
{
	Char localbuf_space[MAX_CHAR_LOCATION +1], piecebuf_space[MAX_CHAR_LOCATION +1];
	CharPtr localbuf , piecebuf ;
	CharPtr temp ;
	SeqIntPtr sintp;
	SeqPntPtr spp;
	SeqIdPtr this_sidp=NULL, sidp=NULL;
	SeqIntPtr whole_intPtr =NULL;
	Boolean retval=TRUE;
	Boolean whole_trouble;

	localbuf = &(localbuf_space[0]);
	piecebuf = &(piecebuf_space[0]);
	temp = localbuf;

	this_sidp = bsp->id;
	sidp = this_sidp;

	localbuf[0] = '\0';
	buf[0] = '\0';

	switch ( location -> choice){
		case SEQLOC_MIX:
		case SEQLOC_EQUIV:
		case SEQLOC_PACKED_INT:
		case  SEQLOC_PACKED_PNT:
		case  SEQLOC_NULL:
			if (ASN2FF_SHOW_ERROR_MSG == TRUE)
				ErrPostEx(SEV_INFO, CTX_NCBI2GB,CTX_2GB_LOCATION_TROUBLE,
				"Unexpected internal complex type");
			retval = FALSE;
		break;
		case  SEQLOC_EMPTY:
			break;
		case  SEQLOC_WHOLE:
		whole_trouble=TRUE;
		sidp = (SeqIdPtr) location -> data.ptrvalue;
		if (sidp){
            bsp = BioseqFind(sidp);
			if (bsp) {
				sintp = whole_intPtr = MemNew( sizeof(SeqInt) );
				whole_intPtr -> id = sidp;
				whole_intPtr -> from = 0;
				whole_intPtr -> to = -1;
				if ( bsp -> length > 0)
				{
					whole_intPtr -> to = bsp -> length -1 ;
					whole_trouble=FALSE;
				}
			}
		}
		if (whole_trouble) {
			if (ASN2FF_SHOW_ERROR_MSG == TRUE) {
				ErrPostEx(SEV_INFO, CTX_NCBI2GB,CTX_2GB_LOCATION_TROUBLE,
	"FlatLocElement: whole location without being able to look up limits: %s",
	 					sidp?SeqIdWrite (sidp, localbuf, PRINTID_FASTA_LONG, 
	 											MAX_CHAR_LOCATION):"No Id");
			}
			retval = FALSE;
			break;
		} 
		/*
		else if (whole_trouble) {
				SeqIdWrite (sidp, localbuf, 
					PRINTID_FASTA_LONG, MAX_CHAR_LOCATION);
			break;
		}
		*/
/*--- no break on purpose ---*/
		case  SEQLOC_INT:
			if ( location -> choice == SEQLOC_INT){
				sintp = (SeqIntPtr) location -> data.ptrvalue;
			}
		
			if (is_real_id(sintp-> id, this_sidp)){
				if (sintp -> strand == 2) /* minus strand */
					temp = StringMove(temp, "complement("); /* ) vi match */
				FlatLocPoint (sintp->id, this_sidp, piecebuf, 
						sintp -> from, sintp -> if_from );
				temp = StringMove(temp, piecebuf);
				if ( sintp -> to >0 && (sintp -> to != sintp -> from 
						|| sintp -> if_from ||  sintp -> if_to )){
					temp = StringMove(temp,"..");
					FlatLocPoint(NULL, this_sidp, piecebuf, 
						sintp -> to, sintp -> if_to);
					temp = StringMove(temp, piecebuf);
				}
				if (sintp -> strand == 2) /* minus strand */
					/* ( vi match */ temp = StringMove(temp, ")");
			}else{
#ifdef VIRTUALS_NOT_TREATED_AS_NULLS
				StringCpy(localbuf,"No id");
				if (sintp -> id){
					SeqIdWrite (sintp -> id, localbuf, 
						PRINTID_FASTA_LONG, MAX_CHAR_LOCATION);
					if (ASN2FF_SHOW_ERROR_MSG == TRUE)
						ErrPostEx(SEV_INFO, CTX_NCBI2GB,CTX_2GB_LOCATION_TROUBLE,
					"FlatLocElement: interval without being able to use id: %s", 
						localbuf);
				}
				retval = FALSE;
#endif
			}

			break;
		case  SEQLOC_PNT:
			spp = (SeqPntPtr) ( location -> data.ptrvalue);
			if (is_real_id(spp-> id, this_sidp)){
				if (spp -> strand == 2) /* minus strand */
					temp = StringMove(temp, "complement("); /* ) vi match */
				if ( spp -> fuzz){
/*--------
 *  points with fuzz treated as if always come from '^': 
 *  not best, perhaps, but pretty close
 *-------*/
					FlatLocCaret (spp -> id, this_sidp, piecebuf, 
							spp -> point, spp -> fuzz );
				} else {
					FlatLocPoint(spp -> id, this_sidp, piecebuf, 
							spp -> point, spp -> fuzz );
				}
				temp = StringMove(temp, piecebuf);
				if (spp -> strand == 2) /* minus strand */
					/* ( vi match */ temp = StringMove(temp, ")");
			} else {
#ifdef VIRTUALS_NOT_TREATED_AS_NULLS
ErrPostEx(SEV_INFO, CTX_NCBI2GB,CTX_2GB_LOCATION_TROUBLE,
"FlatLocElement: point without being able to use id: %s", sidp?SeqIdWrite (sidp, localbuf, PRINTID_FASTA_LONG):"No Id", MAX_CHAR_LOCATION);
				retval = FALSE;
#endif
			}
			break;
		case SEQLOC_BOND:
		/*
		bondp = (SeqBondPtr) location -> data.ptrvalue;
		spp = bondp -> a;
		FlatLocPoint(spp -> id, this_sidp, piecebuf, 
				spp -> point, spp -> fuzz );
		temp = StringMove(temp, piecebuf);
		temp = StringMove(temp,",");
		spp = bondp -> b;
		FlatLocPoint(NULL, this_sidp, piecebuf, spp -> point, spp -> fuzz );
		temp = StringMove(temp, piecebuf);
		*/
		Bond((SeqBondPtr)location->data.ptrvalue, this_sidp,  localbuf);
			break;
		case SEQLOC_FEAT:
	/*	if ( !  (Flat_Be_quiet&1)) */
		if (ASN2FF_SHOW_ERROR_MSG == TRUE)
			ErrPostEx(SEV_INFO, CTX_NCBI2GB, CTX_2GB_NOT_IMPLEMENTED,
				"FlatLocElement:SEQLOC_FEAT not implemented");
			break;
	}

	if (whole_intPtr)
		MemFree(whole_intPtr);  /* NOT object free ! ! ! */
	
	if (! retval){
		*buf = '\0';
	}else{
		StringMove(buf, localbuf);
	}

	return retval;
}

/****************************************************************************
*Bond
*
*	This function takes a SeqBondPtr and a CharPtr, in buf, and returns a
*	string, in buf.  If both ends of the bond exist, the output
*	is bond(a, b); if only one end exists, the output is bond(a).
*
*	Tom Madden
*
**************************************************************************/

NLM_EXTERN void Bond(SeqBondPtr bondp, SeqIdPtr this_sidp, CharPtr buf)

{
	Char piecebuf1[MAX_CHAR_LOCATION+1];
	Char piecebuf2[MAX_CHAR_LOCATION+1];
	SeqPntPtr spp;

	spp = bondp -> a;
	FlatLocPoint(spp -> id, this_sidp, &(piecebuf1[0]), 
		spp -> point, spp -> fuzz );
	if (bondp->b)
	{
		spp = bondp -> b;
		FlatLocPoint(NULL, this_sidp, &(piecebuf2[0]), 
			spp -> point, spp -> fuzz );
		sprintf(buf, "bond(%s,%s)", piecebuf1, piecebuf2);
	}
	else
	{
		sprintf(buf, "bond(%s)", piecebuf1);

	}
}

/*----------- FlatLoc  ()------*/

NLM_EXTERN CharPtr FlatLoc (BioseqPtr bsp, ValNodePtr location)
{
	CharPtr retval = NULL;
	Int4 max_length, len_used;
	Boolean is_ok = TRUE;
	SeqIdPtr this_sidp;

	if (location){
		this_sidp = bsp -> id;
		max_length = 0;
		do_FlatLoc(& is_ok, TRUE, this_sidp, NULL, NULL, 
			& max_length, bsp, location);
		if (is_ok){
			retval = (CharPtr) MemNew((size_t) (max_length + 10));
			len_used = 0;
			do_FlatLoc(&is_ok, TRUE, this_sidp,  retval, retval, 
					&len_used, bsp, location);
		}
	}

	return retval;
}



/*-------------complement_FlatLoc ()---------------*/

NLM_EXTERN CharPtr
complement_FlatLoc (SeqIdPtr this_sidp, Boolean PNTR is_okPt, CharPtr total_buf, CharPtr temp, Int4Ptr lengthPt, BioseqPtr bsp, ValNodePtr location)
{
	SeqLocRevCmp(location);

	temp = FlatSmartStringMove(total_buf, lengthPt, temp,"complement(");
	temp = do_FlatLoc (is_okPt, FALSE, this_sidp, total_buf, 
		temp, lengthPt, bsp, location);
	temp = FlatSmartStringMove(total_buf, lengthPt, temp,")");

	SeqLocRevCmp(location);

	return temp;
}
/*-------- FlatSmartStringMove()-------*/

NLM_EXTERN CharPtr 
FlatSmartStringMove(CharPtr total_buf, Int4Ptr lengthPt, CharPtr temp, CharPtr string)
{
	CharPtr retval = temp;

	if ( lengthPt){
		(*lengthPt) += StringLen(string);
	}
	if (total_buf){
		retval = StringMove(temp,string);
	}

	return retval;
}
#define FLAT_EQUIV 0
#define FLAT_JOIN 1
#define FLAT_ORDER 2
static CharPtr group_names []= {
"one-of","join","order"};

/*--------- FlatNullAhead()---------*/

NLM_EXTERN Boolean
FlatNullAhead(BioseqPtr bsp, ValNodePtr location)
{
	Boolean retval = FALSE;
	SeqLocPtr next;
	if (location){
		next = location -> next;
		if (next){
			if ( next -> choice == SEQLOC_NULL)
				retval = TRUE;
			if (FlatVirtLoc(bsp, next))
				retval = TRUE;
		}else{
			retval = TRUE;  /* last one always true */
		}
	}

	return retval;
}

								
/*---------- FlatPackedPoint() -----------*/

NLM_EXTERN CharPtr
FlatPackedPoint (CharPtr total_buf, CharPtr temp, Int4Ptr lengthPt, PackSeqPntPtr pspp, SeqIdPtr this_sidp)
{
	Char buf_space[MAX_CHAR_LOCATION +1];
	CharPtr buf;
	int dex;

	buf = &(buf_space[0]);
	for (dex=0; dex < (int) pspp -> used; dex ++){
		FlatLocPoint(pspp -> id, this_sidp, buf, (pspp->pnts)[dex], pspp->fuzz);
		temp = FlatSmartStringMove(total_buf, lengthPt, temp,buf);
	}

	return temp;
}
/*--------- group_FlatLoc ()-----------*/

NLM_EXTERN CharPtr group_FlatLoc (SeqIdPtr this_sidp, Boolean PNTR is_okPt, int which, CharPtr total_buf, CharPtr temp, Int4Ptr lengthPt, BioseqPtr bsp, ValNodePtr location)
{
	Char buf_space[MAX_CHAR_LOCATION +1];
	CharPtr buf;
	SeqLocPtr this_loc;
	Boolean special_mode = FALSE; /* join in order */
	int parens = 1;
	Boolean found_non_virt = FALSE;

	buf = &(buf_space[0]);
	
	 temp = FlatSmartStringMove(total_buf, lengthPt,
		temp,group_names[which]);
	 temp = FlatSmartStringMove(total_buf, lengthPt,
		temp,"(");
	
	for (this_loc = (SeqLocPtr) location -> data.ptrvalue;
			this_loc && *is_okPt; this_loc = this_loc -> next){
		if ( FlatVirtLoc(bsp, this_loc)) {
			if ( this_loc != location && this_loc -> next){
				if (special_mode ){
					special_mode = FALSE;
					temp = FlatSmartStringMove(total_buf, lengthPt,
						temp,")");
					parens --;
				}
			}
			continue;
		}
		if ( found_non_virt &&
				this_loc -> choice !=  SEQLOC_EMPTY &&
				this_loc -> choice !=  SEQLOC_NULL ){
			temp = FlatSmartStringMove(total_buf, lengthPt, temp,",");
		}
		switch ( this_loc -> choice ){
		case SEQLOC_NULL : 
			if ( this_loc != location && this_loc -> next){
				if (special_mode ){
					special_mode = FALSE;
					temp = FlatSmartStringMove(total_buf, lengthPt, temp,")");
					parens --;
				}
			}
			break;
		case SEQLOC_EMPTY :
			break;
		case SEQLOC_WHOLE : 
		case SEQLOC_PNT : 
		case SEQLOC_BOND :
		case SEQLOC_FEAT :
		 found_non_virt = TRUE;
			if (FlatVirtLoc(bsp, this_loc)){
				if ( this_loc != location && this_loc -> next){
					if (special_mode ){
						special_mode = FALSE;
						temp = FlatSmartStringMove(total_buf, lengthPt,
							temp,"),");
						parens --;
					}
				}
			}else{
				if( FlatLocElement(bsp, this_loc, buf)){
					temp = FlatSmartStringMove(total_buf, lengthPt, 
						temp,buf);
				}else{
					temp = NULL;
					* is_okPt = FALSE;
				}
			}
			break;
		case SEQLOC_INT :
		 found_non_virt = TRUE;
			if ( which == FLAT_ORDER 
					&& ! FlatNullAhead(bsp, this_loc)){
				special_mode = TRUE;
				 temp = FlatSmartStringMove(total_buf, lengthPt,
					temp,group_names[FLAT_JOIN]);
				 temp = FlatSmartStringMove(total_buf, lengthPt,
					temp,"(");
				parens ++;
			}
		
			if( FlatLocElement(bsp, this_loc, buf)){
				temp = FlatSmartStringMove(total_buf, lengthPt, 
					temp,buf);
			}else{
				temp = NULL;
				* is_okPt = FALSE;
			}
			break;

		case SEQLOC_PACKED_PNT :
		 found_non_virt = TRUE;
				temp = FlatPackedPoint(total_buf, temp, lengthPt,
					(PackSeqPntPtr)this_loc->data.ptrvalue, this_sidp);
			break;
		case SEQLOC_PACKED_INT :
		case SEQLOC_MIX :
		case SEQLOC_EQUIV :
		{
			ValNodePtr hold_next = this_loc -> next;
			 found_non_virt = TRUE;
			this_loc -> next = NULL;
			temp = do_FlatLoc(is_okPt, FALSE, this_sidp, total_buf, temp, lengthPt, bsp, this_loc);
			this_loc-> next = hold_next;
		}
			break;
		}
	}

	if (* is_okPt){
		while ( parens > 0){
		 temp = FlatSmartStringMove(total_buf, lengthPt,
			temp,")");
			parens --;
		}
	}

	return temp;
}

NLM_EXTERN CharPtr 
do_FlatLoc (Boolean PNTR is_okPt, Boolean ok_to_complement, SeqIdPtr this_sidp, CharPtr total_buf, CharPtr temp, Int4Ptr lengthPt, BioseqPtr bsp, ValNodePtr location)
{
	Char buf_space[MAX_CHAR_LOCATION +1];
	CharPtr buf;
	SeqLocPtr slp;
	SeqLocPtr next_loc = NULL;

	buf = &(buf_space[0]);

	if (location == NULL) {
		return NULL;
	}
	if ( ok_to_complement && 
			SeqLocStrand(location) == Seq_strand_minus ){
		temp = complement_FlatLoc (this_sidp, is_okPt, total_buf, 
			temp, lengthPt, bsp, location);
		return temp;
	}
	for ( slp = location; slp && *is_okPt; slp = slp -> next){
		if ( slp -> choice == SEQLOC_NULL ||
				FlatVirtLoc(bsp, slp))
			continue;
		if ( slp != location){
			 temp = FlatSmartStringMove(total_buf, lengthPt,
				temp,",");
		}
		switch( slp -> choice){
			case  SEQLOC_NULL:
				break;
			case SEQLOC_MIX:
			case SEQLOC_PACKED_INT:
				{
					Boolean found_null = FALSE;
					for (next_loc = (SeqLocPtr) slp -> 
							data.ptrvalue; next_loc;
							next_loc = next_loc -> next){
						if ( next_loc -> choice == SEQLOC_NULL
								|| FlatVirtLoc( bsp, next_loc)){
							found_null = TRUE;
							temp=group_FlatLoc(this_sidp, is_okPt,
								FLAT_ORDER, total_buf, temp, lengthPt, 
								bsp, slp);
							break;
						}
					}
					if ( ! found_null){
						temp=group_FlatLoc(this_sidp, is_okPt,
							FLAT_JOIN, total_buf, temp, lengthPt,
							bsp, slp);
					}
				}
				break;
			case SEQLOC_EQUIV:
				temp=group_FlatLoc(this_sidp, is_okPt,
					FLAT_EQUIV, total_buf, temp,
					lengthPt, bsp, slp);
				break;
			case  SEQLOC_PACKED_PNT:
			temp = FlatPackedPoint(total_buf, temp, lengthPt,
				(PackSeqPntPtr)slp->data.ptrvalue, this_sidp);
			break;
			default:
			if ( ! FlatVirtLoc( bsp, slp))
			if( FlatLocElement(bsp, slp, buf)){
				temp = FlatSmartStringMove(total_buf, lengthPt, 
					temp,buf);
			}else{
				temp = NULL;
				*is_okPt = FALSE;
			}
		}
	}
	return temp;
}

NLM_EXTERN Boolean is_real_id(SeqIdPtr pointIdPtr, SeqIdPtr this_sidp)
{
	Boolean retval = TRUE;
	BioseqPtr bs;
	SeqIdPtr use_id;

		if ( ! SeqIdIn ( pointIdPtr, this_sidp)){
			use_id = pointIdPtr;
                        bs = BioseqFind(use_id);
			if ( bs ){
			if (bs -> repr == Seq_repr_virtual)
				retval = FALSE;
			}
		}
	return retval;
}

NLM_EXTERN Boolean FlatAnnotPartial (SeqFeatPtr sfp, Boolean use_product)
{
	Boolean fuzz=FALSE, retval = TRUE;
	CharPtr str;
	ImpFeatPtr imp;

	if ( sfp -> data.choice == 8) /*  Imp-feat    */
	{
		imp = (ImpFeatPtr) (sfp -> data.value.ptrvalue);
		if (imp ->loc != NULL)
		for( str = imp -> loc; *str; str ++){
			if ( *str == '<' || *str == '>'){
				retval = FALSE;
				break;
			}else if (*str == 'r' && *(str +1) == 'e'){
				if (StringNCmp ("replace",str, (size_t) 7) == 0){
					retval = FALSE;
					break;
				}
			}
		}
		if (retval == TRUE) /* Look for fuzz in the ASN.1 location */
		{
			if (use_product)
				fuzz = LookForFuzz (sfp->product);
			else
				fuzz = LookForFuzz (sfp->location);
			if (fuzz == TRUE)
				retval = FALSE;
		}
	}

	return retval;
}

/************************************************************************
*Boolean LookForFuzz (SeqLocPtr slp)
*
*	Look for fuzz in the location.  For use in FlatAnnotPartial.
************************************************************************/

NLM_EXTERN Boolean LookForFuzz (SeqLocPtr head)
{
	Boolean retval=FALSE;
	IntFuzzPtr ifp;
	PackSeqPntPtr pspp;
	SeqIntPtr sip;
	SeqLocPtr slp;
	SeqPntPtr spp;

	if (head == NULL)
		return retval;

	slp=NULL;
	while ((slp = SeqLocFindNext(head, slp)) != NULL)
	{
		switch (slp->choice)
		{
			case SEQLOC_INT:
				sip = (SeqIntPtr)(slp->data.ptrvalue);
				ifp = sip->if_from;
				if (ifp != NULL)
				{
					if (ifp->choice == 4)
					{
						if (ifp->a != 0)
							retval=TRUE;
					}
					else
						retval = TRUE;	
				}
				ifp = sip->if_to;
				if (ifp != NULL)
				{
					if (ifp->choice == 4)
					{
						if (ifp->a != 0)
							retval=TRUE;
					}
					else
						retval = TRUE;	
				}
				break;
			case SEQLOC_PNT:
				spp = (SeqPntPtr)(slp->data.ptrvalue);
				ifp = spp->fuzz;
				if (ifp != NULL)
				{
					if (ifp->choice == 4)
					{
						if (ifp->a != 0)
							retval=TRUE;
					}
					else
						retval = TRUE;	
				}
				break;
			case SEQLOC_PACKED_PNT:
				pspp = (PackSeqPntPtr)(slp->data.ptrvalue);
				ifp = pspp->fuzz;
				if (ifp != NULL)
				{
					if (ifp->choice == 4)
					{
						if (ifp->a != 0)
							retval=TRUE;
					}
					else
						retval = TRUE;	
				}
				break;
			default:
				break;
		}
		if (retval == TRUE)
			break;
	}
	return retval;
} /* LookForFuzz */



/*****************************************************************************
*											modified by Tatiana 12.08.97
*	collecting and soritng the features features 
*
******************************************************************************/
static OrganizeFeatPtr CreateOrganizeFeat(void)
{
	OrganizeFeatPtr ofp;
	
		ofp = (OrganizeFeatPtr) MemNew(sizeof(OrganizeFeat));
  		MemSet ((Pointer) ofp, 0, sizeof (OrganizeFeat));
		
		return ofp;
}

static void UniqueGeneName(Boolean error_msgs, OrganizeFeatPtr ofp)
{
	SortStructPtr 	newp, p;
	SeqFeatPtr		sfp;
	GeneRefPtr		grp;
	CharPtr 		gene;
	CharPtr 		s, ss;
	SeqLocPtr 		slp;
	Int4 			index, size;
	
	if (ofp == NULL)
		return;
	if ((size = ofp->sfpGenesize) == 0)
		return;
	newp = (SortStructPtr) MemNew((size)*sizeof(SortStruct));
	newp = MemCopy(newp, ofp->Genelist, (size * sizeof(SortStruct)));
	HeapSort((VoidPtr) (newp), 
			(size_t) (size), sizeof(SortStruct), CompareGeneName);
	p = newp;
	if ((sfp = p->sfp) == NULL) {
		MemFree (newp);
		return;
	}
	slp = sfp->location;
	grp = (GeneRefPtr) sfp->data.value.ptrvalue;
	gene = grp->locus;
	for (index = 1, p++; index < size; index++, p++) {
		sfp = p->sfp;
		grp = (GeneRefPtr) sfp->data.value.ptrvalue;
		if (gene == NULL) {
			gene = grp->locus;
			slp = sfp->location;
			continue;
		}
		if (error_msgs) {
			if (grp->locus == NULL) {
				ErrPostEx(SEV_WARNING,ERR_FEATURE_NULLGeneLocus, "No gene locus in %s:", SeqLocPrint(sfp->location));
				continue;
			}
			if (StringCmp(gene, grp->locus) == 0) {
				s = SeqLocPrint(slp);
				ss = SeqLocPrint(sfp->location);
				ErrPostEx(SEV_WARNING, ERR_FEATURE_IdenticalGeneName, "Identical gene locus name [%s] in %s and %s", gene, s,  ss);
				MemFree(s);
				MemFree(ss);
			}
		}
		gene = grp->locus;
		slp = sfp->location;
	}
	MemFree (newp);
	return;
}

/*****************************************************************************
*	Compare two ImpFeats by name and location
*	returns 1 for matching features otherwise returns 0
******************************************************************************/
static CmpImpFeat (ImpFeatPtr f1, ImpFeatPtr f2)
{
    if (f1 == NULL && f2)
		return 0;

    if (f2 == NULL && f1)
		return 0;
		
    if (StringCmp(f1->key, f2->key) != 0)
		return 0;

    if (StringCmp(f1->loc, f2->loc) != 0)
		return 0;

    return 1;
}

static Int2 CompXref (ValNodePtr x1, ValNodePtr x2)
{
	DbtagPtr db1 = NULL, db2;
	CharPtr s1=NULL, s2=NULL;
	
    if (x1 == NULL && x2)
		return 0;
    if (x2 == NULL && x1)
		return 0;
	db1 = x1->data.ptrvalue;
	db2 = x2->data.ptrvalue;
	if (StringCmp(db1->db, db2->db) != 0) {
		return 0;
	}
	if (db1->tag && db1->tag->str) {
		s1 = db1->tag->str;
	}
	if (db2->tag && db2->tag->str) {
		s2 = db2->tag->str;
	}
    if (s1 == NULL && s2)
		return 0;
    if (s2 == NULL && s1)
		return 0;
	if (s1 && s2) {
		if (StringCmp(s1, s2) == 0) {
			return 1;
		} else {
			return 0;
		}
	} else {
		if (db1->tag->id == db2->tag->id) {
			return 1;
		}
	}
	return 0;
}
/*****************************************************************************
*	compare features by location and choice 
*
******************************************************************************/
static Int2 FeatMatch (SeqFeatPtr f1, SeqFeatPtr f2)
{
	Int2 retval = 0;
	
    if (f1 == NULL && f2)
		return 0;
    if (f2 == NULL && f1)
		return 0;
	if ((SeqLocCompare(f1->location, f2->location)) != SLC_A_EQ_B)
		return 0;
	if (f1->data.choice != f2->data.choice)
		return 0;
	switch (f1->data.choice)
	{
		case SEQFEAT_IMP:
			retval = 
			CmpImpFeat(f1->data.value.ptrvalue, f2->data.value.ptrvalue);
			break;
		case SEQFEAT_REGION:
			if (f1->dbxref != NULL || f2->dbxref != NULL) { 
				retval = CompXref(f1->dbxref, f2->dbxref);
			}
			break;
		default:
			break;
	}
	return retval;
}

/*****************************************************************************
*	mark (with boolean 'dup') identical features in the List
*
******************************************************************************/
static void UniqueFeat(SortStructPtr List, Int4 size)
{
	Int4 			i, j, jj, ii;
	Int4 			start;
	SortStructPtr 	p, pp;
	
	for (i = 0, p = List; i < size; i = ii) {
		ii = i + 1;
		if (p->sfp == NULL) {
			continue;
		}
		start = SeqLocStart(p->sfp->location);
		for (pp = p+1; ii < size; ii++, pp++) {
			if (pp->sfp == NULL) {
				continue;
			}
			if (start != SeqLocStart(pp->sfp->location)) {
				break;
			}
		}
		for (j = i; j < ii; j++, p++) {
			for (jj = j+1, pp = p+1; jj < ii; jj++, pp++) {
				if (p->hash == pp->hash) {
					if (FeatMatch(p->sfp, pp->sfp) == 1) {
						pp->dup = TRUE;
					}
				}
				
			}
		}
	}
	return;
}

/*****************************************************************************
*	collect landmarks from map (Gather is not used) 
*
******************************************************************************/
NLM_EXTERN Int2 GetMapFeats(Asn2ffJobPtr ajp, GBEntryPtr gbp)
{
	BioseqPtr 		bsp;
	SeqFeatPtr 		sfp;
	OrganizeFeatPtr ofp;

		if (gbp == NULL || gbp->bsp == NULL) {
			return 0;
		}
		bsp = gbp->bsp;
		BioseqLock(bsp);
		ofp = CreateOrganizeFeat();
		ofp->bsp = bsp;
		ofp->useSeqMgrIndexes = ajp->useSeqMgrIndexes;
		
		if (bsp->seq_ext_type == 3) {   /* map-ext */
			for (sfp = (SeqFeatPtr) bsp->seq_ext; sfp; sfp=sfp->next) {
				switch (sfp->data.choice) {
					case SEQFEAT_GENE: 
		    			ofp->List = EnlargeSortList(ofp->List,
		    											 ofp->sfpListsize);
						ofp->sfpListsize = StoreFeat(ofp->List, sfp, 
						ofp->sfpListsize, bsp, NULL, 0, 0, 0, NULL, NULL, 0);
					break;
					default:
		    			ofp->List = EnlargeSortList(ofp->List, 
		    											ofp->sfpListsize);
						ofp->sfpListsize = StoreFeat(ofp->List, sfp, 
						ofp->sfpListsize, bsp, NULL, 0, 0, 0, NULL, NULL, 0);
					break;
				}
			}
			
		}
		SortOrganizeFeat(ofp);
		gbp->feat = ofp;
		if (ofp == NULL) {
			return 0;
		}
		return (gbp->feat->sfpListsize);
}

/*****************************************************************************
*	function and activity for proteins
*
******************************************************************************/
static SeqFeatPtr AddProtRefInfo(SeqFeatPtr sfp, ProtRefPtr prot)
{
	ValNodePtr vnp;
	
	for (vnp=prot->name; vnp; vnp=vnp->next) {
		if (GBQualPresent("product", sfp->qual) == FALSE) {
			sfp->qual = AddGBQual(sfp->qual, "product", vnp->data.ptrvalue);
		} else {
			sfp->qual = AddGBQual(sfp->qual, "note", vnp->data.ptrvalue);
		}
	}
	if (prot->desc) {
		sfp->qual = AddGBQual(sfp->qual, "note", prot->desc);
	}
	for (vnp=prot->ec; vnp; vnp=vnp->next) {
		sfp->qual=AddGBQual(sfp->qual, "EC_number", vnp->data.ptrvalue);
	}
	for (vnp=prot->activity; vnp; vnp=vnp->next) {
		sfp->qual=AddGBQual(sfp->qual, "function", vnp->data.ptrvalue);
	}
	
	return sfp;
}

/*****************************************************************************
*	add site description notes
*
******************************************************************************/
NLM_EXTERN void AddSiteNoteQual(SeqFeatPtr sfp_in, SeqFeatPtr sfp)
{
	if (sfp_in == NULL) {
		return;
	}
	if (sfp_in->data.choice != SEQFEAT_SITE) {
		return;
	}
	switch (sfp_in->data.value.intvalue) {
		case 1:
			sfp->qual = AddGBQual(sfp->qual, "note", "active site");
			break;
		case 2:
			sfp->qual = AddGBQual(sfp->qual, "note", "binding site");
			break;
		case 3:
			sfp->qual = AddGBQual(sfp->qual, "note", "cleavage site");
			break;
		case 4:
			sfp->qual = AddGBQual(sfp->qual, "note", "inhibit site");
			break;
		case 5:
			sfp->qual = AddGBQual(sfp->qual, "note", "modified site");
			break;
		case 6:
			sfp->qual = AddGBQual(sfp->qual, "note", "glycosylation site");
			break;
		case 7:
			sfp->qual = AddGBQual(sfp->qual, "note", "myristoylation site");
			break;
		case 8:
			sfp->qual = AddGBQual(sfp->qual, "note", "mutagenized site");
			break;
		case 9:
			sfp->qual = AddGBQual(sfp->qual, "note", "metal-binding site");
			break;
		case 10:
			sfp->qual = AddGBQual(sfp->qual, "note", "phosphorylation site");
			break;
		case 11:
			sfp->qual = AddGBQual(sfp->qual, "note", "acetylation site");
			break;
		case 12:
			sfp->qual = AddGBQual(sfp->qual, "note", "amidation site");
			break;
		case 13:
			sfp->qual = AddGBQual(sfp->qual, "note", "methylation site");
			break;
		case 14:
			sfp->qual = AddGBQual(sfp->qual, "note", "hydroxylation site");
			break;
		case 15:
				sfp->qual = AddGBQual(sfp->qual, "note", "sulfatation site");
			break;
		case 16:
			sfp->qual = AddGBQual(sfp->qual, "note", 
			"oxidative-deamination site");
			break;
		case 17:
			sfp->qual = AddGBQual(sfp->qual, "note", 	
			"pyrrolidone-carboxylic-acid site");
			break;
		case 18:
			sfp->qual = AddGBQual(sfp->qual, "note", 
				"gamma-carboxyglutamic-acid site");
			break;
		case 19:
			sfp->qual = AddGBQual(sfp->qual, "note", "blocked site");
			break;
		case 20:
			sfp->qual = AddGBQual(sfp->qual, "note", "lipid-binding site");
			break;
		case 21:
			sfp->qual = AddGBQual(sfp->qual, "note", "np-binding site");
			break;
		case 22:
				sfp->qual = AddGBQual(sfp->qual, "note", "DNA binding site");
			break;
		case 23:
			sfp->qual = AddGBQual(sfp->qual, "note", "signal-peptide site");
			break;
		case 24:
			sfp->qual = AddGBQual(sfp->qual, "note", 
				"transit-peptide site");
			break;
		case 25:
			sfp->qual = AddGBQual(sfp->qual, "note", 
						"transmembrane-region site");
			break;
		default:
			sfp->qual = AddGBQual(sfp->qual, "note", "unclassified site");
			break;
	}
}

/*****************************************************************************
*	protein features are shown as misc_feats on nucleotide records
*
******************************************************************************/
static SeqFeatPtr CreateImpFeatFromProt(Uint1 format, SeqFeatPtr psfp, SeqFeatPtr cds, SeqLocPtr new_loc)
{
	SeqFeatPtr		sfp;
	ImpFeatPtr 		ifp;
	ProtRefPtr 		prot;
	GBQualPtr 		q;
	Uint2 			retval;
	Char 			buf[2];
	CdRegionPtr 	cdr;
	CharPtr 		tmp;
	GeneRefPtr		grp;
	SeqFeatXrefPtr	xrp;
	
	if (psfp->data.choice == SEQFEAT_PSEC_STR) {
		return NULL;
	}
	sfp = SeqFeatNew();
	ifp = ImpFeatNew();
	sfp->data.choice = SEQFEAT_IMP;
	sfp->data.value.ptrvalue = ifp;
	ifp->key = StringSave("misc_feature");
	if (psfp->data.choice == SEQFEAT_PROT) { 
		prot = psfp->data.value.ptrvalue;
		if (prot->processed == 0 || prot->processed == 1) {
			SeqFeatFree(sfp);
			return NULL;
		}
		if (prot->processed == 2) {
			MemFree(ifp->key);
			ifp->key = StringSave("mat_peptide");
		} else if (prot->processed == 3) {
			MemFree(ifp->key);
			ifp->key = StringSave("sig_peptide");
		} else if (prot->processed == 4) {
			MemFree(ifp->key);
			ifp->key = StringSave("transit_peptide");
		}
		sfp = AddProtRefInfo(sfp, prot);
	} else if (psfp->data.choice == SEQFEAT_BOND) {
		if (psfp->data.value.intvalue == 1) {
			sfp->qual = AddGBQual(sfp->qual, "note", "disulfide bond");
		} else if (psfp->data.value.intvalue == 2) {
			sfp->qual = AddGBQual(sfp->qual, "note", "thiolester bond");
		} else if (psfp->data.value.intvalue == 3) {
			sfp->qual = AddGBQual(sfp->qual, "note", "xlink bond");
		} else if (psfp->data.value.intvalue == 4) {
			sfp->qual = AddGBQual(sfp->qual, "note", "thioether bond");
		} else {
			sfp->qual = AddGBQual(sfp->qual, "note", "bond");
		}
	} else if (psfp->data.choice == SEQFEAT_SITE) {
			AddSiteNoteQual(psfp, sfp);
	} else if (psfp->data.choice == SEQFEAT_REGION) {
		tmp = MemNew(StringLen(psfp->data.value.ptrvalue) + 9);
		sprintf(tmp, "Region: %s", (CharPtr) psfp->data.value.ptrvalue);
		sfp->qual = AddGBQual(sfp->qual, "note", tmp);
		tmp = MemFree(tmp);
	} else {
		SeqFeatFree(sfp);
		return NULL;
	}
	sfp->excpt = psfp->excpt;
	for (xrp=psfp->xref; xrp; xrp=xrp->next) {
		if (xrp->data.choice == SEQFEAT_GENE) {
			grp = (GeneRefPtr) xrp->data.value.ptrvalue;
			sfp->xref = AsnIoMemCopy(xrp, 
				(AsnReadFunc) SeqFeatXrefAsnRead, 
					(AsnWriteFunc) SeqFeatXrefAsnWrite);
			break;
		}
	}
	if ((sfp->partial = psfp->partial) == TRUE) {
		cdr = (CdRegionPtr) cds->data.value.ptrvalue;
		if (cdr->frame) {
			sprintf(buf, "%d", cdr->frame); 
		} else {
			sprintf(buf, "1"); 
		}
		sfp->qual = AddGBQualEx(&(ifp->key), sfp->qual, "codon_start", buf);
	}
	sfp->exp_ev = psfp->exp_ev;
	if (psfp->comment)
		sfp->comment = StringSave(psfp->comment);
	if (psfp->title)
		sfp->comment = StringSave(psfp->title);
	for (q=psfp->qual; q; q=q->next) {
		sfp->qual = AddGBQual(sfp->qual, q->qual, q->val);
	}
	if (format == GENPEPT_FMT || format == EMBLPEPT_FMT) {
		sfp->location = AsnIoMemCopy(psfp->location, 
			(AsnReadFunc) SeqLocAsnRead, (AsnWriteFunc) SeqLocAsnWrite);
		return sfp;
	}
	if (new_loc) {
		sfp->location = aaFeatLoc_to_dnaFeatLoc(cds, new_loc);
	} else {
		sfp->location = aaFeatLoc_to_dnaFeatLoc(cds, psfp->location);
	}
	if (sfp->location == NULL) {
		SeqFeatFree(sfp);
		return NULL;
	}
	if (sfp->partial == FALSE) {
		retval = SeqLocPartialCheck(sfp->location);
		if (retval > SLP_COMPLETE && retval < SLP_NOSTART) {
			sfp->partial = TRUE;
		}
	}
	return sfp;
}
static Boolean CheckNewSfpLoc(SeqFeatPtr sfp, BioseqPtr bsp)
{
	SeqLocPtr slp, slp1 = NULL;
	
	while ((slp1=SeqLocFindNext(sfp->location, slp1)) != NULL) {
		slp = slp1;
	}
	if (SeqIdForSameBioseq(SeqLocId(slp), bsp->id)) {
		if (SeqLocLen(slp) <= BioseqGetLen(bsp)) {
			return TRUE;
		}
	}
	return FALSE;
}

/*****************************************************************************
*	gather callback for collecting gene features
*
******************************************************************************/
static Boolean get_genes (GatherContextPtr gcp)
{
	BioseqPtr		bsp = NULL;
	OrganizeFeatPtr ofp;
	SeqFeatPtr		sfp;
	Boolean 		temp=FALSE;
	
	ofp = gcp->userdata;
	if (gcp->thistype != OBJ_SEQFEAT) {
		return TRUE;
	}
	sfp = (SeqFeatPtr) (gcp->thisitem);
	bsp = ofp->bsp;
	if (sfp->data.choice != SEQFEAT_GENE) {
		return TRUE;
	}
	if ((gcp->tempload == TRUE) && (! gcp->hold)) {
		temp = TRUE;
	}
	ofp->Genelist = EnlargeSortList(ofp->Genelist,ofp->sfpGenesize);
	ofp->sfpGenesize = StoreFeatTemp(ofp->Genelist, sfp, 
	ofp->sfpGenesize, bsp, ofp->seg_bsp, gcp->entityID, 
					gcp->itemID, gcp->thistype, gcp->new_loc, NULL, 0, temp);
	return TRUE;
}

/*****************************************************************************
*	gather callback for collecting all features
*
******************************************************************************/
static Boolean get_feats (GatherContextPtr gcp)
{
	BioseqPtr		bsp = NULL;
	OrganizeFeatPtr ofp;
	SeqFeatPtr		sfp;
	ImpFeatPtr  	ifp;
	SeqIdPtr		xid;
	GatherRange		gr;
	Boolean 		r_trunc;
	OrganizeProtPtr opp;
	BioseqPtr 		p_bsp;
	SeqEntryPtr 	sep;
	GatherScope 	gs;
	SeqFeatPtr 		new_sfp, psfp;
	Int4 			index;
	Boolean 		temp = FALSE;
	Uint2           entityID;
	SeqMgrFeatContext fcontext;
	
	ofp = gcp->userdata;
	if (gcp->thistype != OBJ_SEQFEAT) {
		return TRUE;
	}
	sfp = (SeqFeatPtr) (gcp->thisitem);
/* do sorting within EntityId */ 
	if (gcp->entityID != ofp->oldID) {
		ofp->lock_bsp = BioseqFindCore(SeqLocId(sfp->location));
		BioseqLock(ofp->lock_bsp);
		SortOrganizeFeat(ofp);
		ofp->oldID = gcp->entityID;
	}
	gr = gcp->extremes;
	r_trunc = gr.r_trunc;
	if (sfp->data.choice != SEQFEAT_CDREGION && 
					sfp->data.choice != SEQFEAT_GENE) {
		if ((!ofp->embl_feat && ASN2FF_SHOW_GB_STYLE 
					&& !(ofp->showSeqLoc)) && r_trunc) {
			return TRUE;
		}
	}
	bsp = ofp->bsp;
	if ((gcp->tempload == TRUE) && (! gcp->hold)) {
		temp = TRUE;
	}
	switch (sfp->data.choice) {
		case SEQFEAT_GENE: 
		    ofp->Genelist = EnlargeSortList(ofp->Genelist,ofp->sfpGenesize);
			ofp->sfpGenesize = StoreFeatTemp(ofp->Genelist, sfp, 
				ofp->sfpGenesize, bsp, ofp->seg_bsp, gcp->entityID, 
					gcp->itemID, gcp->thistype, gcp->new_loc, NULL, 0, temp);
			if (ofp->show_gene) {
				if (r_trunc && !ofp->embl_feat && ASN2FF_SHOW_GB_STYLE
				 					&& !(ofp->showSeqLoc)) {
					break;
				}
			    ofp->List = EnlargeSortList(ofp->List, ofp->sfpListsize);
				ofp->sfpListsize = StoreFeatTemp(ofp->List, sfp, 
					ofp->sfpListsize, bsp, ofp->seg_bsp, gcp->entityID, 
					gcp->itemID, gcp->thistype, gcp->new_loc, NULL, 0, temp);
			}
		break;
		case SEQFEAT_BIOSRC: /* save in both lists */
		    ofp->Biosrclist = 
		    EnlargeSortList(ofp->Biosrclist, ofp->biosrcsize);
			ofp->biosrcsize = StoreFeatTemp(ofp->Biosrclist, sfp, 
				ofp->biosrcsize, bsp, ofp->seg_bsp, gcp->entityID, 
					gcp->itemID, gcp->thistype, gcp->new_loc, NULL, 0, temp);
			ofp->List = EnlargeSortList(ofp->List, ofp->sfpListsize);
			    ofp->sfpListsize = StoreFeatTemp(ofp->List, sfp, 
				    ofp->sfpListsize, bsp, ofp->seg_bsp, gcp->entityID, 
				    gcp->itemID, gcp->thistype, gcp->new_loc, NULL, 0, temp);
		break;
		case SEQFEAT_ORG: 
		    ofp->Orglist = 
		    EnlargeSortList(ofp->Orglist, ofp->sfpOrgsize);
			ofp->sfpOrgsize = StoreFeatTemp(ofp->Orglist, sfp, 
				ofp->sfpOrgsize, bsp, ofp->seg_bsp, gcp->entityID, 
					gcp->itemID, gcp->thistype, gcp->new_loc, NULL, 0, temp);
		break;
	case SEQFEAT_PUB: /* Pubs are already captured by "StorePubInfo". */
		break;
	case SEQFEAT_IMP: /* This case must be before the generic case */
		ifp = (ImpFeatPtr) sfp->data.value.ptrvalue;
		if (StringCmp(ifp->key, "source") == 0) {
/* Capture only the first source feat that covers the
		entire entry, the others go among the generic features.*/
			if (ofp->sfpSourcesize == 0) {
				if (bsp->length != -1 &&
					bsp->length == SeqLocLen(sfp->location)) {
				       ofp->Sourcelist = 
						   EnlargeSortList(ofp->Sourcelist, 
												   ofp->sfpSourcesize);
					ofp->sfpSourcesize =
						StoreFeatTemp(ofp->Sourcelist, sfp, ofp->sfpSourcesize, 
						bsp, ofp->seg_bsp,gcp->entityID, gcp->itemID, 
						gcp->thistype, gcp->new_loc, NULL, 0, temp);
					break;
				}
			}
		} else if (StringCmp(ifp->key, "Site-ref") == 0 && sfp->cit == NULL) {
/* if "Site-ref" has a pub, put out info as a pub */
			ofp->Siteslist =
			   EnlargeSortList(ofp->Siteslist, ofp->sfpSitesize);
			ofp->sfpSitesize = 
			StoreFeatTemp(ofp->Siteslist, sfp, ofp->sfpSitesize, bsp, 
				ofp->seg_bsp, gcp->entityID, gcp->itemID, 
					gcp->thistype, gcp->new_loc, NULL, 0, temp);
			break;
		} else if (StringCmp(ifp->key, "Site-ref") == 0 && sfp->cit != NULL) {
/* Check to see if this was already put out as a pub*/
			break;
		} else {
/* If none of the above is true, execute generic. */
			ofp->List = EnlargeSortList(ofp->List, ofp->sfpListsize);
			ofp->sfpListsize = 
			StoreFeatTemp(ofp->List, sfp, ofp->sfpListsize, bsp, 
				ofp->seg_bsp, gcp->entityID, gcp->itemID, 
					gcp->thistype, gcp->new_loc, NULL, 0, temp);
		}
		break;
/* Look to see if an Xref goes out as a SeqFeat or as a 2nd accession */
	case SEQFEAT_SEQ:
		xid=CheckXrefFeat(bsp, sfp);
		if (xid == NULL) {
		      ofp->Xreflist = EnlargeSortList(ofp->Xreflist, 	
										      ofp->sfpXrefsize);
			ofp->sfpXrefsize = StoreFeatTemp(ofp->Xreflist, sfp,
				ofp->sfpXrefsize, bsp, ofp->seg_bsp, gcp->entityID, 
				gcp->itemID, gcp->thistype, gcp->new_loc, NULL, 0, temp);
			break;

		}
		break;
	case SEQFEAT_COMMENT:
/* The following assures a valid comment */
		if (sfp->comment == NULL || StringLen(sfp->comment) == 0)
				break;
		if (bsp && bsp->length != -1 &&
			bsp->length == SeqLocLen(sfp->location)) {
/** will go to COMMENT field **/
		      ofp->Commlist = 
		      EnlargeSortList(ofp->Commlist, ofp->sfpCommsize);
			ofp->sfpCommsize =
			StoreFeatTemp(ofp->Commlist, sfp, ofp->sfpCommsize, 
				bsp, ofp->seg_bsp, gcp->entityID, gcp->itemID, 
					gcp->thistype, gcp->new_loc, NULL, 0, temp);
		} else {
/** will go to misc_feature **/
			ofp->List = EnlargeSortList(ofp->List, ofp->sfpListsize);
	    	ofp->sfpListsize = StoreFeatTemp(ofp->List, sfp, 
		    	ofp->sfpListsize, bsp, ofp->seg_bsp, gcp->entityID, 
			    	gcp->itemID, gcp->thistype, gcp->new_loc, NULL, 0, temp);
		}
		break;
	case SEQFEAT_CDREGION:
		if (r_trunc != TRUE || ofp->embl_feat || ofp->showSeqLoc
										 || !ASN2FF_SHOW_GB_STYLE) {
			ofp->List = EnlargeSortList(ofp->List, ofp->sfpListsize);
	    	ofp->sfpListsize = StoreFeatTemp(ofp->List, sfp, 
		   		 ofp->sfpListsize, bsp, ofp->seg_bsp, gcp->entityID, 
			    	gcp->itemID, gcp->thistype, gcp->new_loc, gcp->extra_loc, 
				    	gcp->extra_loc_cnt, temp);
		}
/* Look for Prot-ref features, create ImpFeats */
		if (sfp != NULL && sfp->product != NULL && ofp->format != GENPEPT_FMT) {
			p_bsp = BioseqFindCore(SeqLocId(sfp->product));
			if (p_bsp != NULL)    /*Bioseq is (or has been) in memory */
			{
				if (ofp->useSeqMgrIndexes) {
					entityID = ObjMgrGetEntityIDForPointer (p_bsp);
					if (SeqMgrFeaturesAreIndexed (entityID) == 0) {
						SeqMgrIndexFeatures (entityID, NULL);
					}
					psfp = SeqMgrGetBestProteinFeature (p_bsp, NULL);
					if (psfp != NULL) {
						psfp = SeqMgrGetNextFeature (p_bsp, NULL, 0, 0, &fcontext);
						while (psfp != NULL) {
							new_sfp = CreateImpFeatFromProt(ofp->format, psfp, sfp, NULL);
							if (new_sfp != NULL) {
								if (CheckNewSfpLoc(new_sfp, bsp)) {
					    			ofp->List = EnlargeSortList(ofp->List, 
														    ofp->sfpListsize);
									ofp->sfpListsize = 
										StoreFeatFree(ofp->List,new_sfp, 
										ofp->sfpListsize, bsp, ofp->seg_bsp, 
										fcontext.entityID, fcontext.itemID, 
										OBJ_SEQFEAT, NULL, NULL, 0, TRUE);
								}
/* if opp->list[index].slp !=NULL I shoud use it*/
							}
							psfp = SeqMgrGetNextFeature (p_bsp, psfp, 0, 0, &fcontext);
						}
					}
					return TRUE;
				}
				opp = (OrganizeProtPtr) MemNew(sizeof(OrganizeProt));
				opp->size = 0;
				sep = SeqEntryNew();
				sep->choice = 1;
				sep->data.ptrvalue = p_bsp;
				MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
				gs.get_feats_location = TRUE;
				gs.target = sfp->product;
				GatherSeqEntry(sep, opp, get_prot_feats, &gs);
				for (index=0; index < opp->size; index++) {
					if ((psfp = opp->list[index].sfp) == NULL) {
						continue;
					}
					new_sfp = CreateImpFeatFromProt(ofp->format, psfp, sfp, 
												opp->list[index].slp);
					if (new_sfp != NULL) {
						if (CheckNewSfpLoc(new_sfp, bsp)) {
					    	ofp->List = EnlargeSortList(ofp->List, 
												    ofp->sfpListsize);
							ofp->sfpListsize = 
								StoreFeatFree(ofp->List,new_sfp, 
								ofp->sfpListsize, bsp, ofp->seg_bsp, 
								opp->list[index].entityID, 
								opp->list[index].itemID, 
								opp->list[index].itemtype, 
								NULL, NULL, 0, TRUE);
						}
/* if opp->list[index].slp !=NULL I shoud use it*/
					}
				}
				if(opp->list != NULL && opp->list->nsp != NULL)
					NoteStructFree(opp->list->nsp);
				MemFree(opp->list);
				MemFree(opp);
				MemFree(sep);
			}
		}
		break;
	case SEQFEAT_RNA:
		ofp->List = EnlargeSortList(ofp->List, ofp->sfpListsize);
	    	ofp->sfpListsize = StoreFeatTemp(ofp->List, sfp, 
		    	ofp->sfpListsize, bsp, ofp->seg_bsp, gcp->entityID, 
			    	gcp->itemID, gcp->thistype, gcp->new_loc, gcp->extra_loc, 
				    	gcp->extra_loc_cnt, temp);
		break;
	case SEQFEAT_RSITE:
/* do not gather this type*/
		break;
	default:
/* If none of the above is true, execute generic. */
		ofp->List = EnlargeSortList(ofp->List, ofp->sfpListsize);
	    ofp->sfpListsize = StoreFeatTemp(ofp->List, sfp, 
		    	ofp->sfpListsize, bsp, ofp->seg_bsp, gcp->entityID, 
			    	gcp->itemID, gcp->thistype, gcp->new_loc, NULL, 0, temp);
		break;
	}
	return TRUE;
}

static Boolean is_embl(GBEntryPtr gbp)
{
	CharPtr prefix = EMBL_AC;
	static CharPtr	embl_accpref[EMBL_PREFNUM] = {"AJ", "AL", "AM", "AN", "AX"};
	Boolean retval = FALSE;
	Int2 i;
	
	if (gbp == NULL || gbp->accession == NULL)
		return FALSE;
	if (IS_DIGIT(gbp->accession[1]) && 
		StringChr(prefix, gbp->accession[0]) != NULL) {
		retval = TRUE;
	}  else {
    	for (i = 0; i < EMBL_PREFNUM; i++) {
			if (StringNCmp(gbp->accession, embl_accpref[i], 2) == 0) {
				retval = TRUE;
	    	}
	    }
    }
	return retval;
}

/****************************************************************************
*	void GetGeneRefInfo (GeneStructPtr gsp, NoteStructPtr nsp, GeneRefPtr grp)
*
*	gsp: GeneStructPtr containing gene information
*	grp: GeneRefPtr from a sfp of type gene or a sfp xref.
*
*	If fields are empty on the gsp, and the relevant information
*	is given by the grp, that field is filled on the gsp
****************************************************************************/

static void GeneRefInfoToGsp (GeneStructPtr gsp, GeneRefPtr grp, SeqFeatPtr sfp)

{
	ValNodePtr syn, vsyn = NULL;
	
	if (grp == NULL) {
		return;
	}
	syn=grp->syn;
	if (grp->locus != NULL) {
		if (gsp->gene != NULL && 
			StringCmp(gsp->gene->data.ptrvalue, grp->locus) != 0) {
			if (syn != NULL) {
				vsyn = ValNodeCopyStr(&(vsyn), 1, syn->data.ptrvalue);
				gsp->gene->next=vsyn;
			}
			return;
		}
		if (gsp->gene == NULL) {
			gsp->gene = ValNodeCopyStr(&(gsp->gene), 0, grp->locus);
		}
	} else if (grp->desc != NULL) {
		gsp->gene = ValNodeCopyStr(&(gsp->gene), 0, grp->desc);
	}
	if (syn != NULL) {
			vsyn = ValNodeCopyStr(&(vsyn), 1, syn->data.ptrvalue);
		if (gsp->gene == NULL) {
			gsp->gene = vsyn;
		} else {
			gsp->gene->next=vsyn;
		}
	}
	if (gsp->map[0] == NULL && grp->maploc)
		gsp->map[0] = grp->maploc;
	if (grp->pseudo) {
		gsp->pseudo = TRUE;
	} else if (sfp != NULL && sfp->pseudo) {
		gsp->pseudo = TRUE;
	} else {
		gsp->pseudo = FALSE;
	}

	gsp->grp = AsnIoMemCopy((GeneRefPtr)grp, (AsnReadFunc) GeneRefAsnRead, (AsnWriteFunc) GeneRefAsnWrite);
	return;
}

/*****************************************************************************
*	gather genes for particular CDS (for GenPept an d EmblPept
*
******************************************************************************/
static OrganizeFeatPtr GetGeneListForCds(Uint2 entityID, BioseqPtr bsp)
{
    OrganizeFeatPtr	ofp=NULL;
    GatherScope 	gsc;
    SeqLocPtr 		slp;
    
    ofp = CreateOrganizeFeat();
    MemSet ((Pointer) (&gsc), 0, sizeof (GatherScope));
    MemSet ((Pointer) (gsc.ignore), (int)(TRUE), 
            (size_t) (OBJ_MAX * sizeof(Boolean)));
    gsc.ignore[OBJ_SEQANNOT] = FALSE;
    gsc.ignore[OBJ_SEQFEAT] = FALSE;
    gsc.get_feats_location = TRUE;
    gsc.seglevels = 1;
    if (bsp != NULL) {
        slp = ValNodeNew(NULL);
        slp->choice = SEQLOC_WHOLE;
        slp->data.ptrvalue = (SeqIdPtr) SeqIdDup (SeqIdFindBest (bsp->id, 0));
        gsc.target = slp;
    } else {
        gsc.target = NULL;
    }
    ofp->bsp = bsp;
    ofp->seg_bsp = NULL;
    
    GatherEntity(entityID, ofp, get_genes, &gsc);
    
    if((slp = gsc.target) != NULL) {
        SeqIdFree(slp->data.ptrvalue);
        ValNodeFree(slp);
    }
    
    return ofp;
}

/***************************************************************************
*	This function assigns genes to the given sfp by comparing locations.
*	of sfp with the location from the list of genes using SeqLocAinB
*	(sfp have to be in the gene).  
*	If SeqLocAinB returns "0", there is an exact match and this gene
*	is judged "the best" match to the sfp.`
*	If SeqLocAinB returns the diff > 0, slp is contained within 
*	gene_loc and the difference between the two features is measured. 
*	The lowest difference gives the "best_gene"
*	and that information is stored in the GeneStructPtr (gsp) by
*	GeneRefInfoToGsp.  
***************************************************************************/	
								
NLM_EXTERN void MatchNAGeneToFeat (Boolean non_strict, OrganizeFeatPtr ofp, SortStructPtr p)
{

	Boolean 		bind_to_feat=FALSE;
	NoteStructPtr 	nsp; /* UNUSED */
	GeneStructPtr 	gsp;
	GeneRefPtr 		grp=NULL;
	ImpFeatPtr 		ifp;
	Int4 			best_gene = -1, index;
	Int4 			diff_lowest, diff_current;
	SeqFeatPtr		gene = NULL, best_gene_feat = NULL, sfp;
	Uint1			sg, sf;
	
	if (p == NULL)
		return;
	if ((sfp = p->sfp) == NULL)
		return;
	if (sfp->data.choice == SEQFEAT_BIOSRC || sfp->data.choice == SEQFEAT_GENE)	
		return;
	if (sfp->data.choice == SEQFEAT_IMP)	{
		ifp = sfp->data.value.ptrvalue;
		if (ifp && StringCmp(ifp->key, "repeat_region") == 0) {
			return;
		}
	}
	if (non_strict == FALSE) { /* binding is limited to RNA and CDS */
		if (sfp->data.choice == SEQFEAT_CDREGION || 
							sfp->data.choice == SEQFEAT_RNA) {
			bind_to_feat = TRUE;	
		} else if (sfp->data.choice == SEQFEAT_IMP) {
			ifp = (ImpFeatPtr) sfp->data.value.ptrvalue;
			if (StringCmp(ifp->key, "CDS") == 0)
				bind_to_feat = TRUE;	
		}
	} else {
		bind_to_feat = TRUE;	
	}
	if (bind_to_feat == FALSE)
		return;

	best_gene_feat = SeqMgrGetOverlappingGene (sfp->location, NULL);
	if (best_gene_feat != NULL) {
		grp = best_gene_feat->data.value.ptrvalue;
		if (grp != NULL) {
			gsp = p->gsp;
		/*	GetDBXrefFromGene(grp, sfp);*/
	/********		gsp->grp = AsnIoMemCopy(grp, 
	(AsnReadFunc) GeneRefAsnRead, (AsnWriteFunc) GeneRefAsnWrite); ****/ /*** it is redone in GeneRefInfoToGsp (EY) */
			GeneRefInfoToGsp(gsp, grp, best_gene_feat);  /*copy GeRefInfo to GeneStruct */
			GetGeneQuals(sfp, gsp); /* copy quals info to GenStruct */
			return;
		}
	}
	nsp = p->nsp;
	gsp = p->gsp;
	diff_lowest = -1;
	p = ofp->Genelist;
	for (index=0; index < ofp->sfpGenesize; index++, p++) {
		if ((gene = p->sfp) == NULL) {
			continue;
		}
		sg = SeqLocStrand(gene->location);
		sf = SeqLocStrand(sfp->location);
		if (sf == sg ||
         		(sg == Seq_strand_unknown && sf != Seq_strand_minus) ||
          			(sf == Seq_strand_unknown && sg != Seq_strand_minus)) {
			diff_current = SeqLocAinB(sfp->location, gene->location);
		} else {
			continue;
		}
		if (! diff_current)   /* perfect match */ {
			best_gene = index;
			best_gene_feat = gene;
			break;
		} else if (diff_current > 0) {
			if ((diff_lowest == -1) || (diff_current < diff_lowest)) {
				diff_lowest = diff_current;
				best_gene = index;
				best_gene_feat = gene;
			}
		}

	}
	if (best_gene == -1) {	/*no gene found that completely contains CDS*/
		return;
	} else {
 		if (best_gene_feat != NULL) {
			grp = best_gene_feat->data.value.ptrvalue;
		}
	}
	/*	GetDBXrefFromGene(grp, sfp);*/
			gsp->grp = AsnIoMemCopy(grp, 
			(AsnReadFunc) GeneRefAsnRead, (AsnWriteFunc) GeneRefAsnWrite);
	GeneRefInfoToGsp(gsp, grp, best_gene_feat);  /*copy GeRefInfo to GeneStruct */
	GetGeneQuals(sfp, gsp); /* copy quals info to GenStruct */
	
	return;
}

static Boolean CheckCdregionGeneXref (SortStructPtr p, Uint1 format)
{
	SeqFeatPtr		sfp;
	GeneRefPtr		grp;
	GeneStructPtr 	gsp;
	NoteStructPtr 	nsp;
	ProtRefPtr 		prp;
	SeqFeatXrefPtr 	xrp;
	Boolean retval = FALSE;

	if (p == NULL) {
		return retval;
	}
	if ((sfp = p->sfp) == NULL)
		return retval;
	gsp = p->gsp;
	nsp = p->nsp;
	for (xrp=sfp->xref; xrp; xrp=xrp->next) {
		if (xrp->data.choice == SEQFEAT_GENE) {
			retval = TRUE;
			grp = (GeneRefPtr) xrp->data.value.ptrvalue;
			GeneRefInfoToGsp(gsp, grp, sfp);
		} else if (xrp->data.choice == SEQFEAT_PROT) {
			prp = (ProtRefPtr) xrp->data.value.ptrvalue;
			GetProtRefInfo(format, gsp, nsp, prp);
		}
	}

	return retval;
}


NLM_EXTERN Boolean GetGeneQuals(SeqFeatPtr sfp_in, GeneStructPtr gsp)
{
	Boolean 		has_gene = FALSE;
	GBQualPtr 		qual1;

	for (qual1=sfp_in->qual; qual1; qual1=qual1->next) {
		if (StringCmp(qual1->qual, "gene") == 0) {
			has_gene = TRUE;
			if (gsp->gene == NULL) {
				gsp->gene = ValNodeCopyStr(&(gsp->gene), 0, qual1->val);
			}
		} else if (StringCmp(qual1->qual, "product") == 0) {
			if (gsp->product)
				ValNodeCopyStr(&(gsp->product), 0, qual1->val); 
			else
				gsp->product = ValNodeCopyStr(&(gsp->product), 0, qual1->val);
		} else if (StringCmp(qual1->qual, "standard_name") == 0) {
			if (gsp->standard_name)
				ValNodeCopyStr(&(gsp->standard_name), 0, qual1->val); 
			else
				gsp->standard_name = ValNodeCopyStr(&(gsp->standard_name), 0, qual1->val);
		} else if (gsp->map[0] == NULL 
			&& StringCmp(qual1->qual, "map") == 0) {
			gsp->map[0] = qual1->val;
		} else if (StringCmp(qual1->qual, "EC_number") == 0) {
			if (gsp->ECNum)
				ValNodeCopyStr(&(gsp->ECNum), 0, qual1->val); 
			else
				gsp->ECNum = ValNodeCopyStr(&(gsp->ECNum), 0, qual1->val);
		}
	}

	return has_gene;
}

/*****************************************************************************
*	see MatchNAGeneToFeat (above) for details
*
******************************************************************************/
NLM_EXTERN void MatchAAGeneToFeat (OrganizeFeatPtr ofp, SortStructPtr p)
{
	GeneRefPtr			grp = NULL;
	Int4 				best_gene = -1, index;
	SeqFeatPtr			gene = NULL, best_gene_feat = NULL, sfp;
	NoteStructPtr 		nsp; /* UNUSED */
	GeneStructPtr 		gsp;
	BioseqPtr 			bsp;
	Int4 				diff_lowest, diff_current;
	OrganizeFeatPtr 	gofp;
	
	if (p == NULL)
		return;
	if ((sfp = p->sfp) == NULL)
		return;
	if (SeqLocLen(sfp->location) == -1)
		return;	 /*SeqLocLen failed on CDS location */

/* Only look on the CDS!! for genpept.		*/
	if (sfp->data.choice != SEQFEAT_CDREGION) {
		return;
	}
	bsp = BioseqFind(SeqLocId(sfp->location));

	best_gene_feat = SeqMgrGetOverlappingGene (sfp->location, NULL);
	if (best_gene_feat != NULL) {
		grp = best_gene_feat->data.value.ptrvalue;
		gsp = p->gsp;
		GeneRefInfoToGsp(gsp, grp, best_gene_feat);  /*copy GeRefInfo to GeneStruct */
		if (bsp && bsp->id->choice == SEQID_OTHER) {
			GetDBXrefFromGene(grp, sfp);
		}
		return;
	}

	gofp = GetGeneListForCds(p->entityID, bsp);
	if (gofp == NULL) {
		return;
	}
	nsp = p->nsp;
	gsp = p->gsp;
	if (bsp == NULL && gofp->sfpGenesize == 1) {
		gene = gofp->Genelist->sfp;
 		if (gene != NULL) {
			grp = gene->data.value.ptrvalue;
			GeneRefInfoToGsp(gsp, grp, gene);  /*copy GeRefInfo to GeneStruct */
		}
		MemFree(gofp->Genelist);
		MemFree(gofp);
		return; /* first and best gene */
	}
	diff_lowest = -1;
	for (p=gofp->Genelist, index=0; index < gofp->sfpGenesize; p++, index++) {
		if ((gene = p->sfp) == NULL) {
				continue;	
		}
		diff_current = SeqLocAinB(sfp->location, gene->location);
		if (! diff_current)   /* perfect match */ {
			best_gene = index;
			best_gene_feat = gene;
			break;
		} else if (diff_current > 0) {
			if ((diff_lowest == -1) || (diff_current < diff_lowest)) {
				diff_lowest = diff_current;
				best_gene = index;
				best_gene_feat = gene;
			}
		}
	}
	if (best_gene == -1) {
		return;
	}
	if (best_gene_feat != NULL) {
		grp = best_gene_feat->data.value.ptrvalue;
		GeneRefInfoToGsp(gsp, grp, best_gene_feat);  /*copy GeRefInfo to GeneStruct */
		if (bsp && bsp->id->choice == SEQID_OTHER) {
			GetDBXrefFromGene(grp, sfp);
		}
	}
	MemFree(gofp->Genelist);
	MemFree(gofp);
	return;
}

/*****************************************************************************
*	do sorting and gene mapping within one entity (that should be locked)
*
******************************************************************************/
NLM_EXTERN void SortOrganizeFeat(OrganizeFeatPtr ofp)
{
	SortStructPtr 	p;
	GeneStructPtr 	gsp;
	Int4 			index;
	SeqFeatPtr 		sfp;
	
	if (ofp == NULL)
		return;
		if (ofp->sfpListsize > 0 && ofp->sortListsize < ofp->sfpListsize) {
			HeapSort((VoidPtr) (ofp->List + ofp->sortListsize), 
				(size_t) (ofp->sfpListsize - ofp->sortListsize), 
					sizeof(SortStruct), CompareSfpForHeap);
			UniqueFeat(ofp->List + ofp->sortListsize,
						ofp->sfpListsize - ofp->sortListsize);
		}
	if (ofp->sfpCommsize > 0 && ofp->sortCommsize < ofp->sfpCommsize)
		HeapSort((VoidPtr) (ofp->Commlist + ofp->sortCommsize), 
			(size_t) (ofp->sfpCommsize - ofp->sortCommsize), 
				sizeof(SortStruct), CompareSfpForHeap);
	if (ofp->sfpGenesize > 0 && ofp->sortGenesize < ofp->sfpGenesize)
		HeapSort((VoidPtr) (ofp->Genelist + ofp->sortGenesize), 
			(size_t) (ofp->sfpGenesize - ofp->sortGenesize), 
				sizeof(SortStruct), CompareSfpForHeap);
	if (ofp->sfpOrgsize > 0 && ofp->sortOrgsize < ofp->sfpOrgsize)
		HeapSort((VoidPtr) (ofp->Orglist + ofp->sortOrgsize), 
			(size_t) (ofp->sfpOrgsize - ofp->sortOrgsize), 
					sizeof(SortStruct), CompareSfpForHeap);
	if (ofp->sfpSitesize > 0 && ofp->sortSitesize < ofp->sfpSitesize)
		HeapSort((VoidPtr) (ofp->Siteslist + ofp->sortSitesize), 
			(size_t) (ofp->sfpSitesize - ofp->sortSitesize), 
				sizeof(SortStruct), CompareSfpForHeap);
	if (ofp->sfpSourcesize > 0 && ofp->sortSourcesize < ofp->sfpSourcesize)
		HeapSort((VoidPtr) (ofp->Sourcelist + ofp->sortSourcesize), 
			(size_t) (ofp->sfpSourcesize - ofp->sortSourcesize), 
				sizeof(SortStruct),
					CompareSfpForHeap);
	ofp->sortListsize = ofp->sfpListsize;
	ofp->sortCommsize = ofp->sfpCommsize;
	ofp->sortGenesize = ofp->sfpGenesize;
	ofp->sortOrgsize = ofp->sfpOrgsize;
	ofp->sortSitesize = ofp->sfpSitesize;
	ofp->sortSourcesize = ofp->sfpSourcesize;

	p = ofp->List;
	for (index=0; index < ofp->sfpListsize; index++, p++) {
		sfp = p->sfp;
		if (p == NULL)
			continue;
		gsp = GeneStructNew();
		p->gsp = gsp;
		if (CheckCdregionGeneXref(p, ofp->format) == FALSE) {
			if (ofp->format == EMBLPEPT_FMT || ofp->format == GENPEPT_FMT) {
				MatchAAGeneToFeat(ofp, p);
			} else {
				MatchNAGeneToFeat(ofp->non_strict, ofp, p);
			}
		}
		GetGeneQuals(sfp, gsp);
	}
/*	if (ofp->sfpListsize > 0) {
		BioseqUnlock(ofp->lock_bsp);
	}
*/
	BioseqUnlock(ofp->lock_bsp);
	return;
}

/*
static Boolean is_mRNA_set(SeqEntryPtr sep, BioseqPtr bsp)
{
	BioseqSetPtr 	bssp;
	ValNodePtr 		vnp;
	MolInfoPtr 		mip = NULL;
	
	if (sep == NULL) {
		return FALSE;
	}
	if (IS_Bioseq(sep)) {
		return FALSE;
	}
	bssp = (BioseqSetPtr) sep->data.ptrvalue;
	if (bssp->_class != BioseqseqSet_class_gen_prod_set) {
		return FALSE;
	}
	for (vnp = bsp->descr; vnp; vnp=vnp->next) {
		if (vnp->choice == Seq_descr_molinfo) {
			mip = (MolInfoPtr) vnp->data.ptrvalue;
			break;
		}
	}
	if (mip == NULL) {
		return FALSE;
	}
	if (mip->biomol != 3) {
		return FALSE;
	}
	return TRUE;
}
*/
	
/*****************************************************************************
*	Gather all features in one GBEntry and process within
*	each entity (in callback)
*
******************************************************************************/
NLM_EXTERN void OrganizeSeqFeat(Asn2ffJobPtr ajp, GBEntryPtr gbp)
{
	BioseqPtr		bsp;
	GatherScope 	gsc;
	ValNodePtr		slp = NULL;
	OrganizeFeatPtr ofp;
	SeqFeatPtr 		mrna, gene = NULL, newg;
	GeneRefPtr 		grp;
	SeqMgrFeatContext fcontext;
	SeqIdPtr		sip;

	ofp = CreateOrganizeFeat();
	ofp->lock_bsp = NULL;
	if ((bsp = gbp->bsp) == NULL)
		return;
	ofp->embl_feat = is_embl(gbp);
	MemSet ((Pointer) (&gsc), 0, sizeof (GatherScope));
	MemSet ((Pointer) (gsc.ignore), (int)(TRUE),
			(size_t) (OBJ_MAX * sizeof(Boolean)));
	gsc.ignore[OBJ_SEQANNOT] = FALSE;
	gsc.ignore[OBJ_SEQFEAT] = FALSE;
	gsc.get_feats_location = TRUE;
	if (ajp->ignore_top)
		gsc.ignore_top = TRUE;
		gsc.seglevels = 1;
	if (ajp->format == GENPEPT_FMT) {
		gsc.get_feats_product = TRUE;
	}
	gsc.seglevels = 0;
	if (ajp->slp != NULL) {
		gsc.target = ajp->slp;
		gsc.convert_loc = TRUE;
		gsc.newid = bsp->id;
	} else {
		slp = ValNodeNew(NULL);
		slp->choice = SEQLOC_WHOLE;
		slp->data.ptrvalue = (SeqIdPtr) SeqIdDup (SeqIdFindBest (bsp->id, 0));
		gsc.target = slp;
		if (ajp->only_one) {
			gsc.convert_loc = TRUE;
			gsc.newid = bsp->id;
		}
	}
	ofp->bsp = bsp;
	ofp->showSeqLoc = (ajp->slp) ? TRUE : FALSE;
	ofp->useSeqMgrIndexes = ajp->useSeqMgrIndexes;
	ofp->seg_bsp = ajp->asn2ffwep->seg;
	ofp->format = ajp->format;
	ofp->non_strict = ajp->non_strict;
	for (sip=bsp->id; sip; sip=sip->next) { /* non_strict binding for REFSEQ*/
		if (sip->choice == SEQID_OTHER) {
			ofp->non_strict = FALSE;
		}
	}
	ofp->show_gene = ajp->show_gene;
	if (ajp->format == GENPEPT_FMT && bsp->repr == Seq_repr_raw && ajp->useSeqMgrIndexes) {
		gsc.useSeqMgrIndexes = TRUE;
	}
	GatherEntity(ajp->entityID, ofp, get_feats, &gsc);
	if (slp) {
		SeqLocFree(slp);
	}
	ofp->lock_bsp = ofp->bsp;
	BioseqLock(ofp->lock_bsp);

	if (/* is_mRNA_set(ajp->sep, bsp) && */ ajp->useSeqMgrIndexes) {
		mrna = SeqMgrGetRNAgivenProduct(bsp, NULL);
		if (mrna) {
			gene = SeqMgrGetOverlappingGene(mrna->location, &fcontext);
		}
		if (gene) {
			grp = (GeneRefPtr) gene->data.value.ptrvalue;
			newg = SeqFeatNew();
			newg->data.choice = SEQFEAT_GENE;
			newg->location = SeqLocIntNew(0, bsp->length-1, 
				SeqLocStrand(gene->location), SeqIdDup(bsp->id));
			newg->data.value.ptrvalue = AsnIoMemCopy(grp, 
			(AsnReadFunc) GeneRefAsnRead, (AsnWriteFunc) GeneRefAsnWrite);
			ofp->Genelist = EnlargeSortList(ofp->Genelist,ofp->sfpGenesize);
			ofp->sfpGenesize = StoreFeat(ofp->Genelist, newg, 
			ofp->sfpGenesize, bsp, ofp->seg_bsp, fcontext.entityID, fcontext.itemID, OBJ_SEQFEAT, NULL, NULL, 0);
			ofp->List = EnlargeSortList(ofp->List, ofp->sfpListsize);
			ofp->sfpListsize = StoreFeat(ofp->List, newg, 
			ofp->sfpListsize, bsp, ofp->seg_bsp, fcontext.entityID, fcontext.itemID, OBJ_SEQFEAT, NULL, NULL, 0);
		}
	}
	
	SortOrganizeFeat(ofp);  /* for the last entity */
	UniqueGeneName(ajp->error_msgs, ofp);
	ofp->source_notes = NoteStructNew(ofp->source_notes);
	gbp->feat = ofp;

	return;
}

/*****************************************************************************
*	Gather all features
*
******************************************************************************/
NLM_EXTERN void GetSeqFeat(Asn2ffJobPtr ajp)
{
	GBEntryPtr 		gbp;

	for (gbp = ajp->asn2ffwep->gbp; gbp; gbp = gbp->next) {
		if (gbp->locus) {
			flat2asn_delete_locus_user_string();
			flat2asn_install_locus_user_string(gbp->locus);
		}
		if (gbp->accession) {
			flat2asn_delete_accession_user_string();
			flat2asn_install_accession_user_string(gbp->accession);
		}
		OrganizeSeqFeat(ajp, gbp);
	}

}



