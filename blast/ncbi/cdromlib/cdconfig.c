/* 
 * ===========================================================================
 *
 *                             COPYRIGHT NOTICE
 *               National Center for Biotechnology Information
 *
 *  This software/database is a "United States Government Work" under the
 *  terms of the United States Copyright Act.  It was written as part of
 *  the author's official duties as a Government employee and thus cannot
 *  be copyrighted.  This software/database is freely available to the
 *  public for use without a copyright notice.  Restrictions cannot be
 *  placed on its present or future use.
 *
 *  Although all reasonable efforts have been taken to ensure the accuracy
 *  and reliability of the software and data, the National Library of
 *  Medicine (NLM) and the U. S. Government do not and cannot warrant the
 *  performance or results that may be obtained by using this software or
 *  data.  The NLM and the U. S. Government disclaim all warranties as to
 *  performance, merchantability or fitness for any particular purpose.
 *
 *  Please see that the author is suitably cited in any work or product
 *  based on this material.
 *
 * ===========================================================================
 *
 * RCS $Id: cdconfig.c,v 6.1 1998/12/04 23:35:07 kans Exp $
 *
 * Author:   Jonathan Epstein
 * 
 * Version Creation Date:   11-92
 *
 * File Description: 
 *  Complex configuration mechanism used in conjunction with the low-level
 *  data access library for the Molecular Sequence Data CD-ROM library produced 
 *  by the NCBI.
 *
 * Modifications:  
 * --------------------------------------------------------------------------
 * Rev   Date      Name      Description of modification
 * ----  --------  --------  ------------------------------------------------
 * 1.17  08-22-94  Schuler   Fixed one function not returning a value
 *
 *
 * 05-19-95 Schuler     Added rcs Log directive for automatic insertion of
 *                      modification comments.
 *
 * Revision $Log: cdconfig.c,v $
 * Revision Revision 6.1  1998/12/04 23:35:07  kans
 * Revision KludgeCdromlibGetAppParam supplies ncbi config values for psrventr
 * Revision
 * Revision Revision 6.0  1997/08/25 18:12:45  madden
 * Revision Revision changed to 6.0
 * Revision
 * Revision Revision 5.1  1997/06/26 21:55:25  vakatov
 * Revision [PC] DLL'd "ncbicdr.lib", "ncbiacc.lib", "ncbinacc.lib" and "ncbicacc.lib"
 * Revision
 * Revision Revision 5.0  1996/05/28 13:55:34  ostell
 * Revision Set to revision 5.0
 * Revision
 * Revision 4.1  1995/08/24  20:44:01  epstein
 * add more stuff for genomes
 *
 * Revision 4.0  1995/07/26  13:50:32  ostell
 * force revision to 4.0
 *
 * Revision 1.22  1995/05/16  14:36:20  schuler
 * Automatic comment insertion enabled
 *
 *
 * ==========================================================================
 */

#define REVISION_STR  "$Revision: 6.1 $"

#include <ncbi.h>
#include <cdromlib.h>

/*****************************************************************************
*
*   Kludge to eliminate need for old entries in ncbi config file
*
*****************************************************************************/

static CharPtr kludgecdromstrings [] = {
  "ENTR_LINK.CHANNELS", "LINKS_FROM_NET",
  "ENTR_REF.CHANNELS", "REFERENCE_FROM_NET",
  "ENTR_SEQ.CHANNELS", "SEQUENCE_FROM_NET",
  "ENTREZ_NET.FORMAL_NAME", "",
  "ENTREZ_NET.RESOURCE_NAME", "Entrez",
  "ENTREZ_NET.RESOURCE_TYPE", "Entrez",
  "ENTREZ_NET.SERVICE_NAME", "Entrez",
  "ENTREZ_NET.TYPE", "NET",
  "LINKS_FROM_NET.ENTR_REF__ENTR_REF", "1",
  "LINKS_FROM_NET.ENTR_REF__ENTR_SEQ", "1",
  "LINKS_FROM_NET.ENTR_SEQ__ENTR_REF", "1",
  "LINKS_FROM_NET.ENTR_SEQ__ENTR_SEQ", "1",
  "LINKS_FROM_NET.INFO", "1",
  "LINKS_FROM_NET.MEDIA", "ENTREZ_NET",
  "NCBI.MEDIA", "ENTREZ_NET",
  "REFERENCE_FROM_NET.BOOLEANS", "1",
  "REFERENCE_FROM_NET.DOCSUMS", "1",
  "REFERENCE_FROM_NET.MEDIA", "ENTREZ_NET",
  "REFERENCE_FROM_NET.RECORDS", "1",
  "REFERENCE_FROM_NET.TERMS", "1",
  "SEQUENCE_FROM_NET.BOOLEANS", "1",
  "SEQUENCE_FROM_NET.DOCSUMS", "1",
  "SEQUENCE_FROM_NET.MEDIA", "ENTREZ_NET",
  "SEQUENCE_FROM_NET.RECORDS", "1",
  "SEQUENCE_FROM_NET.TERMS", "1",
  NULL, NULL
};

/*
#include <stdarg.h>
void CDECL   Nlm_WriteLog VPROTO((char *format, ...));
*/

static Int2 KludgeCdromlibGetAppParam (CharPtr file, CharPtr section, CharPtr type,
                                       CharPtr dflt, CharPtr buf, Int2 buflen)

{
  Int2  i;
  Char  str [256];

  if (buf == NULL || buflen < 1) return 0;
  *buf = '\0';
  if (dflt != NULL) {
    StringNCpy_0 (buf, dflt, buflen);
  }
  if (StringICmp (file, "NCBI") != 0) return 0;
  if (StringLen (section) > 127 || StringLen (type) > 127) return 0;
  StringCpy (str, section);
  StringCat (str, ".");
  StringCat (str, type);
  for (i = 0; kludgecdromstrings [i] != NULL; i += 2) {
    if (StringICmp (kludgecdromstrings [i], str) == 0) {
      StringNCpy_0 (buf, kludgecdromstrings [i + 1], buflen);
      return StringLen (buf);
    }
  }
  /*
  GetAppParam (file, section, type, dflt, buf, buflen);
  Nlm_WriteLog ("  \"%s.%s\", \"%s\",\n", section, type, buf);
  */
  return StringLen (buf);
}

/*  =========================================================================
 *      VARIABLES
 */


static MediaPtr CurMedia = NULL;
static MediaPtr LRUMediaList = NULL;

static MediaPtr MediaList = NULL;
static DrasticActionsPtr DrasticList = NULL;
static DataSourceDbPtr DataSourceDbList = NULL;
static DataSourceDbPtr CurDataSourceDb = NULL;
static int numcfginits = 0;
static EntrezInfoPtr MergedEntrezInfo = NULL;
static Boolean SoleMedia = FALSE;

static char * sCdConfError[] =
{
#ifdef WIN_MSWIN
    "No data source %Fs for section %Fs field %Fs"
#else
    "No data source %s for section %s field %s"
#endif
};

int _nouveau;


/*****************************************************************************
*
*   Private Function Prototypes
*
*****************************************************************************/

static void     FreeConfig                PROTO((void));
static Boolean  ChangeMedia               PROTO((MediaPtr newmedia));
static Boolean  IsDrasticAction           PROTO((MediaPtr fromMedia,
                                                MediaPtr toMedia));
static DataSourceDbPtr LoadDataSourceInfo PROTO((CharPtr section,
                                              CharPtr field1, CharPtr field2));
static void     MarkMRU                   PROTO((MediaPtr m));
static MediaPtr FindLRU                   PROTO((void));
static void     MarkLRUineligible         PROTO((MediaPtr media));
static void     UnMarkLRUineligible       PROTO((void));
static void     InitLRUeligibility        PROTO((Int2 mask));
static Boolean  AllCdDrivesAreFull        PROTO((void));
static Boolean  IsLiveMedia               PROTO((MediaPtr media));



/***************************************************************************
*
*   ConfigInit()
*
*
***************************************************************************/

NLM_EXTERN void ConfigInit(void)
{
    numcfginits++;
}

/***************************************************************************
*
*   ConfigFini()
*
*
***************************************************************************/

NLM_EXTERN void ConfigFini(void)
{
    if (numcfginits > 0)
        numcfginits--;

    if (numcfginits == 0)
        FreeConfig();
}

/***************************************************************************
*
*   LoadDataSourceInfo()
*
*   Called to load from the config file the information for a given
*   section/field1/field2 combination, e.g., "ENTR_LINK"/"ENTR_REF"/"ENTR_SEQ",
*   or "ENTR_REF"/"DOCSUMS"/NULL.
*
*   The resulting data is stored as a linked list, sorted by decreasing
*   priority. These "DataSourceDb" lists in turn are stored, in no particular
*   order, in a master list, headed by DataSourceDbList.
*
***************************************************************************/

static DataSourceDbPtr LoadDataSourceInfo (CharPtr section, CharPtr field1,
                                           CharPtr field2)
{
    DataSourceDbPtr dsdp;
    DataSourcePtr   dsp;
    char            channels[100];
    CharPtr         d1;
    CharPtr         d2;
    CharPtr         d3;
    CharPtr         d4;
    CharPtr         word;
    CharPtr         channel;
    Boolean         no_drastic_action;
    Boolean         done;
    DataSourcePtr   trailptr;
    char            buf[100];
    Int2            priority;
    MediaPtr        media;
    char            field[100];
    char            priostr[20];

    /* search the existing "data source database" to see if this entry is    */
    /* already loaded                                                        */
    for (dsdp = DataSourceDbList; dsdp != NULL; dsdp = dsdp->next)
    {
        if ((section != NULL || dsdp->key.section != NULL) &&
            StringCmp(section, dsdp->key.section) != 0)
            continue;
        if ((field1 != NULL || dsdp->key.field1 != NULL) &&
            StringCmp(field1, dsdp->key.field1) != 0)
            continue;
        if ((field2 != NULL || dsdp->key.field2 != NULL) &&
            StringCmp(field2, dsdp->key.field2) != 0)
            continue;
        
        /* already loaded */
        return dsdp;
    }

    if (KludgeCdromlibGetAppParam("NCBI", section, "CHANNELS", "", channels, 
                        sizeof(channels)) == 0)
    {
        return NULL; /* can't find CHANNELS keyword */
    }

    /* insert at head of DB list (for simplicity) */
    dsdp = (DataSourceDbPtr) MemNew(sizeof(DataSourceDb));
    dsdp->next = DataSourceDbList;
    DataSourceDbList = dsdp;

    dsdp->key.section = section == NULL ? NULL : StringSave(section);
    dsdp->key.field1 = field1 == NULL ? NULL : StringSave(field1);

    StrCpy(field, field1);
    if (field2 == NULL)
    { /* normal case */
        dsdp->key.field2 = NULL;
    }
    else { /* contraction of two fields */
        dsdp->key.field2 = StringSave(field2);
        StrCat(field, "__");
        StrCat(field, field2);
    }

    dsdp->list = NULL;

    d2 = channels;
    StrCat (d2, "\n");
    done = FALSE;

    /* Note that we don't use strtok() because it's not re-entrant */
    while ((d1 = StringPBrk(d2, ",\n\r")) != NULL && !done)
    {
        if (*d1 == '\n' || *d1 == '\r' || *d1 == '\0')
            done = TRUE;
        *d1 = '\0';
        word = d2;
        d2 = d1 + 1;
        d2 += StringSpn(d2, " \t"); /* remove leading spaces and tabs from next word */
        KludgeCdromlibGetAppParam("NCBI", word, field, field2 == NULL ? "1" : "0", buf, sizeof(buf));
        no_drastic_action = FALSE;
        
        /* Fields with field2 != NULL represent an NxN matrix of values; therefore,  */
        /* we default the priorities to 0, to avoid having to declare all NxN values */
        /* in the configuration file.                                                */
        if (field2 == NULL)
            priority = 1;
        else
            priority = 0;
        channel = StringSave(word);

        /* Parse the field which was just read, which contains a priority    */
        /* and may contain some other things as well.                        */
        d4 = buf;
        StrCat (d4, "\n");
        if ((d3 = StringPBrk(d4, ",\n\r")) != NULL)
        {
            *d3 = '\0';
            word = d4;
            d4 = d3 + 1;
            StrCpy(priostr, word); /* workaround for Windows */
            priority = atoi(priostr);
            d4 += StringSpn(d4, " \t"); /* remove leading spaces and tabs from next word */
            if (StrCmp(d4, "NO_DRASTIC_ACTION\n") == 0)
                no_drastic_action = TRUE;
        }
            
        if (KludgeCdromlibGetAppParam("NCBI", channel, "MEDIA", "", buf, sizeof(buf)) == 0)
        {
            return NULL; /* media not specified */
        }
        media = PreInitMedia(buf);

        /* override kludge for MEDIA; this enables a caller to make a     */
        /* particular CHANNEL active, without needing to know any of the  */
        /* names of the fields in that channel, except for "MEDIA"        */
        if (StrCmp(field, "MEDIA") == 0)
        {
            /* the really kludgy part of the kludge is to give preference to */
            /* network-based media; there's no good reason for this ... it   */
            /* just makes it possible to run the multi-CD/network combo      */
            /* version without ejecting CD-ROMs upon startup                 */
            if (media->media_type == MEDIUM_NETWORK)
                priority = 1;
            else
                priority = 0;
        }

        dsp = (DataSourcePtr) MemNew(sizeof(DataSource));

        /* order list by decreasing priorities */
        dsp->next = NULL;
        if (dsdp->list == NULL)
            dsdp->list = dsp;
        else {
            if (dsdp->list->next == NULL) /* one-element list */
            {
                if (dsdp->list->priority < priority)
                {
                    dsp->next = dsdp->list;
                    dsdp->list = dsp;
                }
                else { /* inserted element had lower priority; place at end of list */
                    dsdp->list->next = dsp;
                }
            }
            else {
                /* The "trailptr" points one element behind the element       */
                /* which is being examined, so that the new element can be    */
                /* inserted BEFORE the element with lower priority            */
                for (trailptr = dsdp->list; trailptr->next != NULL;
                     trailptr = trailptr->next)
                {
                    if (trailptr->next->priority < priority)
                    {
                        dsp->next = trailptr->next;
                        trailptr->next = dsp;
                        break;
                    }
                }
                if (trailptr->next == NULL) /* lowest priority in list */
                { /* insert at end */
                    trailptr->next = dsp;
                }
                    
            }
        }

        dsp->channel = channel;
        dsp->priority = priority;
        dsp->no_drastic_action = no_drastic_action;
        dsp->media = media;
    }

    return dsdp;
}

    
/***************************************************************************
*
*   SetSoleMedia()
*
*   For backwards-compatability with "old-style" configuration files,
*   we need the ability to specify a "sole media". A sole-media, by implication,
*   is the "valid" medium for all requested data sources, and should never
*   be ejected (if it is a CD-ROM, which is likely).
*
***************************************************************************/

NLM_EXTERN void SetSoleMedia(void)
{
    SoleMedia = TRUE;
}


/***************************************************************************
*
*   SelectDataSource()
*
*   Find a "data source" for the specified section/field1/field2 combination,
*   e.g., "ENTR_LINK"/"ENTR_REF"/"ENTR_SEQ", or "ENTR_REF"/"DOCSUMS"/NULL.
*
*   Returns TRUE if we are able to satisfy the request.
*
***************************************************************************/

NLM_EXTERN Boolean SelectDataSource (CharPtr section, CharPtr field1, CharPtr field2)
{
    DataSourceDbPtr dsdp;
    DataSourcePtr dsp;
    
#ifdef _NEW_CdEntrez_
	if (_nouveau)
		return TRUE;    /***** KLUDGE *****/
#endif

    /* for backwards compatability with "old-style" config files */
    if (SoleMedia)
    { /* use a single medium, rather than traversing a list */
        CurDataSourceDb = NULL;
        if (CurMedia == NULL)
        { /* no media yet initialized; so initialize it */
            if (MediaList != NULL)
            { /* MediaList points to single media entry */
                return ChangeMedia(MediaList);
            }
            return FALSE;
        }
        else { /* already set-up */
            return TRUE;
        }
    }

    if ((dsdp = LoadDataSourceInfo(section, field1, field2)) == NULL)
    {
        ErrPost (CTX_NCBICD, ERR_CD_NO_DATASRC, sCdConfError[0], "", section,
                 field1);
        CurDataSourceDb = NULL;
        return FALSE;
    }

    CurDataSourceDb = dsdp;

    if (dsdp->list == NULL)
    {
        ErrPost (CTX_NCBICD, ERR_CD_NO_DATASRC, sCdConfError[0], "list ",
                 section, field1);
        return FALSE;
    }

    /* Mark all entries as being "not yet searched", except for "invalid" */
    /* entries                                                            */
    for (dsp = dsdp->list; dsp != NULL; dsp = dsp->next)
    {
        dsp->already_searched = dsp->media == NULL || dsp->media->invalid;
    }

    return SelectNextDataSource();
}

/***************************************************************************
*
*   SelectDataSourceByType()
*                  *   A short-hand mechanism for selecting the appropriate SelectDataSource()
*   section.
*
****************************************************************************/

NLM_EXTERN Boolean SelectDataSourceByType (DocType type, CharPtr field1, CharPtr field2)
{
    CharPtr section;

#ifdef _NEW_CdEntrez_
	if (_nouveau)
		return TRUE;    /***** KLUDGE *****/
#endif

    switch (type) {
    case TYP_AA:
    case TYP_NT:
    case TYP_SEQ:
    case TYP_ST:
    case TYP_CH:
        section = ENTR_SEQ_CHAN;
        break;
    case TYP_ML:
        section = ENTR_REF_CHAN;
        break;
    default:
        return FALSE;
    }

    return SelectDataSource(section, field1, field2);
}

/***************************************************************************
*
*   SelectDataSourceByType()
*
*   A short-hand mechanism for specifying the SelectDataSource() arguments
*   for the "ENTR_LINK" section. Note that entries in the "ENTR_LINK" section
*   are specified by the "from-type" name, followed by two underscores, followed
*   by the "to-type" name.
*
****************************************************************************/

NLM_EXTERN Boolean SelectDataLinksByTypes (DocType type, DocType link_to_type)
{
    CharPtr field1;
    CharPtr field2;

#ifdef _NEW_CdEntrez_
	if (_nouveau)
		return TRUE;    /***** KLUDGE *****/
#endif

    switch (type) {
    case TYP_AA:
    case TYP_NT:
    case TYP_SEQ:
    case TYP_ST:
    case TYP_CH:
        field1 = ENTR_SEQ_CHAN;
        break;
    case TYP_ML:
        field1 = ENTR_REF_CHAN;
        break;
    default:
        return FALSE;
    }

    switch (link_to_type) {
    case TYP_AA:
    case TYP_NT:
    case TYP_SEQ:
    case TYP_ST:
    case TYP_CH:
        field2 = ENTR_SEQ_CHAN;
        break;
    case TYP_ML:
        field2 = ENTR_REF_CHAN;
        break;
    default:
        return FALSE;
    }

    return SelectDataSource(ENTR_LINKS_CHAN, field1, field2);
}

/***************************************************************************
*
*   SelectNextDataSource()
*
*   Called subsequent to SelectDataSource(), to find the next {or first,
*   when called by SelectDataSource()} available data source for the
*   previously selected section/field1/field2 combination.
*
****************************************************************************/

NLM_EXTERN Boolean SelectNextDataSource (void)
{
    DataSourceDbPtr dsdp = CurDataSourceDb;
    DataSourcePtr dsp;
    DataSourcePtr best_entry;
    DataSourcePtr best_on_live_media;
    
#ifdef _NEW_CdEntrez_
	if (_nouveau)
		return FALSE;    /***** KLUDGE *****/
#endif

    if (dsdp == NULL)
    { /* no current data source "database" */
        return FALSE;
    }

    /* loop forever (until return) */
    while (TRUE)
    {
        best_on_live_media = NULL;
        
        /* Try to find an entry associated with the current media, provided  */
        /* the current media is a live media (select the one which appears   */
        /* first in the list, and thus has the highest priority              */
        for (dsp = dsdp->list; dsp != NULL; dsp = dsp->next)
        {
            if (dsp->priority > 0 && !dsp->already_searched &&
                dsp->media == CurMedia && IsLiveMedia(dsp->media))
            {
                best_on_live_media = dsp;
                break;
            }
        }
    
        /* Try to find an entry associated with a live media (select         */
        /* the one which appears first in the list, and thus has the highest */
        /* priority).                                                        */
        for (dsp = dsdp->list; best_on_live_media == NULL && dsp != NULL;
             dsp = dsp->next)
        {
            if (dsp->priority > 0 && !dsp->already_searched &&
                IsLiveMedia(dsp->media))
            {
                best_on_live_media = dsp;
                break;
            }
        }
    
        /* Search for an entry with a higher priority than that associated   */
        /* with best_on_live_media, for which drastic-actions are not         */
        /* disallowed (unless switching to that media is not a "drastic      */
        /* action")                                                          */
        best_entry = best_on_live_media;

        for (dsp = dsdp->list; dsp != NULL && dsp->priority > 0; dsp = dsp->next)
        {
            if (dsp->already_searched)
                continue;
    
            if (best_on_live_media != NULL &&
                dsp->priority <= best_on_live_media->priority)
            {
                break; /* nothing found better than the "live" media */
            }
    
            if (!(dsp->no_drastic_action) || best_on_live_media == NULL ||
                !IsDrasticAction(CurMedia, dsp->media))
            { /* found a better medium */
                best_entry = dsp;
                break;
            }
        }
    
        if (best_entry == NULL)
        {
            return FALSE;
        }
    
        for (dsp = dsdp->list; dsp != NULL; dsp = dsp->next)
        { /* mark all entries associated with this media as "already searched" */
            if (dsp->media == best_entry->media)
                dsp->already_searched = TRUE;
        }
    
        if (best_entry->media != CurMedia)
        {
            if (ChangeMedia (best_entry->media))
                return TRUE;
        }
        else { /* the current Media was fine */
            return TRUE;
        }
    }
}

/***************************************************************************
*
*   FreeConfig()
*
*   Free and reset all configuration data. This should be used sparingly
*   since it resets all configuration information gathered to date.
*
****************************************************************************/

static void FreeConfig(void)
{
    MediaPtr mp, nextmp;
    DataSourceDbPtr dsdp, nextdsdp;
    DataSourcePtr dsp, nextdsp;
    DrasticActionsPtr dap, nextdap;
    CdMediaInfoPtr cmip;
    Int2 i;

    /* Free all the dynamically allocated lists of config data */

    for (mp = MediaList; mp != NULL; mp = nextmp)
    {
        MemFree(mp->media_alias);
        MemFree(mp->formal_name);

        if (mp != CurMedia)
        { /* CdFini()/NetFini() will take care of this for us for CurMedia */
            EntrezInfoFree(mp->entrez_info);
        }

        switch(mp->media_type) {
        case MEDIUM_CD:
        case MEDIUM_DISK:
            cmip = (CdMediaInfoPtr) mp->media_info;

            if (cmip == NULL)
                break;
            if (cmip->device_name != NULL)
                MemFree (cmip->device_name);
            if (cmip->mount_point != NULL)
                MemFree (cmip->mount_point);
            if (cmip->mount_cmd != NULL)
                MemFree (cmip->mount_cmd);
            if (mp != CurMedia)
            { /* CdFini() will take care of this for us for CurMedia */
                for (i = 0; i < NDIR; i++)
                {
                    if (cmip->sPath[i] != NULL)
                        MemFree(cmip->sPath[i]);
                }
            } else {
                /* to avoid future references to a freed data structure */
                CurMedia = NULL;
            }

            break;
        default:
            break;
        }
        MemFree(mp->media_info);
        nextmp = mp->next;
        MemFree(mp);
    }

    MediaList = NULL;
    CurMedia = NULL;
    LRUMediaList = NULL;

    for (dsdp = DataSourceDbList; dsdp != NULL; dsdp = nextdsdp)
    {
        MemFree(dsdp->key.section);
        MemFree(dsdp->key.field1);
        MemFree(dsdp->key.field2);
        /* Each DataSourceDbList entry references one or more DataSource */
        /* entries                                                       */
        for (dsp = dsdp->list; dsp != NULL; dsp = nextdsp)
        {
            MemFree(dsp->channel);
            nextdsp = dsp->next;
            MemFree(dsp);
        }
        nextdsdp = dsdp->next;
        MemFree(dsdp);
    }

    DataSourceDbList = NULL;

    for (dap = DrasticList; dap != NULL; dap = nextdap)
    {
        nextdap = dap->next;
        MemFree(dap);
    }

    DrasticList = NULL;

    if (MergedEntrezInfo != NULL)
        EntrezInfoFree (MergedEntrezInfo);
    MergedEntrezInfo = NULL;
    SoleMedia = FALSE;
}

/***************************************************************************
*
*   GetCurMedia()
*
*   Obtain the current media. Provided to allow the CurMedia variable to be
*   "hidden" within this file.
*
****************************************************************************/

NLM_EXTERN MediaPtr GetCurMedia (void)
{
    return CurMedia;
}

/***************************************************************************
*
*   CurMediaType()
*
*   Obtain the current media type, or MEDIUM_UNKNOWN, if the media type has
*   not yet been set.
*
****************************************************************************/

NLM_EXTERN Int2 CurMediaType (void)
{

#ifdef _NEW_CdEntrez_
	if (_nouveau)
		return MEDIUM_CD;    /***** KLUDGE *****/
#endif


    if (CurMedia == NULL)
        return MEDIUM_UNKNOWN;

    return CurMedia->media_type;
}

/***************************************************************************
*
*   SetCurMedia()
*
*   Set the current media. Provided to allow the CurMedia variable to be
*   "hidden" within this file. To be used cautiously.
*
****************************************************************************/

NLM_EXTERN MediaPtr SetCurMedia (MediaPtr NewMedia)
{
    CurMedia = NewMedia;
    return CurMedia;
}

/***************************************************************************
*
*   IsDrasticAction()
*
*   Used to see whether a switch between two media is considered to be a
*   "drastic action", i.e., something to be avoided. When it becomes known
*   whether a given switch is considered to be "drastic", that information
*   is stored in a linked list. Requests which cannot be satisfied from that
*   list are searched in the configuration file, and the results are 
*   subsequently stored in that list.
*
****************************************************************************/

static Boolean IsDrasticAction(MediaPtr fromMedia, MediaPtr toMedia)
{
    DrasticActionsPtr d;
    char drastic_to[60];
    char buf[10];

    /* First try to obtain "drastic" information from a cached list */
    for (d = DrasticList; d != NULL; d = d->next)
    {
        if (d->from_media == fromMedia && d->to_media == toMedia)
            return d->is_drastic;
    }

    /* If unable to find it on the cached list, search the config file */
    /* and then add the information to the cached list                 */

    StrCpy(drastic_to, "DRASTIC_TO_");
    StrCat(drastic_to, toMedia->media_alias);

    d = (DrasticActionsPtr) MemNew(sizeof(DrasticActions));
    d->from_media = fromMedia;
    d->to_media = toMedia;
    d->is_drastic = KludgeCdromlibGetAppParam("NCBI", fromMedia->media_alias, drastic_to,
                                     "", buf, sizeof(buf)) > 0;
    d->next = DrasticList;
    DrasticList = d;
    return d->is_drastic;
}

/***************************************************************************
*
*   ChangeMedia()
*
*   Make the specified media the "active" media. This requires "swapping out"
*   the old media, if one is active, and may require, e.g., ejecting a CD-ROM
*   to make room for a new one.
*
****************************************************************************/

static Boolean ChangeMedia(MediaPtr newMedia)
{
    if (newMedia == NULL)
        return FALSE;

    if (newMedia == CurMedia)
        return TRUE;
    
    if (CurMedia != NULL)
    {
        CurMedia->is_live = FALSE;
    }

    if (newMedia->media_type == MEDIUM_CD)
    {
        MediaPtr toBeEjected;
        CdMediaInfoPtr cdm;
        char msg[100];
        Int2 numTries = 0;

        InitLRUeligibility(MEDIUM_CD);
        /* the media which we want to be inserted is ineligible for ejection */
        MarkLRUineligible(newMedia);

        while (! CdIsInserted(newMedia))
        {
            if (numTries++ > 3)
                return FALSE;
            if (AllCdDrivesAreFull())
            {
                if ((toBeEjected = FindLRU()) != NULL)
                { /* it's the user's problem if there are none to be ejected */
                    MarkLRUineligible(toBeEjected);
                    if (toBeEjected->media_type == MEDIUM_CD &&
                        CdIsInserted(toBeEjected) &&
                        (cdm = (CdMediaInfoPtr) toBeEjected->media_info) !=
                        NULL)
                    {
                        if (toBeEjected->entrez_info != NULL)
                        {
                            EjectCd(toBeEjected->entrez_info->volume_label,
                                    cdm->device_name, cdm->raw_device_name,
                                    cdm->mount_point, cdm->mount_cmd);
                        }
                    }
                }
            }

            StrCpy (msg, "Please insert <");
            StrCat (msg, newMedia->formal_name);
            StrCat (msg, ">; select OK when ready");
            if (Message(MSG_OKC, msg) == ANS_CANCEL)
            {
                return FALSE;
            }

            if ((cdm = (CdMediaInfoPtr) newMedia->media_info) != NULL &&
                newMedia->entrez_info != NULL)
            {
                MountCd(newMedia->entrez_info->volume_label, cdm->device_name,
                        cdm->mount_point, cdm->mount_cmd);
            }
        }
    }

    if (CurMedia != NULL && CurMedia->swapOutMedia != NULL)
    {
        CurMedia->swapOutMedia(CurMedia);
    }
    if (newMedia->swapInMedia != NULL)
    {
        newMedia->swapInMedia(newMedia);
    }
    MarkMRU(newMedia);

    CurMedia = newMedia;
    CurMedia->is_live = TRUE;
    return TRUE;
}

/***************************************************************************
*
*   PreInitMedia()
*
*   Perform the pre-initialization of a media. This is the initialization which
*   which may be performed when, e.g., a Network Service is not yet available,
*   or a CD-ROM is not yet inserted into a CD-ROM drive. Subsequently 
*   initialization may be performed at a later time  when the media becomes
*   fully available.
*
****************************************************************************/

NLM_EXTERN MediaPtr PreInitMedia (CharPtr mediaName)
{
    char buf[80];
    MediaPtr m;

    for (m = MediaList; m != NULL; m = m->next)
    {
        if (StrCmp(mediaName, m->media_alias) == 0)
            return m; /* already have it */
    }

    if (KludgeCdromlibGetAppParam("NCBI", mediaName, "TYPE", "", buf, sizeof(buf)) == 0 && !SoleMedia)
        return NULL;

    m = (MediaPtr) MemNew(sizeof(Media));

    m->media_type = MEDIUM_UNKNOWN;
    if (SoleMedia)
        m->media_type = MEDIUM_DISK;
        
    if (StrCmp(buf, "CD") == 0)
        m->media_type = MEDIUM_CD;
    if (StrCmp(buf, "NET") == 0)
        m->media_type = MEDIUM_NETWORK;
    if (StrCmp(buf, "HARDDISK") == 0)
        m->media_type = MEDIUM_DISK;
    
    if (KludgeCdromlibGetAppParam("NCBI", mediaName, "FORMAL_NAME", "", buf, sizeof(buf)) != 0)
    {
        m->formal_name = StringSave(buf);
    }
    else {
        m->formal_name = StringSave(mediaName);
    }
    
    m->inited_partial = FALSE;
    m->inited_total = FALSE;
    m->invalid = FALSE;
    m->in_use = FALSE;
    m->is_live = FALSE;
    m->media_info = NULL;
    m->swapOutMedia = NULL;
    m->swapInMedia = NULL;
    m->finiMedia = NULL;
    m->media_alias = StringSave(mediaName);
    m->next = MediaList;
    m->LRUineligible = FALSE;
    if (LRUMediaList == NULL)
    { /* now a single element in the list */
        m->next_lru = m;
        m->prev_lru = m;
    }
    else { /* insert into double-linked circular list */
        m->prev_lru = LRUMediaList->prev_lru;
        m->next_lru = LRUMediaList;
        LRUMediaList->prev_lru->next_lru = m;
        LRUMediaList->prev_lru = m;
    }
    LRUMediaList = m;
    MediaList = m;

    return m;
}

/***************************************************************************
*
*   CdIsInserted()
*
*   Test to see whether the specified CD-ROM is currently inserted into a
*   CD-ROM drive.
*
****************************************************************************/

NLM_EXTERN Boolean CdIsInserted(MediaPtr media)
{
    Boolean retval = FALSE;
#ifdef _OLD_CdEntrez_
    CdMediaInfoPtr cdm;
    AsnIoPtr aip;
    EntrezInfoPtr vi;
    EntrezInfoPtr pre_init_vi;
    ErrDesc err;
    short erract;
#endif

#ifdef _NEW_CdEntrez_
	if (_nouveau)
		retval = TRUE;    /***** KLUDGE *****/
#endif

#ifdef _OLD_CdEntrez_
    if (media == NULL)
        return FALSE;
    if (media->is_live)
        return TRUE;
    if ( (cdm = (CdMediaInfoPtr) media->media_info) == NULL)
        return FALSE;
    if ( (pre_init_vi = media->entrez_info) == NULL)
        return FALSE;
    
    ErrGetOpts(&erract, NULL);
    ErrSetOpts(ERR_IGNORE, 0);

    if ((aip = EntrezInfoOpen(cdm->sPath[DIR_ROOT])) != NULL)
    {
        vi = EntrezInfoAsnRead(aip, NULL);
        AsnIoClose(aip);
        if (vi != NULL)
        {
            if (StrCmp(vi->volume_label, pre_init_vi->volume_label) ==
                0 &&
                vi->version == pre_init_vi->version &&
                vi->issue == pre_init_vi->issue &&
                vi->format == pre_init_vi->format)
                retval = TRUE;
            EntrezInfoFree(vi);
        }
    }
    else {
        ErrFetch(&err); /* discard error */
    }
    
    ErrSetOpts(erract, 0); /* restore previous error status */

#endif
    return retval;
}

/***************************************************************************
*
*   EntrezInfoMerge()
*
*   Perform an "optomistic merge" of all the available EntrezInfo data
*   from the different known media. If there is only a single known medium,
*   then this degrades to returning the EntrezInfo for that medium. Otherwise,
*   the "type" entries with the maximum amount of sub-entries are selected.
*
****************************************************************************/

NLM_EXTERN  EntrezInfoPtr EntrezInfoMerge(void)
{
    EntrezInfoPtr new;
    Int2 MaxTypes = 0;
    MediaPtr mp;
    Int2 i;
    Int2 f;
    EntrezInfoPtr cip;
    EntrezInfoPtr InfoWithMaxTypes;
    EntrezTypeDataPtr besttype;
    Int4 count;
    Int2 mediaCount = 0;

    /* first, find out how many types there are, and save a pointer to an */
    /* entry which has that many types                                    */
    for (mp = MediaList; mp != NULL; mp = mp->next)
    {
        if ( (cip = mp->entrez_info) == NULL)
            continue;
        mediaCount++;
        if (cip->type_count <= MaxTypes)
            continue;
        MaxTypes = cip->type_count;
        InfoWithMaxTypes = cip;
    }

    if (MaxTypes == 0)
        return NULL;

    if (MergedEntrezInfo != NULL)
        EntrezInfoFree(MergedEntrezInfo);

    new = (EntrezInfoPtr) MemNew(sizeof(EntrezInfo));
    MergedEntrezInfo = new;
    new->volume_label = StringSave(InfoWithMaxTypes->volume_label);
    new->version = InfoWithMaxTypes->version;
    new->issue = InfoWithMaxTypes->issue;
    new->format = InfoWithMaxTypes->format;
    new->descr = StringSave(InfoWithMaxTypes->descr);

    /* there is no way to merge compression information if there is more than */
    /* one medium                                                             */
    if (mediaCount <= 1 && ! InfoWithMaxTypes->no_compression) {
        new->no_compression = InfoWithMaxTypes->no_compression;
        new->huff_count = InfoWithMaxTypes->huff_count;
        new->huff_left = MemDup (InfoWithMaxTypes->huff_left,
                                 sizeof(new->huff_left[0]) * new->huff_count);
        new->huff_right = MemDup (InfoWithMaxTypes->huff_right,
                                  sizeof(new->huff_right[0]) * new->huff_count);
    } else {
        new->no_compression = TRUE;
        new->huff_count = 0;
        new->huff_left = NULL;
        new->huff_right = NULL;
    }

    new->type_count = MaxTypes;
    new->type_bucket_size = InfoWithMaxTypes->type_bucket_size;
    new->field_count = InfoWithMaxTypes->field_count;
    new->field_bucket_size = InfoWithMaxTypes->field_bucket_size;

    new->type_names = (CharPtr PNTR) MemNew(sizeof(CharPtr) * MaxTypes);
    new->field_names = (CharPtr PNTR) MemNew(sizeof(CharPtr) *
                        new->field_count);
    new->types = (EntrezTypeDataPtr) MemNew(sizeof(EntrezTypeData) * MaxTypes);
    if (InfoWithMaxTypes->type_info != NULL)
    {
        new->type_info = (EntrezTypeInfo PNTR) MemNew(sizeof(EntrezTypeInfo) * MaxTypes);
    }
    if (InfoWithMaxTypes->field_info != NULL)
    {
        new->field_info = (EntrezFieldInfo PNTR)
                             MemNew(sizeof(EntrezFieldInfo) *
                             InfoWithMaxTypes->field_count);
    }

    for (i = 0; i < InfoWithMaxTypes->field_count; i++)
    {
        new->field_names[i] = StringSave(InfoWithMaxTypes->field_names[i]);
        if (new->field_info != NULL)
        {
            new->field_info[i] = InfoWithMaxTypes->field_info[i];
            new->field_info[i].name = StringSave(InfoWithMaxTypes->field_info[i].name);
            new->field_info[i].descr = StringSave(InfoWithMaxTypes->field_info[i].descr);
        }
    }

    for (i = 0; i < MaxTypes; i++)
    {
        new->type_names[i] = StringSave(InfoWithMaxTypes->type_names[i]);
        if (new->type_info != NULL)
        {
            new->type_info[i].id = InfoWithMaxTypes->type_info[i].id;
            StrCpy(new->type_info[i].tag,InfoWithMaxTypes->type_info[i].tag);
            new->type_info[i].name = StringSave(InfoWithMaxTypes->type_info[i].name);
            new->type_info[i].descr = StringSave(InfoWithMaxTypes->type_info[i].descr);
            new->type_info[i].asntype = StringSave(InfoWithMaxTypes->type_info[i].asntype);
        }

        count = 0;

        /* for each "type", traverse the media list and find the "best" one */
        for (mp = MediaList; mp != NULL; mp = mp->next)
        {
            if ( (cip = mp->entrez_info) == NULL)
                continue;
            if (cip->type_count < i)
                continue;
            if (cip->types[i].num < count)
                continue;
            /* the "best" type is the one with the most entries */
            besttype = &(cip->types[i]);
            count = besttype->num;
        }
        new->types[i].num = besttype->num;
        new->types[i].num_uids = besttype->num_uids;
        new->types[i].minuid = besttype->minuid;
        new->types[i].maxuid = besttype->maxuid;
        new->types[i].num_bucket = besttype->num_bucket;
        new->types[i].fields = (EntrezFieldDataPtr) MemNew(
                                sizeof(EntrezFieldData) *
                                InfoWithMaxTypes->field_count);
        for (f = 0; f < InfoWithMaxTypes->field_count; f++)
        {
            new->types[i].fields[f].num_terms =
                                    besttype->fields[f].num_terms;
            new->types[i].fields[f].num_bucket =
                                    besttype->fields[f].num_bucket;
        }

    }

    return new;
}

/***************************************************************************
*
*   FindLRU()
*
*   Find the "least-recently-used" entry in a doubly-linked-queue of media,
*   with the additional constraint that the selected medium may not be marked
*   as "ineligible".
*
****************************************************************************/

static MediaPtr FindLRU(void)
{
    MediaPtr media;
    
    if (LRUMediaList == NULL)
        return NULL;
    
    media = LRUMediaList;
    
    /* traverse media list, beginning with Least-Recently-Used, searching for an */
    /* eligible candidate                                                        */
    do {
        media = media->prev_lru;
        if (! media->LRUineligible)
            return media;
    } while (media != NULL && media != LRUMediaList);
    
    return NULL;
}

/***************************************************************************
*
*   MarkLRUineligible()
*
*   Mark a medium as "ineligible" to be considered in a search for a
*   least-recently-used medium.
*
****************************************************************************/

static void MarkLRUineligible(MediaPtr media)
{
    if (media != NULL)
    {
        media->LRUineligible = TRUE;
    }
}

/***************************************************************************
*
*   InitLRUeligibility()
*
*   Mark all media as not-"ineligible", i.e., eligible, to be considered
*   in a search for the least-recently-used medium ... provided that they
*   are of the specified media-type. All other media are ineligible.
*
****************************************************************************/

static void InitLRUeligibility(Int2 mask)
{
    MediaPtr m = LRUMediaList;

    if (m == NULL)
        return;

    do {
        if (m->media_type & mask)
            m->LRUineligible = FALSE;
        else
            m->LRUineligible = TRUE;
        m = m->next_lru;
    } while (m != LRUMediaList);
}

/***************************************************************************
*
*   MarkMRU()
*
*   Mark this medium as the "most-recently-used" by moving it to the
*   head (or tail, depending upon how you look at it) of a doubly-linked
*   queue.
*
****************************************************************************/

static void MarkMRU(MediaPtr m)
{ /* mark this entry as most recently used by moving to head of Queue */

    if (m == NULL || LRUMediaList == NULL)
        return;
    
    /* already Most-Recently-Used; and besides, the algorithm that follows  */
    /* will produce spaghetti if m already points to the head of the queue  */
    if (m == LRUMediaList)
        return;

    /* delete entry from neighbors' links */
    m->prev_lru->next_lru = m->next_lru;
    m->next_lru->prev_lru = m->prev_lru;

    /* insert at beginning of list */
    m->prev_lru = LRUMediaList->prev_lru;
    m->next_lru = LRUMediaList;
    LRUMediaList->prev_lru->next_lru = m;
    LRUMediaList->prev_lru = m;
    LRUMediaList = m;
}

/***************************************************************************
*
*   AllCdDrivesAreFull()
*
*   Returns TRUE if all availabe CD-ROM drives are currently full. This
*   should be done by searching the known CD-ROM drives for a "vacancy",
*   but this is currently difficult to perform. 
*
****************************************************************************/

static Boolean AllCdDrivesAreFull(void)
{ /* for now, be pessimistic */
    return TRUE;
}

/***************************************************************************
*
*   IsLiveMedia()
*
*   Indicates whether the specified media is "live". This means either,
*   e.g., a currently inserted CD-ROM, or a currently active network service,
*   etc.
*
****************************************************************************/

static Boolean  IsLiveMedia (MediaPtr media)
{
    if (media == NULL)
        return FALSE;

    if (media->invalid)
        return FALSE;

    if (media->media_type == MEDIUM_DISK)
        return CdIsInserted(media);

    if (media->media_type == MEDIUM_CD)
        return CdIsInserted(media);

    if (media->media_type == MEDIUM_NETWORK)
        return TRUE;

    /* else */
    return FALSE;
}

NLM_EXTERN Int2 ParseMedia(ConfCtlProc initfunc, Int2 media_mask)
{
    char media[256];
    CharPtr m1,m2;
    CharPtr word;
    Boolean done;
    MediaPtr mp;
    Int2 numProcessed = 0;

    KludgeCdromlibGetAppParam ("ncbi", "NCBI", "MEDIA", "", media, sizeof media);

    /* This is a work-around to provide backwards compatibility for old       */
    /* config files which do not specify MEDIA                                */
    if (media[0] == '\0')
    {
        StrCpy(media, "NCBI");
        SetSoleMedia();
    }

    m2 = media;
    StrCat(m2, "\n");
    done = FALSE;

    /* parse "MEDIA", looking for all data sources; note that we don't use */
    /* strtok(), because it's not re-entrant                               */
    while ((m1 = StringPBrk(m2, ",\n\r")) != NULL && !done)
    {
        if (*m1 == '\n' || *m1 == '\r' || *m1 == '\0')
            done = TRUE;
        *m1 = '\0';
        word = m2;
        m2 = m1 + 1;
        m2 += StringSpn(m2, " \t"); /* remove leading spaces and tabs from next word */

        mp = PreInitMedia(word);
        if (mp->media_type & media_mask)
        {
            if (initfunc != NULL)
            {
                if (initfunc(word))
                {
                    mp->in_use = TRUE;
                    mp->invalid = FALSE;
                    numProcessed++;
                }
            }
            else {
                mp->in_use = TRUE;
                mp->invalid = FALSE;
                numProcessed++;
            }
        }
        else {
            if (! mp->in_use)
                mp->invalid = TRUE;
        }
    }

    return numProcessed;
}
