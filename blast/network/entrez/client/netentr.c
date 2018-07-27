/*   netentr.c
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
* File Name:  netentr.c
*
* Author:  Ostell, Kans, Epstein
*
* Version Creation Date:   06/02/92
*
* $Revision: 6.7 $
*
* File Description: 
*       entrez index access library for Network Entrez
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
* 06-08-93 Schuler     Changed datatype for Monitor from Handle to MonitorPtr
*
* 8-16-94  Brylawski   Added routines to access medline term explosion,
*                      mesh tree browsing, and on-the-fly neighboring.
*                       
* 11-20-94 Brylawski   Modifed on-the-fly neighboring to permit the client
*                      to pass more information about user preferences.
*
* ==========================================================================
*
*
* RCS Modification History:
* $Log: netentr.c,v $
* Revision 6.7  1999/05/28 15:39:31  kans
* missing # on else and endif
*
* Revision 6.6  1999/05/28 15:28:19  beloslyu
* set the netEntrezVersion from the defined macro
*
* Revision 6.5  1999/05/28 13:42:35  beloslyu
* Change the version of netentrez
*
* Revision 6.4  1998/08/24 21:00:50  kans
* fixed -v -fd warnings
*
* Revision 6.3  1998/03/20 18:40:07  kans
* all xxxListAsnRead functions check for NULL, return before dereferencing NULL pointer
*
* Revision 6.2  1998/03/18 18:57:15  kans
* return if selp == NULL to avoid dereferencing
*
* Revision 6.1  1998/02/21 22:34:34  kans
* NetEntHierarchyGet passes type and field
*
* Revision 6.0  1997/08/25 18:34:53  madden
* Revision changed to 6.0
*
* Revision 5.7  1997/07/29 21:24:09  vakatov
* [WIN32,DLL]  DLL'zation of "netentr.lib"
*
* Revision 5.6  1997/06/05 15:52:14  epstein
* change name of NetInit function per Eric Hackborn's request
*
* Revision 5.5  1997/05/27 18:11:32  kans
* NI_Service get failure uses ErrPostEx, not Message
*
* Revision 5.4  1997/04/02 20:59:49  epstein
* Increased version number so network entrez server can detect older clients
*
 * Revision 5.3  1996/09/24  16:07:21  epstein
 * Add ASN.1 spec version so server can discriminate between clients based upon their spec version
 *
 * Revision 5.2  1996/08/14  22:58:11  epstein
 * tweaks for biostruc annots
 *
 * Revision 5.1  1996/08/14  19:43:54  epstein
 * add annot get by feat ids, and also add date filtering support
 *
 * Revision 5.0  1996/05/28  14:10:21  ostell
 * Set to revision 5.0
 *
 * Revision 4.13  1996/03/29  18:55:41  epstein
 * add support for structure alignments
 *
 * Revision 4.12  1996/03/26  16:30:29  epstein
 * migrate byte-swapping functions to ncbimisc.[ch]
 *
 * Revision 4.11  1995/12/27  21:36:02  epstein
 * eliminate duplicate SkipSpaces() function which conflicts with new ncbistr.c function
 *
 * Revision 4.10  1995/11/13  22:02:00  epstein
 * fix BLAST bad-count bug (per S. Shavirin)
 *
 * Revision 4.9  1995/10/02  15:28:27  epstein
 * support range-checking
 *
 * Revision 4.8  1995/09/21  16:17:55  kans
 * moved prototype for MsgSetReadTimeout
 *
 * Revision 4.7  1995/09/21  14:55:46  epstein
 * add higher timeouts for evaluating boolean expressions
 *
 * Revision 4.6  1995/09/11  19:28:49  epstein
 * add some more bullet-proofing when reestablish connections
 *
 * Revision 4.5  1995/08/28  17:27:36  epstein
 * work around compiler nit
 *
 * Revision 4.4  1995/08/24  20:48:35  epstein
 * adjust timeouts to accomodate slow cluster analysis
 *
 * Revision 4.2  1995/08/22  19:35:24  epstein
 * add clustering support
 *
 * Revision 4.1  1995/08/11  20:27:22  epstein
 * add max-models support for biostrucs
 *
 * Revision 4.0  1995/07/26  13:54:59  ostell
 * force revision to 4.0
 *
 * Revision 1.48  1995/07/11  14:51:50  epstein
 * fix-up for new docsum implementation
 *
 * Revision 1.47  1995/06/29  16:36:28  epstein
 * add biostruc-complexity, and use new biostrucX ASN.1 construct to communicate with server
 *
 * Revision 1.46  95/06/07  16:10:50  epstein
 * add return value to SwapInEntrez()
 * 
 * Revision 1.45  95/06/07  15:47:27  epstein
 * make BLAST search cancellable in conjunction with Monitor
 * 
 * Revision 1.44  95/05/17  17:53:08  epstein
 * add RCS log revision history
 * 
*/

#include <accentr.h>
#include <ncbinet.h>
#include <netentr.h>
#include <netlib.h>
#include <netpriv.h>
#include <objneten.h>

typedef struct namedItem {
    Boolean   knownToServer;
    CharPtr   term;
    Char      tmpFileName[PATH_MAX];
    DocType   type;
    DocField  field;
} NamedItem, PNTR NamedItemPtr;

static void NEAR s_NetEntrezFini PROTO((void));
static ByteStorePtr NEAR s_NetEntTLEvalX PROTO((ValNodePtr elst));
static Int2 NEAR s_NetDocSumListGet PROTO((DocSumPtr PNTR result, Int2 numuid, DocType type, DocUidPtr uids, Int2 defer_count));
static Int2 NEAR s_NetLinkUidList PROTO((LinkSetPtr PNTR result, DocType type, DocType link_to_type, Int2 numuid, DocUidPtr uids, Boolean mark_missing));
static LinkSetPtr NEAR s_NetUidLinks PROTO((DocType type, DocUid uid, DocType link_to_type));
static Int2 NEAR s_NetTermListByTerm PROTO((DocType type, DocField field, CharPtr term, Int2 numterms, TermListProc proc, Int2Ptr first_page));
static Int2 NEAR s_NetTermListByPage PROTO((DocType type, DocField field, Int2 page, Int2 numpage, TermListProc proc));
static Boolean NEAR s_NetEntrezFindTerm PROTO((DocType type, DocField field, CharPtr term, Int4Ptr spcl, Int4Ptr totl));
static void NEAR s_NetEntrezCreateNamedUidList PROTO((CharPtr term, DocType type, DocField field, Int4 num, DocUidPtr uids));
static Int2 NEAR s_NetEntSeqEntryListGet PROTO((SeqEntryPtr PNTR result, Int2 numuid, DocUidPtr uids, Int2 retcode, Boolean mark_missing));
static
Int2 NEAR s_NetEntMedlineEntryListGet PROTO((MedlineEntryPtr PNTR result, Int2 numuid, DocUidPtr uids, Boolean mark_missing));

static NamedListPtr KnownNamedTerm PROTO((CharPtr term, Boolean onlyIfNotKnown, DocType type, DocField field));
static void CleanupNamedUidLists PROTO((void));
static AsnTypePtr NetEntReadAsn PROTO((void));
static Boolean ReestablishNetEntrez PROTO((void));
static Boolean SwapInEntrez PROTO((VoidPtr med));
static void countChars PROTO((CharPtr str));

static NI_HandPtr Entrez_ni = NULL;
static AsnIoPtr   Entrez_asnin = NULL;
static AsnIoPtr   Entrez_asnout = NULL;
static EntrezInfoPtr vi = NULL;
static CharPtr    infoBuf = NULL;
static size_t     charCount = 0;
static ValNodePtr namedTerms = NULL;
#if defined(NETENTREZVERSION)
static CharPtr    netEntrezVersion = NETENTREZVERSION;
#else
static CharPtr    netEntrezVersion = "2.02c2ASN1SPEC6 ";
#endif
static CharPtr    userApplId = NULL;
static Boolean    userWarnings;
static EntrezExtraInfoPtr eeip = NULL;
static Int4       serviceAttempts = 0;

static AsnTypePtr ENTREZ_BACK = NULL;
static AsnTypePtr ENTREZ_BACK_blast_bad_count = NULL;
static AsnTypePtr ENTREZ_BACK_blast_job_progress = NULL;
static AsnTypePtr ENTREZ_BACK_blast_job_start = NULL;
static AsnTypePtr ENTREZ_BACK_blast_link_set = NULL;
static AsnTypePtr ENTREZ_BACK_cluster_arts = NULL;
static AsnTypePtr ENTREZ_BACK_docsumX = NULL;
static AsnTypePtr ENTREZ_BACK_error = NULL;
static AsnTypePtr ENTREZ_BACK_evalX = NULL;
static AsnTypePtr ENTREZ_BACK_eval_count = NULL;
static AsnTypePtr ENTREZ_BACK_extrainfo = NULL;
static AsnTypePtr ENTREZ_BACK_get_hierarchy = NULL;
static AsnTypePtr ENTREZ_BACK_init = NULL;
static AsnTypePtr ENTREZ_BACK_init_e_info = NULL;
static AsnTypePtr ENTREZ_BACK_neighbortext = NULL;
static AsnTypePtr ENTREZ_REQUEST = NULL;
static AsnTypePtr ENTREZ_REQUEST_blast = NULL;
static AsnTypePtr ENTREZ_REQUEST_bypage = NULL;
static AsnTypePtr ENTREZ_REQUEST_byterm = NULL;
static AsnTypePtr ENTREZ_REQUEST_cluster_arts = NULL;
static AsnTypePtr ENTREZ_REQUEST_createnamed = NULL;
static AsnTypePtr ENTREZ_REQUEST_docsumX = NULL;
static AsnTypePtr ENTREZ_REQUEST_evalX = NULL;
static AsnTypePtr ENTREZ_REQUEST_eval_count = NULL;
static AsnTypePtr ENTREZ_REQUEST_extrainfo = NULL;
static AsnTypePtr ENTREZ_REQUEST_findseqid = NULL;
static AsnTypePtr ENTREZ_REQUEST_findterm = NULL;
static AsnTypePtr ENTREZ_REQUEST_fini = NULL;
static AsnTypePtr ENTREZ_REQUEST_get_hierarchy = NULL;
static AsnTypePtr ENTREZ_REQUEST_getbiostr = NULL;
static AsnTypePtr ENTREZ_REQUEST_getbiostrX = NULL;
static AsnTypePtr ENTREZ_REQUEST_getbiostrX_get = NULL;
static AsnTypePtr REQUEST_getbiostrX_complexity = NULL;
static AsnTypePtr REQUEST_getbiostrX_max_models = NULL;
static AsnTypePtr ENTREZ_REQUEST_getbiostrannot = NULL;
static AsnTypePtr REQUEST_getbiostr_feat_ids = NULL;
static AsnTypePtr REQUEST_getbiostr_annot_by_fid = NULL;

static AsnTypePtr ENTREZ_REQUEST_getmle = NULL;
static AsnTypePtr ENTREZ_REQUEST_getseq = NULL;
static AsnTypePtr ENTREZ_REQUEST_initX = NULL;
static AsnTypePtr ENTREZ_REQUEST_initX_version = NULL;
static AsnTypePtr ENTREZ_REQUEST_linkuidlist = NULL;
static AsnTypePtr ENTREZ_REQUEST_neighbortext = NULL;
static AsnTypePtr ENTREZ_REQUEST_seqidforgi = NULL;
static AsnTypePtr ENTREZ_REQUEST_uidlinks = NULL;
static AsnTypePtr ENTREZ_TERMGET_cls = NULL;
static AsnTypePtr ENTREZ_TERMGET_terms = NULL;

extern EntrezInfoPtr LIBCALL EntrezInfoAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
EntrezInfoPtr NetEntrezGetInfo PROTO((void));
static void RemoveNonPrintingCharacters(CharPtr str);

extern void MsgSetReadTimeout PROTO((MHandPtr mh, int t));


static void NEAR FindAsnType (AsnTypePtr PNTR atp, AsnModulePtr amp, CharPtr str)

{
  if (atp != NULL && (*atp) == NULL) {
    *atp = AsnTypeFind (amp, str);
  }
}


/*****************************************************************************
*
*   NetEntrezInit ()
*
*****************************************************************************/

NLM_EXTERN Boolean CDECL NetEntrezInit (CharPtr appl_id, Boolean no_warnings)

{
    AsnIoPtr   asnin;
    AsnIoPtr   asnout;
    AsnTypePtr atp;
    MediaPtr   media;
    DataVal    av;
    CharPtr    versionId;
    AsnModulePtr amp;

    NetEntAsnLoad();
    amp = AsnAllModPtr();

    FindAsnType(&ENTREZ_BACK, amp, "Entrez-back");
    FindAsnType(&ENTREZ_BACK_blast_bad_count, amp, "Entrez-back.blast.bad-count");
    FindAsnType(&ENTREZ_BACK_blast_job_progress, amp, "Entrez-back.blast.job-progress");
    FindAsnType(&ENTREZ_BACK_blast_job_start, amp, "Entrez-back.blast.job-start");
    FindAsnType(&ENTREZ_BACK_blast_link_set, amp, "Entrez-back.blast.link-set");
    FindAsnType(&ENTREZ_BACK_cluster_arts, amp, "Entrez-back.cluster-arts");
    FindAsnType(&ENTREZ_BACK_docsumX, amp, "Entrez-back.docsumX");
    FindAsnType(&ENTREZ_BACK_error, amp, "Entrez-back.error");
    FindAsnType(&ENTREZ_BACK_evalX, amp, "Entrez-back.evalX");
    FindAsnType(&ENTREZ_BACK_eval_count, amp, "Entrez-back.eval-count");
    FindAsnType(&ENTREZ_BACK_extrainfo, amp, "Entrez-back.extrainfo");
    FindAsnType(&ENTREZ_BACK_get_hierarchy, amp, "Entrez-back.get-hierarchy");
    FindAsnType(&ENTREZ_BACK_init, amp, "Entrez-back.init");
    FindAsnType(&ENTREZ_BACK_init_e_info, amp, "Entrez-back.init.e-info");
    FindAsnType(&ENTREZ_BACK_neighbortext, amp, "Entrez-back.neighbortext");
    FindAsnType(&ENTREZ_REQUEST, amp, "Entrez-request");
    FindAsnType(&ENTREZ_REQUEST_blast, amp, "Entrez-request.blast");
    FindAsnType(&ENTREZ_REQUEST_bypage, amp, "Entrez-request.bypage");
    FindAsnType(&ENTREZ_REQUEST_byterm, amp, "Entrez-request.byterm");
    FindAsnType(&ENTREZ_REQUEST_cluster_arts, amp, "Entrez-request.cluster-arts");
    FindAsnType(&ENTREZ_REQUEST_createnamed, amp, "Entrez-request.createnamed");
    FindAsnType(&ENTREZ_REQUEST_docsumX, amp, "Entrez-request.docsumX");
    FindAsnType(&ENTREZ_REQUEST_evalX, amp, "Entrez-request.evalX");
    FindAsnType(&ENTREZ_REQUEST_eval_count, amp, "Entrez-request.eval-count");
    FindAsnType(&ENTREZ_REQUEST_extrainfo, amp, "Entrez-request.extrainfo");
    FindAsnType(&ENTREZ_REQUEST_findseqid, amp, "Entrez-request.findseqid");
    FindAsnType(&ENTREZ_REQUEST_findterm, amp, "Entrez-request.findterm");
    FindAsnType(&ENTREZ_REQUEST_fini, amp, "Entrez-request.fini");
    FindAsnType(&ENTREZ_REQUEST_get_hierarchy, amp, "Entrez-request.get-hierarchy");
    FindAsnType(&ENTREZ_REQUEST_getbiostr, amp, "Entrez-request.getbiostr");
    FindAsnType(&ENTREZ_REQUEST_getbiostrX, amp, "Entrez-request.getbiostrX");
    FindAsnType(&REQUEST_getbiostrX_complexity, amp, "Entrez-request.getbiostrX.complexity");
    FindAsnType(&REQUEST_getbiostrX_max_models, amp, "Entrez-request.getbiostrX.max-models");
    FindAsnType(&ENTREZ_REQUEST_getbiostrX_get, amp, "Entrez-request.getbiostrX.get");
    FindAsnType(&ENTREZ_REQUEST_getbiostrannot, amp, "Entrez-request.getbiostrannot");
    FindAsnType(&REQUEST_getbiostr_feat_ids, amp, "Entrez-request.getbiostr-feat-ids");
    FindAsnType(&REQUEST_getbiostr_annot_by_fid, amp, "Entrez-request.getbiostr-annot-by-fid");
    FindAsnType(&ENTREZ_REQUEST_getmle, amp, "Entrez-request.getmle");
    FindAsnType(&ENTREZ_REQUEST_getseq, amp, "Entrez-request.getseq");
    FindAsnType(&ENTREZ_REQUEST_initX, amp, "Entrez-request.initX");
    FindAsnType(&ENTREZ_REQUEST_initX_version, amp, "Entrez-request.initX.version");
    FindAsnType(&ENTREZ_REQUEST_linkuidlist, amp, "Entrez-request.linkuidlist");
    FindAsnType(&ENTREZ_REQUEST_neighbortext, amp, "Entrez-request.neighbortext");
    FindAsnType(&ENTREZ_REQUEST_seqidforgi, amp, "Entrez-request.seqidforgi");
    FindAsnType(&ENTREZ_REQUEST_uidlinks, amp, "Entrez-request.uidlinks");
    FindAsnType(&ENTREZ_TERMGET_cls, amp, "Entrez-termget.cls");
    FindAsnType(&ENTREZ_TERMGET_terms, amp, "Entrez-termget.terms");

    if (! NetEInit())
        return FALSE;

    userWarnings = no_warnings;

    Entrez_ni = NetServiceGet("ENTR_LINK", "Entrez", SwapInEntrez, Entrez_ni);
    if (Entrez_ni == NULL)
    {
    	serviceAttempts++;
        ErrPostEx (SEV_ERROR, 0, 0, "NI_ServiceGet [%s] (%s) <%ld>",
        	ni_errlist[ni_errno], ni_errtext, (long) serviceAttempts);
        Entrez_asnin = NULL;
        Entrez_asnout = NULL;
        NetEntrezFini();
        return FALSE;
    }

    asnin = Entrez_ni->raip;
    asnout = Entrez_ni->waip;

    Entrez_asnin = asnin;
    Entrez_asnout = asnout;

    /* don't get upset if non-printing chars are encountered */
    asnin->fix_non_print = 1;

    /**********************************************************/

    AsnWrite(asnout, ENTREZ_REQUEST, NULL);
    AsnStartStruct(asnout, ENTREZ_REQUEST_initX);
    if (netEntrezVersion != NULL) {
        if (appl_id == NULL) {
            av.ptrvalue = netEntrezVersion;
            AsnWrite(asnout, ENTREZ_REQUEST_initX_version, &av);
        } else {
            versionId = MemNew(StrLen(netEntrezVersion) + StrLen(appl_id) + 4);
            sprintf (versionId, "%s (%s)", netEntrezVersion, appl_id);
            userApplId = StringSave(appl_id);
            av.ptrvalue = versionId;
            AsnWrite(asnout, ENTREZ_REQUEST_initX_version, &av);
            MemFree (versionId);
        }
    }
    AsnEndStruct(asnout, ENTREZ_REQUEST_initX);
    AsnIoReset(asnout);


    if ((atp = NetEntReadAsn()) == NULL)
    {
        return FALSE;
    }
    else
    {
        AsnReadVal(asnin, atp, NULL);   /* read the NULL */
        atp = AsnReadId(asnin, amp, atp);
        if (atp == ENTREZ_BACK_init_e_info)
        {
            if (vi != NULL)
            {
                EntrezInfoFree (vi);
                vi = NULL;
            }
            vi = EntrezInfoAsnRead(asnin, atp);
            if ((media = GetCurMedia()) != NULL &&
                media->media_type == MEDIUM_NETWORK)
            {
                media->entrez_info = vi;
            }
            atp = AsnReadId(asnin, amp, atp);
        }
        if (atp != ENTREZ_BACK_init)
            return FALSE;
        AsnReadVal(asnin, atp, NULL);   /* read the NULL */
        ConfigInit();
        return  TRUE;
    }
}

/*****************************************************************************
*
*   NetEntrezFini ()
*
*****************************************************************************/

static void NEAR s_NetEntrezFini (void)

{
    AsnTypePtr atp;
    AsnIoPtr asnout = Entrez_asnout;
    AsnIoPtr asnin = Entrez_asnin;

    if (asnout != NULL && asnin != NULL)
    {
        AsnWrite(asnout, ENTREZ_REQUEST, NULL);
        AsnWrite(asnout, ENTREZ_REQUEST_fini, NULL);
        AsnIoReset(asnout);
 
        if ((atp = NetEntReadAsn()) != NULL)
            AsnReadVal(asnin, atp, NULL);   /* read the NULL */
    }

    if (infoBuf != NULL)
    {
        MemFree(infoBuf);
        infoBuf = NULL;
    }

    if (userApplId != NULL)
    {
        MemFree(userApplId);
        userApplId = NULL;
    }
    if (eeip != NULL)
    {
        EntrezExtraInfoFree(eeip);
        eeip = NULL;
    }

    NI_ServiceDisconnect(Entrez_ni);
    NetFini();
    ConfigFini();
}

/* the only thing done here is to suppress errors */
NLM_EXTERN void CDECL NetEntrezFini (void)

{
    short erract;
    ErrDesc err;

    ErrGetOpts(&erract, NULL);
    ErrSetOpts(ERR_CONTINUE, 0);
    ErrFetch(&err);

    s_NetEntrezFini();

    ErrSetOpts(erract, 0);
    ErrFetch(&err);

    CleanupNamedUidLists ();
}

/*****************************************************************************
*
*   NetEntrezGetInfo()
*       returns global Entrez information
*
*****************************************************************************/
NLM_EXTERN EntrezInfoPtr CDECL
NetEntrezGetInfo(void)
{
    return vi;
}

/*****************************************************************************
*
*   NetEntrezDetailedInfo()
*       returns NULL, or a string to some descriptive text
*
*****************************************************************************/
NLM_EXTERN CharPtr CDECL
NetEntrezDetailedInfo(void)
{
    charCount = 0;
    DumpNetStats(SUBSYS_CLI_ENTREZ, countChars);

    if (infoBuf != NULL)
        MemFree(infoBuf);
    infoBuf = MemNew(charCount + 540);
    GetClientInfo(infoBuf);
    return infoBuf;
}

static EntrezExtraInfoPtr
GetEntrezExtraInfo(void)
{
    AsnTypePtr atp;
    DataVal av;

    if (eeip == NULL && Entrez_asnout != NULL)
    {
        AsnWrite(Entrez_asnout, ENTREZ_REQUEST, NULL);
        av.intvalue = 4; /* number of currently known fields */
        AsnWrite(Entrez_asnout, ENTREZ_REQUEST_extrainfo, &av);
        AsnIoReset(Entrez_asnout);
        if ((atp = NetEntReadAsn()) != NULL)
            eeip = EntrezExtraInfoAsnRead(Entrez_asnin, ENTREZ_BACK_extrainfo);
    }

    return eeip;
}

/*****************************************************************************
*
*   NetEntGetMaxLinks()
*       returns max links in link set allowed by system
*
*****************************************************************************/

NLM_EXTERN Int4 NetEntGetMaxLinks(void)
{
    EntrezExtraInfoPtr myeeip;

    if ((myeeip = GetEntrezExtraInfo()) == NULL)
    {
        return -1;
    }

    return myeeip->maxlinks;
}

/*****************************************************************************
*
*   NetEntTLNew ()
*
*****************************************************************************/

NLM_EXTERN ValNodePtr CDECL NetEntTLNew (DocType type)

{
  ValNodePtr anp;

  anp = ValNodeNew(NULL);
  anp->data.intvalue = (Int4)type;
  return anp;
}

/*****************************************************************************
*
*   NetEntTLAddTerm (elst, term, type, field, special)
*       Adds a term node to a boolean algebraic term query.
*
*****************************************************************************/

NLM_EXTERN ValNodePtr CDECL NetEntTLAddTerm(ValNodePtr elst, CharPtr term, DocType type,
                                  DocField field, Boolean special, CharPtr highRange)

{
    ValNodePtr anp;
    TermDataPtr termdatp;
    NamedListPtr nlp;
  
    anp = NULL;
    if (elst != NULL) {
        if (elst->data.intvalue != (Int4)type)
            return NULL;
        anp = ValNodeNew (elst);
        if (anp != NULL) {
            if ((nlp = KnownNamedTerm(term, TRUE, type, field)) != NULL &&
                nlp->uids != NULL) {
                NetEntrezCreateNamedUidList (term, type, field, nlp->uids->numid,
                                             nlp->uids->ids);
            }
  
            if (special) {
                anp->choice = SPECIALTERM;
            } else {
                anp->choice = TOTALTERM;
            }
  
            termdatp = (TermDataPtr) MemNew(sizeof(TermData));
            termdatp->field = field;
            termdatp->type = type;
            termdatp->term = StringSave(term);
            termdatp->highRange = StringSave(highRange);
            anp->data.ptrvalue = (Pointer) termdatp;
        }
    }
    LOG_STAT(SUBSYS_CLI_ENTREZ, "Boolean terms", 1);
    return anp;
}

/*****************************************************************************
*
*   NetEntTLFree (elst)
*       Frees a boolean algebraic term query list.
*
*****************************************************************************/

NLM_EXTERN ValNodePtr CDECL NetEntTLFree (ValNodePtr elst)

{
    ValNodePtr anp;
    TermDataPtr trmp;

    if (elst != NULL) {
        elst->data.intvalue = 0;   /* clear type */
        for (anp = elst->next; anp != NULL; anp = anp->next) {
            if (anp->choice == SPECIALTERM || anp->choice == TOTALTERM) {
                trmp = (TermDataPtr) anp->data.ptrvalue;
                MemFree (trmp->term);
                trmp->term = NULL;
                MemFree (trmp->highRange);
                trmp->highRange = NULL;
            }
        }
        ValNodeFreeData (elst);
    }
    return NULL;
}

/*****************************************************************************
*
*   NetEntTLEval (elst)
*       Evaluates a boolean algebraic term query list, returning a pointer to
*       a LinkSetPtr containing the resultant unique identifiers.
*
*****************************************************************************/


NLM_EXTERN LinkSetPtr CDECL NetEntTLEval (ValNodePtr elst)

{
    ByteStorePtr bsp;
    LinkSetPtr lsp;
    Int4 numlinks;

    if ((bsp = NetEntTLEvalX(elst)) != NULL)
    {
        numlinks = BSLen(bsp) / sizeof(DocUid);
        lsp = LinkSetNew();
        lsp->num = numlinks;
        if (numlinks <= EntrezGetUserMaxLinks())
        {
                lsp->uids = MemNew((size_t)(numlinks * sizeof(DocUid)));                        BSSeek (bsp, 0L, 0);
                BSRead(bsp, lsp->uids, (numlinks * sizeof(DocUid)));
        }
        BSFree(bsp);
    }
    return lsp;

}




/*****************************************************************************
*
*   NetEntTLEvalX (elst)
*       Evaluates a boolean algebraic term query list, returning a pointer to
*       a ByteStore containing the resultant unique identifiers.
*
*****************************************************************************/

static ByteStorePtr NEAR s_NetEntTLEvalX (ValNodePtr elst)

{
    ByteStorePtr bsp = NULL;
    AsnTypePtr atp;
    DataVal av;

    if (elst == NULL)
        return NULL;
    if (elst->next == NULL) /* expression without terms */
        return NULL;
    AsnWrite(Entrez_asnout, ENTREZ_REQUEST, NULL);
    AsnStartStruct(Entrez_asnout, ENTREZ_REQUEST_evalX);
    av.intvalue = elst->data.intvalue;
    AsnWrite(Entrez_asnout, ENTREZ_TERMGET_cls, &av);
    BoolExprAsnWrite (elst->next, Entrez_asnout, ENTREZ_TERMGET_terms);
    AsnEndStruct(Entrez_asnout, ENTREZ_REQUEST_evalX);
    AsnIoReset(Entrez_asnout);

    if ((atp = NetEntReadAsn()) == NULL)
        return NULL;

    if (atp == ENTREZ_BACK_evalX)
    {
        AsnReadVal(Entrez_asnin, atp, &av);
        bsp = (ByteStorePtr) av.ptrvalue;
#ifdef IS_LITTLE_ENDIAN
        {
            Int4 datum;
            Int4 offset;
            Int4 i;

            for (i = BSLen(bsp) / sizeof(Int4), offset = 0; i > 0;
                 i--, offset += sizeof(Int4))
            {
                BSSeek(bsp, offset, SEEK_SET);
                BSRead(bsp, &datum, sizeof(Int4));
                datum = SwapUint4(datum);
                BSSeek(bsp, offset, SEEK_SET);
                BSWrite(bsp, &datum, sizeof(Int4));
            }
        }
#endif /* IS_LITTLE_ENDIAN */
    }
    LOG_STAT(SUBSYS_CLI_ENTREZ, "Boolean expressions evaluated", 1);
    LOG_STAT(SUBSYS_CLI_ENTREZ, "Resulting UIDs from Boolean evaluation", bsp == NULL ? 0 : BSLen(bsp) / sizeof(DocUid));
    return bsp;
}

NLM_EXTERN ByteStorePtr CDECL NetEntTLEvalX (ValNodePtr elst)

{
    int i;
    ByteStorePtr retval;
    short erract;
    ErrDesc err;

    for (i = 0; i < ENT_SERV_RETRIES; i++)
    {
        if (i > 0)
        {
            if (! ReestablishNetEntrez())
                break;
        }

        ErrGetOpts(&erract, NULL);
        ErrSetOpts(ERR_CONTINUE, 0);
        ErrFetch(&err);
        MsgSetReadTimeout(Entrez_ni, 1200);
        retval = s_NetEntTLEvalX(elst);
        MsgSetReadTimeout(Entrez_ni, 60);
        ErrSetOpts(erract, 0);
        if (! ErrFetch(&err))
            return retval; /* success */
    }

    ErrPost(CTX_UNKNOWN, 1, "NetEntTLEval failure");
    return NULL;
}

/*****************************************************************************
*
*   NetEntTLEvalCount (elst)
*       Evaluates a boolean algebraic term query list, returning a count of
*       the resultant unique identifiers.
*
*****************************************************************************/

static Int4 NEAR s_NetEntTLEvalCount (ValNodePtr elst)

{
    Int4 retval = 0;
    AsnTypePtr atp;
    DataVal av;

    if (elst->next == NULL) /* expression without terms */
        return 0;
    AsnWrite(Entrez_asnout, ENTREZ_REQUEST, NULL);
    AsnStartStruct(Entrez_asnout, ENTREZ_REQUEST_eval_count);
    av.intvalue = elst->data.intvalue;
    AsnWrite(Entrez_asnout, ENTREZ_TERMGET_cls, &av);
    BoolExprAsnWrite (elst->next, Entrez_asnout, ENTREZ_TERMGET_terms);
    AsnEndStruct(Entrez_asnout, ENTREZ_REQUEST_eval_count);
    AsnIoReset(Entrez_asnout);

    MsgSetReadTimeout(Entrez_ni, 1200);
    if ((atp = NetEntReadAsn()) == NULL)
        return 0;
    MsgSetReadTimeout(Entrez_ni, 60);

    if (atp == ENTREZ_BACK_eval_count)
    {
        AsnReadVal(Entrez_asnin, atp, &av);
        retval = (Int4) av.intvalue;
    }
    LOG_STAT(SUBSYS_CLI_ENTREZ, "Boolean expressions counted", 1);
    LOG_STAT(SUBSYS_CLI_ENTREZ, "Resulting UIDs from Boolean count-evaluation", retval);
    return retval;
}

NLM_EXTERN Int4 CDECL NetEntTLEvalCount (ValNodePtr elst)

{
    int i;
    Int4 retval;
    short erract;
    ErrDesc err;

    for (i = 0; i < ENT_SERV_RETRIES; i++)
    {
        if (i > 0)
        {
            if (! ReestablishNetEntrez())
                break;
        }

        ErrGetOpts(&erract, NULL);
        ErrSetOpts(ERR_CONTINUE, 0);
        ErrFetch(&err);
        retval = s_NetEntTLEvalCount(elst);
        ErrSetOpts(erract, 0);
        if (! ErrFetch(&err))
            return retval; /* success */
    }

    ErrPost(CTX_UNKNOWN, 1, "NetEntTLEvalCount failure");
    return 0;
}


/*****************************************************************************
*
*   NetDocSumListGet (result, numuid, type, uids)
*       returns a count of entries read
*       head of linked list is in result
*
*****************************************************************************/
static Int2 NEAR s_NetDocSumListGet (DocSumPtr PNTR result, Int2 numuid,
                                      DocType type, DocUidPtr uids,
                                      Int2 defer_count)

{
    AsnTypePtr atp;
    EntrezIdsPtr eip;
    Int2 retval;
    EntrezDocGetPtr edgp;
    AsnModulePtr amp;
    NewSummaryListPtr newlist;

    NetEntAsnLoad();
    amp = AsnAllModPtr();

    AsnWrite(Entrez_asnout, ENTREZ_REQUEST, NULL);
    edgp = EntrezDocGetNew();
    edgp->ids = EntrezIdsNew();
    eip = edgp->ids;
    edgp->cls = type;
    edgp->mark_missing = FALSE;
    eip->numid = numuid;
    eip->ids = uids;
    edgp->defer_count = defer_count;
    EntrezDocGetAsnWrite(edgp, Entrez_asnout, ENTREZ_REQUEST_docsumX);
    eip->ids = NULL; /* for clean memory free */
    EntrezDocGetFree(edgp);

    AsnIoReset(Entrez_asnout);

    if ((atp = NetEntReadAsn()) == NULL)
        return -1;

    newlist = NewSummaryListAsnRead(Entrez_asnin, atp);
    if (newlist == NULL)
        return -1;
    if (newlist->type != type)
    {
        NewSummaryListFree(newlist);
        return -1;
    }
    MemCopy (result, newlist->data, (size_t) newlist->num * sizeof(DocSumPtr));
    retval = newlist->num;
    MemFree(newlist->data);
    newlist->data = NULL; /* to avoid freeing actual DocSums */
    NewSummaryListFree (newlist);

    LOG_STAT(SUBSYS_CLI_ENTREZ, "Document summaries retrieved", retval);
    LOG_STAT(SUBSYS_CLI_ENTREZ, "Document summaries requested", numuid);

    return retval;
}

NLM_EXTERN Int2 CDECL NetDocSumListGet (DocSumPtr PNTR result, Int2 numuid,
                          DocType type, DocUidPtr uids, Int2 defer_count)
{
    int i;
    int retval;
    short erract;
    ErrDesc err;

    /* default is to obtain these from the Entrez server */

    for (i = 0; i < ENT_SERV_RETRIES; i++)
    {
        if (i > 0)
        {
            if (! ReestablishNetEntrez())
                break;
        }

        ErrGetOpts(&erract, NULL);
        ErrSetOpts(ERR_CONTINUE, 0);
        ErrFetch(&err);
        retval = s_NetDocSumListGet (result, numuid, type, uids, defer_count);
        ErrSetOpts(erract, 0);
        if (! ErrFetch(&err))
        {
            goto expand_uids;
        }
    }

    ErrPost(CTX_UNKNOWN, 1, "DocSumGet failure");
    return -1;

expand_uids:
    if (retval < 0 || retval == numuid)
        return retval;

    {
        Int2 i,j;
        DocSumPtr dsp;
#define USE_TEMP_MEMORY
#ifdef USE_TEMP_MEMORY
        DocSumPtr PNTR temp;
    
        /* some UIDs are missing; we must find which ones and insert NULLs */
        if ((temp = MemNew(sizeof(DocSumPtr) * numuid)) == NULL)
        {
            ErrPost(CTX_UNKNOWN, 1, "DocSumGet failure <memory allocation>");
            return -1;
        }
    
        for (i = 0; i < numuid; i++)
        {
            temp[i] = result[i];
            result[i] = NULL;
        }

        for (i = 0, j = 0; i < retval; i++) {
            dsp = temp[i];
            if (dsp != NULL) {
                while (j < numuid && dsp->uid != uids[j]) {
                    j++;
                }
                if (j < numuid) {
                    result[j] = dsp;
                    j++;
                }
            }
        }

        MemFree (temp);
#else
        /* more dangerous algorithm ... operates on result in-place */
        for (i = retval - 1, j = numuid - 1; i >= 0; i--) {
            dsp = result[i];
            if (dsp != NULL) {
                while (j >= 0 && dsp->uid != uids[j]) {
                    result[j] = NULL;
                    j--;
                }
                if (j >= 0)
                {
                    result[j] = dsp;
                    j--;
                }
            }
        }

        for (; j >= 0; j--)
        { /* null out any unassigned bottom entries */
            result[j] = NULL;
        }
#endif /* USE_TEMP_MEMORY */

        return retval;
    }
}
        

/*****************************************************************************
*
*   DocSumPtr NetDocSum(type, uid)
*
*****************************************************************************/

NLM_EXTERN DocSumPtr CDECL NetDocSum (DocType type, DocUid uid)
{
    DocSumPtr dsp = NULL;

    NetDocSumListGet(&dsp, 1, type, &uid, 0);
    return dsp;
}

/*****************************************************************************
*
*   NetLinkUidList(type, link_to_type, numuid, uids)
*       returns count of input uids processed
*       returns -1 on error
*       if neighbors (type == link_to_type)
*           sums weights for same uids
*       if (more than EntrezUserMaxLinks() uids, frees uids and weights,
*           but leaves num set)
*
*****************************************************************************/
static
Int2 NEAR s_NetLinkUidList (LinkSetPtr PNTR result, DocType type,
                              DocType   link_to_type, Int2 numuid, DocUidPtr uids,
                              Boolean mark_missing)

{
    LinkSetGetPtr lsgp;
    MarkedLinkSetPtr mls;
    AsnTypePtr atp;
    Int2 retval;

    lsgp = LinkSetGetNew();
    lsgp->query_cls = type;
    lsgp->link_to_cls = link_to_type;
    lsgp->max = EntrezGetUserMaxLinks();
    lsgp->mark_missing = mark_missing;
    lsgp->query_size = numuid;
    lsgp->query = MemNew(sizeof(DocUid) * numuid);

    MemCopy (lsgp->query, uids, sizeof(DocUid) * numuid);
    AsnWrite(Entrez_asnout, ENTREZ_REQUEST, NULL);
    LinkSetGetAsnWrite(lsgp, Entrez_asnout, ENTREZ_REQUEST_linkuidlist);
    AsnIoReset(Entrez_asnout);

    LinkSetGetFree (lsgp);

    if ((atp = NetEntReadAsn()) == NULL)
        return -1;
    if ((mls = MarkedLinkSetAsnRead(Entrez_asnin, atp)) == NULL)
        return -1;
    *result = mls->link_set;
    retval = mls->uids_processed;
    if (mark_missing && mls->marked_missing != NULL)
        MemCopy (uids, mls->marked_missing->ids, numuid * sizeof(DocUid));
    mls->link_set = NULL; /* for clean memory free */
    MarkedLinkSetFree (mls);

    LOG_STAT(SUBSYS_CLI_ENTREZ, "UID attempted for neighbors", numuid);
    LOG_STAT(SUBSYS_CLI_ENTREZ, "UID processed for neighbors", retval);
    LOG_STAT(SUBSYS_CLI_ENTREZ, "UID neighbors found", *result == NULL ? 0 : (*result)->num);
    return retval;
}

NLM_EXTERN Int2 CDECL NetLinkUidList (LinkSetPtr PNTR result, DocType type,
                            DocType link_to_type, Int2 numuid, DocUidPtr uids,
                            Boolean mark_missing)
{
    int i;
    int retval;
    short erract;
    ErrDesc err;
    DocUidPtr local_uids;

    /* make a local copy, to handle modifications */
    local_uids = MemNew(sizeof(DocUid) * numuid);

    for (i = 0; i < ENT_SERV_RETRIES; i++)
    {
        MemCopy(local_uids, uids, sizeof(DocUid) * numuid);
        if (i > 0)
        {
            if (! ReestablishNetEntrez())
                break;
        }

        ErrGetOpts(&erract, NULL);
        ErrSetOpts(ERR_CONTINUE, 0);
        ErrFetch(&err);
        retval = s_NetLinkUidList(result, type, link_to_type, numuid,
                      local_uids, mark_missing);
        ErrSetOpts(erract, 0);
        if (! ErrFetch(&err))
        {
            MemCopy(uids, local_uids, sizeof(DocUid) * numuid);
            MemFree(local_uids);
            return retval; /* success */
        }
    }

    MemFree(local_uids);
    ErrPost(CTX_UNKNOWN, 1, "NetLinkUidList failure");
    return -1;
}

/*****************************************************************************
*
*   NetUidLinks()
*       retrieves links to other uids
*
*****************************************************************************/
static
LinkSetPtr NEAR s_NetUidLinks(DocType type, DocUid uid, DocType link_to_type)

{
    LinkSetPtr lsp;
    LinkSetGetPtr lsgp;
    MarkedLinkSetPtr mls;
    AsnTypePtr atp;

    lsgp = LinkSetGetNew();
    lsgp->query_cls = type;
    lsgp->link_to_cls = link_to_type;
    lsgp->max = EntrezGetUserMaxLinks();
    lsgp->mark_missing = FALSE;
    lsgp->query_size = 1;
    lsgp->query = MemNew(sizeof(DocUid));
    lsgp->query[0] = uid;

    AsnWrite(Entrez_asnout, ENTREZ_REQUEST, NULL);
    LinkSetGetAsnWrite(lsgp, Entrez_asnout, ENTREZ_REQUEST_uidlinks);
    AsnIoReset(Entrez_asnout);

    LinkSetGetFree (lsgp);

    if ((atp = NetEntReadAsn()) == NULL)
        return NULL;
    if ((mls = MarkedLinkSetAsnRead(Entrez_asnin, atp)) == NULL)
        return NULL;
    lsp = mls->link_set;
    mls->link_set = NULL; /* for clean memory free */
    MarkedLinkSetFree (mls);

    return lsp;
}

NLM_EXTERN LinkSetPtr CDECL NetUidLinks(DocType type, DocUid uid, DocType link_to_type)
{
    int i;
    LinkSetPtr retval;
    short erract;
    ErrDesc err;

    for (i = 0; i < ENT_SERV_RETRIES; i++)
    {
        if (i > 0)
        {
            if (! ReestablishNetEntrez())
                break;
        }

        ErrGetOpts(&erract, NULL);
        ErrSetOpts(ERR_CONTINUE, 0);
        ErrFetch(&err);
        retval = s_NetUidLinks(type, uid, link_to_type);
        ErrSetOpts(erract, 0);
        if (! ErrFetch(&err))
            return retval; /* success */
    }

    ErrPost(CTX_UNKNOWN, 1, "NetUidLinks failure");
    return NULL;
}


/* The number of pages or terms to be obtain from the network in one request */
#define NUM_NET_PAGES       4
#define NUM_NET_TERMS       50

/* The maximum number of terms to obtain in one TermListByTerm; this value   */
/* need not depend upon NUM_NET_TERMS                                        */
#define OBTAIN_IN_ONE_READ 100

/*****************************************************************************
*
*   NetTermListByTerm (type, field, term, numterms, proc, first_page)
*       Gets Terms starting with page near term
*       returns number of complete pages read
*       sets first_page to first page read
*
*****************************************************************************/
static
Int2 NEAR s_NetTermListByTerm (DocType type, DocField field, CharPtr term,
                                 Int2 numterms, TermListProc proc,
                                 Int2Ptr first_page)

{
    AsnIoPtr asnin = Entrez_asnin;
    AsnIoPtr asnout = Entrez_asnout;
    int i;
    TermRespPtr p;
    TermByTermPtr tbt;
    TermByPagePtr tbp;
    AsnTypePtr atp;
    struct termresp PNTR termp;
    int retval = 0;
    int first_read = TRUE;
    Boolean done = FALSE;
    int next_page;
    Int4 max_terms;
    int terms_to_read;

    if (numterms == 0) /* 0 ==> infinity */
        max_terms = INT4_MAX;
    else
        max_terms = numterms;

    while (! done) {
        terms_to_read = MIN(max_terms, NUM_NET_TERMS);
        if (first_read)
        {
            tbt = TermByTermNew();
    
            AsnWrite(Entrez_asnout, ENTREZ_REQUEST, NULL);
            tbt->type = type;
            tbt->fld = field;
            tbt->term = StringSave(term);

            /* For efficiency, if the caller has only requested a few terms, */
            /* then obtain them all in one shot; also note that if we're not */
            /* going to obtain all of the requested terms in a single shot,  */
            /* then num_terms must be 0, because we need to read an integral */
            /* number of pages.                                              */
            if (max_terms <= OBTAIN_IN_ONE_READ)
                tbt->num_terms = max_terms;
            else
                tbt->num_terms = 0; /* bounded by # of pages, not terms */

            TermByTermAsnWrite(tbt, Entrez_asnout, ENTREZ_REQUEST_byterm);
            TermByTermFree (tbt);
        }
        else { /* we already know what page */
            tbp = TermByPageNew();
    
            AsnWrite(Entrez_asnout, ENTREZ_REQUEST, NULL);
            tbp->type = type;
            tbp->fld = field;
            tbp->page = next_page;
            tbp->num_pages = NUM_NET_PAGES;
            TermByPageAsnWrite(tbp, Entrez_asnout, ENTREZ_REQUEST_bypage);
            TermByPageFree (tbp);
        }

        AsnIoReset(Entrez_asnout);
    
        /* now, read back response */
        if ((atp = NetEntReadAsn()) == NULL)
            return -1;
        if ((p = TermRespAsnRead(asnin, atp)) == NULL)
            return -1;
    
        if (first_read)
        {
            *first_page = p->first_page;
            next_page = p->first_page;
            first_read = FALSE;
        }

        if (p->num_terms == 0) /* end of file */
            done = TRUE;

        termp = p->termresp;
    
        LOG_STAT(SUBSYS_CLI_ENTREZ, "Terms read (by term)", p->num_terms);
        for (i = 0; i < p->num_terms && !done; i++, termp++)
        {
            /* StringSave() is needed because application frees the term */
            if (! proc(termp->term, termp->special_count, termp->total_count) )
                done = TRUE;
            termp->term = NULL; 
        }
    
        retval += p->num_pages_read;
        max_terms -= p->num_terms;
        next_page += p->num_pages_read;

        TermRespFree (p);

        if (max_terms <= 0)
            done = TRUE;
    }

    LOG_STAT(SUBSYS_CLI_ENTREZ, "Term pages read (by term)", retval);
    return retval; /* page count */
}

NLM_EXTERN Int2 CDECL NetTermListByTerm (DocType type, DocField field, CharPtr term,
                               Int2 numterms, TermListProc proc,
                               Int2Ptr first_page)
{
    int i;
    int retval;
    short erract;
    ErrDesc err;

    for (i = 0; i < ENT_SERV_RETRIES; i++)
    {
        if (i > 0)
        {
            if (! ReestablishNetEntrez())
                break;
        }

        ErrGetOpts(&erract, NULL);
        ErrSetOpts(ERR_CONTINUE, 0);
        ErrFetch(&err);
        retval = s_NetTermListByTerm (type, field, term, numterms,
                                      proc, first_page);
        ErrSetOpts(erract, 0);
        if (! ErrFetch(&err))
            return retval; /* success */
    }

    ErrPost(CTX_UNKNOWN, 1, "NetTermListByTerm failure");
    return 0;
}

/*****************************************************************************
*
*   NetTermListByPage (type, field, page, numpage, proc)
*       Gets terms starting at page, for numpage, by calling proc
*       returns number of complete pages read
*
*****************************************************************************/

static
Int2 NEAR s_NetTermListByPage (DocType type, DocField field, Int2 page,
                                 Int2 numpage, TermListProc proc)

{
    AsnIoPtr asnin = Entrez_asnin;
    AsnIoPtr asnout = Entrez_asnout;
    TermRespPtr p;
    int i;
    struct termresp PNTR termp;
    TermByPagePtr tbp;
    AsnTypePtr atp;
    Boolean done = FALSE;
    int retval = 0;
    int next_page = page;
    int pages_to_read;
    Int4 max_pages;

    if (numpage == 0) /* 0 ==> infinity */
        max_pages = INT4_MAX;
    else
        max_pages = numpage;

    while (! done) {
        pages_to_read = MIN(max_pages, NUM_NET_PAGES);
        tbp = TermByPageNew();
    
        AsnWrite(Entrez_asnout, ENTREZ_REQUEST, NULL);
        tbp->type = type;
        tbp->fld = field;
        tbp->page = next_page;
        tbp->num_pages = pages_to_read;
        TermByPageAsnWrite(tbp, Entrez_asnout, ENTREZ_REQUEST_bypage);
        TermByPageFree(tbp);
        AsnIoReset(Entrez_asnout);
    
        /* now, read back response */
        if ((atp = NetEntReadAsn()) == NULL)
            return -1;
        if ((p = TermRespAsnRead(asnin, atp)) == NULL)
            return -1;
    
        if (p->num_terms == 0) /* end of file */
            done = TRUE;

        termp = p->termresp;
    
        LOG_STAT(SUBSYS_CLI_ENTREZ, "Terms read (by page)", p->num_terms);
        for (i = 0; i < p->num_terms && !done; i++, termp++)
        {
            /* StringSave() is needed because application frees the term */
            if (! proc(termp->term, termp->special_count, termp->total_count) )
                done = TRUE;
            termp->term = NULL;
        }
    
        retval += p->num_pages_read;
        TermRespFree (p);

        next_page += pages_to_read;
        max_pages -= pages_to_read;

        if (max_pages <= 0)
            done = TRUE;
    }
    LOG_STAT(SUBSYS_CLI_ENTREZ, "Term pages read (by page)", retval);
    return retval; /* page count */
}

NLM_EXTERN Int2 CDECL NetTermListByPage (DocType type, DocField field, Int2 page,
                               Int2 numpage, TermListProc proc)
{
    int i;
    int retval;
    short erract;
    ErrDesc err;

    for (i = 0; i < ENT_SERV_RETRIES; i++)
    {
        if (i > 0)
        {
            if (! ReestablishNetEntrez())
                break;
        }

        ErrGetOpts(&erract, NULL);
        ErrSetOpts(ERR_CONTINUE, 0);
        ErrFetch(&err);
        retval = s_NetTermListByPage (type, field, page, numpage, proc);
        ErrSetOpts(erract, 0);
        if (! ErrFetch(&err))
            return retval; /* success */
    }

    ErrPost(CTX_UNKNOWN, 1, "NetTermListByPage failure");
    return 0;
}

/*****************************************************************************
*
*   NetEntrezFindTerm(type, field, term, spec, total)
*       returns count of special and total for a term
*       if term ends with  "...", does a truncated merge of the term
*
*****************************************************************************/
static
Boolean NEAR s_NetEntrezFindTerm (DocType type, DocField field, 
                                  CharPtr term, Int4Ptr spcl, Int4Ptr totl)

{
    AsnIoPtr asnin = Entrez_asnin;
    AsnIoPtr asnout = Entrez_asnout;
    TermCountsPtr tcs;
    TermLookupPtr tlp;
    Boolean found;
    AsnTypePtr atp;
    
    tlp = TermLookupNew();
    tlp->type = type;
    tlp->fld = field;
    tlp->term = StringSave(term);
    AsnWrite(asnout, ENTREZ_REQUEST, NULL);
    TermLookupAsnWrite(tlp, asnout, ENTREZ_REQUEST_findterm);
    TermLookupFree(tlp);
    AsnIoReset(asnout);

    /* now, read back response */
    if ((atp = NetEntReadAsn()) == NULL)
        return FALSE;
    if ((tcs = TermCountsAsnRead(asnin, atp)) == NULL)
        return FALSE;
    LOG_STAT(SUBSYS_CLI_ENTREZ, "Term lookup attempts", 1);
    LOG_STAT(SUBSYS_CLI_ENTREZ, "Terms found (special)", tcs->spec_count);
    LOG_STAT(SUBSYS_CLI_ENTREZ, "Terms found (total)", tcs->tot_count);
    *spcl = tcs->spec_count;
    *totl = tcs->tot_count;
    found = tcs->found;

    TermCountsFree(tcs);

    return found;
}

NLM_EXTERN Boolean CDECL NetEntrezFindTerm (DocType type, DocField field,
                                 CharPtr term, Int4Ptr spcl, Int4Ptr totl)

{
    int i;
    int retval;
    short erract;
    ErrDesc err;

    for (i = 0; i < ENT_SERV_RETRIES; i++)
    {
        if (i > 0)
        {
            if (! ReestablishNetEntrez())
                break;
        }

        ErrGetOpts(&erract, NULL);
        ErrSetOpts(ERR_CONTINUE, 0);
        ErrFetch(&err);
        retval = s_NetEntrezFindTerm (type, field, term, spcl, totl);
        ErrSetOpts(erract, 0);
        if (! ErrFetch(&err))
            return retval; /* success */
    }

    ErrPost(CTX_UNKNOWN, 1, "NetEntrezFindTerm failure");
    return FALSE;
}


static void CleanupNamedUidLists (void)
{
    ValNodePtr node;
    NamedItemPtr nip;
    ValNodePtr nextNode;

    for (node = namedTerms; node != NULL; node = nextNode)
    {
        nip = (NamedItemPtr) node->data.ptrvalue;

        nextNode = node->next;

        if (nip != NULL) {
            FileRemove (nip->tmpFileName);
            MemFree (nip->term);
            MemFree (nip);
        }

        MemFree (node);
    }

    namedTerms = NULL;
}

/**** Check to see if a term is already known to the Network server ********/
static NamedListPtr KnownNamedTerm (CharPtr term, Boolean onlyIfNotKnown,
                                    DocType type, DocField field)
{
    static NamedList dummy;
    ValNodePtr node;
    NamedItemPtr nip;
    AsnIoPtr aip;
    NamedListPtr nlp;


    for (node = namedTerms; node != NULL; node = node->next)
    {
        nip = (NamedItemPtr) node->data.ptrvalue;

        if (nip == NULL) {
            continue;
        }

        if (StringICmp (term, nip->term) == 0 && type == nip->type &&
            field == nip->field)
        { /* match */
            if (onlyIfNotKnown) {
                if (! nip->knownToServer) {
                    /* read the file and load the data structure */
                    aip = AsnIoOpen (nip->tmpFileName, "rb");
                    nlp = NamedListAsnRead(aip, NULL);
                    AsnIoClose (aip);
                    return nlp;
                } else {
                    return NULL;
                }
            } else {
                return &dummy;
            }
        }
    }

    return NULL; /* not found */
}

/**** Creates a term node from the uid parameter ********/
static void NEAR s_NetEntrezCreateNamedUidList (CharPtr term, DocType type, DocField field, Int4 num, DocUidPtr uids)
{
    AsnIoPtr asnin = Entrez_asnin;
    AsnIoPtr asnout = Entrez_asnout;
    NamedListPtr nlp;
    AsnTypePtr atp;
    NamedItemPtr namedItem;
    AsnIoPtr aip;
    ValNodePtr node;

    nlp = NamedListNew();
    nlp->type = type;
    nlp->fld = field;
    nlp->term = StringSave(term);
    nlp->uids = EntrezIdsNew();
    nlp->uids->numid = num;
    nlp->uids->ids = (DocUidPtr) MemDup(uids, (size_t) num * sizeof(DocUid));
    AsnWrite(asnout, ENTREZ_REQUEST, NULL);
    NamedListAsnWrite(nlp, asnout, ENTREZ_REQUEST_createnamed);
    
    AsnIoReset(asnout);

    if (KnownNamedTerm(term, FALSE, type, field) == NULL) {
        node = ValNodeNew (namedTerms);
        if (namedTerms == NULL) {
          namedTerms = node;
        }
        namedItem = MemNew (sizeof(NamedItem));
        if (node != NULL && namedItem != NULL) {
            namedItem->knownToServer = TRUE;
            TmpNam (namedItem->tmpFileName);
            namedItem->term = StringSave(term);
            namedItem->type = type;
            namedItem->field = field;
            node->data.ptrvalue = (VoidPtr) namedItem;
            aip = AsnIoOpen (namedItem->tmpFileName, "wb");
            NamedListAsnWrite (nlp, aip, NULL);
            AsnIoClose (aip);
        }
    }

    NamedListFree(nlp);
 
    if ((atp = NetEntReadAsn()) == NULL)
        return;
    if (AsnReadVal(asnin, atp, NULL) < 0)   /* read the NULL */
        return;

    LOG_STAT(SUBSYS_CLI_ENTREZ, "Named terms created", 1);
    LOG_STAT(SUBSYS_CLI_ENTREZ, "Named terms created (UID count)", num);
}

NLM_EXTERN void CDECL NetEntrezCreateNamedUidListX (CharPtr term, DocType type, DocField field, ByteStorePtr bsp)
{}

NLM_EXTERN void CDECL NetEntrezCreateNamedUidList (CharPtr term, DocType type, DocField field, Int4 num, DocUidPtr uids)
{
    int i;
    short erract;
    ErrDesc err;

    for (i = 0; i < ENT_SERV_RETRIES; i++)
    {
        if (i > 0)
        {
            if (! ReestablishNetEntrez())
                break;
        }

        ErrGetOpts(&erract, NULL);
        ErrSetOpts(ERR_CONTINUE, 0);
        ErrFetch(&err);
        s_NetEntrezCreateNamedUidList (term, type, field, num, uids);
        ErrSetOpts(erract, 0);
        if (! ErrFetch(&err))
            return; /* success */
    }

    ErrPost(CTX_UNKNOWN, 1, "NetEntrezCreateNamedUidList failure");
    return;
}


static AsnTypePtr NetEntReadAsn(void)
{
    AsnTypePtr atp;
    DataVal av;
    AsnIoPtr asnin = Entrez_asnin;
    AsnModulePtr amp;

    NetEntAsnLoad();
    amp = AsnAllModPtr();

    atp = ENTREZ_BACK;
    if ((atp = AsnReadId(asnin, amp, atp)) != ENTREZ_BACK)
    {
        ErrPost(CTX_UNKNOWN, 1, "EOF on response from Entrez server");
        return NULL;
    }
    AsnReadVal(asnin, atp, &av);
    atp = AsnReadId(asnin, amp, atp);   /* read the CHOICE */
    if (atp == ENTREZ_BACK_error)
    {
        AsnReadVal(asnin, atp, &av);
        return NULL;
    }
    else
        return atp;
}

static Boolean ReestablishNetEntrez(void)
{
    Monitor *mon;
    Boolean retval;
    NamedItemPtr nip;
    ValNodePtr node;
    CharPtr uApplId = NULL;

    if (userApplId != NULL)
    { /* make a copy because this can get wiped out by NetEntrezInit() */
        uApplId = StringSave(userApplId); 
    }

    mon = MonitorStrNew("Re-establishing Entrez Service", 40);
    MonitorStrValue(mon, "Requesting Entrez service");
    NetFini();
    retval = TRUE;

    /* clear state information for named UID lists */
    for (node = namedTerms; node != NULL; node = node->next)
    {
        nip = (NamedItemPtr) node->data.ptrvalue;

        if (nip != NULL) {
            nip->knownToServer = FALSE;
        }
    }

    LOG_STAT(SUBSYS_CLI_ENTREZ, "Service Re-establishments", 1);

    if (! NetEntrezInit(uApplId, userWarnings))
    {
        MonitorStrValue(mon, "Entrez get failed; re-contacting dispatcher");
        retval = FALSE;
        if (ForceNetInit())
        { /* successfully established contact w/dispatcher */
            MonitorStrValue(mon, "Entrez get failed; re-requesting Entrez service");
            retval = NetEntrezInit(uApplId, userWarnings);
        }
        else {
            ErrPost(CTX_UNKNOWN, 1, "Unable to re-contact dispatcher");
            ErrShow();
            LOG_STAT(SUBSYS_CLI_ENTREZ, "Service Re-establishments \"retries\"", 1);
        }
    }

    MonitorFree(mon);

    if (Entrez_asnin == NULL || Entrez_asnout == NULL)
    {
        retval = FALSE;
    }

    if (! retval )
    {
        ErrPost(CTX_UNKNOWN, 1, "Unable to re-establish Entrez service");
        ErrShow();
    } else {
        ConfigFini(); /* to balance the extra ConfigInit() */
    }

    MemFree (uApplId);
    return retval;
}


static Boolean SwapInEntrez (VoidPtr med)
{
    MediaPtr media = (MediaPtr) med;
    NetMediaInfoPtr nmi;

    if (media == NULL || (nmi = (NetMediaInfoPtr) media->media_info) == NULL)
        return FALSE;

    if (nmi->sessionHandle == NULL)
        return FALSE;

    Entrez_ni = nmi->sessionHandle;

    Entrez_asnin = Entrez_ni->raip;
    Entrez_asnout = Entrez_ni->waip;

    return TRUE;
}

static void countChars(CharPtr str)
{
    charCount += StringLen(str) + 5;
}

/*****************************************************************************
*
*   NetEntMedlineEntryListGet (result, numuid, uids, mark_missing)
*       returns a count of entries read
*       if (mark_missing) ids which could not be located are made negative
*
*****************************************************************************/
static
Int2 NEAR s_NetEntMedlineEntryListGet (MedlineEntryPtr PNTR result, Int2 numuid,
                                    DocUidPtr uids, Boolean mark_missing)
{
    AsnTypePtr atp;
    EntrezDocGetPtr edgp;
    MedlineEntryListPtr mllp;
    int count;

    edgp = EntrezDocGetNew();

    edgp->cls = TYP_ML; /* unused */
    edgp->ids = EntrezIdsNew();
    edgp->ids->numid = numuid;
    edgp->ids->ids = uids;
    edgp->mark_missing = mark_missing;

    AsnWrite(Entrez_asnout, ENTREZ_REQUEST, NULL);
    EntrezDocGetAsnWrite(edgp, Entrez_asnout, ENTREZ_REQUEST_getmle);
    AsnIoReset(Entrez_asnout);
    edgp->ids->ids = NULL; /* for clean memory free */
    EntrezDocGetFree(edgp);

    if ((atp = NetEntReadAsn()) == NULL)
        return 0;

    if ((mllp = MedlineEntryListAsnRead(Entrez_asnin, atp)) == NULL)
        return 0;

    MemCopy (result, mllp->data, (size_t) mllp->num * sizeof(MedlineEntryPtr));

    /* note that the structures underlying mllp->data are not freed; they */
    /* are used by the caller                                             */
    MemFree(mllp->data);
    mllp->data = NULL; /* for clean free */
    if (mark_missing && mllp->marked_missing)
    {
        MemCopy (uids, mllp->marked_missing->ids,
             (size_t) mllp->marked_missing->numid * sizeof(DocUid));
    }
    count = mllp->num;
    MedlineEntryListFree (mllp);
    LOG_STAT(SUBSYS_CLI_ENTREZ, "Medline entries retrieved", count);
    LOG_STAT(SUBSYS_CLI_ENTREZ, "Medline entries requested", numuid);
    
    return count;
}

NLM_EXTERN Int2 NetEntMedlineEntryListGet (MedlineEntryPtr PNTR result, Int2 numuid,
                                    DocUidPtr uids, Boolean mark_missing)
{
    int i;
    int retval;
    short erract;
    ErrDesc err;
    DocUidPtr local_uids;

    /* make a local copy, to handle modifications */
    local_uids = MemNew(sizeof(DocUid) * numuid);

    for (i = 0; i < ENT_SERV_RETRIES; i++)
    {
        MemCopy(local_uids, uids, sizeof(DocUid) * numuid);
        if (i > 0)
        {
            if (! ReestablishNetEntrez())
                break;
        }

        ErrGetOpts(&erract, NULL);
        ErrSetOpts(ERR_CONTINUE, 0);
        ErrFetch(&err);
        retval = s_NetEntMedlineEntryListGet(result, numuid, local_uids,
                                          mark_missing);
        ErrSetOpts(erract, 0);
        if (! ErrFetch(&err))
        {
            MemCopy(uids, local_uids, sizeof(DocUid) * numuid);
            MemFree(local_uids);
            return retval; /* success */
        }
    }

    MemFree(local_uids);
    ErrPost(CTX_UNKNOWN, 1, "NetMedlineEntryListGet failure");
    return -1;
}

/*****************************************************************************
*
*   NetEntSeqEntryListGet (result, numuid, uids, mark_missing)
*       returns a count of entries read
*       if (mark_missing) ids which could not be located are made negative
*
*****************************************************************************/
static
Int2 NEAR s_NetEntSeqEntryListGet (SeqEntryPtr PNTR result, Int2 numuid, DocUidPtr uids, Int2 retcode, Boolean mark_missing)
{
    AsnTypePtr atp;
    EntrezIdsPtr glp;
    SeqEntryListPtr selp;
    EntrezSeqGetPtr sgsp;
    int count;

    sgsp = EntrezSeqGetNew();
    glp = EntrezIdsNew();
    sgsp->ids = glp;

    glp->numid = numuid;
    glp->ids = uids;
    sgsp->mark_missing = mark_missing;
    sgsp->retype = retcode;

    AsnWrite(Entrez_asnout, ENTREZ_REQUEST, NULL);
    EntrezSeqGetAsnWrite(sgsp, Entrez_asnout, ENTREZ_REQUEST_getseq);
    AsnIoReset(Entrez_asnout);
    glp->ids = NULL; /* for clean memory free */
    EntrezSeqGetFree(sgsp);

    if ((atp = NetEntReadAsn()) == NULL)
        return 0;

    if ((selp = SeqEntryListAsnRead(Entrez_asnin, atp)) == NULL)
        return 0;

    MemCopy (result, selp->data, (size_t) selp->num * sizeof(SeqEntryPtr));
    /* note that the structures underlying selp->data are not freed; they */
    /* are used by the caller                                             */
    MemFree(selp->data);
    selp->data = NULL; /* for clean free */
    if (mark_missing && selp->marked_missing)
    {
        MemCopy (uids, selp->marked_missing->ids,
             (size_t) selp->marked_missing->numid * sizeof(DocUid));
    }
    count = selp->num;
    SeqEntryListFree (selp);
    LOG_STAT(SUBSYS_CLI_ENTREZ, "Sequence entries retrieved", count);
    LOG_STAT(SUBSYS_CLI_ENTREZ, "Sequence entries requested", numuid);
    
    return count;
}

NLM_EXTERN Int2 CDECL NetEntSeqEntryListGet (SeqEntryPtr PNTR result, Int2 numuid,
                               DocUidPtr uids, Int2 retcode, Boolean mark_missing)
{
    int i;
    int retval;
    short erract;
    ErrDesc err;
    DocUidPtr local_uids;

    /* make a local copy, to handle modifications */
    local_uids = MemNew(sizeof(DocUid) * numuid);

    for (i = 0; i < ENT_SERV_RETRIES; i++)
    {
        MemCopy(local_uids, uids, sizeof(DocUid) * numuid);
        if (i > 0)
        {
            if (! ReestablishNetEntrez())
                break;
        }

        ErrGetOpts(&erract, NULL);
        ErrSetOpts(ERR_CONTINUE, 0);
        ErrFetch(&err);
        retval = s_NetEntSeqEntryListGet(result, numuid, local_uids, retcode,
                                      mark_missing);
        ErrSetOpts(erract, 0);
        if (! ErrFetch(&err))
        {
            MemCopy(uids, local_uids, sizeof(DocUid) * numuid);
            MemFree(local_uids);
            return retval; /* success */
        }
    }

    MemFree(local_uids);
    ErrPost(CTX_UNKNOWN, 1, "NetSeqEntryListGet failure");
    return -1;
}


#ifdef Biostruc_supported
/*****************************************************************************
*
*   NetEntrezBiostrucListGet (result, numuid, uids, mark_missing)
*       returns a count of entries read
*       if (mark_missing) ids which could not be located are made negative
*
*****************************************************************************/
static
Int2 NEAR s_NetEntrezBiostrucListGet (BiostrucPtr PNTR result, Int4 mdlLvl,
                                      Int4 maxModels, Int2 numuid,
                                      DocUidPtr uids, Boolean mark_missing)
{
    AsnTypePtr atp;
    EntrezDocGetPtr edgp;
    BiostrucListPtr bslp;
    int count;
    DataVal av;

    edgp = EntrezDocGetNew();

    edgp->cls = TYP_ST; /* unused */
    edgp->ids = EntrezIdsNew();
    edgp->ids->numid = numuid;
    edgp->ids->ids = uids;
    edgp->mark_missing = mark_missing;

    av.intvalue = mdlLvl;
    AsnWrite(Entrez_asnout, ENTREZ_REQUEST, NULL);
    AsnStartStruct(Entrez_asnout, ENTREZ_REQUEST_getbiostrX);
    AsnWrite(Entrez_asnout, REQUEST_getbiostrX_complexity, &av);
    EntrezDocGetAsnWrite(edgp, Entrez_asnout, ENTREZ_REQUEST_getbiostrX_get);
    av.intvalue = maxModels;
    AsnWrite(Entrez_asnout, REQUEST_getbiostrX_max_models, &av);
    AsnEndStruct(Entrez_asnout, ENTREZ_REQUEST_getbiostrX);
    AsnIoReset(Entrez_asnout);
    edgp->ids->ids = NULL; /* for clean memory free */
    EntrezDocGetFree(edgp);

    if ((atp = NetEntReadAsn()) == NULL)
        return 0;

    if ((bslp = BiostrucListAsnRead(Entrez_asnin, atp)) == NULL)
        return 0;

    MemCopy (result, bslp->data, (size_t) bslp->num * sizeof(BiostrucPtr));

    /* note that the structures underlying bslp->data are not freed; they */
    /* are used by the caller                                             */
    MemFree(bslp->data);
    bslp->data = NULL; /* for clean free */
    if (mark_missing && bslp->marked_missing)
    {
        MemCopy (uids, bslp->marked_missing->ids,
             (size_t) bslp->marked_missing->numid * sizeof(DocUid));
    }
    count = bslp->num;
    BiostrucListFree (bslp);
    LOG_STAT(SUBSYS_CLI_ENTREZ, "Biostruc entries retrieved", count);
    LOG_STAT(SUBSYS_CLI_ENTREZ, "Biostruc entries requested", numuid);
    
    return count;
}

NLM_EXTERN Int2 NetEntrezBiostrucListGet (BiostrucPtr PNTR result, Int4 mdlLvl, Int4 maxModels, Int2 numuid,
                                    DocUidPtr uids, Boolean mark_missing)
{
    int i;
    int retval;
    short erract;
    ErrDesc err;
    DocUidPtr local_uids;

    /* make a local copy, to handle modifications */
    local_uids = MemNew(sizeof(DocUid) * numuid);

    for (i = 0; i < ENT_SERV_RETRIES; i++)
    {
        MemCopy(local_uids, uids, sizeof(DocUid) * numuid);
        if (i > 0)
        {
            if (! ReestablishNetEntrez())
                break;
        }

        ErrGetOpts(&erract, NULL);
        ErrSetOpts(ERR_CONTINUE, 0);
        ErrFetch(&err);
        retval = s_NetEntrezBiostrucListGet(result, mdlLvl, maxModels, numuid, local_uids,
                                          mark_missing);
        ErrSetOpts(erract, 0);
        if (! ErrFetch(&err))
        {
            MemCopy(uids, local_uids, sizeof(DocUid) * numuid);
            MemFree(local_uids);
            return retval; /* success */
        }
    }

    MemFree(local_uids);
    ErrPost(CTX_UNKNOWN, 1, "NetBiostrucListGet failure");
    return -1;
}

static
BiostrucAnnotSetPtr NEAR s_NetEntrezBiostrucAnnotSetGet (DocUid gi)
{
    DataVal av;
    BiostrucAnnotSetPtr retval;
    AsnTypePtr atp;

    AsnWrite(Entrez_asnout, ENTREZ_REQUEST, NULL);
    av.intvalue = gi;
    AsnWrite(Entrez_asnout, ENTREZ_REQUEST_getbiostrannot, &av);
    AsnIoReset(Entrez_asnout);

    if ((atp = NetEntReadAsn()) == NULL)
        return NULL;

    retval = BiostrucAnnotSetAsnRead(Entrez_asnin, atp);

    LOG_STAT(SUBSYS_CLI_ENTREZ, "BiostrucAnnotSets retrieved", 1);

    return retval;
}

NLM_EXTERN BiostrucAnnotSetPtr CDECL NetEntrezBiostrucAnnotSetGet (DocUid gi)
{
    int i;
    BiostrucAnnotSetPtr retval;
    short erract;
    ErrDesc err;

    for (i = 0; i < ENT_SERV_RETRIES; i++)
    {
        if (i > 0)
        {
            if (! ReestablishNetEntrez())
                break;
        }

        ErrGetOpts(&erract, NULL);
        ErrSetOpts(ERR_CONTINUE, 0);
        ErrFetch(&err);
        retval = s_NetEntrezBiostrucAnnotSetGet(gi);
        ErrSetOpts(erract, 0);
        if (! ErrFetch(&err))
        {
            return retval; /* success */
        }
    }

    ErrPost(CTX_UNKNOWN, 1, "NetBiostrucAnnotSet failure");
    return NULL;
}

static
LinkSetPtr NEAR s_NetEntrezBiostrucFeatIds(DocUid mmdbid, Int2 feature_type, Int4 feature_set_id)
{
    LinkSetPtr retval;
    GetFeatIdsPtr gfi;
    AsnTypePtr atp;

    AsnWrite(Entrez_asnout, ENTREZ_REQUEST, NULL);
    gfi = GetFeatIdsNew();
    gfi->mmdbid = mmdbid;
    gfi->feature_type = feature_type;
    gfi->feature_set_id = feature_set_id;
    GetFeatIdsAsnWrite(gfi, Entrez_asnout, REQUEST_getbiostr_feat_ids);
    AsnIoReset(Entrez_asnout);
    GetFeatIdsFree(gfi);

    if ((atp = NetEntReadAsn()) == NULL)
        return NULL;

    retval = LinkSetAsnRead(Entrez_asnin, atp);

    LOG_STAT(SUBSYS_CLI_ENTREZ, "BiostrucAnnotFeatids retrieved", 1);

    return retval;
}

NLM_EXTERN LinkSetPtr CDECL NetEntrezBiostrucFeatIds(DocUid mmdbid, Int2 feature_type, Int4 feature_set_id)
{
    int i;
    LinkSetPtr retval;
    short erract;
    ErrDesc err;

    for (i = 0; i < ENT_SERV_RETRIES; i++)
    {
        if (i > 0)
        {
            if (! ReestablishNetEntrez())
                break;
        }

        ErrGetOpts(&erract, NULL);
        ErrSetOpts(ERR_CONTINUE, 0);
        ErrFetch(&err);
        retval = s_NetEntrezBiostrucFeatIds(mmdbid, feature_type, feature_set_id);
        ErrSetOpts(erract, 0);
        if (! ErrFetch(&err))
        {
            return retval; /* success */
        }
    }

    ErrPost(CTX_UNKNOWN, 1, "NetBiostrucAnnotFeatIds failure");
    return NULL;
}

static
BiostrucAnnotSetPtr NEAR s_NetEntrezBiostrucAnnotSetGetByFid (DocUid mmdbid, Int4 feature_id, Int4 feature_set_id)
{
    BiostrucAnnotSetPtr retval;
    AsnTypePtr atp;
    GetByFidPtr gbf;

    gbf = GetByFidNew();
    gbf->mmdbid = mmdbid;
    gbf->feature_id = feature_id;
    gbf->feature_set_id = feature_set_id;
    GetByFidAsnWrite(gbf, Entrez_asnout, REQUEST_getbiostr_annot_by_fid);
    AsnIoReset(Entrez_asnout);
    GetByFidFree(gbf);

    if ((atp = NetEntReadAsn()) == NULL)
        return NULL;

    retval = BiostrucAnnotSetAsnRead(Entrez_asnin, atp);

    LOG_STAT(SUBSYS_CLI_ENTREZ, "BiostrucAnnotSetsByFid retrieved", 1);

    return retval;
}

NLM_EXTERN BiostrucAnnotSetPtr CDECL NetEntrezBiostrucAnnotSetGetByFid (DocUid mmdbid, Int4 feature_id, Int4 feature_set_id)
{
    int i;
    BiostrucAnnotSetPtr retval;
    short erract;
    ErrDesc err;

    for (i = 0; i < ENT_SERV_RETRIES; i++)
    {
        if (i > 0)
        {
            if (! ReestablishNetEntrez())
                break;
        }

        ErrGetOpts(&erract, NULL);
        ErrSetOpts(ERR_CONTINUE, 0);
        ErrFetch(&err);
        retval = s_NetEntrezBiostrucAnnotSetGetByFid (mmdbid, feature_id, feature_set_id);
        ErrSetOpts(erract, 0);
        if (! ErrFetch(&err))
        {
            return retval; /* success */
        }
    }

    ErrPost(CTX_UNKNOWN, 1, "NetBiostrucAnnotByFid failure");
    return NULL;
}
#endif /* Biostruc_supported */


static
SeqIdPtr NEAR s_NetSeqIdForGI (DocUid gi)
{
    DataVal av;
    SeqIdPtr retval;
    AsnTypePtr atp;

    AsnWrite(Entrez_asnout, ENTREZ_REQUEST, NULL);
    av.intvalue = gi;
    AsnWrite(Entrez_asnout, ENTREZ_REQUEST_seqidforgi, &av);
    AsnIoReset(Entrez_asnout);

    if ((atp = NetEntReadAsn()) == NULL)
        return NULL;

    retval = SeqIdAsnRead(Entrez_asnin, atp);

    LOG_STAT(SUBSYS_CLI_ENTREZ, "Sequence IDs looked up by GI", 1);

    return retval;
}

NLM_EXTERN SeqIdPtr CDECL NetSeqIdForGI (DocUid gi)
{
    int i;
    SeqIdPtr retval;
    short erract;
    ErrDesc err;

    for (i = 0; i < ENT_SERV_RETRIES; i++)
    {
        if (i > 0)
        {
            if (! ReestablishNetEntrez())
                break;
        }

        ErrGetOpts(&erract, NULL);
        ErrSetOpts(ERR_CONTINUE, 0);
        ErrFetch(&err);
        retval = s_NetSeqIdForGI(gi);
        ErrSetOpts(erract, 0);
        if (! ErrFetch(&err))
        {
            return retval; /* success */
        }
    }

    ErrPost(CTX_UNKNOWN, 1, "NetSeqIdForGI failure");
    return NULL;
}

static
Int4 NEAR s_NetFindSeqId (SeqIdPtr sip)
{
    DataVal av;
    AsnTypePtr atp;

    AsnWrite(Entrez_asnout, ENTREZ_REQUEST, NULL);
    SeqIdAsnWrite(sip, Entrez_asnout, ENTREZ_REQUEST_findseqid);
    AsnIoReset(Entrez_asnout);

    if ((atp = NetEntReadAsn()) == NULL)
        return 0;

    AsnReadVal(Entrez_asnin, atp, &av);

    LOG_STAT(SUBSYS_CLI_ENTREZ, "Sequence GIs looked up by SeqID", 1);

    return av.intvalue;
}

NLM_EXTERN Int4 CDECL NetEntrezFindSeqId (SeqIdPtr sip)
{
    int i;
    Int4 retval;
    short erract;
    ErrDesc err;

    for (i = 0; i < ENT_SERV_RETRIES; i++)
    {
        if (i > 0)
        {
            if (! ReestablishNetEntrez())
                break;
        }

        ErrGetOpts(&erract, NULL);
        ErrSetOpts(ERR_CONTINUE, 0);
        ErrFetch(&err);
        retval = s_NetFindSeqId(sip);
        ErrSetOpts(erract, 0);
        if (! ErrFetch(&err))
        {
            return retval; /* success */
        }
    }

    ErrPost(CTX_UNKNOWN, 1, "NetFindSeqId failure");

    return 0;
}

static EntrezHierarchyPtr s_NetEntHierarchyGet(CharPtr term, DocType type, DocField field)
{
  AsnIoPtr asnin = Entrez_asnin;
  AsnIoPtr asnout = Entrez_asnout;
  EntrezHierarchyPtr ehp;
  TermLookupPtr tlp;
  AsnTypePtr atp;
    
  tlp = TermLookupNew();
  tlp->type = type;
  tlp->fld = field;
  tlp->term = StringSave(term);
  AsnWrite(asnout, ENTREZ_REQUEST, NULL);
  TermLookupAsnWrite(tlp, asnout, ENTREZ_REQUEST_get_hierarchy);
  TermLookupFree(tlp);
  AsnIoReset(asnout);

  /* now, read back response */
  if ((atp = NetEntReadAsn()) == NULL)
    return FALSE;

  if ( (ehp = EntrezHierarchyAsnRead(asnin,ENTREZ_BACK_get_hierarchy)) == NULL)
    return  NULL;

  if (ehp->numInLineage == 0 && ehp->numChildren == 0)
  {
      EntrezHierarchyFree(ehp);
      return NULL;
  }

  return (ehp);
}


NLM_EXTERN EntrezHierarchyPtr NetEntHierarchyGet (CharPtr term, DocType type, DocField field)
{
  int i;
  EntrezHierarchyPtr ehp;
  short erract;
  ErrDesc err;
  
  for (i = 0; i < ENT_SERV_RETRIES; i++)
    {
      if (i > 0)
        {
          if (! ReestablishNetEntrez())
            break;
        }
      
      ErrGetOpts(&erract, NULL);
      ErrSetOpts(ERR_CONTINUE, 0);
      ErrFetch(&err);
      ehp = s_NetEntHierarchyGet(term, type, field);
      ErrSetOpts(erract, 0);
      if (! ErrFetch(&err))
        {
          return ehp; /* success */
        }
    }
  
  ErrPost(CTX_UNKNOWN, 1, "NetEntMeshTreeGet failure");
  
  return NULL;
}

static Int2 s_DatabaseFromName (CharPtr name)

{
  Int2           db;
  EntrezInfoPtr  eip;

  if (name == NULL) return 0;
  eip = EntrezGetInfo ();
  if (eip == NULL || eip->type_info == NULL) return 0;
  for (db = 0; db < eip->type_count; db++) {
    if (StringICmp (eip->type_info [db].name, name) == 0) {
      return eip->type_info [db].id;
    }
  }
  return 0;
}

static Int2 s_FieldFromTag (CharPtr tag)

{
  EntrezInfoPtr  eip;
  Int2           fld;

  if (tag == NULL) return 0;
  eip = EntrezGetInfo ();
  if (eip == NULL || eip->field_info == NULL) return 0;
  for (fld = 0; fld < eip->field_count; fld++) {
    if (StringICmp (eip->field_info [fld].tag, tag) == 0) {
      return eip->field_info [fld].id;
    }
  }
  return 0;
}

NLM_EXTERN EntrezHierarchyPtr NetEntMeshHierarchyGet(CharPtr term)
{
  Int2  fld_mesh_hier;
  Int2  typ_ml;

  typ_ml = s_DatabaseFromName ("MEDLINE");
  fld_mesh_hier = s_FieldFromTag ("MESH");
  return NetEntHierarchyGet (term, typ_ml, fld_mesh_hier);
}


static LinkSetPtr s_NetEntDoNeighborText(EntrezNeighborTextPtr entp)
{
  LinkSetPtr lsp = NULL;
  AsnTypePtr atp;
  AsnIoPtr asnin = Entrez_asnin;
  AsnIoPtr asnout = Entrez_asnout;

  RemoveNonPrintingCharacters(entp->normalText);
  RemoveNonPrintingCharacters(entp->specialText);

  if  ( (*SkipSpaces(entp->normalText) == '\0')
       && (*SkipSpaces(entp->specialText) == '\0') )
    return (NULL);

  AsnWrite(asnout, ENTREZ_REQUEST, NULL);
  EntrezNeighborTextAsnWrite(entp,asnout,ENTREZ_REQUEST_neighbortext);
  AsnIoReset(asnout);

  if ( (atp = NetEntReadAsn()) == NULL)
    return NULL;

  if ( (lsp = LinkSetAsnRead(asnin, ENTREZ_BACK_neighbortext)) == NULL)
    return(NULL);

  return (lsp);
}



NLM_EXTERN LinkSetPtr CDECL NetEntDoNeighborText (EntrezNeighborTextPtr entp)
{
  int i;
  LinkSetPtr lsp;
  short erract;
  ErrDesc err;
  
  for (i = 0; i < ENT_SERV_RETRIES; i++)
    {
      if (i > 0)
        {
          if (! ReestablishNetEntrez())
            break;
        }
      
      ErrGetOpts(&erract, NULL);
      ErrSetOpts(ERR_CONTINUE, 0);
      ErrFetch(&err);
      lsp = s_NetEntDoNeighborText(entp);
/* debug       if (lsp == NULL)
        fprintf(stderr,"failed to get linkset pointer. \n"); */
      ErrSetOpts(erract, 0);
      if (! ErrFetch(&err))
        return lsp; /* success */
    }
  
  ErrPost(CTX_UNKNOWN, 1, "NetEntDoNeighborText failure");
  return NULL;
}


NLM_EXTERN Boolean CDECL NetEntCanNeighborText(void)
{
    EntrezExtraInfoPtr myeeip;

    if ((myeeip = GetEntrezExtraInfo()) == NULL)
    {
        return FALSE;
    }

    return myeeip->canneighbortext;
}


NLM_EXTERN Boolean CDECL NetEntExpandedMedlineFeatures(void)
{
    EntrezExtraInfoPtr myeeip;

    if ((myeeip = GetEntrezExtraInfo()) == NULL)
    {
        return FALSE;
    }

    return myeeip->expanded_medline;
}


NLM_EXTERN Boolean CDECL NetEntCanBlast(void)
{
    EntrezExtraInfoPtr myeeip;

    if ((myeeip = GetEntrezExtraInfo()) == NULL)
    {
        return FALSE;
    }

    return myeeip->canblast;
}


static void RemoveNonPrintingCharacters(CharPtr str)
{
  while (*str)
    {
      if ( (*str < ' ') || (*str > 0x7E) )
        *str = ' ';
      str++;
    }
}

static Boolean
blastProgressMonitor (Int4 value, Boolean isStart, Boolean destroyIt)
{
    static MonitorPtr mon = NULL;
    Boolean retval = TRUE;

    if (destroyIt)
    {
        if (mon != NULL)
        {
            MonitorFree(mon);
            mon = NULL;
        }
        return TRUE;
    }

    if (isStart)
    {
        if (mon != NULL)
        {
            MonitorFree(mon);
            mon = NULL;
        }
        mon = MonitorIntNew("BLAST progress", 0, value);
    } else {
        if (mon != NULL)
        {
            retval = MonitorIntValue(mon, value);
        }
    }

    return retval;
}

static LinkSetPtr
s_NetEntBlastBioseq(BioseqPtr bsp, DocType db, CharPtr program, CharPtr database, CharPtr options, Boolean usemonitor, BoolPtr noMoreTriesPtr)
{
     EntrezBlastreqPtr ebrp;
     LinkSetPtr lsp;
     AsnTypePtr atp;
     DataVal av;
     AsnModulePtr amp;

     NetEntAsnLoad();
     amp = AsnAllModPtr();

     ebrp = EntrezBlastreqNew();
     ebrp->bsp = bsp;
     ebrp->bsp_database = db;
     ebrp->program = StringSave(program);
     ebrp->database = StringSave(database);
     ebrp->options = StringSave(options);
     ebrp->showprogress = usemonitor;
     EntrezBlastreqAsnWrite(ebrp, Entrez_asnout, ENTREZ_REQUEST_blast);
     ebrp->bsp = NULL; /* we don't "own" this so have no business freeing it */
     EntrezBlastreqFree(ebrp);
     AsnIoReset(Entrez_asnout);

     while (TRUE)
     {
         if ( (atp = NetEntReadAsn()) == NULL)
           return NULL;
         AsnReadVal(Entrez_asnin, atp, NULL);
         if ( (atp = AsnReadId(Entrez_asnin, amp, atp)) == NULL)
           return NULL;
         if (atp == ENTREZ_BACK_blast_bad_count)
         {
             AsnReadVal(Entrez_asnin, atp, NULL);
             blastProgressMonitor(0, FALSE, TRUE);
             return NULL;
         } else
         if (atp == ENTREZ_BACK_blast_link_set)
         {
             blastProgressMonitor(0, FALSE, TRUE);
             lsp = LinkSetAsnRead(Entrez_asnin, atp);
             return (lsp);
         } else
         if (atp == ENTREZ_BACK_blast_job_start)
         {
            AsnReadVal(Entrez_asnin, atp, &av);
            if (usemonitor)
                blastProgressMonitor(av.intvalue, TRUE, FALSE);
         } else
         if (atp == ENTREZ_BACK_blast_job_progress)
         {
            AsnReadVal(Entrez_asnin, atp, &av);
            if (usemonitor)
                if (! blastProgressMonitor(av.intvalue, FALSE, FALSE))
                { /* user cancelled BLAST search */
                    blastProgressMonitor(0, FALSE, TRUE);
                    /* drop our connection since we don't know what state we're in */
                    ReestablishNetEntrez();
                    if (noMoreTriesPtr != NULL)
                        *noMoreTriesPtr = TRUE;
                    return NULL;
                }
         } else {
             return NULL;
         }
     }
}

NLM_EXTERN LinkSetPtr LIBCALL
NetEntBlastBioseq(BioseqPtr bsp, DocType db, CharPtr program, CharPtr database, CharPtr options, Boolean usemonitor)
{
  int i;
  LinkSetPtr lsp;
  short erract;
  ErrDesc err;
  Boolean noMoreTries;
  
  for (i = 0; i < ENT_SERV_RETRIES; i++)
    {
      if (i > 0)
        {
          if (! ReestablishNetEntrez())
            break;
        }
      
      ErrGetOpts(&erract, NULL);
      ErrSetOpts(ERR_CONTINUE, 0);
      ErrFetch(&err);
      MsgSetReadTimeout(Entrez_ni, 1200);
      noMoreTries = FALSE;
      lsp = s_NetEntBlastBioseq(bsp, db, program, database, options, usemonitor, &noMoreTries);
      MsgSetReadTimeout(Entrez_ni, 60);
      ErrSetOpts(erract, 0);
      if (noMoreTries || ! ErrFetch(&err))
        return lsp;
    }

  ErrPost(CTX_UNKNOWN, 1, "NetEntBlastBioseq failure");
  return NULL;
}
  
static Int4 s_NetEntClusterAnalysis(DocUidPtr uids, Int4 numuids, DocField fld, Int4 minCluster, Int4 maxCluster, Int4 maxTerms, CharPtr *terms, Int4Ptr termTotals)
{
    ClusterArticlesPtr clap;
    ClusterRespPtr clrp;
    EntrezIdsPtr eip;
    Int4 numTerms;
    Int4 i;
    ValNodePtr vnpTerms;
    ValNodePtr vnpCounts;
    AsnTypePtr atp;

    clap = ClusterArticlesNew();
    eip = EntrezIdsNew();
    eip->ids = uids;
    eip->numid = numuids;
    clap->ids = eip;
    clap->fld = fld;
    clap->min_cluster = minCluster;
    clap->max_cluster = maxCluster;
    clap->max_terms = maxTerms;
    ClusterArticlesAsnWrite(clap, Entrez_asnout, ENTREZ_REQUEST_cluster_arts);
    AsnIoReset(Entrez_asnout);
    eip->ids = NULL; /* for clean free */
    ClusterArticlesFree(clap);

    if ( (atp = NetEntReadAsn()) == NULL)
      return -1;
    if ( (clrp = ClusterRespAsnRead(Entrez_asnin, ENTREZ_BACK_cluster_arts)) == NULL)
      return -1;
    numTerms = clrp->count;

    for (vnpTerms = clrp->terms, vnpCounts = clrp->term_counts, i = 0;
         vnpTerms != NULL && vnpCounts != NULL; i++, vnpTerms = vnpTerms->next,
         vnpCounts = vnpCounts->next)
    {
        terms[i] = (CharPtr) vnpTerms->data.ptrvalue;
        vnpTerms->data.ptrvalue = NULL; /* for clean free */
        termTotals[i] = (Int4) vnpCounts->data.intvalue;
    }

    ClusterRespFree(clrp);
    return numTerms;
}

NLM_EXTERN Int4 LIBCALL NetEntClusterAnalysis(DocUidPtr uids, Int4 numuids, DocField fld, Int4 minCluster, Int4 maxCluster, Int4 maxTerms, CharPtr *terms, Int4Ptr termTotals)
{
    int i;
    Int4 retval;
    short erract;
    ErrDesc err;
    extern void MsgSetReadTimeout PROTO((MHandPtr mh, int t));

    for (i = 0; i < ENT_SERV_RETRIES; i++)
    {
        if (i > 0)
        {
            if (! ReestablishNetEntrez())
                break;
        }

        ErrGetOpts(&erract, NULL);
        ErrSetOpts(ERR_CONTINUE, 0);
        ErrFetch(&err);
        MsgSetReadTimeout(Entrez_ni, 1200);
        retval = s_NetEntClusterAnalysis(uids, numuids, fld, minCluster, maxCluster, maxTerms, terms, termTotals);
        MsgSetReadTimeout(Entrez_ni, 60);
        ErrSetOpts(erract, 0);
        if (! ErrFetch(&err))
        {
            return retval; /* success */
        }
    }

    ErrPost(CTX_UNKNOWN, 1, "NetEntClusterAnalysis failure");

    return -1;
}
