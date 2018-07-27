/*   accentr.c
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
 * RCS $Id: accentr.c,v 6.9 2001/09/04 23:28:55 juran Exp $
 *
 * Author:  Ostell
 *
 * Version Creation Date:   4/23/92
 *
 * File Description: 
 *       entrez index access library for Entrez
 *
 * Modifications:  
 * --------------------------------------------------------------------------
 * Date     Name        Description of modification
 * -------  ----------  -----------------------------------------------------
 * 08-16-94 Brylawski   Added access to mesh term explosion, mesh tree browsing,
 *                      and on-the-fly neighboring.
 *
 * 10-06-94 Schuler     Added EntrezBiostrucGet() function
 *
 * 11-20-94 Brylawski   Permitted client to modify on-the-fly neighboring
 *                      via .entrezrc parameters
 *
 * 05-19-95 Schuler     Added rcs Log directive for automatic insertion of
 *                      modification comments.
 *
 * $Log: accentr.c,v $
 * Revision 6.9  2001/09/04 23:28:55  juran
 * Always assume network access in Mac OS.
 *
 * Revision 6.8  2001/08/16 19:40:33  kans
 * reverted to not assume network - UNIX still does cdrom access compile - and cleaned up junk in log instructions
 *
 * Revision 6.7  2001/08/15 20:57:07  juran
 * Always assume network access.
 *
 * Revision 6.6  2000/01/12 20:17:12  vakatov
 * Get rid of the LIBCALL specifier at EntrezSeqEntryGet()
 *
 * Revision 6.5  1999/01/06 14:18:35  grisha
 * add defines to switch ID0/ID1 usage
 *
 * Revision 6.4  1998/12/08 20:38:47  kans
 * EntrezGIForSeqIdFunc aborts on local, other, general, gi, notset before connecting to Entrez network server
 *
 * Revision 6.3  1998/06/12 19:19:06  kans
 * fixed unix compiler warnings
 *
 * Revision 6.2  1998/02/23 17:59:38  kans
 * mesh and taxonomy hierarchy modes both call NetEntHierarchyGet(term,db,fld)
 *
 * Revision 6.1  1997/09/19 13:16:19  kans
 * removed or ifdef unused variables
 *
 * Revision 6.0  1997/08/25 18:12:26  madden
 * changed to 6.0
 *
 * Revision 5.38  1997/07/28 13:30:35  ostell
 * Moved GetUniGeneIDForSeqId() to seqmgr.c
 *
 * Revision 5.37  1997/07/09 19:25:57  kuzio
 * kludge for corn; and tidied NCBICG
 *
 * Revision 5.36  1997/06/26 21:55:10  vakatov
 * [PC] DLL'd "ncbicdr.lib", "ncbiacc.lib", "ncbinacc.lib" and "ncbicacc.lib"
 *
 * Revision 5.35  1997/05/22 20:35:44  epstein
 * add error strings when fetching Medline and Sequence entries, to identify which UID encountered the problem
 *
 * Revision 5.34  1997/04/18 16:01:08  shavirin
 * Removed annoying compiler messages "Statement not reached"
 *
 * Revision 5.33  1997/03/17  16:26:23  brandon
 * added PMEntrezDetailedInfo
 *
 * Revision 5.32  1997/03/13  15:37:14  brandon
 * added support for retcodes in PmEntrezsequencegets
 *
 * Revision 5.31  1997/02/24  17:27:14  brandon
 * added support for PubMed SeqId functions.
 *
 * Revision 5.30  1997/02/24  16:47:51  brandon
 * added support for named uid lists.
 *
 * Revision 5.29  1997/02/19  16:28:29  brandon
 * fixed EntrezSeqEntryListGet for PM
 *
 * Revision 5.28  1997/02/10  20:35:35  brandon
 * fixed EntrezSeqEntryGet for PMEntrez
 *
 * Revision 5.27  1997/02/05  15:53:14  brandon
 * added PMEntSeqEntryListGet
 *
 * Revision 5.26  1997/02/04  15:29:27  brandon
 * fixed typo
 *
 * Revision 5.25  1997/02/03  19:36:48  brandon
 * added line for PMEntrezHierarchyGet
 *
 * Revision 5.24  1997/01/24  22:26:21  kuzio
 * NCBICG 9000000 kludge
 *
 * Revision 5.23  1997/01/23  17:07:19  brandon
 * modified EntrezTLFree
 *
 * Revision 5.22  1997/01/19  15:39:45  brandon
 * *** empty log message ***
 *
 * Revision 5.21  1997/01/16  18:17:43  brandon
 * added SelectDataSource wrapper
 *
 * Revision 5.20  1997/01/14  18:39:16  brandon
 * added wrapper function GetCurMediaType()
 *
 * Revision 5.19  1997/01/13  21:34:02  brandon
 * *** empty log message ***
 *
 * Revision 5.18  1996/12/23  17:50:38  brandon
 * modified EntrezInit
 *
 * Revision 5.17  1996/12/23  04:25:48  brandon
 * *** empty log message ***
 *
 * Revision 5.16  1996/12/23  02:02:29  brandon
 * *** empty log message ***
 *
 * Revision 5.15  1996/12/23  02:00:35  brandon
 * modified PMAdjustEntrezInfo
 *
 * Revision 5.14  1996/12/04  15:06:32  epstein
 * restore network fetching of MEDLINE entries (accidentally deleted in version 5.12
 *
 * Revision 5.13  1996/12/03  19:52:11  brandon
 * added ifdefs for pubmed
 *
 * Revision 5.12  1996/12/02  22:24:02  brandon
 * *** empty log message ***
 *
 * Revision 5.11  1996/12/02  22:10:08  brandon
 * *** empty log message ***
 *
 * Revision 5.10  1996/09/13  14:43:17  brandon
 * modifed pubmed routines
 *
 * Revision 5.9  1996/08/30  16:32:58  brandon
 * fixed PM links
 *
 * Revision 5.8  1996/08/14  19:55:53  epstein
 * add APIs for fetching pieces of biostruc annots
 *
 * Revision 5.7  1996/08/14  17:08:29  brandon
 * *** empty log message ***
 *
 * Revision 5.5  1996/08/14  15:15:05  brandon
 * added date parameter to tleval functions
 *
 * Revision 5.4  1996/07/30  15:28:28  epstein
 * correct logic when retrieving docsums
 *
 * Revision 5.3  1996/07/30  15:27:14  epstein
 * add kludge for retrieving docsums from ID
 *
 * Revision 5.2  1996/07/26  20:28:03  epstein
 * remove special casing of FLD_PROT for PubMed
 *
 * Revision 5.1  1996/07/26  20:19:34  zjing
 * fix in EntrezBioseqFetchFunc for gis of segmented sequence
 *
 * Revision 5.0  1996/05/28  13:55:34  ostell
 * Set to revision 5.0
 *
 * Revision 4.26  1996/05/15  15:03:09  kans
 * added a conditional to suppress an unused variable warning
 *
 * Revision 4.25  1996/05/10  19:35:00  epstein
 * add Pubmed support
 *
 *
 * Revision 4.24  1996/05/02  20:56:08  epstein
 * fix logic of obtaining entries from ID server
 *
 * Revision 4.23  1996/04/18  16:51:38  kuzio
 * add KLUDGE_YGG
 *
 * Revision 4.22  1996/04/10  14:17:55  ostell
 * reverted Kludge CEGC to original CESC
 *
 * Revision 4.19  1996/03/29  18:52:44  epstein
 * add support for structure alignments
 *
 * Revision 4.18  1996/03/28  21:23:42  epstein
 * move ID-backending from srventr into accentr
 *
 * Revision 4.17  1996/03/26  16:29:40  epstein
 * migrate byte-swapping functions to ncbimisc.[ch]
 *
 * Revision 4.16  1996/03/15  02:46:44  ostell
 * added Check to EntrezGIForSeqID() to remove calls for SeqId types that
 * will not be satisfied.
 *
 * Revision 4.15  1996/03/11  21:51:03  ostell
 * made GetUniGeneIDForSeqId() externally visible
 *
 * Revision 4.14  1996/02/21  20:29:43  ostell
 * added KLUDGE_HUMGEN
 *
 * Revision 4.13  1996/02/16  16:32:40  kans
 * CESC and BSNR kludges, allow UniGene as well as UNIGENE
 *
 * Revision 4.12  1995/12/05  00:56:43  ostell
 * fixed UniGene fetch problem in Fetch
 *
 * Revision 4.10  1995/10/19  20:27:49  epstein
 * correct EntrezInit/EntrezFini logic (again)
 *
 * Revision 4.9  1995/10/11  13:39:20  epstein
 * add EntrezIsInited() function
 *
 * Revision 4.8  1995/10/02  15:27:11  epstein
 * add range-checking for Net and MB
 *
 * Revision 4.6  1995/09/26  21:41:45  ostell
 * set hierHead to NULL after freeing data in FreeHierarchyList()
 *
 * Revision 4.5  1995/08/28  18:50:12  epstein
 * more protection against multiple EntrezFini calls
 *
 * Revision 4.4  1995/08/25  20:22:00  epstein
 * add counter to avoid unnecessary EntrezInits
 *
 * Revision 4.3  1995/08/22  19:37:02  epstein
 * add network-based clustering support
 *
 * Revision 4.1  1995/08/11  20:25:38  epstein
 * add max-models support for biostrucs
 *
 * Revision 4.0  1995/07/26  13:50:32  ostell
 * force revision to 4.0
 *
 * Revision 2.71  1995/07/25  18:47:57  kans
 * revert to no Biostruc_supported
 *
 * Revision 2.70  1995/07/11  14:53:39  epstein
 *  move DocSumFree to objentr.c
 *
 * Revision 2.69  1995/06/29  15:57:51  epstein
 * added Complexity argument when fetching structures
 *
 * Revision 2.68  95/06/20  15:42:45  epstein
 * add support for BigMed-based Entrez API
 * 
 * Revision 2.67  95/06/19  16:42:25  ostell
 * added fake_seqid in EntrezBioseqFetch. When "sip" was used in
 * BioseqFindInSeqEntry() call, it was not initialized in the case of a
 * previously cached SeqEntry (only gi set).
 * 
 * Revision 2.66  1995/05/24  17:14:41  ostell
 * added code to maintain connection through multiple nested calles to
 * EntrezBioseqFetchEnable and Disable
 *
 * Revision 2.65  1995/05/16  14:36:20  schuler
 * Automatic comment insertion enabled
 *
 *
 *
 * ==========================================================================
 */

#define REVISION_STR "$Revision: 6.9 $"

#include <accentr.h>
#include <seqmgr.h>
#include <sequtil.h>

#include <cdconfig.h>
/* Must not assume network - unused cdromlib stuff is still tangled in */
/* Okay to assume network in Mac OS. */
#ifdef __MACOS__
#define _NETENT_
#endif
#ifdef _NETENT_
#include <netentr.h>      /* support network access */
#else
#ifdef _MBENTREZ_
#include <mbentrez.h>
#define USE_ID0 1
#define USE_IDxARCH 1
#endif
#ifdef _PMENTREZ_
#include <pmentrez.h>
#define USE_ID1 1
#define USE_IDxARCH 1
#endif
#ifdef USE_ID0
#include <id0arch.h>
#endif
#ifdef USE_ID1
#include <id1arch.h>
#endif
#ifndef _CDENTREZ_
#include <cdentrez.h>      /* support cdrom access */
#endif
#endif /* _NETENT_ */

#ifdef _NET_AND_CD_
#include <netentr.h>      /* support network access */
#endif /* _NET_AND_CD_ */

#define MAX_SECS_IN_ADAPTIVE_MATRIX 3
#define MAX_CHUNK_IN_ADAPTIVE_MATRIX 12
#define ADAPTIVE_MATRIX_INITIAL_CONDITION 8

#define HIERARCHY_CACHE_SIZE 200

#ifdef USE_IDxARCH
#ifdef USE_ID0
#define IDxArchInit ID0ArchInit
#define IDxArchFini ID0ArchFini
#define IDxArchSeqEntryGet ID0ArchSeqEntryGet
#define IDxArchGIGet ID0ArchGIGet
#endif
#ifdef USE_ID1
#define IDxArchInit ID1ArchInit
#define IDxArchFini ID1ArchFini
#define IDxArchSeqEntryGet ID1ArchSeqEntryGet
#define IDxArchGIGet ID1ArchGIGet
#endif
#endif /* USE_IDxARCH */

#ifdef _NETENTREZ_
Uint1 AdaptiveDocSumMatrix[MAX_CHUNK_IN_ADAPTIVE_MATRIX+1][MAX_SECS_IN_ADAPTIVE_MATRIX+1] = {
  8,  6,  3,  3,
  8,  6,  3,  3,
  8,  6,  3,  3,
  8,  6,  3,  3,
  9,  7,  4,  3,
  9,  8,  5,  3,
 10,  9,  5,  3,
 10, 10,  5,  3,
 11, 10,  5,  3,
 11, 11,  5,  3,
 12, 11,  5,  3,
 12, 11,  5,  3,
 12, 11,  5,  3
};
#endif /* _NETENTREZ_ */

                           /* maximum link records returned */
static Int4 MaxLinks = (Int4)(INT2_MAX / sizeof(DocUid));

static DocUidPtr      queueduids = NULL; /* Used to queue CD-ROM uid query list */
static DocSumPtr PNTR queuedsums = NULL; /* Used to queue network retrieved DocSums */
static Int2           numqueued = 0;
static DocType        queuedtype;
static CharPtr        CombinedInfo = NULL;
static DocType        lastTermType;
static Int2           lastBooleanMediaType;
static Int2           entrezInitCount = 0;
#ifdef USE_IDxARCH
static Boolean        idConnected = FALSE;
static Boolean        idCantConnect= FALSE;
static void DisconnID(void);
#endif


static Int2 GetCurMediaType(void)
{
#ifdef _PMENTREZ_
  return MEDIUM_DISK;
#else
  return CurMediaType();
#endif
}

static Boolean LIBCALL DoSelectDataSource(CharPtr section, CharPtr field1, CharPtr field2)
{
#ifdef _PMENTREZ_
  return TRUE;
#else
  return SelectDataSource (section, field1, field2);
#endif
}

static Boolean LIBCALL DoSelectDataSourceByType(DocType type, CharPtr field1, CharPtr field2)
{
#ifdef _PMENTREZ_
  return TRUE;
#else
  return SelectDataSourceByType(type, field1, field2);
#endif
}

/*****************************************************************************
*
*   EntrezInit ()
*   
*****************************************************************************/

NLM_EXTERN Boolean LIBCALL EntrezInit (CharPtr appl_id, Boolean no_warnings, BoolPtr is_network)

{
    Boolean cdretval = FALSE;
    Boolean netretval = FALSE;
    Boolean mbretval = TRUE;
    queueduids = NULL;
    queuedsums = NULL;
    numqueued = 0;

    if (is_network != NULL)
    {
        *is_network = FALSE;
    }

    if (entrezInitCount > 0)
    {
	entrezInitCount++;
	return TRUE;
    }

#ifdef _PMENTREZ_
    mbretval = PMEntrezInit(no_warnings);
#else

#ifdef _CDENTREZ_
    cdretval = CdEntrezInit(no_warnings);
#endif

#ifdef _MBENTREZ_
    mbretval = MBEntrezInit(no_warnings);
#endif

#endif

#ifdef _NETENTREZ_
    if (is_network != NULL)
    {
        *is_network = TRUE;
    }

    netretval = NetEntrezInit(appl_id, no_warnings);

#endif

#ifdef _PMENTREZ_

    MaxLinks = INT4_MAX;

#else

    if (!cdretval && !netretval)
    { /* partial success (one of two) is O.K. */
        return FALSE;
    }

    MaxLinks = EntrezGetMaxLinks();

#endif

    if (mbretval)
        entrezInitCount++;
    return mbretval;
}

/*****************************************************************************
*
*   EntrezIsInited ()
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL EntrezIsInited(void)
{
  return (entrezInitCount > 0);
}

/*****************************************************************************
*
*   EntrezFini ()
*
*****************************************************************************/

static void ClearQueuedDocSums (void)

{
    Int2  i;

    if (queueduids != NULL) {
        queueduids = (DocUidPtr) MemFree (queueduids);
    }
    if (queuedsums != NULL) {
        for (i = 0; i < numqueued; i++) {
          if (queuedsums [i] != NULL) {
            queuedsums [i] = DocSumFree (queuedsums [i]);
          }
        }
        queuedsums = (DocSumPtr PNTR) MemFree (queuedsums);
    }
    numqueued = 0;
}

static void FreeHierarchyList PROTO((void));

NLM_EXTERN void LIBCALL EntrezFini (void)

{
    ClearQueuedDocSums ();
    FreeHierarchyList ();
    if (CombinedInfo != NULL)
    {
        CombinedInfo = (CharPtr) MemFree(CombinedInfo);
    }

    if (entrezInitCount <= 0)
        return;
    if (--entrezInitCount > 0)
        return;

#ifdef USE_IDxARCH
    DisconnID ();
#endif
#ifdef _CDENTREZ_
    CdEntrezFini();
#endif
#ifdef _MBENTREZ_
    MBEntrezFini();
#endif
#ifdef _PMENTREZ_
    PMEntrezFini();
#endif
#ifdef _NETENTREZ_
    NetEntrezFini();
#endif
}

/*****************************************************************************
*
*   EntrezInfoPtr EntrezGetInfo()
*
*****************************************************************************/
NLM_EXTERN EntrezInfoPtr LIBCALL EntrezGetInfo (void)

{
    static EntrezInfoPtr cip = NULL;

#if defined(_CDENTREZ_) && (defined(_MBENTREZ_) || defined(_PMENTREZ_))
#ifdef _MBENTREZ_
    return MBAdjustEntrezInfo (CdEntrezGetInfo());
#else
    return PMEntrezGetInfo();
#endif
#else
    if (cip == NULL)
        cip = EntrezInfoMerge();

    /* as a last resort, fall back on the old, faithful function */
    if (cip == NULL)
#ifdef _CDENTREZ_
        return CdEntrezGetInfo();
#else
        return NetEntrezGetInfo();
#endif
    else
        return cip;
#endif
}

/*****************************************************************************
*
*   CharPtr EntrezDetailedInfo()
*       returns either NULL (if no info is available), or a pointer to a
*       statically allocated string containing formatted status information
*
*****************************************************************************/
NLM_EXTERN CharPtr LIBCALL EntrezDetailedInfo (void)

{
    CharPtr cdinfo = NULL;
    CharPtr netinfo = NULL;
    CharPtr extrainfo = NULL;
    Int2 extrainfoLen = 0;
    DocType db;
    CharPtr str;
    EntrezInfoPtr eip;

#ifdef _PMENTREZ_
    cdinfo = PMEntrezDetailedInfo();
#endif

#ifdef _CDENTREZ_
    cdinfo = CdEntrezDetailedInfo();
#endif
#ifdef _NETENTREZ_
    netinfo = NetEntrezDetailedInfo();
#endif
    if ((eip = EntrezGetInfo()) != NULL)
    {
        extrainfo = MemNew(StringLen(eip->descr) + 400);
        StrCpy(extrainfo, "\n\nCURRENT MEDIA\n  ");
        StrCat(extrainfo, eip->descr);
        extrainfoLen = StringLen(extrainfo);
        sprintf(&extrainfo[extrainfoLen], "\n  Version %ld.%ld", 
                (long) eip->version, (long) eip->issue);
        extrainfoLen = StringLen(extrainfo);
        if (eip->types != NULL)
        {
            for (db = 0; db < eip->type_count; db++)
            {
                if (eip->types[0].num > 0)
                {
                    if (eip->type_info != NULL)
                    {
                        str = eip->type_info[db].name;
                    } else {
                        str = eip->type_names[db];
                    }
                    sprintf(&extrainfo[extrainfoLen], "\n  # %s entries = %ld",
                            str, (long) (eip->types[db].num));
                    extrainfoLen = StringLen(extrainfo);
                }
            }
        }
        StrCat(extrainfo, "\n");
        extrainfoLen = StringLen(extrainfo);
    }

    if (CombinedInfo != NULL)
        MemFree(CombinedInfo);
    if ((CombinedInfo = MemNew(StringLen(cdinfo) + StringLen(netinfo) + 4 + extrainfoLen)) == NULL)
    {
        MemFree(extrainfo);
        return cdinfo; /* better than nothing */
    }
    StringCpy(CombinedInfo, cdinfo);
    StringCat(CombinedInfo, "\n\n");
    StringCat(CombinedInfo, netinfo);
    StringCat(CombinedInfo, extrainfo);
    MemFree(extrainfo);

    return CombinedInfo;
}

/*****************************************************************************
*
*   EntrezGetMaxLinks()
*       returns max links in link set allowed by system
*
*****************************************************************************/
NLM_EXTERN Int4 LIBCALL EntrezGetMaxLinks (void)

{
    Int4 maxlinks = INT4_MAX;
    Int4 links = 0;
    Boolean cdSelected = FALSE;

    if (! DoSelectDataSource(ENTR_LINKS_CHAN, "INFO", NULL))
    {
        /* ERROR, data unobtainable */
        return -1;
    }

    do {
        switch (GetCurMediaType()) {
#ifdef _CDENTREZ_
        case MEDIUM_CD:
            /* for ergonomics, check a maximum of one CD */
            cdSelected = TRUE;
            /* no break */
        case MEDIUM_DISK:
            links = CdEntGetMaxLinks();
            break;
#endif
#ifdef _NETENTREZ_
        case MEDIUM_NETWORK:
            links = NetEntGetMaxLinks();
            break;
#endif
        default:
            break;
        }
        maxlinks = MIN(maxlinks, links);
    } while (!cdSelected && SelectNextDataSource());

    if (maxlinks == INT4_MAX)
        maxlinks = -1;
    return maxlinks;
}

/*****************************************************************************
*
*   EntrezSetUserMaxLinks(num)
*       user settable maximum
*       can't be > EntrezGetMaxLinks()
*
*****************************************************************************/
NLM_EXTERN Int4 LIBCALL EntrezSetUserMaxLinks (Int4 num)

{
    Int4 big;

    big = EntrezGetMaxLinks();
    if (big < num)
        num = big;
    MaxLinks = num;
    return num;
}

/*****************************************************************************
*
*   EntrezGetUserMaxLinks()
*       returns current value of MaxLinks
*
*****************************************************************************/
NLM_EXTERN Int4 LIBCALL EntrezGetUserMaxLinks (void)

{
    return MaxLinks;
}

/*****************************************************************************
*
*   EntrezCreateNamedUidList(term, type, field, num, uids)
*       Creates a term node in the entrez set structure if one does not
*       yet exist, and loads the posting file from the uid parameter.
*
*****************************************************************************/
NLM_EXTERN void LIBCALL EntrezCreateNamedUidList (CharPtr term, DocType type, DocField field, Int4 num, DocUidPtr uids)

{

#ifdef _PMENTREZ_
  PMEntrezCreateNamedUIDList(term,type,num,uids);
  return;
#else

    if (! DoSelectDataSourceByType(type, "TERMS", NULL))
    {
        return;
    }

    switch (GetCurMediaType ()) {
#ifdef _CDENTREZ_
    case MEDIUM_CD:
    case MEDIUM_DISK:
        CdEntrezCreateNamedUidList (term, type, field, num, uids);
        break;
#endif
#ifdef _NETENTREZ_
    case MEDIUM_NETWORK:
        NetEntrezCreateNamedUidList (term, type, field, num, uids);
        break;
#endif
    default:
        break;
    }
#endif /* _PMENTREZ_ */
}

/*****************************************************************************
*
*   EntrezCreateNamedUidListX(term, type, field, bsp)
*       Creates a term node in the entrez set structure if one does not
*       yet exist, and loads the posting file from the uid parameter.
*
*****************************************************************************/
NLM_EXTERN void LIBCALL EntrezCreateNamedUidListX (CharPtr term, DocType type, DocField field, ByteStorePtr bsp)

{

#ifdef _PMENTREZ_
  PMEntrezCreateNamedUIDListBSP(term,type,bsp);
  return;
#else

    if (! DoSelectDataSourceByType(type, "TERMS", NULL))
    {
        return;
    }

    switch (GetCurMediaType ()) {
#ifdef _CDENTREZ_
    case MEDIUM_CD:
    case MEDIUM_DISK:
        CdEntrezCreateNamedUidListX (term, type, field, bsp);
        break;
#endif
#ifdef _NETENTREZ_
    case MEDIUM_NETWORK:
        NetEntrezCreateNamedUidListX (term, type, field, bsp);
        break;
#endif
    default:
        break;
    }
#endif /* _PMENTREZ_ */
}


/*****************************************************************************
*
*   EntrezTLNew (type)
*       Creates linked list of asn nodes for constructing boolean query on
*       terms.  First node points to processing info.
*       Remaining nodes contain symbols for AND,
*       OR, LEFT PARENTHESIS, RIGHT PARENTHESIS, or a SPECIAL or TOTAL term
*       specification.  The term specification nodes point to a CdTerm or a
*       string.
*
*****************************************************************************/

NLM_EXTERN ValNodePtr LIBCALL EntrezTLNew (DocType type)

{
    if (! DoSelectDataSourceByType(type, "BOOLEANS", NULL))
    {
        return NULL;
    }

    switch ( lastBooleanMediaType = GetCurMediaType() ) {
#ifdef _CDENTREZ_
    case MEDIUM_CD:
    case MEDIUM_DISK:
#ifdef _MBENTREZ_
        if (type == TYP_ML)
            return MBEntTLNew(type);
#endif
#ifdef _PMENTREZ_
	  return PMEntTLNew(type);
#else
          return CdEntTLNew(type);
#endif /* _PMENTREZ_ */
#endif
#ifdef _NETENTREZ_
    case MEDIUM_NETWORK:
        return NetEntTLNew(type);
#endif
    default:
        return NULL;
    }
}

/*****************************************************************************
*
*   EntrezTLAddTerm (elst, term, type, field, special)
*       Adds a term node to a boolean algebraic term query.
*
*****************************************************************************/

NLM_EXTERN ValNodePtr LIBCALL EntrezTLAddTerm (ValNodePtr elst, CharPtr term, DocType type, DocField field, Boolean special)
{
    return EntrezTLAddTermWithRange (elst, term, type, field, special, NULL);
}

NLM_EXTERN ValNodePtr LIBCALL EntrezTLAddTermWithRange (ValNodePtr elst, CharPtr term, DocType type, DocField field, Boolean special, CharPtr highRange)

{
    switch (lastBooleanMediaType) {
#ifdef _CDENTREZ_
    case MEDIUM_CD:
    case MEDIUM_DISK:
#ifdef _MBENTREZ_
        if (type == TYP_ML)
            return MBEntTLAddTerm (elst, term, type, field, special, highRange);
#endif

#ifdef _PMENTREZ_
	return PMEntTLAddTerm (elst, term, type, field, special, highRange);
#else
        return CdEntTLAddTerm (elst, term, type, field, special, highRange);
#endif /* _PMENTREZ_ */

#endif
#ifdef _NETENTREZ_
    case MEDIUM_NETWORK:
        return NetEntTLAddTerm (elst, term, type, field, special, highRange);
#endif
    default:
        return NULL;
    }
}

/*****************************************************************************
*
*   EntrezTLFree (elst)
*       Frees a boolean algebraic term query list.
*
*****************************************************************************/

NLM_EXTERN ValNodePtr LIBCALL EntrezTLFree (ValNodePtr elst)

{
#ifdef _PMENTREZ_
  return PMEntTLFree(elst);
#else

    switch (lastBooleanMediaType) {
#ifdef _CDENTREZ_
    case MEDIUM_CD:
    case MEDIUM_DISK:
#ifdef _MBENTREZ_
      {
	if (elst != NULL && elst->choice == NULLSYM)
	  { 
	    CdTermPtr eset = (CdTermPtr) elst->data.ptrvalue;
	    
	    if (eset != NULL && eset->type == TYP_ML)
	      {
		return MBEntTLFree(elst);
	      }
	  }
      }
#endif
      return CdEntTLFree(elst);
#endif

#ifdef _NETENTREZ_
    case MEDIUM_NETWORK:
        return NetEntTLFree(elst);
#endif
    default:
        return NULL;

    }

#endif

}

/*****************************************************************************
*
*   EntrezTLAddLParen (elst)
*       Adds a LEFT PAREN node to a boolean algebraic term query.
*
*****************************************************************************/

NLM_EXTERN ValNodePtr LIBCALL EntrezTLAddLParen (ValNodePtr elst)

{
  ValNodePtr anp;

  anp = NULL;
  if (elst != NULL) {
    anp = ValNodeNew (elst);
    if (anp != NULL) {
      anp->choice = LPAREN;
    }
  }
  return anp;
}

/*****************************************************************************
*
*   EntrezTLAddRParen (elst)
*       Adds a RIGHT PAREN node to a boolean algebraic term query.
*
*****************************************************************************/

NLM_EXTERN ValNodePtr LIBCALL EntrezTLAddRParen (ValNodePtr elst)

{
  ValNodePtr anp;

  anp = NULL;
  if (elst != NULL) {
    anp = ValNodeNew (elst);
    if (anp != NULL) {
      anp->choice = RPAREN;
    }
  }
  return anp;
}

/*****************************************************************************
*
*   CdEntTLAddAND (elst)
*       Adds an AND node to a boolean algebraic term query.
*
*****************************************************************************/

NLM_EXTERN ValNodePtr LIBCALL EntrezTLAddAND (ValNodePtr elst)

{
  ValNodePtr anp;

  anp = NULL;
  if (elst != NULL) {
    anp = ValNodeNew (elst);
    if (anp != NULL) {
      anp->choice = ANDSYMBL;
    }
  }
  return anp;
}

/*****************************************************************************
*
*   EntrezTLAddOR (elst)
*       Adds an OR node to a boolean algebraic term query.
*
*****************************************************************************/

NLM_EXTERN ValNodePtr LIBCALL EntrezTLAddOR (ValNodePtr elst)

{
  ValNodePtr anp;

  anp = NULL;
  if (elst != NULL) {
    anp = ValNodeNew (elst);
    if (anp != NULL) {
      anp->choice = ORSYMBL;
    }
  }
  return anp;
}

/*****************************************************************************
*
*   EntrezTLAddBUTNOT (elst)
*       Adds an BUTNOT node to a boolean algebraic term query.
*
*****************************************************************************/

NLM_EXTERN ValNodePtr LIBCALL EntrezTLAddBUTNOT (ValNodePtr elst)

{
  ValNodePtr anp;

  anp = NULL;
  if (elst != NULL) {
    anp = ValNodeNew (elst);
    if (anp != NULL) {
      anp->choice = BUTNOTSYMBL;
    }
  }
  return anp;
}

/*****************************************************************************
*
*   EntrezTLEval (elst)
*       Evaluates a boolean algebraic term query list, returning a pointer to
*       a LinkSet containing the resultant unique identifiers.
*
*****************************************************************************/


NLM_EXTERN LinkSetPtr LIBCALL EntrezPMTLEval (ValNodePtr elst, void * edc)
{

    switch (lastBooleanMediaType) {
#ifdef _CDENTREZ_
    case MEDIUM_CD:
    case MEDIUM_DISK:
#if defined(_MBENTREZ_) || defined(_PMENTREZ_)
        {
	    if (elst != NULL && elst->choice == NULLSYM)
	    { 
		CdTermPtr eset = (CdTermPtr) elst->data.ptrvalue;
#ifdef _MBENTREZ_
		if (eset != NULL && eset->type == TYP_ML)
		{
                    return MBEntTLEval(elst);
#else
		if (eset != NULL && DatabaseExistsInPubMed(eset->type))
		{
                    return PMEntTLEval(eset -> type,elst,edc);
#endif
		}
	    }
	}
#endif
        return CdEntTLEval(elst);
#endif
#ifdef _NETENTREZ_
    case MEDIUM_NETWORK:
        return NetEntTLEval(elst);
#endif
    default:
        return NULL;
    }
}

NLM_EXTERN LinkSetPtr LIBCALL EntrezTLEval (ValNodePtr elst)
{
  return EntrezPMTLEval (elst,NULL);
}

/*****************************************************************************
*
*   EntrezTLEvalCount (elst)
*       Evaluates a boolean algebraic term query list, returning the count
*       of resulting UIDs.
*****************************************************************************/

NLM_EXTERN Int4 LIBCALL EntrezPMTLEvalCount (ValNodePtr elst, void * edc)
{
    Int4 retval;

    switch (lastBooleanMediaType) {
#ifdef _CDENTREZ_
    case MEDIUM_CD:
    case MEDIUM_DISK:
#if defined(_MBENTREZ_) || defined(_PMENTREZ_)
        {
	    if (elst != NULL && elst->choice == NULLSYM)
	    { 
		CdTermPtr eset = (CdTermPtr) elst->data.ptrvalue;

#ifdef _MBENTREZ_
		if (eset != NULL && eset->type == TYP_ML)
		{
		  return MBEntTLEvalCount(elst);
#else
		if (eset != NULL && DatabaseExistsInPubMed(eset->type))
		{
		 return PMEntTLEvalCount(eset -> type, elst,edc);
#endif
		}
	    }
	}
#endif
        retval = CdEntTLEvalCount(elst);
        return retval;
#endif
#ifdef _NETENTREZ_
    case MEDIUM_NETWORK:
        retval = NetEntTLEvalCount(elst);
        return retval;
#endif
    default:
        return 0;
    }
}

NLM_EXTERN Int4 LIBCALL EntrezTLEvalCount (ValNodePtr elst)
{
  return EntrezPMTLEvalCount (elst, NULL);
}


/*****************************************************************************
*
*   EntrezTLEvalX (elst)
*       Evaluates a boolean algebraic term query list, returning a pointer to
*       a ByteStore containing the resultant unique identifiers.  The number
*       of UIDs is calculated as BSLen (bsp) / sizeof (DocUid).
*
*****************************************************************************/

NLM_EXTERN ByteStorePtr LIBCALL EntrezPMTLEvalX (ValNodePtr elst, void * edc)
{
    ByteStorePtr bsp;

    switch (lastBooleanMediaType) {
#ifdef _CDENTREZ_
    case MEDIUM_CD:
    case MEDIUM_DISK:
#if defined(_MBENTREZ_) || defined(_PMENTREZ_)
        {
	    if (elst != NULL && elst->choice == NULLSYM)
	    { 
		CdTermPtr eset = (CdTermPtr) elst->data.ptrvalue;
#ifdef _MBENTREZ_
		if (eset != NULL && eset->type == TYP_ML)
		{
                    bsp = MBEntTLEvalX(elst);
#else
		if (eset != NULL && DatabaseExistsInPubMed(eset->type))
		{
                    bsp = PMEntTLEvalX(eset -> type, elst,edc);
#endif
                    if (bsp != NULL) {
                        BSSeek (bsp, 0L, 0);
                    }
                    return bsp;
		}
	    }
	}
#endif
        bsp = CdEntTLEvalX(elst);
        if (bsp != NULL) {
            BSSeek (bsp, 0L, 0);
        }
        return bsp;
#endif
#ifdef _NETENTREZ_
    case MEDIUM_NETWORK:
        bsp = NetEntTLEvalX(elst);
        if (bsp != NULL) {
            BSSeek (bsp, 0L, 0);
        }
        return bsp;
#endif
    default:
        return NULL;
    }
}

NLM_EXTERN ByteStorePtr LIBCALL EntrezTLEvalX (ValNodePtr elst)
{
  return(EntrezPMTLEvalX(elst,NULL));
}


#ifdef USE_IDxARCH
static Boolean
ConnectID(void)
{
    if (!idConnected && !idCantConnect)
    {
        if (IDxArchInit())
        {
            idConnected = TRUE;
        } else {
            idCantConnect = TRUE;
        }
    }

    return idConnected;
}

static void
DisconnID(void)
{
    if (idConnected)
        IDxArchFini();
    idConnected = FALSE;
    idCantConnect = FALSE;
}
#endif /* USE_IDxARCH */

/*****************************************************************************
*
*   DocSumPtr EntrezDocSum(type, uid)
*
*****************************************************************************/
NLM_EXTERN DocSumPtr LIBCALL EntrezDocSum (DocType type, DocUid uid)

{
    DocSumPtr dsp;

#ifdef _PMENTREZ_
    return PMDocSum(type, uid);
#else

    if (! DoSelectDataSourceByType(type, "DOCSUMS", NULL))
    { /* ERROR, data unobtainable */
        return NULL;
    }

    do {
        switch (GetCurMediaType())
        {
#ifdef _CDENTREZ_
        case MEDIUM_CD:
        case MEDIUM_DISK:
#ifdef _MBENTREZ_
            if (type == TYP_ML)
	    {
		return MBDocSum(type, uid);
	    }
#endif
            if ((dsp = CdDocSum (type, uid)) != NULL)
                return dsp;
#ifdef USE_IDxARCH
            if (type != TYP_ST && type != TYP_CH && ConnectID())
            {
                SeqEntryPtr sep;
                Char fname[PATH_MAX];
                AsnIoPtr aip;
                DocSumPtr dsp = NULL;

                if ((sep = IDxArchSeqEntryGet(uid, NULL, 0)) != NULL)
                {
                    TmpNam(fname);
                    aip = AsnIoOpen(fname, "w");
                    SeqEntryAsnWrite(sep, aip, NULL);
                    AsnIoClose(aip);
                    aip = AsnIoOpen(fname, "r");
                    dsp = CdSeqSumAsnRead(aip, uid);
                    SeqEntryFree(sep);
                    AsnIoClose(aip);
                    FileRemove(fname);
                    return dsp;
                }
            }
#endif /* USE_IDxARCH */
            break;
#endif
#ifdef _NETENTREZ_
        case MEDIUM_NETWORK:
            if ((dsp = NetDocSum (type, uid)) != NULL)
                return dsp;
            break;
#endif
        default:
            break;
        }
    } while (SelectNextDataSource());

    return NULL;

#endif
}

/*****************************************************************************
*
*   EntrezDocSumListGet(numuid, type, uids, callback)
*
*****************************************************************************/
static Int2 DocSumQueueGet (Int2 numuid, DocType type, DocSumListCallBack callback)

{
  DocSumPtr  dsp;
  int        count = 0;
  int        i;
  Boolean    goOn;

  if (callback == NULL) {
    ClearQueuedDocSums ();
    return 0;
  }
  if (type != queuedtype) {
    ClearQueuedDocSums ();
    return 0;
  }
  goOn = TRUE;
  count = 0;
  if (numuid > numqueued) {
    numuid = numqueued;
  }

  i = 0;
  while (i < numqueued && goOn) {
    if (queueduids [i] != 0) {
      if (queuedsums != NULL) {
        dsp = queuedsums [i];
      } else {
        dsp = EntrezDocSum (type, queueduids [i]);
      }
      if (dsp != NULL) {
        count++;
        goOn = callback (dsp, dsp->uid);
        queueduids [i] = 0;
        if (queuedsums != NULL) {
          queuedsums [i] = NULL;
        }
      } else {
        count++;
        goOn = callback (NULL, queueduids [i]);
        queueduids [i] = 0;
        if (queuedsums != NULL) {
          queuedsums [i] = NULL;
        }
      }
    }
    i++;
  }
  if (i >= numqueued) {
    if (queuedsums != NULL) {
      queuedsums = (DocSumPtr PNTR) MemFree (queuedsums);
    }
    queueduids = (DocUidPtr) MemFree (queueduids);
    numqueued = 0;
  }

  return count;
}

NLM_EXTERN Int2 LIBCALL EntrezDocSumListGet (Int2 numuid, DocType type, DocUidPtr uids,
                                 DocSumListCallBack callback)

{
  DocSumPtr  dsp;
  int        count = 0;
  int        i;
  Boolean    goOn;
  DocUidPtr  uidlist;
#ifdef _NETENTREZ_
  Int2       chunkSize;
  Int2       chunkStart;
  Int2       predictedNextChunk;
  Int2       actual;
  DocSumPtr  PNTR result;
  static Int2 adaptiveChunkSize = ADAPTIVE_MATRIX_INITIAL_CONDITION;
  time_t     timeElapsed, startTime;
#endif

  /* For ergonomics, DocSums are fetched from the CD-ROM and displayed */
  /* one at a time. For efficiency, all DocSums are fetched from the   */
  /* network at once, and subsequently all are displayed.              */

  if (uids == NULL && queueduids != NULL && callback != NULL) {
    return DocSumQueueGet (numuid, queuedtype, callback);
  }
  ClearQueuedDocSums ();
  queuedtype = type;
  if (numuid == 0) {
    return 0;
  }
  if (uids == NULL) {
    return 0;
  }
  goOn = TRUE;
  count = 0;
  uidlist = (DocUidPtr) MemNew (sizeof (DocUid) * numuid);
  if (uidlist != NULL) {
    for (i = 0; i < numuid; i++) {
      uidlist [i] = uids [i];
    }
#ifdef _NETENTREZ_

    if (! DoSelectDataSourceByType(type, "DOCSUMS", NULL))
    { /* ERROR, data unobtainable */
      uidlist = (DocUidPtr) MemFree(uidlist);
      return 0;
    }

    /* if the first-found media type is NET, try to get them all from the  */
    /* net; otherwise, to heck with it, just get them one at a time, below */
    if (GetCurMediaType() == MEDIUM_NETWORK) {
      result = (DocSumPtr PNTR) MemNew (sizeof (DocSumPtr) * numuid);
      if (result != NULL) {
        i = 0;
        chunkStart = 0;
        chunkSize = adaptiveChunkSize;
        while (chunkStart < numuid && goOn)
        {
          /* if ( the callback told us that it doesn't need any more right   */
          /*      now, OR if we would be getting too many ) THEN             */
          /*    get all of the remaining docsums in one shot                 */
          if (callback == NULL || !goOn || (chunkSize + chunkStart) >= numuid)
            chunkSize = numuid - chunkStart;

          /* Ask for some to be computed in advance, because it can take the*/
          /* application a long time to process the ones which it _does_    */
          /* receive this time                                              */
          predictedNextChunk = MIN(numuid - (chunkStart+chunkSize), chunkSize);
          startTime = GetSecs();
          actual = NetDocSumListGet (&result[chunkStart], (Int2)(chunkSize +
                                     predictedNextChunk), type,
                                     &uids[chunkStart], predictedNextChunk);
          chunkStart += chunkSize;

          timeElapsed = GetSecs() - startTime;
          if (timeElapsed < 0)
              timeElapsed = 0;
          if (timeElapsed > MAX_SECS_IN_ADAPTIVE_MATRIX)
              timeElapsed = MAX_SECS_IN_ADAPTIVE_MATRIX;
          if (chunkSize > MAX_CHUNK_IN_ADAPTIVE_MATRIX)
              chunkSize = MAX_CHUNK_IN_ADAPTIVE_MATRIX;
          adaptiveChunkSize = AdaptiveDocSumMatrix[chunkSize][timeElapsed];
          chunkSize = adaptiveChunkSize;

          if (goOn && callback != NULL) {
            while (i < chunkStart && goOn) {
              dsp = result [i];
              if (dsp != NULL) {
                count++;
                goOn = callback (dsp, dsp->uid);
                uidlist [i] = 0;
                result [i] = NULL;
              } else {
                count++;
                goOn = callback (NULL, uidlist [i]);
                uidlist [i] = 0;
                result [i] = NULL;
              }
              i++;
            }
          }
        }

        if (count < numuid) { /* enqueue remaining docsums */
          queueduids = uidlist;
          queuedsums = result;
          numqueued = numuid;
        } else {
          MemFree (uidlist);
          MemFree (result);
        }
      }
      return count;
    }
#endif

    /* get them one at a time */
    if (callback != NULL) {
      i = 0;
      while (i < numuid && goOn) {
        dsp = EntrezDocSum (type, uidlist [i]);
        if (dsp != NULL) {
          count++;
          goOn = callback (dsp, dsp->uid);
          uidlist [i] = 0;
        } else {
          count++;
          goOn = callback (NULL, uidlist [i]);
          uidlist [i] = 0;
        }
        i++;
      }
    }
    if (count < numuid) {
      queueduids = uidlist;
      numqueued = numuid;
    } else {
      MemFree (uidlist);
    }
  }
  return count;
}


/*****************************************************************************
*
*   EntrezTermListByTerm (type, field, term, numterms, proc, first_page)
*       Gets Terms starting at term
*       returns pages read
*       sets first_page to first page read
*
*****************************************************************************/
NLM_EXTERN Int2 LIBCALL EntrezTermListByTerm (DocType type, DocField field, CharPtr term, Int2 numterms, TermListProc proc, Int2Ptr first_page)

{
    if (! DoSelectDataSourceByType(type, "TERMS", NULL))
    {
        return 0;
    }


    lastTermType = type;

    switch (GetCurMediaType())
    {
#ifdef _CDENTREZ_
    case MEDIUM_CD:
    case MEDIUM_DISK:
#ifdef _MBENTREZ_
        if (type == TYP_ML && field != FLD_PROT /* FLD_PROT is MeSH glossary */)
        {
            return MBTermListByTerm(type, field, term, numterms, proc, first_page);
        }
#endif
#ifdef _PMENTREZ_
	if (DatabaseExistsInPubMed(type))
        {
            return PMTermListByTerm(type, field, term, numterms, proc, first_page);
        }
#endif
        return CdTermListByTerm(type, field, term, numterms, proc, first_page);
#endif
#ifdef _NETENTREZ_
    case MEDIUM_NETWORK:
        return NetTermListByTerm(type, field, term, numterms, proc, first_page);
#endif
    default:
        return 0;
    }
}

/*****************************************************************************
*
*   EntrezTermListByPage (type, field, page, numpage, proc)
*       Gets terms starting at page, for numpage, by calling proc
*       returns pages read
*
*****************************************************************************/
NLM_EXTERN Int2 LIBCALL EntrezTermListByPage (DocType type, DocField field, Int2 page, Int2 numpage, TermListProc proc)

{
    if (! DoSelectDataSourceByType(type, "TERMS", NULL))
    {
        return 0;
    }

    lastTermType = type;

    switch (GetCurMediaType())
    {
#ifdef _CDENTREZ_
    case MEDIUM_CD:
    case MEDIUM_DISK:
#ifdef _MBENTREZ_
        if (type == TYP_ML && field != FLD_PROT /* FLD_PROT is MeSH glossary */)
        {
            return MBTermListByPage(type, field, page, numpage, proc);
        }
#endif
#ifdef _PMENTREZ_
	if (DatabaseExistsInPubMed(type))
        {
            return PMTermListByPage(type, field, page, numpage, proc);
        }
#endif
        return CdTermListByPage(type, field, page, numpage, proc);
#endif
#ifdef _NETENTREZ_
    case MEDIUM_NETWORK:
        return NetTermListByPage(type, field, page, numpage, proc);
#endif
    default:
        return 0;
    }
}

/*****************************************************************************
*
*   EntrezFindTerm(type, field, term, spec, total)
*       returns count of special and total for a term
*       if term ends with  "...", does a truncated merge of the term
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL EntrezFindTerm (DocType type, DocField field, CharPtr term, Int4Ptr spcl, Int4Ptr totl)

{
    if (! DoSelectDataSourceByType(type, "TERMS", NULL))
    {
        return 0;
    }

    switch (GetCurMediaType())
    {
#ifdef _CDENTREZ_
    case MEDIUM_CD:
    case MEDIUM_DISK:
#ifdef _MBENTREZ_
        if (type == TYP_ML && field != FLD_PROT /* FLD_PROT is MeSH glossary */)
        {
            return MBEntrezFindTerm (type, field, term, spcl, totl);
        }
#endif
#ifdef _PMENTREZ_
	if (DatabaseExistsInPubMed(type))
        {
            return PMEntrezFindTerm (type, field, term, spcl, totl);
        }
#endif
        return CdEntrezFindTerm (type, field, term, spcl, totl);
#endif
#ifdef _NETENTREZ_
    case MEDIUM_NETWORK:
        return NetEntrezFindTerm (type, field, term, spcl, totl);
#endif
    default:
        return 0;
    }
}


/*****************************************************************************
*
*   EntrezUidLinks()
*       retrieves links to other uids
*
*****************************************************************************/
NLM_EXTERN LinkSetPtr LIBCALL EntrezUidLinks (DocType type, DocUid uid, DocType link_to_type)

{
    LinkSetPtr lsp;

    if (! SelectDataLinksByTypes(type, link_to_type))
    {
        return NULL;
    }

    do {
        switch (GetCurMediaType())
        {
#ifdef _CDENTREZ_
        case MEDIUM_CD:
        case MEDIUM_DISK:
#ifdef _MBENTREZ_
            if (type == TYP_ML && link_to_type == TYP_ML)
            {
                return MBUidLinks(type, uid, link_to_type);
            }
#endif
#ifdef _PMENTREZ_
	    return PMUidLinks(type, uid, link_to_type);
#else
            if ((lsp = CdUidLinks(type, uid, link_to_type)) != NULL)
                return lsp;
            break;
#endif /* _PMENTREZ_ */

#endif
#ifdef _NETENTREZ_
        case MEDIUM_NETWORK:
            if ((lsp = NetUidLinks(type, uid, link_to_type)) != NULL)
                return lsp;
            break;
#endif
        default:
            break;
        }
    } while (SelectNextDataSource());

    return NULL;
}


/*****************************************************************************
*
*   EntrezLinkUidList(type, link_to_type, numuid, uids)
*       returns count of input uids processed
*       returns -1 on error
*       if neighbors (type == link_to_type)
*           sums weights for same uids
*       if (more than EntrezUserMaxLinks() uids, frees uids and weights,
*           but leaves num set)
*
*****************************************************************************/
NLM_EXTERN Int2 LIBCALL EntrezLinkUidList (LinkSetPtr PNTR result, DocType type, DocType link_to_type, Int2 numuid, Int4Ptr uids, Boolean mark_missing)

{
    Int2 obtained_ids = 0;
    Int4Ptr local_uids;
    Int4Ptr best_obtained_uids;
    Int2 best_obtained_ids = 0;
    LinkSetPtr best_lsp = NULL;
    LinkSetPtr lsp;
    Int2 i;
    Int2 missing = 0;
    Int2 fewest_missing = numuid + 1;
    Boolean badErr = FALSE;

    if (numuid == 0)
        return 0;

    if (! SelectDataLinksByTypes(type, link_to_type))
    { /* ERROR, data unobtainable */
        return 0;
    }

    local_uids = (Int4Ptr) MemNew(numuid * sizeof(Int4));
    best_obtained_uids = (Int4Ptr) MemNew(numuid * sizeof(Int4));

    /* Try the various available data sources, and return "successfully" if   */
    /* a data source is located with neighbors for all requested UIDs.        */
    /* If no such data source can be located, then try all data sources,      */
    /* selecting the one for which the most possible UIDs have neighbors.     */
    /* When two or more UIDs have the same number of UIDs with neighbors, use */
    /* the number of obtained neighbors as a tie-breaker.                     */

    do {
        MemCopy (local_uids, uids, numuid * sizeof(*uids));

        switch (GetCurMediaType())
        {
#ifdef _CDENTREZ_
        case MEDIUM_CD:
        case MEDIUM_DISK:
#ifdef _MBENTREZ_
            if (type == TYP_ML && link_to_type == TYP_ML)
            {
		obtained_ids = MBLinkUidList(&lsp, type, link_to_type, numuid,
				             local_uids, TRUE);
                break;
            }
#endif
#ifdef _PMENTREZ_
	    obtained_ids = PMLinkUidList(&lsp, type, link_to_type, numuid,
					 local_uids, TRUE);
	    break;
		
#else
            obtained_ids =  CdLinkUidList(&lsp, type, link_to_type, numuid,
                                          local_uids, TRUE);
            break;

#endif /* _PMENTREZ_ */
#endif
#ifdef _NETENTREZ_
        case MEDIUM_NETWORK:
            obtained_ids =  NetLinkUidList(&lsp, type, link_to_type, numuid,
                                           local_uids, TRUE);
            break;
#endif
        default:
            badErr = TRUE;
            break;
        }

        if (badErr)
            break;

        for (missing = 0, i = 0; i < numuid; i++)
        {
            if (local_uids[i] < 0)
                missing++;
        }

        if (missing == 0) /* success */
        {
            break;
        }

        if (missing <= fewest_missing)
        {
            if (missing < fewest_missing || best_lsp == NULL ||
                lsp->num > best_lsp->num)
            { /* found a better match */
                if (best_lsp != NULL)
                    LinkSetFree(best_lsp);
                best_lsp = lsp;
                fewest_missing = missing;
                best_obtained_ids = obtained_ids;
                MemCopy(best_obtained_uids, local_uids, numuid * sizeof(*uids));
            }
        }
        else {
            LinkSetFree(lsp);
        }
    } while (SelectNextDataSource()); /* until we've run out of options */

    if (missing == 0)
    { /* found neighbors for all UIDs */
        if (best_lsp != NULL)
            LinkSetFree(best_lsp);
        *result = lsp;
        if (mark_missing)
            MemCopy(uids, local_uids, numuid * sizeof(*uids));
        MemFree(local_uids);
        MemFree(best_obtained_uids);
        return obtained_ids;
    }
    else { /* couldn't find links for all; return best */
        *result = best_lsp;
        if (mark_missing)
            MemCopy(uids, best_obtained_uids, numuid * sizeof(*uids));
        MemFree(local_uids);
        MemFree(best_obtained_uids);
        return best_obtained_ids;
    }
}

/*****************************************************************************
*
*   EntrezMedlineEntryListGet (result, numuid, uids, mark_missing)
*       returns a count of entries read
*       if (mark_missing) ids which could not be located are made negative
*
*****************************************************************************/
NLM_EXTERN Int2 LIBCALL EntrezMedlineEntryListGet(MedlineEntryPtr PNTR result, Int2 numuid,
                         Int4Ptr uids, Boolean mark_missing)
{
    Int2    obtained_ids = 0;
    Int4Ptr local_uids;
    int     unmatched;
    Int4Ptr map;
    Boolean first_time;
    MedlineEntryPtr PNTR res;
    Int2    i;

    if (numuid == 0)
        return 0;

#ifdef _PMENTREZ_
    return PMEntMedlineEntryListGet (result, numuid, uids);
#else

    first_time = TRUE;

    if (! DoSelectDataSource(ENTR_REF_CHAN, "RECORDS", NULL)) /* MEDLINE DATA (Abstracts) */
    {
        /* ERROR, data unobtainable */
        return 0;
    }

    local_uids = (Int4Ptr) MemDup(uids, numuid * sizeof(*uids));
    res = (MedlineEntryPtr PNTR) MemNew(numuid * sizeof(MedlineEntryPtr));
    map = (Int4Ptr) MemNew(numuid * sizeof(Int4));

    do {
        switch (GetCurMediaType())
        {
#ifdef _CDENTREZ_
        case MEDIUM_CD:
        case MEDIUM_DISK:
#ifdef _MBENTREZ_
            obtained_ids += MBEntMedlineEntryListGet (res, numuid, local_uids, TRUE);
            break;
#endif
            obtained_ids += CdEntMedlineEntryListGet (res, numuid, local_uids, TRUE);
        break;
#endif
#ifdef _NETENTREZ_
        case MEDIUM_NETWORK:
            obtained_ids += NetEntMedlineEntryListGet (res, numuid, local_uids, TRUE);
            break;
#endif
        default:
            break;
        }

        for (unmatched = 0, i = 0; i < numuid; i++)
        {
            if (local_uids[i] >= 0) /* found this one */
            {
                if (first_time)
                {
                    result[i] = res[i];
                }
                else
                { /* map the location */
                    result[map[i]] = res[i];
                }
            }
            else {
                if (first_time)
                {
                    result[i] = NULL;
                    if (mark_missing)
                        uids[i] = local_uids[i];
                    map[unmatched] = i;
                }
                else {
                    map[unmatched] = map[i];
                }
                local_uids[unmatched] = ABS(local_uids[i]);
                unmatched++;
            }
        }

        numuid = unmatched;
        first_time = FALSE;

    } while (numuid > 0 && SelectNextDataSource()); /* until we've run out of options */

    MemFree (local_uids);
    MemFree (res);
    MemFree (map);

    return obtained_ids;
#endif
}

/*****************************************************************************
*
*   EntrezMedlineEntryGet(uid)
*       get one MedlineEntry
*
*****************************************************************************/
NLM_EXTERN MedlineEntryPtr LIBCALL EntrezMedlineEntryGet (Int4 uid)

{
    MedlineEntryPtr mep = NULL;
    ErrStrId errorID;
    char errorString[35];
    
        sprintf(errorString, "(EntrezMedlineEntryGet: %ld)",(long)uid);
        errorID = Nlm_ErrUserInstall(errorString, 0);
                
    EntrezMedlineEntryListGet(&mep, 1, &uid, FALSE);
    Nlm_ErrUserDelete(errorID);

    return mep;
}


/*****************************************************************************
*
*   EntrezPubmedEntryListGet (result, numuid, uids, mark_missing)
*       returns a count of entries read
*       if (mark_missing) ids which could not be located are made negative
*
*****************************************************************************/
#ifdef _PMENTREZ_

Int2 LIBCALL EntrezPubmedEntryListGet(PubmedEntryPtr PNTR result, Int2 numuid,
                         Int4Ptr uids, Boolean mark_missing)
{
    Int2    obtained_ids = 0;
    Int4Ptr local_uids;
    int     unmatched;
    Int4Ptr map;
    Boolean first_time;
    PubmedEntryPtr PNTR res;
    Int2    i;

    if (numuid == 0)
        return 0;

    first_time = TRUE;

    if (! DoSelectDataSource(ENTR_REF_CHAN, "RECORDS", NULL)) 
      {
        /* ERROR, data unobtainable */
        return 0;
      }
    
    local_uids = (Int4Ptr) MemDup(uids, numuid * sizeof(*uids));
    res = (PubmedEntryPtr PNTR) MemNew(numuid * sizeof(PubmedEntryPtr));
    map = (Int4Ptr) MemNew(numuid * sizeof(Int4));

    do {
        switch (GetCurMediaType())
        {
#ifdef _CDENTREZ_
        case MEDIUM_CD:
        case MEDIUM_DISK:
#ifdef _PMENTREZ_
            obtained_ids += PubmedEntryListGet (res, numuid, local_uids, TRUE);
            break;
#endif
        break;
#endif
#ifdef _NETENTREZ_
        case MEDIUM_NETWORK:
            obtained_ids += NetEntPubmedEntryListGet (res, numuid, local_uids, TRUE);
            break;
#endif
        default:
            break;
        }

        for (unmatched = 0, i = 0; i < numuid; i++)
        {
            if (local_uids[i] >= 0) /* found this one */
            {
                if (first_time)
                {
                    result[i] = res[i];
                }
                else
                { /* map the location */
                    result[map[i]] = res[i];
                }
            }
            else {
                if (first_time)
                {
                    result[i] = NULL;
                    if (mark_missing)
                        uids[i] = local_uids[i];
                    map[unmatched] = i;
                }
                else {
                    map[unmatched] = map[i];
                }
                local_uids[unmatched] = ABS(local_uids[i]);
                unmatched++;
            }
        }

        numuid = unmatched;
        first_time = FALSE;

    } while (numuid > 0 && SelectNextDataSource()); /* until we've run out of options */

    MemFree (local_uids);
    MemFree (res);
    MemFree (map);

    return obtained_ids;
}

/*****************************************************************************
*
*   EntrezPubmedEntryGet(uid)
*       get one PubmedEntry
*
*****************************************************************************/
PubmedEntryPtr LIBCALL EntrezPubmedEntryGet (Int4 uid)

{
    PubmedEntryPtr mep = NULL;

    EntrezPubmedEntryListGet(&mep, 1, &uid, FALSE);
    return mep;
}

#endif

/*****************************************************************************
*
*   EntrezBiostrucGet(uid, Int4 mdlLvl, Int4 maxModels)
*       get one Biostruc
*
*****************************************************************************/

#ifdef Biostruc_supported

NLM_EXTERN BiostrucPtr LIBCALL EntrezBiostrucGet (DocUid uid, Int4 mdlLvl, Int4 maxModels)
{
    BiostrucPtr retval = NULL;

#ifdef _PMENTREZ_
    return PMEntrezBiostrucGet(uid,mdlLvl,maxModels);
#else

    if (! DoSelectDataSource(ENTR_SEQ_CHAN, "RECORDS", NULL)) /* SEQUENCE DATA (Abstracts) */
    {
        /* ERROR, data unobtainable */
        return NULL;
    }

    switch (GetCurMediaType())
    {
#ifdef _CDENTREZ_
    case MEDIUM_CD:
    case MEDIUM_DISK:
        return CdEntrezBiostrucGet(uid, mdlLvl, maxModels);
#endif    
#ifdef _NETENTREZ_
    case MEDIUM_NETWORK:
        NetEntrezBiostrucListGet(&retval, mdlLvl, maxModels, 1, &uid, FALSE);
        return retval;
#endif    
    default:
        return NULL;
    }
#endif
}

NLM_EXTERN BiostrucAnnotSetPtr LIBCALL EntrezBiostrucAnnotSetGet (DocUid uid)
{
    if (! DoSelectDataSource(ENTR_SEQ_CHAN, "RECORDS", NULL)) /* SEQUENCE DATA (Abstracts) */
    {
        /* ERROR, data unobtainable */
        return NULL;
    }

    switch (GetCurMediaType())
    {
#ifdef _CDENTREZ_
    case MEDIUM_CD:
    case MEDIUM_DISK:
        return CdEntrezBiostrucAnnotSetGet(uid);
#endif    
#ifdef _NETENTREZ_
    case MEDIUM_NETWORK:
        return NetEntrezBiostrucAnnotSetGet(uid);
#endif    
    default:
        return NULL;
    }
}

NLM_EXTERN LinkSetPtr LIBCALL EntrezBiostrucFeatIds(DocUid mmdbid, Int2 feature_type, Int4 feature_set_id)
{
    if (! DoSelectDataSource(ENTR_SEQ_CHAN, "RECORDS", NULL)) /* SEQUENCE DATA (Abstracts) */
    {
        /* ERROR, data unobtainable */
        return NULL;
    }

    switch (GetCurMediaType())
    {
#ifdef _CDENTREZ_
    case MEDIUM_CD:
    case MEDIUM_DISK:
        return CdEntrezBiostrucFeatIds(mmdbid, feature_type, feature_set_id);
#endif    
#ifdef _NETENTREZ_
    case MEDIUM_NETWORK:
        return NetEntrezBiostrucFeatIds(mmdbid, feature_type, feature_set_id);
#endif    
    default:
        return NULL;
    }
}


NLM_EXTERN BiostrucAnnotSetPtr LIBCALL EntrezBiostrucAnnotSetGetByFid (DocUid mmdbid, Int4 feature_id, Int4 feature_set_id)
{
    if (! DoSelectDataSource(ENTR_SEQ_CHAN, "RECORDS", NULL)) /* SEQUENCE DATA (Abstracts) */
    {
        /* ERROR, data unobtainable */
        return NULL;
    }

    switch (GetCurMediaType())
    {
#ifdef _CDENTREZ_
    case MEDIUM_CD:
    case MEDIUM_DISK:
        return CdEntrezBiostrucAnnotSetGetByFid (mmdbid, feature_id, feature_set_id);
#endif    
#ifdef _NETENTREZ_
    case MEDIUM_NETWORK:
        return NetEntrezBiostrucAnnotSetGetByFid (mmdbid, feature_id, feature_set_id);
#endif    
    default:
        return NULL;
    }
}

#else

NLM_EXTERN BiostrucPtr LIBCALL EntrezBiostrucGet (DocUid uid, Int4 mdlLvl, Int4 maxModels)
{
        return NULL;
}

NLM_EXTERN BiostrucAnnotSetPtr LIBCALL EntrezBiostrucAnnotSetGet (DocUid uid)
{
        return NULL;
}

NLM_EXTERN LinkSetPtr LIBCALL EntrezBiostrucFeatIds(DocUid mmdbid, Int2 feature_type, Int4 feature_set_id)
{
        return NULL;
}

NLM_EXTERN BiostrucAnnotSetPtr LIBCALL EntrezBiostrucAnnotSetGetByFid (DocUid mmdbid, Int4 feature_id, Int4 feature_set_id)
{
        return NULL;
}
#endif



/*****************************************************************************
*
*   EntrezSeqEntryListGet (result, numuid, uids, retcode, mark_missing)
*       returns a count of entries read
*       if (mark_missing) ids which could not be located are made negative
*       retcode is defined in objsset.h 
*
*****************************************************************************/
NLM_EXTERN Int2 LIBCALL EntrezSeqEntryListGet (SeqEntryPtr PNTR result, Int2 numuid, Int4Ptr uids, Int2 retcode, Boolean mark_missing)

{
    Int2    obtained_ids = 0;
    Int4Ptr local_uids;
    int     unmatched;
    Int4Ptr map;
    Boolean first_time;
    SeqEntryPtr PNTR res;
    Int2    i;
    /* DocType type; */

    if (numuid == 0)
        return 0;

#ifdef _PMENTREZ_
    return PMEntSeqEntryListGet (result, numuid, uids,retcode);
#else

    first_time = TRUE;

    if (! DoSelectDataSource(ENTR_SEQ_CHAN, "RECORDS", NULL)) /* SEQUENCE DATA (Abstracts) */
    {
        /* ERROR, data unobtainable */
        return 0;
    }

    local_uids = (Int4Ptr) MemDup(uids, numuid * sizeof(*uids));
    res = (SeqEntryPtr PNTR) MemNew(numuid * sizeof(SeqEntryPtr));
    map = (Int4Ptr) MemNew(numuid * sizeof(Int4));

    do {
        switch (GetCurMediaType())
        {
#ifdef _CDENTREZ_
        case MEDIUM_CD:
        case MEDIUM_DISK:
            if (retcode == -2)
                obtained_ids = 0;
            else
                obtained_ids += CdEntSeqEntryListGet (res, numuid, local_uids,
                                                      retcode, TRUE);
#ifdef USE_IDxARCH
            if (retcode != -1 && obtained_ids < numuid && ConnectID())
            {
                Int2 i;
        
                /* for entries which are to be explicitly retrieved from ID */
                if (retcode == -2)
                    retcode = 0;
        
                for (i = 0; i < numuid; i++)
                {
                    if (res[i] == NULL)
                    {
                        if ((res[i] = IDxArchSeqEntryGet(ABS(local_uids[i]),
							 NULL, retcode))
                             != NULL)
                        {
                            local_uids[i] = ABS(local_uids[i]);
                            obtained_ids++;
                        }
                    }
                }
            }
#endif /* USE_IDxARCH */
        break;
#endif
#ifdef _NETENTREZ_
        case MEDIUM_NETWORK:
            obtained_ids += NetEntSeqEntryListGet (res, numuid, local_uids,
                                                retcode, TRUE);
            break;
#endif
        default:
            break;
        }

        for (unmatched = 0, i = 0; i < numuid; i++)
        {
            if (local_uids[i] >= 0) /* found this one */
            {
                if (first_time)
                {
                    result[i] = res[i];
                }
                else
                { /* map the location */
                    result[map[i]] = res[i];
                }
            }
            else {
                if (first_time)
                {
                    result[i] = NULL;
                    if (mark_missing)
                        uids[i] = local_uids[i];
                    map[unmatched] = i;
                }
                else {
                    map[unmatched] = map[i];
                }
                local_uids[unmatched] = ABS(local_uids[i]);
                unmatched++;
            }
        }

        numuid = unmatched;
        first_time = FALSE;

    } while (numuid > 0 && SelectNextDataSource()); /* until we've run out of options */

    MemFree (local_uids);
    MemFree (res);
    MemFree (map);

    return obtained_ids;
#endif
}

/*****************************************************************************
*
*   EntrezSeqEntryGet(uid, retcode)
*       get one SeqEntry
*
*****************************************************************************/
NLM_EXTERN SeqEntryPtr EntrezSeqEntryGet (Int4 uid, Int2 retcode)

{
    SeqEntryPtr sep = NULL;
    ErrStrId errorID;
    char errorString[32];
    
	if (! uid) return sep;
        sprintf(errorString, "(EntrezSeqEntryGet: %ld)",(long)uid);
        errorID = Nlm_ErrUserInstall(errorString, 0);
                
    EntrezSeqEntryListGet(&sep, 1, &uid, retcode, FALSE);
    Nlm_ErrUserDelete(errorID);
    return sep;
}

/*****************************************************************************
*
*   EntrezFindSeqId(sip)
*       given a Seq-id, get the uid.
*       returns 0 on failure
*
*****************************************************************************/
NLM_EXTERN Int4 LIBCALL EntrezFindSeqId (SeqIdPtr sip)
{
#ifdef _CDENTREZ_
  Int4 uid;
#endif

#ifdef _PMENTREZ_
  return PMEntrezSeqIdToGI(sip);
#else

	if (sip == NULL) return 0;
	switch (sip->choice)
	{
		case SEQID_NOT_SET:
		case SEQID_LOCAL:
		case SEQID_OTHER:
		case SEQID_GENERAL:
			return 0;
		default:
			break;
	}

    if (! DoSelectDataSourceByType(TYP_SEQ, "TERMS", NULL))
    {
        return 0;
    }

    switch (GetCurMediaType())
    {
#ifdef _CDENTREZ_
    case MEDIUM_CD:
    case MEDIUM_DISK:
        uid = CdEntrezFindSeqId(sip);
#ifdef USE_IDxARCH
        if (uid == 0 && ConnectID())
        {
 	    TextSeqIdPtr tsip;

            switch (sip->choice) {
            case SEQID_GENBANK:
            case SEQID_EMBL:
            case SEQID_PIR:
            case SEQID_SWISSPROT:
            case SEQID_DDBJ:
            case SEQID_PRF:
            case SEQID_OTHER:
                if ((tsip = (TextSeqIdPtr) sip->data.ptrvalue) != NULL)
                {
                    if (tsip->name != NULL)
                    {
                        StringUpper(tsip->name);
                    }
                    if (tsip->accession != NULL)
                    {
                        StringUpper(tsip->accession);
                    }
                    if (sip->choice == SEQID_PIR)
                    {
                        if (tsip->name == NULL)
                        { /* move accession to name */
                            tsip->accession = tsip->name;
                            tsip->name = NULL;
                        } else { /* destroy accession */
                            tsip->accession = (CharPtr) MemFree (tsip->accession);
                        }
                    } else {
                        if (tsip->name != NULL && tsip->accession != NULL)
                        {
                            tsip->name = (CharPtr) MemFree (tsip->name);
                        }
                    }
                }
                break;
            default:
                break;
            }
            uid = IDxArchGIGet(sip);
        }
#endif    
        return uid;
#endif
#ifdef _NETENTREZ_
    case MEDIUM_NETWORK:
        return NetEntrezFindSeqId(sip);
#endif    
    default:
        return 0;
    }
#endif /* _PMENTREZ_ */

}


/*****************************************************************************
*
*   EntrezSeqIdForGI(gi)
*
*****************************************************************************/
NLM_EXTERN SeqIdPtr LIBCALL EntrezSeqIdForGI (Int4 gi)
{
    SeqIdPtr sip = NULL;

#ifdef _PMENTREZ_
    return PMEntrezGIToSeqId(gi);
#else

    if (! gi) return sip;
    
    if (! DoSelectDataSourceByType(TYP_SEQ, "DOCSUMS", NULL))
    { /* ERROR, data unobtainable */
        return NULL;
    }

    do {
        switch (GetCurMediaType())
        {
#ifdef _CDENTREZ_
        case MEDIUM_CD:
        case MEDIUM_DISK:
            if ((sip = CdSeqIdForGI (gi)) != NULL)
                return sip;
            break;
#endif
#ifdef _NETENTREZ_
        case MEDIUM_NETWORK:
            if ((sip = NetSeqIdForGI (gi)) != NULL)
                return sip;
            break;
#endif
        default:
            break;
        }
    } while (SelectNextDataSource());

    return sip;

#endif /* _PMENTREZ_ */
}


/*****************************************************************************
*
*   EntrezMlSumListGet (result, numuid, uids)
*       returns a count of entries read
*       head of linked list is in result
*
*****************************************************************************/
NLM_EXTERN Int2 LIBCALL EntrezMlSumListGet (DocSumPtr PNTR result, Int2 numuid, Int4Ptr uids)

{
    Int2 obtained_ids = 0;
    Int4Ptr local_uids;
    Int2 unmatched;
    Int4Ptr map;
    Boolean first_time = TRUE;
    DocSumPtr PNTR res;
    Int2    i;

    if (numuid == 0)
        return 0;

    if (! DoSelectDataSource(ENTR_LINKS_CHAN, ENTR_REF_CHAN, ENTR_REF_CHAN))
    { /* ERROR, data unobtainable */
        return 0;
    }

    /* Iteratively try different data sources, working on an incrementally    */
    /* reduced number of UIDs, until either docsums have been found for all   */
    /* UIDs or,  all data sources have been exhausted. The algorithm uses a   */
    /* mapping mechanism to ensure that requests from a small subset of the   */
    /* initial requests always map their results back to the original result  */
    /* and uids vectors in a correct manner.                                  */

    local_uids = (Int4Ptr) MemDup(uids, numuid * sizeof(*uids));
    res = (DocSumPtr PNTR) MemNew(numuid * sizeof(DocSumPtr));
    map = (Int4Ptr) MemNew(numuid * sizeof(Int4));

    do {
        switch (GetCurMediaType())
        {
#ifdef _CDENTREZ_
        case MEDIUM_CD:
        case MEDIUM_DISK:
#ifdef _MBENTREZ_
            obtained_ids = MBEntMlSumListGet (res, numuid, local_uids);
            break;
#endif
#ifdef _PMENTREZ_
            obtained_ids = PMEntMlSumListGet (res, numuid, local_uids);
            break;
#else
            obtained_ids = CdEntMlSumListGet (res, numuid, local_uids);
            break;
#endif  /* _PMENTREZ_ */

#endif
#ifdef _NETENTREZ_
        case MEDIUM_NETWORK:
            obtained_ids = NetDocSumListGet (res, numuid, TYP_ML, local_uids, 0);
            break;
#endif
        default:
            break;
        }

        for (unmatched = 0, i = 0; i < numuid; i++)
        {
            if (res[i] != NULL) /* found this one */
            {
                if (first_time)
                {
                    result[i] = res[i];
                }
                else
                { /* map the location */
                    result[map[i]] = res[i];
                }
            }
            else { /* not found */
                if (first_time)
                {
                    result[i] = NULL;
                    map[unmatched] = i;
                }
                else {
                    map[unmatched] = map[i];
                }
                local_uids[unmatched] = local_uids[i];
                unmatched++;
            }
        }
    
        numuid = unmatched;
        first_time = FALSE;

    } while (numuid > 0 && SelectNextDataSource()); /* until we've run out of options */

    MemFree (local_uids);
    MemFree (res);
    MemFree (map);

    return obtained_ids;
}

/*****************************************************************************
*
*   EntrezSeqSumListGet (result, numuid, uids)
*       returns a count of entries read
*       head of linked list is in result
*
*****************************************************************************/
NLM_EXTERN Int2 LIBCALL EntrezSeqSumListGet (DocSumPtr PNTR result, Int2 numuid, DocType type, Int4Ptr uids)          /* Gi numbers */

{
    Int2      obtained_ids = 0;
    Int4Ptr   local_uids;
    Int2      unmatched;
    Int4Ptr   map;
    Boolean   first_time = TRUE;
    DocSumPtr PNTR res;
    Int2      i;

    if (numuid == 0)
        return 0;

    if (! DoSelectDataSource(ENTR_LINKS_CHAN, ENTR_SEQ_CHAN, ENTR_SEQ_CHAN))
    { /* ERROR, data unobtainable */
        return 0;
    }

    /* Iteratively try different data sources, working on an incrementally    */
    /* reduced number of UIDs, until either docsums have been found for all   */
    /* UIDs or,  all data sources have been exhausted. The algorithm uses a   */
    /* mapping mechanism to ensure that requests from a small subset of the   */
    /* initial requests always map their results back to the original result  */
    /* and uids vectors in a correct manner.                                  */

    local_uids = (Int4Ptr) MemDup(uids, numuid * sizeof(*uids));
    res = (DocSumPtr PNTR) MemNew(numuid * sizeof(DocSumPtr));
    map = (Int4Ptr) MemNew(numuid * sizeof(Int4));

    do {
        switch (GetCurMediaType())
        {
#ifdef _CDENTREZ_
        case MEDIUM_CD:
        case MEDIUM_DISK:
            obtained_ids = CdEntSeqSumListGet (res, numuid, type, local_uids);
        break;
#endif
#ifdef _NETENTREZ_
        case MEDIUM_NETWORK:
            obtained_ids = NetDocSumListGet (res, numuid, type, local_uids, 0);
            break;
#endif
        default:
            break;
        }

        for (unmatched = 0, i = 0; i < numuid; i++)
        {
#ifdef USE_IDxARCH
            if (res[i] == NULL) /* missing this one, try again */
	    {
                if ((res[i] = EntrezDocSum (type, local_uids [i])) != NULL)
                    obtained_ids++;
	    }
#endif /* USE_IDxARCH */
            if (res[i] != NULL) /* found this one */
            {
                if (first_time)
                {
                    result[i] = res[i];
                }
                else
                { /* map the location */
                    result[map[i]] = res[i];
                }
            }
            else { /* not found */
                if (first_time)
                {
                    result[i] = NULL;
                    map[unmatched] = i;
                }
                else {
                    map[unmatched] = map[i];
                }
                local_uids[unmatched] = local_uids[i];
                unmatched++;
            }
        }
    
        numuid = unmatched;
        first_time = FALSE;

    } while (numuid > 0 && SelectNextDataSource()); /* until we've run out of options */

    MemFree (local_uids);
    MemFree (res);
    MemFree (map);

    return obtained_ids;
}

typedef struct cachedParent {
    CharPtr term;
    CharPtr parent;
    DocType db;
    DocField fld;
} CachedParent, PNTR CachedParentPtr;

static ValNodePtr hierHead = NULL;

static void
FreeHierarchyList(void)
{
    ValNodePtr v;
    CachedParentPtr q;

    for (v = hierHead; v != NULL; v = v->next)
    {
        q = (CachedParentPtr) v->data.ptrvalue;
        MemFree(q->term);
        MemFree(q->parent);
        MemFree(q);
        v->data.ptrvalue = NULL;
    }
    hierHead = ValNodeFree(hierHead);
}

static EntrezHierarchyPtr DoEntrezHierarchyGet(CharPtr term, DocType db, DocField fld)
{
  switch (GetCurMediaType())
    {
#ifdef _CDENTREZ_
    case MEDIUM_CD:
    case MEDIUM_DISK:
#ifdef _MBENTREZ_
      return NULL;
#endif
      return NULL;
#endif
      
#ifdef _NETENTREZ_
    case MEDIUM_NETWORK:
      return NetEntHierarchyGet(term,db,fld);
#endif
    default:
      return NULL;
    }
}


NLM_EXTERN EntrezHierarchyPtr LIBCALL EntrezHierarchyGet(CharPtr term, DocType db, DocField fld)
{
#ifdef _PMENTREZ_

  return PMEntrezHierarchyGet(term,db,fld);

#else

    /*
    if (term == NULL || (db != TYP_ML && fld != FLD_ORGN_HIER) ||
       (db == TYP_ML && fld != FLD_MESH_HIER))
    {
        return NULL;
    }
    */
  
    return DoEntrezHierarchyGet (term,db,fld);

#endif
}

NLM_EXTERN Boolean LIBCALL EntrezCanNeighborText (void)
{
    if (! SelectDataLinksByTypes(TYP_ML, TYP_ML))
    { /* ERROR, data unobtainable */
        return FALSE;
    }

    switch (GetCurMediaType())
    {
#ifdef _CDENTREZ_
    case MEDIUM_CD:
    case MEDIUM_DISK:
#ifdef _MBENTREZ_
        return TRUE;
#endif
        return FALSE;
#endif
#ifdef _NETENTREZ_
    case MEDIUM_NETWORK:
        return NetEntCanNeighborText();
#endif
    default:
        return FALSE;
    }
}


NLM_EXTERN LinkSetPtr LIBCALL EntrezDoNeighborText (EntrezNeighborTextPtr entp)
{
    if (! SelectDataLinksByTypes(TYP_ML, TYP_ML))
    { /* ERROR, data unobtainable */
        return NULL;
    }

    switch (GetCurMediaType()) {
#ifdef _CDENTREZ_
    case MEDIUM_CD:
    case MEDIUM_DISK:
#ifdef _MBENTREZ_
        return MBEntDoNeighborText(entp);
#endif
        return NULL;
#endif
#ifdef _NETENTREZ_
    case MEDIUM_NETWORK:
        return NetEntDoNeighborText(entp);
#endif
    default:
        return NULL;
    }
}


/**************************************************
*
*    EntrezNeighborTextNew()
*
**************************************************/

NLM_EXTERN EntrezNeighborTextPtr LIBCALL EntrezNeighborTextNew(void)
{
   EntrezNeighborTextPtr ptr = MemNew((size_t) sizeof(EntrezNeighborText));

   return ptr;

}


/**************************************************
*
*    EntrezNeighborTextFree()
*
**************************************************/

NLM_EXTERN EntrezNeighborTextPtr LIBCALL EntrezNeighborTextFree(EntrezNeighborTextPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> normalText);
   MemFree(ptr -> specialText);
   return MemFree(ptr);
}


NLM_EXTERN Boolean LIBCALL EntrezExpandedMedlineFeatures (void)
{

    if (! SelectDataLinksByTypes(TYP_ML, TYP_ML))
    { /* ERROR, data unobtainable */
        return FALSE;
    }

    switch (GetCurMediaType())
    {
#ifdef _CDENTREZ_
    case MEDIUM_CD:
    case MEDIUM_DISK:
#ifdef _MBENTREZ_
        return TRUE;
#endif
        return FALSE;
#endif
#ifdef _NETENTREZ_
    case MEDIUM_NETWORK:
        return NetEntExpandedMedlineFeatures();
#endif
    default:
        return FALSE;
    }
}



NLM_EXTERN EntrezHierarchyPtr LIBCALL EntrezHierarchyFree(EntrezHierarchyPtr ehp)
{
      Int2 i;

      if (ehp == NULL)
         return NULL;
      for (i = 0; i < ehp->numInLineage; i++)
         MemFree (ehp->lineage[i]);
      MemFree (ehp->lineage);
      for (i = 0; i < ehp->numChildren; i++)
      {
          MemFree (ehp->children[i].name);
      }
      MemFree (ehp->children);
      MemFree (ehp->term);
      MemFree (ehp->canonicalForm);
      MemFree (ehp);

      return NULL;
}

typedef struct entrezfetch {
        Uint1 EntrezBioseqFetchState;
        CharPtr EntrezBioseqFetchAppName;
        Boolean is_net;
		  Uint2 ctr;        /* counts iterations of enable disable */
} EntrezFetchStruct, PNTR EntrezFetchStructPtr;

typedef struct entrezfetchuserdata {
	Int4 gi;
	Int2 retcode;
} EntrezFetchUserData, PNTR EntrezFetchUserDataPtr;

#define EBFS_DISABLE 0     /* EntrezBioseqFetch not in use (default) */
#define EBFS_INIT    1     /* EntrezBioseqFetchEnable called, but not EntrezInit */
#define EBFS_READY   2     /* EntrezInit has been called */

static CharPtr procname[3] = {
	"EntrezBioseqFetch",
	"EntrezSeqIdForGI",
	"EntrezGIForSeqId" };

static Int2 LIBCALLBACK EntrezBioseqFetchFunc PROTO((Pointer data));
static Int2 LIBCALLBACK EntrezSeqIdForGIFunc PROTO((Pointer data));
static Int2 LIBCALLBACK EntrezGIForSeqIdFunc PROTO((Pointer data));

/*****************************************************************************
*
*   The Following two functions allow access by BioseqFetch using the
*   SeqMgr.  The application should call EntrezBioseqFetchEnable() at the start
*   of the application and EntrezBioseqFetchDisable() at the end; This
*   will make EntrezBioseqFetch() the "remote" access procedure for the
*   SeqMgr. EntrezInit() will only be called on the first fetch unless "now"
*   is true;
*   
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL EntrezBioseqFetchEnable(CharPtr progname, Boolean now)
{
        Boolean result;
        EntrezFetchStructPtr efsp;
        ObjMgrPtr omp;
        ObjMgrProcPtr ompp;

              /* check if already enabled ***/

        omp = ObjMgrGet();
        ompp = ObjMgrProcFind(omp, 0, procname[0], OMPROC_FETCH);
        if (ompp != NULL)   /* already initialized */
        		efsp = (EntrezFetchStructPtr)(ompp->procdata);
		  else
		  {
	         efsp = MemNew(sizeof(EntrezFetchStruct));
		      efsp->EntrezBioseqFetchAppName = StringSave(progname);

      		ObjMgrProcLoad(OMPROC_FETCH, procname[0], procname[0], OBJ_SEQID, 0,OBJ_BIOSEQ,0,
                        (Pointer)efsp, EntrezBioseqFetchFunc, PROC_PRIORITY_DEFAULT);

				ObjMgrProcLoad(OMPROC_FETCH, procname[1], procname[1], OBJ_SEQID, SEQID_GI,OBJ_SEQID,0,
                        (Pointer)efsp, EntrezSeqIdForGIFunc, PROC_PRIORITY_DEFAULT);

      		ObjMgrProcLoad(OMPROC_FETCH, procname[2], procname[2], OBJ_SEQID, 0,OBJ_SEQID,SEQID_GI,
                        (Pointer)efsp, EntrezGIForSeqIdFunc, PROC_PRIORITY_DEFAULT);
		  }

		  efsp->ctr++;    /* count number of enables */

		  if (efsp->EntrezBioseqFetchState == EBFS_READY)  /* nothing more to do */
			  return TRUE;

        if (now)
        {
                result = EntrezInit(progname, TRUE, &(efsp->is_net));
                if (! result)
                {
                        return result;
                }
                efsp->EntrezBioseqFetchState = EBFS_READY;
        }
        else
                efsp->EntrezBioseqFetchState = EBFS_INIT;


        return TRUE;

}

/*****************************************************************************
*
*   EntrezBioseqFetchDisable()
*
*****************************************************************************/
NLM_EXTERN void LIBCALL EntrezBioseqFetchDisable(void)
{
        ObjMgrPtr omp;
        ObjMgrProcPtr ompp;
        EntrezFetchStructPtr efsp;

        omp = ObjMgrGet();
        ompp = ObjMgrProcFind(omp, 0, procname[0], OMPROC_FETCH);
        if (ompp == NULL)   /* not initialized */
                return;

        efsp = (EntrezFetchStructPtr)(ompp->procdata);
		  if (! efsp->ctr)   /* no enables active */
			  return;

		  efsp->ctr--;
		  if (efsp->ctr)   /* connection still pending */
			  return;

        if (efsp->EntrezBioseqFetchState == EBFS_READY)
                EntrezFini();

		  efsp->EntrezBioseqFetchState = EBFS_DISABLE;  /* not active */

        return;
}

/*****************************************************************************
*
*   EntrezFetchFreeFunc(ptr)
*     removes EntrezFetchUserData
*
*****************************************************************************/
static Pointer LIBCALLBACK EntrezFetchFreeFunc (Pointer ptr)
{
	EntrezFetchUserDataPtr efudp;

	efudp = (EntrezFetchUserDataPtr)ptr;
	return MemFree(efudp);
}

/*****************************************************************************
*
*   EntrezBioseqFetchFunc(data)
*       callback for EntrezBioseqFetch
*
*****************************************************************************/
static Int2 LIBCALLBACK EntrezBioseqFetchFunc (Pointer data)
{
        OMProcControlPtr ompcp;
        ObjMgrProcPtr ompp;
        EntrezFetchStructPtr efsp;
        SeqIdPtr sip;
        OMUserDataPtr omdp;
        Boolean result;
        BioseqPtr bsp = NULL;
        Int4 gi=0;
        SeqEntryPtr sep;
	/* ValNode fake_seqid; */
			EntrezFetchUserDataPtr efudp;
			Int2 retcode=0;

        ompcp = (OMProcControlPtr)data;
        ompp = ompcp->proc;
        efsp = (EntrezFetchStructPtr)(ompp->procdata);

		  if (efsp->EntrezBioseqFetchState == EBFS_DISABLE)  /* shut off */
		  {
		  	    return OM_MSG_RET_OK;    /* not done, go on to next */
		  }
        else if (efsp->EntrezBioseqFetchState == EBFS_INIT)
        {
                result = EntrezInit(efsp->EntrezBioseqFetchAppName, TRUE, &(efsp->is_net));
                if (! result)
                    return OM_MSG_RET_ERROR;
                efsp->EntrezBioseqFetchState = EBFS_READY;
        }

        if (efsp->EntrezBioseqFetchState != EBFS_READY)
                return OM_MSG_RET_ERROR;

                                        /* check for cached gi */
        if (ompcp->input_entityID)
        {
                omdp = ObjMgrGetUserData(ompcp->input_entityID,ompp->procid, OMPROC_FETCH, 0);
                if (omdp != NULL)
					 {
						efudp = (EntrezFetchUserDataPtr)(omdp->userdata.ptrvalue);
						if (efudp != NULL)
						{
							gi = efudp->gi;
							retcode = efudp->retcode;
						}
					}
        }

        if (! gi)           /* not cached, try input id */
        {
                sip = (SeqIdPtr)(ompcp->input_data);
                if (sip == NULL)
                        return OM_MSG_RET_ERROR;

                if (sip->choice != SEQID_GI)
					 {
						   gi = GetUniGeneIDForSeqId(sip);  /* UniGene? */
							if (gi)                          /* yes */
								retcode = -1;
							else
                        gi = EntrezFindSeqId(sip);
					 }
                else
                        gi = sip->data.intvalue;
        }

        if (! gi) return OM_MSG_RET_OK;  /* no error but call next proc */

        sep = EntrezSeqEntryGet(gi, retcode);
        if (sep == NULL) return OM_MSG_RET_ERROR;

	    /* if (retcode >= 0)
		{
			fake_seqid.choice = SEQID_GI;
			fake_seqid.data.intvalue = gi;
			fake_seqid.next = NULL;
			sip = &fake_seqid;
		} */

        sip = (SeqIdPtr)(ompcp->input_data);
        bsp = BioseqFindInSeqEntry(sip, sep);
        ompcp->output_data = (Pointer)bsp;
        ompcp->output_entityID = ObjMgrGetEntityIDForChoice(sep);

                  /* store the cache info */
        omdp = ObjMgrAddUserData(ompcp->output_entityID, ompp->procid, OMPROC_FETCH, 0);
		  efudp = (EntrezFetchUserDataPtr)MemNew(sizeof(EntrezFetchUserData));
        omdp->userdata.ptrvalue = efudp;
		  efudp->gi = gi;
		  efudp->retcode = retcode;
		  omdp->freefunc = EntrezFetchFreeFunc;

        return OM_MSG_RET_DONE;   /* data found, don't call further functions */
}

/*****************************************************************************
*
*   EntrezSeqIdForGIFunc(data)
*       callback for EntrezSeqIdForGI
*
*****************************************************************************/
static Int2 LIBCALLBACK EntrezSeqIdForGIFunc (Pointer data)
{
        OMProcControlPtr ompcp;
        ObjMgrProcPtr ompp;
        EntrezFetchStructPtr efsp;
        SeqIdPtr sip, sip2;
		  Boolean result;

        ompcp = (OMProcControlPtr)data;
        ompp = ompcp->proc;
        efsp = (EntrezFetchStructPtr)(ompp->procdata);

		  if (efsp->EntrezBioseqFetchState == EBFS_DISABLE)  /* shut off */
		  {
		  	    return OM_MSG_RET_OK;    /* not done, go on to next */
		  }
        else if (efsp->EntrezBioseqFetchState == EBFS_INIT)
        {
                result = EntrezInit(efsp->EntrezBioseqFetchAppName, TRUE, &(efsp->is_net));
                if (! result)
                    return OM_MSG_RET_ERROR;
                efsp->EntrezBioseqFetchState = EBFS_READY;
        }

        if (efsp->EntrezBioseqFetchState != EBFS_READY)
                return OM_MSG_RET_ERROR;

        sip = (SeqIdPtr)(ompcp->input_data);
        if (sip == NULL)
                return OM_MSG_RET_ERROR;
		  if (sip->choice != SEQID_GI)
					 return OM_MSG_RET_ERROR;

        sip2 = EntrezSeqIdForGI(sip->data.intvalue);
        if (sip2 == NULL) return OM_MSG_RET_OK;

        ompcp->output_data = (Pointer)sip2;
							/* not registering right now */

        return OM_MSG_RET_DONE;   /* data found, don't call further functions */
}

/*****************************************************************************
*
*   EntrezGIForSeqIdFunc(data)
*       callback for EntrezGIForSeqId
*
*****************************************************************************/
static Int2 LIBCALLBACK EntrezGIForSeqIdFunc (Pointer data)
{
        OMProcControlPtr ompcp;
        ObjMgrProcPtr ompp;
        EntrezFetchStructPtr efsp;
        SeqIdPtr sip, sip2;
		  Int4 gi;
		  Boolean result;

        ompcp = (OMProcControlPtr)data;
		if (ompcp == NULL)
                return OM_MSG_RET_ERROR;

        sip = (SeqIdPtr)(ompcp->input_data);
        if (sip == NULL)
            return OM_MSG_RET_ERROR;

		switch (sip->choice) {
			case SEQID_NOT_SET:
			case SEQID_LOCAL:
			case SEQID_OTHER:
			case SEQID_GENERAL:
			case SEQID_GI:
				return OM_MSG_RET_ERROR;
			default:
				break;
		}

        ompp = ompcp->proc;
        efsp = (EntrezFetchStructPtr)(ompp->procdata);

		  if (efsp->EntrezBioseqFetchState == EBFS_DISABLE)  /* shut off */
		  {
		  	    return OM_MSG_RET_OK;    /* not done, go on to next */
		  }
        else if (efsp->EntrezBioseqFetchState == EBFS_INIT)
        {
                result = EntrezInit(efsp->EntrezBioseqFetchAppName, TRUE, &(efsp->is_net));
                if (! result)
                    return OM_MSG_RET_ERROR;
                efsp->EntrezBioseqFetchState = EBFS_READY;
        }

        if (efsp->EntrezBioseqFetchState != EBFS_READY)
                return OM_MSG_RET_ERROR;

		  gi = EntrezFindSeqId(sip);
        if (! gi) return OM_MSG_RET_OK;

		  sip2 = ValNodeNew(NULL);
		  sip2->choice = SEQID_GI;
		  sip2->data.intvalue = gi;

        ompcp->output_data = (Pointer)sip2;
							/* not registering right now */

        return OM_MSG_RET_DONE;   /* data found, don't call further functions */
}

NLM_EXTERN Boolean LIBCALL EntrezCanBlast(void)
{
    if (! SelectDataLinksByTypes(TYP_AA, TYP_AA))
    { /* ERROR, data unobtainable */
        return FALSE;
    }

    switch (GetCurMediaType())
    {
#ifdef _CDENTREZ_
    case MEDIUM_CD:
    case MEDIUM_DISK:
        return FALSE;
#endif
#ifdef _NETENTREZ_
    case MEDIUM_NETWORK:
        return NetEntCanBlast();
#endif
    default:
        return FALSE;
    }
}

NLM_EXTERN LinkSetPtr LIBCALL EntrezBlastBioseq(BioseqPtr bsp, DocType db, CharPtr program, CharPtr database, CharPtr options, Boolean usemonitor)
{
    if (! SelectDataLinksByTypes(db, db))
    { /* ERROR, data unobtainable */
        return NULL;
    }

    switch (GetCurMediaType()) {
#ifdef _CDENTREZ_
    case MEDIUM_CD:
    case MEDIUM_DISK:
        return NULL;
#endif
#ifdef _NETENTREZ_
    case MEDIUM_NETWORK:
        return NetEntBlastBioseq(bsp, db, program, database, options, usemonitor);
#endif
    default:
        return NULL;
    }
}

NLM_EXTERN Int4 LIBCALL EntrezClusterAnalysis(DocUidPtr uids, Int4 numuids, DocField fld, Int4 minCluster, Int4 maxCluster, Int4 maxTerms, CharPtr *terms, Int4Ptr termTotals)
{

    if (! SelectDataLinksByTypes(TYP_ML, TYP_ML))
    { /* ERROR, data unobtainable */
        return FALSE;
    }

    switch (GetCurMediaType())
    {
#ifdef _CDENTREZ_
    case MEDIUM_CD:
    case MEDIUM_DISK:
#ifdef _MBENTREZ_
        return MBEntrezClusterAnalysis(uids, numuids, fld, minCluster, maxCluster, maxTerms, terms, termTotals);
#endif
        return -1;
#endif
#ifdef _NETENTREZ_
    case MEDIUM_NETWORK:
        return NetEntClusterAnalysis(uids, numuids, fld, minCluster, maxCluster, maxTerms, terms, termTotals);
#endif
    default:
        return -1;
    }
}


