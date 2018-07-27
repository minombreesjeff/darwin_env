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
* ===========================================================================
*
* File Name:  medutil.c
*
* Author:  James Ostell
*   
* Version Creation Date: 8/31/93
*
* $Revision: 6.21 $
*
* File Description:  Medline Utilities for MedArch
*   Assumes user calls MedArchInit and Fini
*
* Modifications:  
* --------------------------------------------------------------------------
* Date	   Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* ==========================================================================
*
*
* RCS Modification History:
* $Log: medutil.c,v $
* Revision 6.21  2005/02/15 17:45:32  bazhin
* Modified MedlineToISO() function: now in addition to Cit-art.Auth-list.ml
* slot it also looks at Cit-art.Auth-list.Author.Person-id.ml one
* to convert authors names from Medline format to Name-std (last, first,
* initials).
*
* Revision 6.20  2005/01/28 20:43:00  bazhin
* Fixed bug related to preserving input Cit-arts in cases of
* lookup without replacement.
*
* Revision 6.19  2004/11/29 17:50:06  bazhin
* Removed memory leak in FixPubEquiv() function.
*
* Revision 6.18  2004/11/12 17:35:51  bazhin
* Fixed bug related to the converting empty authors list from
* medline format to standard one.
*
* Revision 6.17  2004/11/04 17:53:17  bazhin
* Reverted FixPubEquiv modification, when not lookup for pmid was
* performed, if input Pub-equiv has a Cit-gen with authors, or title,
* or journal blocks filled in. Now it does lookup for pmid even if
* Cit-gen has them.
*
* Revision 6.16  2004/11/02 21:28:35  bazhin
* Modifications related to the new MedArch server to be installed.
* Does not use Medline ids for lookup, and does not look them up.
*
* Revision 6.15  2004/08/18 17:00:56  bazhin
* Fixed bug with medline lookup if both PubMed and Medline ids are present.
*
* Revision 6.14  2004/03/19 18:32:03  bazhin
* One more patch related to the previous one.
*
* Revision 6.13  2004/03/19 14:53:18  bazhin
* Added function PropogateInPress(), which is called from FixPubEquiv().
* It sets Imprint.prepub value to "in-press" if input Cit-art was flagged
* as "in-press", but had Medline or/and Pubmed id(s) and successful lookup
* returned Cit-art with no article references, such as volume and page
* numbers.
*
* Revision 6.12  2004/03/17 16:59:35  bazhin
* Slight modification in ten_authors() function. If the number of input
* authors greater than 10 and number of MedArch ones less or equal 11,
* or input has more than 25 and MedArch has less or equal 26, then
* it will take input set instead of MedArch one.
*
* Revision 6.11  2004/03/16 18:53:37  bazhin
* Now does not ignore in-press'ed Cit-arts for lookup.
*
* Revision 6.10  2003/10/01 13:08:33  bazhin
* Modified ten_authors() function to handle consortiums properly.
*
* Revision 6.9  2003/09/10 18:47:28  bazhin
* "print_pub()" function now is aware of consortiums.
*
* Revision 6.8  2003/03/25 19:14:59  bazhin
* Function "ten_authors()" became public (from static).
*
* Revision 6.7  2003/03/12 20:35:02  bazhin
* "ten_authors()" function now can handle consortiums.
*
* Revision 6.6  2001/03/08 12:47:44  ostell
* made FixPub work if no medline uid returned, but pmid is returned.
*
* Revision 6.5  2000/08/18 17:01:02  kans
* added FetchPubPmId, enhanced FixPubEquiv to handle records with pmid but no muid
*
* Revision 6.4  1999/11/17 18:16:15  bazhin
* If failed to get pub from MedArch server, then does not return NULL,
* but use input one.
* Also removed unused variables.
*
* Revision 6.3  1999/11/08 19:32:44  kans
* removed unnecessary for loop initialization picked up by IRIX C comnpiler
*
* Revision 6.2  1999/05/06 18:53:53  kans
* FixPubEquiv now works on PMID as well as MUID
*
* Revision 6.1  1998/09/14 20:54:51  bazhin
* Changes in "ten_authors()" : if successful medline lookup has returned
* no authors, then will use input ones.
*
* Revision 6.0  1997/08/25 18:35:48  madden
* Revision changed to 6.0
*
* Revision 5.0  1996/05/28 14:11:11  ostell
* Set to revision 5.0
*
 * Revision 4.4  1996/05/22  17:30:09  kans
 * changed name of error file, initialize muid in lookup procedure
 *
 * Revision 4.3  1996/03/12  22:21:07  tatiana
 * add bullet proof to MedlineToISO function
 *
 * Revision 4.2  1995/11/02  23:39:28  tatiana
 * do not accept Medline article with 0 authors
 *
 * Revision 4.1  1995/08/16  14:28:21  tatiana
 * modified get_std_auth to handle spaces between initials
 *
 * Revision 4.0  1995/07/26  13:55:12  ostell
 * force revision to 4.0
 *
 * Revision 1.32  1995/06/06  14:34:39  tatiana
 * bug fixed in print_pub()
 *
 * Revision 1.30  1995/05/25  22:17:14  kans
 * fixed FixPubEquiv
 *
 * Revision 1.29  1995/05/25  18:14:35  kans
 * additional long and int casts in ErrPostEx
 *
 * Revision 1.28  1995/05/24  16:13:47  tatiana
 * add in_press() to avoid lookup for in press articles
 *
 * Revision 1.24  1995/05/17  17:54:05  epstein
 * add RCS log revision history
 *
*/

/** for ErrPostEx() ****/

static char *this_module = "medarch";
#ifdef THIS_MODULE
#undef THIS_MODULE
#endif

#define THIS_MODULE this_module
static char *this_file = __FILE__;
#define THIS_FILE this_file

/**********************/
#include <medutil.h>		   /* the interface */
#include <medarch.h>           /* the medarch interface */
#include <mdrcherr.h>           /* the errors interface */

/**********************************************************/
void print_pub(ValNodePtr pub, Boolean found, Boolean auth, Int4 muid)
{
    ValNodePtr v;
    ValNodePtr title = NULL;
    CitArtPtr  art;
    CitJourPtr jour;
    CitBookPtr book;
    NameStdPtr namestd;
    AuthorPtr  aup;
    CharPtr    s_title = NULL;
    CharPtr    page = NULL;
    CharPtr    vol = NULL;
    CharPtr    last = NULL;
    CharPtr    first = NULL;
    Int2       year = 0;
    ImprintPtr imp = NULL;
    DatePtr    dp;
	
    if(pub == NULL || pub->data.ptrvalue == NULL)
    {
        ErrPostEx(SEV_WARNING, ERR_PRINT_Failed, "Citation NULL");
        return;
    }
    if(pub->choice != PUB_Article)
        return;

    art = pub->data.ptrvalue;
    first = "";
    last = "";
    if(art->authors == NULL)
        ErrPostEx(SEV_WARNING, ERR_PRINT_Failed, "Authors NULL");
    else
    {
        v = art->authors->names;
        if(v == NULL)
            ErrPostEx(SEV_WARNING, ERR_PRINT_Failed, "Authors NULL");
        else
        {
            /* warning! processing only the first of the author name valnodes
               should loop through them all in case first is uninformative */
            if(art->authors->choice == 1)
            {
                aup = v->data.ptrvalue;
                if(aup != NULL)
                {
                    if(aup->name->choice == 2)
                    {
                        namestd = aup->name->data;
                        if(namestd->names[0])
                            last = namestd->names[0];
                        if(namestd->names[4])
                            first = namestd->names[4];
                    }
                    else if(aup->name->choice == 5)
                        last = aup->name->data;
                }
            }
            else
            {
                first = "";
                last = v->data.ptrvalue;
            }
        }
    }

    if(art->from == 1)
    {
        jour = art->fromptr;
        if(jour != NULL)
        {
            title = jour->title;
            imp = jour->imp;
        }
    }
    else if(art->from == 2)
    {
        book = art->fromptr;
        if(book != NULL)
        {
            title = book->title;
            imp = book->imp;
        }
    }

    if(title != NULL)
        s_title = title->data.ptrvalue;
    if(s_title == NULL)
        s_title = "journal unknown";

    if(imp != NULL)
    {
        vol = imp->volume;
        page = imp->pages;
        if(imp->date != NULL)
            year = imp->date->data[1] + 1900;
    }

    if(page == NULL)
        page = "no page number";
    if(vol == NULL)
        vol = "no volume number";

    if(auth)
    {
        ErrPostEx(SEV_ERROR, ERR_REFERENCE_MedArchMatchIgnored,  
                  "Too many author name differences: %ld|%s %s|%s|(%d)|%s|%s", 
                  (long) muid, last, first, s_title, (int) year, vol, page);
        return;
    }
    if(imp != NULL && imp->prepub == 2) /* in-press */ 
    {
        dp = DateCurr();
        if(year && (Int2) (dp->data[1]) + 1900 - year > 2)
        {
            ErrPostEx(SEV_WARNING, ERR_REFERENCE_OldInPress, 
                      "encountered in-press article more than 2 years old: %s %s|%s|(%d)|%s|%s",
                      last, first, s_title, (int) year, vol, page);
        }
        DateFree(dp);
    }
    else
    {
        if(found)
        {
            ErrPostEx(SEV_INFO, ERR_REFERENCE_SuccessfulPmidLookup, 
                      "%ld|%s %s|%s|(%d)|%s|%s", (long) muid, last, first,
                      s_title, (int) year, vol, page);
        }
        else if(muid == 0)
        {
            ErrPostEx(SEV_WARNING, ERR_REFERENCE_PmidNotFound,  
                      "%s %s|%s|(%d)|%s|%s", last, first, s_title,
                      (int) year, vol, page);
        }
        else
        {
            ErrPostEx(SEV_WARNING, ERR_REFERENCE_PmidNotFound,  
                      ">>%ld<<|%s %s|%s|(%d)|%s|%s", (long) muid, last,
                      first, s_title, (int) year, vol, page);
        }
    }
}

/**********************************************************/
static Boolean ten_authors_compare(CitArtPtr capold, CitArtPtr capnew)
{
    ValNodePtr old;
    ValNodePtr new;
    ValNodePtr vnp;
    CharPtr    namesnew[10];
    Int4       numold;
    Int4       numnew;
    Int4       match;
    Int4       i;

    if(capold == NULL || capold->authors == NULL ||
       capold->authors->names == NULL)
        return(TRUE);
    if(capnew == NULL || capnew->authors == NULL ||
       capnew->authors->names == NULL)
        return(FALSE);
    if(capold->authors->choice != capnew->authors->choice ||
       capold->authors->choice == 1)
        return(TRUE);

    old = capold->authors->names;
    new = capnew->authors->names;

    for(numnew = 0, vnp = old; vnp != NULL; vnp = vnp->next)
        if(vnp->data.ptrvalue != NULL)
            numnew++;

    for(numold = 0, vnp = new; vnp != NULL && numold < 10; vnp = vnp->next)
        if(vnp->data.ptrvalue != NULL)
            namesnew[numold++] = vnp->data.ptrvalue;

    for(match = 0, vnp = old; vnp != NULL; vnp = vnp->next)
    {
        if(vnp->data.ptrvalue == NULL)
            continue;

        for(i = 0; i < numnew; i++)
        {
            if(StringICmp(vnp->data.ptrvalue, namesnew[i]) == 0)
            {
                match++;
                break;
            }
        }
    }

    i = (numold < numnew) ? numold : numnew;
    if(i > 3 * match)
        return(FALSE);

    if(numold > 10)
    {
        AuthListFree(capnew->authors);
        capnew->authors = capold->authors;
        capold->authors = NULL;
    }
    return(TRUE);
}

/**********************************************************/
Boolean ten_authors(CitArtPtr art, CitArtPtr art_tmp)
{
    NameStdPtr namestd;
    ValNodePtr v;
    AuthorPtr  aup;
    CharPtr    mu[10];
    CharPtr    oldcon;
    CharPtr    newcon;
    Int2       num;
    Int2       numnew;
    Int2       numtmp;
    Int2       i;
    Int2       match;

    if(art_tmp == NULL)
        return(FALSE);
    if(art_tmp->authors == NULL || art_tmp->authors->names == NULL)
    {
        if(art != NULL && art->authors != NULL)
        {
            if(art_tmp->authors != NULL)
                AuthListFree(art_tmp->authors);
            art_tmp->authors = art->authors;
            art->authors = NULL;
        }
        return(TRUE);
    }
    if(art == NULL || art->authors == NULL || art->authors->names == NULL ||
       art->authors->choice != art_tmp->authors->choice)
        return(TRUE);

    if(art->authors->choice != 1)
        return(ten_authors_compare(art, art_tmp));

    oldcon = NULL;
    for(num = 0, v = art->authors->names; v != NULL; v = v->next)
    {
        aup = v->data.ptrvalue;
        if(aup->name->choice == 2)
            num++;
        else if(aup->name->choice == 5)
            oldcon = aup->name->data;
    }

    newcon = NULL;
    for(numtmp = 0, v = art_tmp->authors->names; v != NULL; v = v->next)
    {
        aup = v->data.ptrvalue;
        if(aup->name->choice == 2)
            numtmp++;
        else if(aup->name->choice == 5)
            newcon = aup->name->data;
    }

    if(oldcon != NULL)
    {
        if(newcon == NULL)
        {
            ErrPostEx(SEV_WARNING, ERR_REFERENCE_NoConsortAuthors,
                      "Publication as returned by MedArch lacks consortium authors of the original publication: \"%s\".",
                      oldcon);
            aup = AuthorNew();
            aup->name = PersonIdNew();
            aup->name->choice = 5;
            aup->name->data = StringSave(oldcon);
            v = ValNodeNew(NULL);
            v->data.ptrvalue = aup;
            v->next = art_tmp->authors->names;
            art_tmp->authors->names = v;
            newcon = oldcon;
        }
        else if(StringICmp(oldcon, newcon) != 0)
        {
            ErrPostEx(SEV_WARNING, ERR_REFERENCE_DiffConsortAuthors,
                      "Consortium author names differ. Original is \"%s\". MedArch's is \"%s\".",
                      oldcon, newcon);
        }
        if(num == 0)
            return(TRUE);
    }

    numnew = 0;
    for(v = art_tmp->authors->names; v != NULL && numnew < 10; v = v->next)
    {
        aup = v->data.ptrvalue;
        if(aup->name->choice != 2)
            continue;
        namestd = aup->name->data;
        mu[numnew++] = namestd->names[0];
    }

    for(match = 0, v = art->authors->names; v != NULL; v = v->next)
    {
        aup = v->data.ptrvalue;
        if(aup->name->choice != 2)
            continue;

        namestd = aup->name->data;
        for(i = 0; i < numnew; i++)
        {
            if(StringICmp(namestd->names[0], mu[i]) == 0)
            {
                match++;
                break;
            }
        }
    }

    i = (num < numnew) ? num : numnew;
    if(i > 3 * match)
        return(FALSE);

    if(numtmp == 0 || (num > 10 && numtmp < 12) || (num > 25 && numtmp < 27))
    {
        AuthListFree(art_tmp->authors);
        art_tmp->authors = art->authors;
        art->authors = NULL;
    }
    return(TRUE);
}

/*****************************************************************************
*
*   FindPub
*   	SeqEntryExplore to process all pubs
*
*****************************************************************************/
void FindPub(SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent)
{
    BioseqPtr        bsp;
    BioseqSetPtr     bssp;
    ValNodePtr       vnp;
    ValNodePtr       pub;
    ValNodePtr       newpub;
    ValNodePtr       prev;
    ValNodePtr       next;
    ValNodePtr       head;
    SeqAnnotPtr      sap;
    SeqFeatPtr       sfp;
    PubdescPtr       pdp;
    FindPubOptionPtr fpop;

    fpop = (FindPubOptionPtr) data;

    if(IS_Bioseq(sep))
    {
        bsp = (BioseqPtr) sep->data.ptrvalue;
        vnp = bsp->descr;
        sap = bsp->annot;
    }
    else
    {
        bssp = (BioseqSetPtr) sep->data.ptrvalue;
        vnp = bssp->descr;
        sap = bssp->annot;
    }

    for(; vnp != NULL; vnp = vnp->next)
    {
        if(vnp->choice != Seq_descr_pub)
            continue;
        pdp = (PubdescPtr) vnp->data.ptrvalue;
        newpub = FixPubEquiv(pdp->pub, fpop);
        pdp->pub = newpub;
    }

    for(; sap != NULL; sap = sap->next)
    {
        if(sap->type != 1)              /* !feature table */
            continue;

        for(sfp = (SeqFeatPtr) sap->data; sfp != NULL; sfp = sfp->next)
        {
            if(sfp->data.choice == 6)   /* pub feature */
            {
                pdp = (PubdescPtr) sfp->data.value.ptrvalue;
                newpub = FixPubEquiv(pdp->pub, fpop);
                pdp->pub = newpub;
            }

            if(sfp->cit == NULL)
                continue;

            head = sfp->cit;
            prev = NULL;
            pub = (ValNodePtr) head->data.ptrvalue;
            for(; pub != NULL; pub = next)
            {
                next = pub->next;
                newpub = FixPub(pub, fpop);
                if(prev == NULL)
                    head->data.ptrvalue = newpub;
                else
                    prev->next = newpub;
                prev = newpub;
            }
            head->choice = 1;           /* set to Pub-set.pub */
        }
    }
}

/*****************************************************************************
*
*   FixPub(pub, fpop)
*   	Tries to make any Pub into muid/cit-art
*
*****************************************************************************/
ValNodePtr FixPub(ValNodePtr pub, FindPubOptionPtr fpop)
{
    ValNodePtr newpub;
    ValNodePtr tmp;
    Int4       pmid;
    CitArtPtr  cit;
	
    if(pub == NULL)
        return(NULL);

    pub->next = NULL;                   /* ready for return */
    newpub = pub;
    switch(pub->choice)
    {
        case PUB_Medline:               /* medline, just split to pubequiv */
            tmp = SplitMedlineEntry((MedlineEntryPtr) pub->data.ptrvalue);
            if(tmp != NULL)
            {
                newpub->choice = PUB_Equiv;
                newpub->data.ptrvalue = tmp;
            }
            break;
        case PUB_Article:
            cit = pub->data.ptrvalue;
            if(cit->from == 2 || in_press(cit)) /* article from book or
                                                   in press */
                return(pub);

            fpop->lookups_attempted++;
            pmid = MedArchCitMatchPmId(pub);
            if(pmid)                    /* matched it */
            {
                print_pub(pub, TRUE, FALSE, pmid);
                fpop->lookups_succeeded++;
                if(fpop->replace_cit)
                {
                    fpop->fetches_attempted++;
                    tmp = FetchPubPmId(pmid);

                    if(tmp != NULL)
                    {
                        if(ten_authors(pub->data.ptrvalue, tmp->data.ptrvalue))
                        {
                            fpop->fetches_succeeded++;
                            PubFree(pub);
                            pub = ValNodeNew(tmp);
                            pub->choice = PUB_PMid;
                            pub->data.intvalue = pmid;
                            newpub = ValNodeNew(NULL);
                            newpub->choice = PUB_Equiv;
                            newpub->data.ptrvalue = tmp;
                        }
                        else 
                        {
                            print_pub(pub, FALSE, TRUE, pmid);
                            newpub = pub;
                            MedlineToISO(pub);
                        }
                    }
                }
                else
                {
                    print_pub(pub, FALSE, FALSE, pmid);
                    newpub = pub;
                    MedlineToISO(pub);
                }
            }
            break;
        case PUB_Equiv:
            tmp = FixPubEquiv((ValNodePtr) pub->data.ptrvalue, fpop);
            pub->data.ptrvalue = tmp;
            newpub = pub;
            break;
        default:
            break;
    }
    return(newpub);
}

/*****************************************************************************
*
*   SplitMedlineEntry(mep)
*      splits a medline entry into 2 pubs (1 muid, 1 Cit-art)
*      converts Cit-art to ISO/GenBank style
*      returns pointer to first (chained) pub
*      deletes original medline entry
*
*****************************************************************************/
ValNodePtr SplitMedlineEntry(MedlineEntryPtr mep)
{
    ValNodePtr uid;
    ValNodePtr cit;

    if(mep == NULL || (mep->pmid < 0 && mep->cit == NULL))
        return(NULL);

    uid = NULL;
    if(mep->pmid > 0)
    {
        uid = ValNodeNew(uid);
        uid->choice = PUB_PMid;
        uid->data.intvalue = mep->pmid;
    }

    if(mep->cit != NULL)
    {
        cit = ValNodeNew(uid);
        cit->choice = PUB_Article;
        cit->data.ptrvalue = mep->cit;
        mep->cit = NULL;
        MedlineToISO(cit);
    }

    MedlineEntryFree(mep);

    return(uid);
}

/**********************************************************/
static Boolean if_inpress_set(CitArtPtr cap)
{
    CitJourPtr jour;
    CitBookPtr book;

    if(cap == NULL || cap->fromptr == NULL)
        return(FALSE);

    if(cap->from == 1)
    {
        jour = cap->fromptr;
        if(jour->imp != NULL && jour->imp->prepub == 2)
            return(TRUE);
    }
    else if(cap->from == 2 || cap->from == 3)
    {
        book = cap->fromptr;
        if(book->imp != NULL && book->imp->prepub == 2)
            return(TRUE);
    }
    return(FALSE);
}

/**********************************************************/
static void PropagateInPress(Boolean inpress, ValNodePtr vnp)
{
    CitJourPtr jour;
    CitBookPtr book;
    CitArtPtr  cap;

    if(inpress == FALSE || vnp == NULL || vnp->data.ptrvalue == NULL)
        return;

    cap = vnp->data.ptrvalue;
    if(in_press(cap) == FALSE || cap->fromptr == NULL)
        return;

    if(cap->from == 1)
    {
        jour = cap->fromptr;
        if(jour->imp != NULL)
            jour->imp->prepub = 2;
    }
    else if(cap->from == 2 || cap->from == 3)
    {
        book = cap->fromptr;
        if(book->imp != NULL)
            book->imp->prepub = 2;
    }
}

/**********************************************************/
static ValNodePtr PubEquivAdd(ValNodePtr headvnp, ValNodePtr addvnp)
{
    ValNodePtr vnp;

    if(headvnp == NULL)
        return(addvnp);
    for(vnp = headvnp; vnp->next != NULL;)
        vnp = vnp->next;
    vnp->next = addvnp;
    return(headvnp);
}

/**********************************************************/
static ValNodePtr PubEquivPutBack(ValNodePtr headvnp, ValNodePtr addvnp)
{
    ValNodePtr vnp;

    if(headvnp == NULL)
        return(addvnp);
    if(addvnp == NULL)
        return(headvnp);

    for(vnp = addvnp; vnp->next != NULL;)
        vnp = vnp->next;
    vnp->next = headvnp;
    return(addvnp);
}

/**********************************************************/
static ValNodePtr FixPubEquivAppend(ValNodePtr pube, ValNodePtr tmp2,
                                    ValNodePtr tmp)
{
    if(tmp2 == NULL)
        return(tmp);
    tmp2->next = tmp;
    return(pube);
}

/**********************************************************/
static ValNodePtr FixPubEquivAppendPmid(ValNodePtr tmp, Int4 muid,
                                        ValNodePtr pmidptr)
{
    Int4 oldpmid;
    Int4 newpmid;

    oldpmid = (pmidptr == NULL) ? 0 : pmidptr->data.intvalue;

    newpmid = MedArchMu2Pm(muid);
    if(oldpmid < 1 && newpmid < 1)
        return(tmp);

    if(oldpmid > 0 && newpmid > 0 && oldpmid != newpmid)
        ErrPostEx(SEV_ERROR, ERR_REFERENCE_PmidMissmatch,
                  "OldPMID=%ld doesn't match lookup (%ld). Keeping lookup.",
                  (long) oldpmid, (long) newpmid);

    if(pmidptr != NULL)
        tmp->next = pmidptr;
    else
    {
        tmp->next = ValNodeNew(NULL);
        tmp->next->choice = PUB_PMid;
    }

    tmp = tmp->next;
    tmp->data.intvalue = (newpmid > 0) ? newpmid : oldpmid;

    return(tmp);
}

/**********************************************************/
ValNodePtr FixPubEquiv(ValNodePtr pube, FindPubOptionPtr fpop)
{
    ValNodePtr tmp;
    ValNodePtr muidptr = NULL;
    ValNodePtr pmidptr = NULL;
    ValNodePtr citartptr = NULL;
    ValNodePtr mepptr = NULL;
    ValNodePtr otherptr = NULL;
    ValNodePtr tmp2;
    ValNodePtr next;
    ValNodePtr new;
    Int4       muid = 0;
    Int4       oldmuid = 0;
    Int4       pmid = 0;
    Int4       oldpmid = 0;
    CitArtPtr  cit;
    Boolean    got;
    Boolean    inpress;

    if(pube == NULL)
        return(NULL);

    for(got = FALSE, tmp = pube; tmp != NULL; tmp = tmp->next)
    {
        if(tmp->choice == PUB_Muid || tmp->choice == PUB_PMid)
        {
            got = TRUE;
            break;
        }
    }

    for(tmp = pube; tmp != NULL; tmp = next)
    {
        next = tmp->next;
        tmp->next = NULL;
        if(tmp->choice == PUB_Muid)
            muidptr = PubEquivAdd(muidptr, tmp);
        else if(tmp->choice == PUB_PMid)
            pmidptr = PubEquivAdd(pmidptr, tmp);
        else if(tmp->choice == PUB_Article)
        {
            cit = tmp->data.ptrvalue;
            if(cit->from == 2 || (in_press(cit) != FALSE && got == FALSE))
                otherptr = PubEquivAdd(otherptr, tmp);
            else
                citartptr = PubEquivAdd(citartptr, tmp);
        }
        else if(tmp->choice == PUB_Medline)
            mepptr = PubEquivAdd(mepptr, tmp);
        else
            otherptr = PubEquivAdd(otherptr, tmp);
    }

    if((muidptr != NULL || pmidptr != NULL) &&  /* got a muid or pmid */
       fpop->always_look == FALSE)
    {
        pube = PubEquivPutBack(NULL, citartptr);
        pube = PubEquivPutBack(pube, muidptr);
        pube = PubEquivPutBack(pube, pmidptr);
        pube = PubEquivPutBack(pube, mepptr);
        pube = PubEquivPutBack(pube, otherptr);
        return(pube);                   /* no changes */
    }

    pube = otherptr;                    /* put back others */
    tmp2 = otherptr;
    if(tmp2 != NULL)
        while(tmp2->next != NULL)
            tmp2 = tmp2->next;

    if(mepptr != NULL)                  /* have a medline entry,
                                           take it first */
    {
        if(mepptr->next != NULL)
        {
            ErrPostEx(SEV_WARNING, ERR_REFERENCE_Multiple_ref,
                      "More than one Medline entry in Pub-equiv");
            mepptr->next = PubEquivFree(mepptr->next);
        }
        PubEquivFree(muidptr);          /* ditch others */
        PubEquivFree(pmidptr);
        PubEquivFree(citartptr);
        tmp = SplitMedlineEntry((MedlineEntryPtr) mepptr->data.ptrvalue);
        ValNodeFree(mepptr);
        pube = FixPubEquivAppend(pube, tmp2, tmp);
        return(pube);
    }

    if(pmidptr != NULL)                 /* have a pmid */
    {
        oldpmid = pmidptr->data.intvalue;
        if(pmidptr->next != NULL)       /* more than one, just take
                                           the first */
        {
            for(tmp = pmidptr->next; tmp != NULL; tmp = tmp->next)
                if(tmp->data.intvalue != pmid)
                    ErrPostEx(SEV_WARNING, ERR_REFERENCE_Multiple_pmid,
                              "Two different pmids in Pub-equiv [%ld] [%ld]",
                              (long) oldpmid, (long) tmp->data.intvalue);
            pmidptr->next = PubEquivFree(pmidptr->next);
        }
    }

    if(muidptr != NULL)                 /* have an muid */
    {
        oldmuid = muidptr->data.intvalue;
        if(muidptr->next != NULL)       /* more than one, just take
                                           the first */
        {
            for(tmp = muidptr->next; tmp != NULL; tmp = tmp->next)
                if(tmp->data.intvalue != muid)
                    ErrPostEx(SEV_WARNING, ERR_REFERENCE_Multiple_muid,
                              "Two different muids in Pub-equiv [%ld] [%ld]",
                              (long) oldmuid, (long) tmp->data.intvalue);
            muidptr->next = PubEquivFree(muidptr->next);
        }
    }

    if(citartptr != NULL)
    {
        if(citartptr->next != NULL)     /* ditch extras */
        {
            ErrPostEx(SEV_WARNING, ERR_REFERENCE_Multiple_ref,
                      "More than one Cit-art in Pub-equiv");
            citartptr->next = PubEquivFree(citartptr->next);
        }

        inpress = if_inpress_set(citartptr->data.ptrvalue);
        fpop->lookups_attempted++;
        pmid = MedArchCitMatchPmId(citartptr);
        if(pmid != 0)                   /* success */
        {
            print_pub(citartptr, TRUE, FALSE, pmid);
            fpop->lookups_succeeded++;
            if(oldpmid > 0 && oldpmid != pmid)  /* already had a pmid */
                ErrPostEx(SEV_ERROR, ERR_REFERENCE_PmidMissmatch,
                          "OldPMID=%ld doesn't match lookup (%ld). Keeping lookup.",
                          (long) oldpmid, (long) pmid);

            if(fpop->replace_cit != FALSE)
            {
                fpop->fetches_attempted++;
                new = FetchPubPmId(pmid);
                if(new != NULL) 
                {
                    fpop->fetches_succeeded++;

                    if(ten_authors(citartptr->data.ptrvalue,
                                   new->data.ptrvalue) != FALSE)
                    {
                        if(pmidptr != NULL)
                            tmp = pmidptr;
                        else
                        {
                            tmp = ValNodeNew(tmp2);
                            tmp->choice = PUB_PMid;
                        }
                        tmp->data.intvalue = pmid;
                        pube = FixPubEquivAppend(pube, tmp2, tmp);
                        tmp->next = new;

                        PubFree(citartptr);
                        citartptr = new;
                    }
                    else
                    {
                        if(pmidptr != NULL)
                            PubFree(pmidptr);
                        print_pub(citartptr, FALSE, TRUE, pmid);
                        PubFree(new);
                        pube = FixPubEquivAppend(pube, tmp2, citartptr);
                    }
                }
                else
                {
                    ErrPostEx(SEV_ERROR, ERR_REFERENCE_FailedToGetPub,
                              "Failed to get pub from MedArch server for pmid = %ld. Input one is preserved.",
                              pmid);
                    if(pmidptr != NULL)
                        tmp = pmidptr;
                    else
                    {
                        tmp = ValNodeNew(NULL);
                        tmp->choice = PUB_PMid;
                    }
                    if(tmp2 == NULL)
                        pube = tmp;
                    else
                        tmp2->next = tmp;
                    tmp->data.intvalue = pmid;
                    tmp->next = citartptr;
                    MedlineToISO(citartptr);
                }
            }
            else
            {
                if(pmidptr != NULL)
                    tmp = pmidptr;
                else
                {
                    tmp = ValNodeNew(NULL);
                    tmp->choice = PUB_PMid;
                }
                if(tmp2 == NULL)
                    pube = tmp;
                else
                    tmp2->next = tmp;
                tmp->data.intvalue = pmid;
                tmp->next = citartptr;
                MedlineToISO(citartptr);
            }
            if(muidptr != NULL)
                PubFree(muidptr);
            PropagateInPress(inpress, citartptr);
            return(pube);
        }

        print_pub(citartptr, FALSE, FALSE, oldmuid);
        pube = FixPubEquivAppend(pube, tmp2, citartptr);
        if(muidptr != NULL)             /* ditch the mismatched muid */
            PubFree(muidptr);
        if(pmidptr != NULL)             /* ditch the mismatched pmid */
            PubFree(pmidptr);
        PropagateInPress(inpress, citartptr);
        return(pube);
    }

    if(oldpmid != 0)                    /* have a pmid but no cit-art */
    {
        fpop->fetches_attempted++;
        tmp = MedArchGetPubPmId(oldpmid);
        if(tmp != NULL)
        {
            fpop->fetches_succeeded++;
            if(fpop->replace_cit != FALSE)
            {
                tmp = MedlineToISO(tmp);
                pube = FixPubEquivAppend(pube, tmp2, tmp);
                tmp2 = tmp;
            }
            else
                PubFree(tmp);
            pube = FixPubEquivAppend(pube, tmp2, pmidptr);
            return(pube);
        }
        ErrPostEx(SEV_WARNING, ERR_REFERENCE_No_reference,
                  "Cant find article for pmid [%ld]", (long) oldpmid);
    }

    if(oldpmid > 0)
        pube = FixPubEquivAppend(pube, tmp2, pmidptr);
    else if(oldmuid > 0)
    {
        pube = FixPubEquivAppend(pube, tmp2, muidptr);
        FixPubEquivAppendPmid(muidptr, oldmuid, pmidptr);
    }

    return(pube);
}

/*****************************************************************************
*
*   FetchPub(muid)
*   	get a pub given a uid
*   	convert to ISO/GenBank Style
*
*****************************************************************************/
ValNodePtr FetchPub (Int4 muid)
{
	ValNodePtr tmp;

	tmp = MedArchGetPub(muid);
	if (tmp == NULL) return NULL;
	tmp = MedlineToISO(tmp);
	return tmp;
}

ValNodePtr FetchPubPmId (Int4 pmid)
{
	ValNodePtr tmp;

	tmp = MedArchGetPubPmId(pmid);
	if (tmp == NULL) return NULL;
	tmp = MedlineToISO(tmp);
	return tmp;
}

/**********************************************************/
static void GetNameStdFromMl(NameStdPtr namestd, CharPtr token)
{
    Char last[80];
    Char initials[20];
    Char suffix[20];

    if(namestd == NULL || token == NULL)
        return;

    SplitMlAuthorName(token, last, initials, suffix);
    namestd->names[0] = StringSave(last);
    if(initials[0] != '\0')
        namestd->names[4] = StringSave(initials);
    if(suffix[0] != '\0')
        namestd->names[5] = StringSave(suffix);
}

/**********************************************************
 *
 *   MedlineToISO(tmp)
 *       converts a MEDLINE citation to ISO/GenBank style
 *
 **********************************************************/
ValNodePtr MedlineToISO (ValNodePtr tmp)
{
    AuthListPtr alp;
    CitJourPtr  cjp;
    ImprintPtr  ip;
    ValNodePtr  titlenode;
    ValNodePtr  oldnames;
    ValNodePtr  curr;
    ValNodePtr  tmp2;
    ValNodePtr  v;
    CitArtPtr   cap;
    CharPtr     titles[1];
    CharPtr     title;
    Boolean     is_iso;
    Int4        titlenum;
    Int1        types[1];
    AuthorPtr   ap;
    NameStdPtr  namestd;

    if(tmp == NULL || tmp->choice != PUB_Article)
        return(tmp);

    cap = (CitArtPtr) tmp->data.ptrvalue;
    alp = cap->authors;
    if(alp != NULL)
    {
        if(alp->choice == 2)            /* ml style names */
        {
            oldnames = alp->names;
            alp->names = NULL;
            curr = NULL;
            alp->choice = 1;            /* make std names */
            for(tmp2 = oldnames; tmp2 != NULL; tmp2 = tmp2->next)
            {
                curr = get_std_auth((CharPtr) tmp2->data.ptrvalue, ML_REF);
                if(curr == NULL)
                    continue;
                if(alp->names == NULL)
                    alp->names = curr;
                else
                {
                    for(v = alp->names; v->next != NULL;)
                        v = v->next;
                    v->next = curr;
                }
            }
            ValNodeFreeData(oldnames);
        }
        else if(alp->choice == 1)       /* std style names */
        {
            for(tmp2 = alp->names; tmp2 != NULL; tmp2 = tmp2->next)
            {
                ap = (AuthorPtr) tmp2->data.ptrvalue;
                if(ap == NULL || ap->name == NULL || ap->name->choice != 3)
                    continue;
                namestd = NameStdNew();
                GetNameStdFromMl(namestd, ap->name->data);
                MemFree(ap->name->data);
                ap->name->data = namestd;
                ap->name->choice = 2;
            }
        }
    }

    if(cap->from != 1)
        return(tmp);

    /* from a journal - get iso_jta
     */
    cjp = (CitJourPtr) cap->fromptr;
    if(cjp == NULL)
        return(tmp);

    is_iso = FALSE;
    for(titlenode = cjp->title; titlenode != NULL; titlenode = titlenode->next)
    {
        if(titlenode->choice == Cit_title_iso_jta)      /* have it */
        {
            is_iso = TRUE;
            break;
        }
    }
    if(is_iso == FALSE)
    {
        titlenode = cjp->title;
        title = (CharPtr) titlenode->data.ptrvalue;
        titlenum = MedArchGetTitles(titles, types, title,
                                    (Int1) titlenode->choice,
                                    Cit_title_iso_jta, 1);
        if(titlenum != 0)
        {
            MemFree(title);
            titlenode->choice = types[0];
            titlenode->data.ptrvalue = titles[0];
        }
    }
    ip = cjp->imp;                      /* remove Eng language */
    if(StringCmp(ip->language, "Eng") == 0)
    {
        MemFree(ip->language);
        ip->language = NULL;
    }

    return(tmp);
}

Boolean AllUpperCase(CharPtr p )
{
	if (p == NULL) return FALSE;
    while( *p != '\0' )
	{
		if( ! IS_UPPER( *p ) )
			return FALSE;
		p++;
	}
    return TRUE;
}


void SplitMlAuthorName( CharPtr name, CharPtr last, CharPtr initials, CharPtr suffix )
{
    CharPtr	p, p2;
    Int2 i;
	Char sbuf[20], ibuf[20];

    /* Clear the ibuf field and transfer the entire name to 'last',
	excluding leading and trailing spaces */

	if (name == NULL) return;

    ibuf[0] = '\0';
    sbuf[0] = '\0';
	last[0] = '\0';
	initials[0] = '\0';
	suffix[0] = '\0';
    while(*name <= ' ')
	{
		name++;
		if (*name == '\0')
			return;
	}
    StringCpy( last, name );

    for(i=StringLen(last)-1;((i >= 0) && (last[i] <= ' ')); i--)
		last[i] = '\0';

    /* Strip off the last token (initials or name suffix (Jr, Sr, suffix.) */

	p = StringRChr(last, (int)' ');
	if (p != NULL)   /* more than just last name */
	{
	    /* Separate the token from the last name */
		
	    p2 = p + 1;
    	while((p > last) && (*p == ' '))
		{
			*p = '\0';
			p--;
		}

	    /* If the last token is not all upper case, and there are more than
		two tokens, see if the next to the last are initials (upper case) */

    	if ( ! AllUpperCase(p2) && (p = StringRChr( last, (int)' ' )) != NULL )
	    {
		/* We have at least three tokens, is the next to last initials? */

			if( AllUpperCase( p + 1 ) )
			{
			    /* Yes - concatenate the last two tokens as initials */

	    		StringCpy( ibuf, p + 1 );
			    StringCpy( sbuf, p2 );
			    while( p > last && (*p == ' ') )
				{
					*p = '\0';
					p--;
				}
			}
	    }
		
		if (ibuf[0] == '\0') /* Only the last token goes in ibuf */
	    	StringCpy( ibuf, p2 );
	}

	     /* now add periods to ibuf and convert suffix */

	for (p = initials, p2 = ibuf; *p2 != '\0'; p2++, p++)
	{
		*p = *p2;
		if (! IS_LOWER(*(p2 + 1)))   /* watch out for foreign names */
		{
			p++;
			*p = '.';
		}
	}
	*p = '\0';

	if (sbuf[0])
	{
		if ( StringCmp(sbuf, "1d")==0)
			p = StringMove (suffix, "I.");
		else if ( StringCmp(sbuf, "2d")==0)
			p = StringMove (suffix, "II.");
		else if ( StringCmp(sbuf, "3d")==0)
			p = StringMove (suffix, "III.");
		else if ( StringCmp(sbuf, "4th")==0)
			p = StringMove (suffix, "IV.");
		else if ( StringCmp(sbuf, "5th")==0)
			p = StringMove (suffix, "V.");
		else if ( StringCmp(sbuf, "6th")==0)
			p = StringMove (suffix, "VI.");
		else if ( StringCmp(sbuf, "Sr")==0)
			p = StringMove (suffix, "Sr.");
		else if ( StringCmp(sbuf, "Jr")==0)
			p = StringMove (suffix, "Jr.");
		else
			p = StringMove (suffix, sbuf);
	}

	return;
}

ValNodePtr get_std_auth(CharPtr token, Uint1 format)
{
   static CharPtr      auth, eptr;
   ValNodePtr  tmp = NULL;
   NameStdPtr  namestd = NULL;
   AuthorPtr  aup;
   PersonIdPtr pid;
   
	if (token == NULL || *token == '\0') {
		return NULL;
	}
	eptr = token + StringLen(token) - 1;
	for (; eptr > token && *eptr == ' '; eptr--);
	namestd = NameStdNew();
	if (format == PIR_REF || format == GB_REF) {
		for(auth = token; *auth != ',' && *auth != '\0'; auth++);
		if (*auth == ',') {
			*auth = '\0'; 
			if (*(auth+1) != '\0') {
				namestd->names[4] = StringSave(auth + 1);
			}
		}
		namestd->names[0] = StringSave(token);
	} else if (format == PDB_REF) {
		for (auth = eptr; auth > token && *auth != '.'; auth--);
		if (*auth == '.') {
			if (*(auth+1) != '\0' && *(auth+1) != '.') {
				namestd->names[0] = StringSave(auth+1);
			}
			*(auth+1) = '\0';
			namestd->names[4] = StringSave(token);
		} else {
			namestd->names[0] = StringSave(token);
		}
	} else if (format == EMBL_REF || format == SP_REF) {
		for(auth = eptr; *auth != ' ' && auth > token; auth--);
		if (*auth == ' ') {
			if (*(auth-1) == '.') {
				for(auth--; *auth != ' ' && auth > token; auth--);
			}
			if (*auth == ' ') {
				*auth = '\0'; 
				namestd->names[0] = StringSave(token);
				if (*(auth+1) != '\0') {
					namestd->names[4] = StringSave(auth+1);
				}
			} else {
				namestd->names[0] = StringSave(token);
			}
		} else {
			namestd->names[0] = StringSave(token);
		}
	} else if (format == ML_REF) {
                GetNameStdFromMl(namestd, token);
	}
	if (namestd != NULL && namestd->names[0] != NULL) {
		pid = PersonIdNew();
		pid->choice = 2;    /*name*/
		pid->data = namestd;
		aup = AuthorNew();
		aup->name = pid;
		tmp = ValNodeNew(NULL);
		tmp->data.ptrvalue = aup;
	} 
	
	return tmp;
}

Boolean in_press(CitArtPtr cit)
{
	ImprintPtr	imp;
	CitJourPtr	jour;
	
	if (cit == NULL) {
		return TRUE;
	}
	if ((jour = cit->fromptr) == NULL || jour->title == NULL) {
		return TRUE;
	}
	if ((imp = jour->imp) == NULL) {
		return TRUE;
	}
	if (imp->volume == NULL || imp->pages == NULL || imp->date == NULL) {
		return TRUE;
	}
	
	return FALSE;
}
