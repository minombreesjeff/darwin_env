/*    asn2ffp.h
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
* File Name:  asn2ffp.h
*
* Author:  Karl Sirotkin, Tom Madden, Tatiana Tatusov
*
* Version Creation Date:   7/15/95
*
* $Revision: 6.33 $
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
 * $Log: asn2ffp.h,v $
 * Revision 6.33  2003/07/22 16:18:27  kans
 * added ZFIN as legal db_xref
 *
 * Revision 6.32  2003/06/10 18:44:10  kans
 * added GeneDB to list of legal db_xrefs
 *
 * Revision 6.31  2003/05/29 20:25:19  kans
 * added Interpro to list of legal dbxrefs
 *
 * Revision 6.30  2002/11/30 20:18:27  kans
 * added GOA to list of legal db_xrefs
 *
 * Revision 6.29  2002/11/27 22:25:17  kans
 * added AceView/WormGenes, NextDB, and WorfDB to legal db_xrefs
 *
 * Revision 6.28  2002/07/12 17:34:35  kans
 * WormBase is now legal dbxref for all records, not just RefSeq
 *
 * Revision 6.27  2002/06/21 15:31:11  kans
 * added GABI db_xref
 *
 * Revision 6.26  2002/06/18 20:59:59  kans
 * added ISFinder as legal db_xref with hotlink
 *
 * Revision 6.25  2002/05/06 22:15:12  kans
 * added IFO and JCM db_xrefs
 *
 * Revision 6.24  2002/02/20 21:59:33  tatiana
 * DBNUM increased for IMGT/LIGM
 *
 * Revision 6.23  2001/11/29 18:29:38  kans
 * added FANTOM_DB to list of legal db_xrefs, incremented DBNUM
 *
 * Revision 6.22  2001/10/15 17:08:44  kans
 * updated legal db_xref list to collaboration + RefSeq
 *
 * Revision 6.21  2001/10/15 13:57:22  kans
 * added BDGP_INS and SoyBase as legal db_xrefs
 *
 * Revision 6.20  2001/09/06 18:55:52  tatiana
 * *** empty log message ***
 *
 * Revision 6.19  2001/03/17 00:51:14  tatiana
 * GeneID added to dbxref array, DBNUM increased
 *
 * Revision 6.18  2001/01/26 19:26:37  kans
 * added niaEST, increased DBNUM
 *
 * Revision 6.17  2001/01/18 23:57:02  kans
 * add GO (gene ontology) to list of legal dbxrefs
 *
 * Revision 6.16  2000/12/05 01:21:23  tatiana
 * DBNUM increased for COG
 *
 * Revision 6.15  2000/10/16 19:10:18  kans
 * added UniSTS and InterimID to legal dbxrefs
 *
 * Revision 6.14  2000/08/28 22:17:19  kans
 * added CDD to list of legal dbxrefs
 *
 * Revision 6.13  2000/07/14 20:24:27  kans
 * added RGD as dbxref with web link
 *
 * Revision 6.12  2000/02/15 22:53:58  kans
 * added dbSNP and RATMAP as legal dbxrefs, put /organelle under ajp->forgrel control
 *
 * Revision 6.11  1999/04/02 20:35:51  kans
 * added prototype for GR_PrintPubs
 *
 * Revision 6.10  1999/03/22 23:19:19  tatiana
 * PrintImpFeatEx() added
 *
 * Revision 6.9  1998/10/19 15:57:04  tatiana
 * DBNUM increased for UniGene tag
 *
 * Revision 6.8  1998/08/19 18:40:03  tatiana
 * DBNUM increased to add RiceGenes dbrag
 *
 * Revision 6.7  1998/05/18 14:41:19  tatiana
 * DBNUM increased to add GI to dbtag array
 *
 * Revision 6.6  1998/04/15 20:50:38  tatiana
 * bugs fixing
 *
 * Revision 6.5  1998/04/15 18:44:45  tatiana
 *  dbtag array moved from asn2ff3.c
 *
 * Revision 6.4  1997/12/15 15:45:59  tatiana
 * features processing has been changed
 *
 * Revision 6.3  1997/12/02 16:44:17  tatiana
 *  missing semicolon in SortOrganizeFeat
 *
 * Revision 6.1  1997/09/16 15:41:01  kans
 * added AddSiteNoteQual (TT)
 *
 * Revision 5.7  1997/06/19 18:37:24  vakatov
 * [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
 *
 * Revision 5.6  1996/11/20 15:26:58  tatiana
 * prototype for GetDefinitionLine
 *
 * Revision 5.5  1996/07/30  16:31:44  tatiana
 * add Boolean arg in CheckNAFeat()
 *
 * Revision 5.4  1996/07/24  12:40:24  tatiana
 * GetCdregionGeneXrefInfo changed number of arguments
 *
 * Revision 5.3  1996/07/12  16:48:49  tatiana
 * *** empty log message ***
 *
 * Revision 5.2  1996/06/11  15:25:32  tatiana
 * add PrintNID PROTO
 *
 * Revision 5.1  1996/06/06  14:50:31  tatiana
 * *** empty log message ***
 *
 * Revision 4.10  1996/05/16  20:59:07  tatiana
 * GetCdregionGeneXrefInfo changed to Boolean
 *
 * Revision 4.9  1996/04/29  18:54:51  tatiana
 * *** empty log message ***
 *
 * Revision 4.9  1996/04/29  18:54:51  tatiana
 * *** empty log message ***
 *
 * Revision 4.7  1996/04/09  14:05:06  tatiana
 * *** empty log message ***
 *
 * Revision 4.6  1996/02/21  20:12:17  tatiana
 * *** empty log message ***
 *
 * Revision 4.5  1996/02/15  17:20:23  tatiana
 * GatherItemWithLock added
 *
 * Revision 4.4  1995/12/20  22:45:30  tatiana
 * new function added
 *
 * Revision 4.1  1995/08/22  15:39:26  tatiana
 * GetVersion name changed to GetBiotableVersion
 *
 * Revision 1.43  1995/07/17  19:33:20  kans
 * parameters combined into Asn2ffJobPtr structure
 *
*
**************************************/

/*****************************************************************************
*
*	Header file for asn2gb files.
*
****************************************************************************/

/*************************************
*
* $Log: asn2ffp.h,v $
* Revision 6.33  2003/07/22 16:18:27  kans
* added ZFIN as legal db_xref
*
* Revision 6.32  2003/06/10 18:44:10  kans
* added GeneDB to list of legal db_xrefs
*
* Revision 6.31  2003/05/29 20:25:19  kans
* added Interpro to list of legal dbxrefs
*
* Revision 6.30  2002/11/30 20:18:27  kans
* added GOA to list of legal db_xrefs
*
* Revision 6.29  2002/11/27 22:25:17  kans
* added AceView/WormGenes, NextDB, and WorfDB to legal db_xrefs
*
* Revision 6.28  2002/07/12 17:34:35  kans
* WormBase is now legal dbxref for all records, not just RefSeq
*
* Revision 6.27  2002/06/21 15:31:11  kans
* added GABI db_xref
*
* Revision 6.26  2002/06/18 20:59:59  kans
* added ISFinder as legal db_xref with hotlink
*
* Revision 6.25  2002/05/06 22:15:12  kans
* added IFO and JCM db_xrefs
*
* Revision 6.24  2002/02/20 21:59:33  tatiana
* DBNUM increased for IMGT/LIGM
*
* Revision 6.23  2001/11/29 18:29:38  kans
* added FANTOM_DB to list of legal db_xrefs, incremented DBNUM
*
* Revision 6.22  2001/10/15 17:08:44  kans
* updated legal db_xref list to collaboration + RefSeq
*
* Revision 6.21  2001/10/15 13:57:22  kans
* added BDGP_INS and SoyBase as legal db_xrefs
*
* Revision 6.20  2001/09/06 18:55:52  tatiana
* *** empty log message ***
*
* Revision 6.19  2001/03/17 00:51:14  tatiana
* GeneID added to dbxref array, DBNUM increased
*
* Revision 6.18  2001/01/26 19:26:37  kans
* added niaEST, increased DBNUM
*
* Revision 6.17  2001/01/18 23:57:02  kans
* add GO (gene ontology) to list of legal dbxrefs
*
* Revision 6.16  2000/12/05 01:21:23  tatiana
* DBNUM increased for COG
*
* Revision 6.15  2000/10/16 19:10:18  kans
* added UniSTS and InterimID to legal dbxrefs
*
* Revision 6.14  2000/08/28 22:17:19  kans
* added CDD to list of legal dbxrefs
*
* Revision 6.13  2000/07/14 20:24:27  kans
* added RGD as dbxref with web link
*
* Revision 6.12  2000/02/15 22:53:58  kans
* added dbSNP and RATMAP as legal dbxrefs, put /organelle under ajp->forgrel control
*
* Revision 6.11  1999/04/02 20:35:51  kans
* added prototype for GR_PrintPubs
*
* Revision 6.10  1999/03/22 23:19:19  tatiana
* PrintImpFeatEx() added
*
* Revision 6.9  1998/10/19 15:57:04  tatiana
* DBNUM increased for UniGene tag
*
* Revision 6.8  1998/08/19 18:40:03  tatiana
* DBNUM increased to add RiceGenes dbrag
*
* Revision 6.7  1998/05/18 14:41:19  tatiana
* DBNUM increased to add GI to dbtag array
*
* Revision 6.6  1998/04/15 20:50:38  tatiana
* bugs fixing
*
* Revision 6.5  1998/04/15 18:44:45  tatiana
*  dbtag array moved from asn2ff3.c
*
* Revision 6.4  1997/12/15 15:45:59  tatiana
* features processing has been changed
*
* Revision 6.3  1997/12/02 16:44:17  tatiana
*  missing semicolon in SortOrganizeFeat
*
* Revision 6.1  1997/09/16 15:41:01  kans
* added AddSiteNoteQual (TT)
*
* Revision 5.7  1997/06/19 18:37:24  vakatov
* [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
*
* Revision 5.6  1996/11/20 15:26:58  tatiana
* prototype for GetDefinitionLine
*
 * Revision 5.5  1996/07/30  16:31:44  tatiana
 * add Boolean arg in CheckNAFeat()
 *
 * Revision 5.4  1996/07/24  12:40:24  tatiana
 * GetCdregionGeneXrefInfo changed number of arguments
 *
 * Revision 5.3  1996/07/12  16:48:49  tatiana
 * *** empty log message ***
 *
 * Revision 5.2  1996/06/11  15:25:32  tatiana
 * add PrintNID PROTO
 *
 * Revision 5.1  1996/06/06  14:50:31  tatiana
 * *** empty log message ***
 *
 * Revision 4.10  1996/05/16  20:59:07  tatiana
 * GetCdregionGeneXrefInfo changed to Boolean
 *
 * Revision 4.9  1996/04/29  18:54:51  tatiana
 * *** empty log message ***
 *
 * Revision 4.9  1996/04/29  18:54:51  tatiana
 * *** empty log message ***
 *
 * Revision 4.7  1996/04/09  14:05:06  tatiana
 * *** empty log message ***
 *
 * Revision 4.6  1996/02/21  20:12:17  tatiana
 * *** empty log message ***
 *
 * Revision 4.5  1996/02/15  17:20:23  tatiana
 * GatherItemWithLock added
 *
 * Revision 4.4  1995/12/20  22:45:30  tatiana
 * new function added
 *
 * Revision 4.1  1995/08/22  15:39:26  tatiana
 * GetVersion name changed to GetBiotableVersion
 *
 * Revision 1.43  1995/07/17  19:33:20  kans
 * parameters combined into Asn2ffJobPtr structure
 *
 * Revision 1.41  1995/06/19  21:40:02  kans
 * Tatiana's first major reorganization, moving printing, adding HTML
 *
 * Revision 1.40  1995/05/22  16:14:20  tatiana
 * add ASN2FF_SHOW_ALL_PUBS to asn2ff_flags
 *
 * Revision 1.39  1995/05/15  21:46:05  ostell
 * added Log line
 *
*
**************************************/
#ifndef _ASN2FFP_
#define _ASN2FFP_

#include <asn2ffg.h>
#include <asn2ff.h>
#include <asn2ff6.h>


#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

NLM_EXTERN  Boolean asn2ff_flags[13];

#define ASN2FF_LOCAL_ID                 asn2ff_flags[0]
#define ASN2FF_LOOK_FOR_SEQ             asn2ff_flags[1]
#define ASN2FF_VALIDATE_FEATURES        asn2ff_flags[2]
#define ASN2FF_IGNORE_PATENT_PUBS       asn2ff_flags[3]
#define ASN2FF_DROP_SHORT_AA            asn2ff_flags[4]
#define ASN2FF_AVOID_LOCUS_COLL         asn2ff_flags[5]
#define ASN2FF_DATE_ERROR_MSG           asn2ff_flags[6]
#define ASN2FF_IUPACAA_ONLY             asn2ff_flags[7]
#define ASN2FF_TRANSL_TABLE             asn2ff_flags[8]
#define ASN2FF_REPORT_LOCUS_COLL        asn2ff_flags[9]
#define ASN2FF_SHOW_ALL_PUBS	        asn2ff_flags[10]
#define ASN2FF_SHOW_ERROR_MSG	        asn2ff_flags[11]
#define ASN2FF_SHOW_GB_STYLE	        asn2ff_flags[12]

#define DBNUM 58
NLM_EXTERN CharPtr dbtag[DBNUM];

NLM_EXTERN void FlatSpliceOff PROTO((SeqEntryPtr the_set, ValNodePtr desc));
NLM_EXTERN void FlatSpliceOn PROTO((SeqEntryPtr the_set, ValNodePtr desc));

NLM_EXTERN void PrintLocusLine PROTO((Asn2ffJobPtr ajp, GBEntryPtr gbp));
NLM_EXTERN void PrintAccessLine PROTO((Asn2ffJobPtr ajp, GBEntryPtr gbp));
NLM_EXTERN void PrintVersionLine PROTO((Asn2ffJobPtr ajp, GBEntryPtr gbp));
NLM_EXTERN void PrintNCBI_GI PROTO((Asn2ffJobPtr ajp, GBEntryPtr gbp));
NLM_EXTERN void PrintNID PROTO((Asn2ffJobPtr ajp, GBEntryPtr gbp));
NLM_EXTERN void GetDefinitionLine PROTO((Asn2ffJobPtr ajp, GBEntryPtr gbp));
NLM_EXTERN void PrintDefinitionLine PROTO((Asn2ffJobPtr ajp, GBEntryPtr gbp));
NLM_EXTERN void PrintKeywordLine PROTO((Asn2ffJobPtr ajp, GBEntryPtr gbp));
NLM_EXTERN void PrintOriginLine PROTO ((Asn2ffJobPtr ajp, GBEntryPtr gbp));
NLM_EXTERN void PrintOrganismLine PROTO ((Asn2ffJobPtr ajp, GBEntryPtr gbp));

NLM_EXTERN void PrintEPLocusLine PROTO((Asn2ffJobPtr ajp, GBEntryPtr gbp));
NLM_EXTERN void PrintSegmentLine PROTO((Asn2ffJobPtr ajp, GBEntryPtr gbp));

NLM_EXTERN void PrintGBSourceLine PROTO ((Asn2ffJobPtr ajp, GBEntryPtr gbp));
NLM_EXTERN void PrintGBOrganismLine PROTO ((Asn2ffJobPtr ajp, GBEntryPtr gbp));

NLM_EXTERN void PrintPubsByNumber PROTO ((Asn2ffJobPtr ajp, GBEntryPtr gbp));
NLM_EXTERN void PrintFeatHeader PROTO ((Asn2ffJobPtr ajp, GBEntryPtr gbp));
NLM_EXTERN void PrintSequence PROTO ((Asn2ffJobPtr ajp, GBEntryPtr gbp, Int4 start, Int4 stop));
NLM_EXTERN void PrintEPSequence PROTO ((Asn2ffJobPtr ajp, GBEntryPtr gbp, Int4 start, Int4 stop));
NLM_EXTERN void PrintBaseCount PROTO ((Asn2ffJobPtr ajp, GBEntryPtr gbp));
NLM_EXTERN ValNodePtr tie_next PROTO((ValNodePtr head, ValNodePtr vnp));
NLM_EXTERN ValNodePtr GatherDescrByChoice PROTO ((Asn2ffJobPtr ajp, GBEntryPtr gbp, Uint1 choice));
NLM_EXTERN ValNodePtr GatherDescrListByChoice PROTO ((Asn2ffJobPtr ajp, GBEntryPtr gbp, Uint1 choice));
NLM_EXTERN ValNodePtr GetOrgRef PROTO ((Asn2ffJobPtr ajp, GBEntryPtr gbp));
NLM_EXTERN ValNodePtr GetBiosource PROTO ((Asn2ffJobPtr ajp, GBEntryPtr gbp));
NLM_EXTERN void EMBL_PrintPubs PROTO ((Asn2ffJobPtr ajp, GBEntryPtr gbp, PubStructPtr psp));
NLM_EXTERN void GB_PrintPubs PROTO ((Asn2ffJobPtr ajp, GBEntryPtr gbp, PubStructPtr psp));
NLM_EXTERN void GR_PrintPubs PROTO ((Asn2ffJobPtr ajp, GBEntryPtr gbp, PubStructPtr psp));
NLM_EXTERN Boolean FlatIgnoreThisPatentPub PROTO ((BioseqPtr bsp, ValNodePtr best, Int4Ptr seqidPt));
NLM_EXTERN CharPtr FlatCleanEquals PROTO ((CharPtr retval));
NLM_EXTERN ValNodePtr GetAuthors PROTO((Asn2ffJobPtr ajp, ValNodePtr the_pub));
NLM_EXTERN CharPtr FlatJournal PROTO ((Asn2ffJobPtr ajp, GBEntryPtr gbp, ValNodePtr the_pub, Int4 pat_seqid, Boolean PNTR submit, Boolean make_index));
NLM_EXTERN ValNodePtr GetKeywordLine PROTO((Asn2ffJobPtr ajp, GBEntryPtr gbp));
NLM_EXTERN void PrintSourceFeat PROTO ((Asn2ffJobPtr ajp, GBEntryPtr gbp));
NLM_EXTERN Int2 PrintImpFeat PROTO ((Asn2ffJobPtr ajp, BioseqPtr bsp, SeqFeatPtr sfp));
NLM_EXTERN Int2 PrintImpFeatEx PROTO ((Asn2ffJobPtr ajp, BioseqPtr bsp, SeqFeatPtr sfp, Int4 gi, Int2 entityID, Int2 itemID));
NLM_EXTERN void PrintNAFeatAwp PROTO ((Asn2ffJobPtr ajp, GBEntryPtr gbp));
NLM_EXTERN void PrintNAFeatByNumber PROTO ((Asn2ffJobPtr ajp, GBEntryPtr gbp));
NLM_EXTERN void PrintAAFeatByNumber PROTO ((Asn2ffJobPtr ajp, GBEntryPtr gbp));
NLM_EXTERN CharPtr FlatLoc PROTO ((BioseqPtr bsp, ValNodePtr location));
NLM_EXTERN Boolean FlatAnnotPartial PROTO ((SeqFeatPtr sfp, Boolean use_product));
NLM_EXTERN Boolean FlatIgnoreThisPatentPub PROTO ((BioseqPtr bsp, ValNodePtr best, Int4Ptr seqidPt));

NLM_EXTERN void PrintCommentByNumber PROTO((Asn2ffJobPtr aip, GBEntryPtr gbp));
NLM_EXTERN void PrintFirstComment PROTO((Asn2ffJobPtr aip, GBEntryPtr gbp));
NLM_EXTERN void GBDescrComFeat PROTO((Asn2ffJobPtr ajp, GBEntryPtr gbp));

NLM_EXTERN Int2 GB_GetSeqDescrComms  PROTO((Asn2ffJobPtr ajp, GBEntryPtr gbp));
NLM_EXTERN Int2 GP_GetSeqDescrComms  PROTO((Asn2ffJobPtr ajp, GBEntryPtr gbp));

NLM_EXTERN Int4 GetGibbsqNumber PROTO ((BioseqPtr bsp));
NLM_EXTERN Int4 GetGibbsqCommentLength PROTO ((GBEntryPtr gbp));
NLM_EXTERN CharPtr GetGibbsqComment PROTO ((GBEntryPtr gbp));
NLM_EXTERN Int4 GetGibbsqStatement PROTO ((GBEntryPtr gbp, CharPtr ptr));
NLM_EXTERN Int2 StorePubInfo PROTO ((Asn2ffJobPtr ajp, BioseqContextPtr bcp, BioseqPtr bsp, ValNodePtr PNTR vnpp));
NLM_EXTERN Int2 StoreNAPubInfo PROTO ((Asn2ffJobPtr ajp, GBEntryPtr gbp, ValNodePtr PNTR vnp, Boolean error_msgs));
NLM_EXTERN void GetGBDate PROTO ((Asn2ffJobPtr ajp, GBEntryPtr gbp));
NLM_EXTERN void GetGPDate PROTO ((Asn2ffJobPtr ajp, GBEntryPtr gbp));
NLM_EXTERN void GetEMBLDate PROTO ((Asn2ffJobPtr ajp, GBEntryPtr gbp));
NLM_EXTERN void GetEntryVersion PROTO ((GBEntryPtr gbp));
NLM_EXTERN Boolean GetGeneQuals PROTO ((SeqFeatPtr sfp_in, GeneStructPtr gsp));
NLM_EXTERN Boolean GetCdregionGeneXrefInfo PROTO ((Asn2ffJobPtr ajp, SeqFeatPtr sfp, GBEntryPtr gbp, Int2 index));
NLM_EXTERN void GetGeneRefInfo PROTO ((GeneStructPtr gsp, NoteStructPtr nsp, GeneRefPtr grp));
NLM_EXTERN void GetDBXrefFromGene PROTO ((GeneRefPtr grp, SeqFeatPtr sfp));
NLM_EXTERN Int2 CompareStringWithGsp PROTO ((GeneStructPtr gsp, CharPtr string));
NLM_EXTERN Boolean CheckNAFeat PROTO ((Boolean is_new, BioseqPtr bsp, SeqFeatPtr sfp));
NLM_EXTERN Boolean CheckAndGetNAFeatLoc PROTO ((BioseqPtr bsp, CharPtr PNTR buffer, SeqFeatPtr sfp, Boolean loc_return));
NLM_EXTERN void GetAAFeatLoc PROTO ((BioseqPtr bsp, CharPtr PNTR buffer, SeqFeatPtr sfp, Boolean use_product));
NLM_EXTERN CharPtr GetGBSourceLine PROTO ((GBBlockPtr gb));

NLM_EXTERN Int2 CheckPubs PROTO ((Asn2ffJobPtr ajp, BioseqPtr bsp, ValNodePtr PNTR vnpp));
NLM_EXTERN CharPtr FlatAuthor PROTO ((Asn2ffJobPtr ajp, ValNodePtr the_pub));
NLM_EXTERN CharPtr FlatPubTitle PROTO ((ValNodePtr the_pub));
NLM_EXTERN void PrintDBSourceLine PROTO ((Asn2ffJobPtr ajp, GBEntryPtr gbp));

NLM_EXTERN void PostARefErrMessage PROTO ((Asn2ffJobPtr ajp, BioseqPtr bsp, PubStructPtr psp, ValNodePtr ext_pub, Int2 status, CharPtr string));

NLM_EXTERN void SeparatePartSuppl PROTO((CharPtr vol_issue, CharPtr part_sub));
NLM_EXTERN void AddExtraAccessions PROTO ((Asn2ffJobPtr ajp, GBEntryPtr gbp));
NLM_EXTERN void PrintTerminator PROTO ((void));
NLM_EXTERN Boolean get_pubs PROTO ((GatherContextPtr gcp));
NLM_EXTERN void GatherItemWithLock PROTO((Uint2 entityID, Uint2 itemID, Uint2 itemtype,
                                   Pointer userdata, GatherItemProc userfunc));
NLM_EXTERN CharPtr format_article PROTO ((Asn2ffJobPtr ajp, BioseqPtr bsp, ValNodePtr the_pub, Boolean make_index));
NLM_EXTERN CharPtr format_bookarticle PROTO ((Asn2ffJobPtr ajp, BioseqPtr bsp, ValNodePtr the_pub, Boolean make_index));
NLM_EXTERN CharPtr format_jourarticle PROTO ((Asn2ffJobPtr ajp, BioseqPtr bsp, ValNodePtr the_pub, Boolean make_index));

NLM_EXTERN void GetProtRefInfo PROTO ((Uint1 format, GeneStructPtr gsp, NoteStructPtr nsp, ProtRefPtr prp));
NLM_EXTERN Int2 GetMapFeats PROTO((Asn2ffJobPtr ajp, GBEntryPtr gbp));
NLM_EXTERN Boolean find_item PROTO ((GatherContextPtr gcp));
NLM_EXTERN Boolean get_prot_feats PROTO((GatherContextPtr gcp));
NLM_EXTERN void AddSiteNoteQual PROTO((SeqFeatPtr sfp_in, SeqFeatPtr sfp));
NLM_EXTERN void MatchAAGeneToFeat PROTO((OrganizeFeatPtr ofp, SortStructPtr p));
NLM_EXTERN void MatchNAGeneToFeat PROTO ((Boolean non_strict, OrganizeFeatPtr ofp, SortStructPtr p));
NLM_EXTERN void SortOrganizeFeat PROTO((OrganizeFeatPtr ofp));
NLM_EXTERN void OrganizeSeqFeat PROTO ((Asn2ffJobPtr ajp, GBEntryPtr gbp));
NLM_EXTERN void GetSeqFeat PROTO ((Asn2ffJobPtr ajp));

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

#endif
