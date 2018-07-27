/*   fakemedarch.c
* ===========================================================================
*
* File Name:  fakemedarch.c
*
* Author:  Chung
*
* Version Creation Date:   04/01/94
*
* $Revision: 6.0 $
*
* File Description: 
*       Fake API for Medline Archive service
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
* ==========================================================================
*/

#include <objmedli.h>
#include <objpub.h>
#include <objall.h>
#include <medarch.h>
#include <objbibli.h>
#include "mapmedarch.h"
#include <objmla.h>

extern CloseDatabaseConnection();
extern InitMedlineDB();
extern MedlineEntryPtr SybaseMedlineEntryGet( Int4 ui );
extern CitArtPtr SybaseMedlinePubGet( Int4 ui );
extern TitleMsgListPtr SybaseMedlineGetTitle( TitleMsgPtr tmsg );
extern int SybaseMedlineCitMatch( CitArtPtr citation );

/*****************************************************************************
*
*   MedArchInit ()
*
*****************************************************************************/

Boolean MedArchInit (void)

{
   return( InitMedlineDB() );
}

/*****************************************************************************
*
*   MedArchFini ()
*
*****************************************************************************/

Boolean MedArchFini (void)

{
    CloseMedlineDB();
}


/*****************************************************************************
*
*   MedArchMedlineEntryListGet
*
*****************************************************************************/

Int4
MedArchMedlineEntryListGet (MedlineEntryPtr PNTR result, Int4 numuid,
                            Int4Ptr uids, Boolean mark_missing)
{
    Int4 j;
    MedlineEntryPtr mep;
    Int4 retval = 0;

    for (j = 0; j < numuid; j++)
    {
        mep = SybaseMedlineEntryGet (uids[j]);

        result[j] = mep;
        if (mep != NULL)
        {
            retval++;
        } else {
            if (mark_missing)
            {
                uids[j] *= -1;
            }
        }
    }
}

/*****************************************************************************
*
*   MedArchMedlineEntryGet
*
*****************************************************************************/

MedlineEntryPtr
MedArchMedlineEntryGet (Int4 uid)
{
    MedlineEntryPtr mep = NULL;

    MedArchMedlineEntryListGet (&mep, 1, &uid, FALSE);
    return mep;
}

/*****************************************************************************
*
*   s_MedArchPubSetEntryGet
*
*****************************************************************************/

static MedlineEntryPtr
s_MedArchPubSetEntryGet (Int4 uid, MedlineEntryPtr lastmep, AsnIoPtr aip,
			 AsnTypePtr atp )
{
    MedlineEntryPtr mep;


    /* write the caller's last entry to a file while waiting for the server */
    /* to compute and send the answer for the current entry                 */

    if (lastmep != NULL && aip != NULL) {
        MedlineEntryAsnWrite (lastmep, aip, atp);
    }

    mep = SybaseMedlineEntryGet (uid);

    return mep;
}


/*****************************************************************************
*
*   MedArchPubSetEntryGet
*
*****************************************************************************/

Int4 MedArchPubSetCreate (AsnIoPtr aip, Int4 numuid, Int4Ptr uids,
                          Boolean mark_missing)
{
    static Boolean alreadyInited = FALSE;
    AsnTypePtr startAtp;
    AsnTypePtr pubSetAtp;
    AsnTypePtr pubSetEAtp;
    AsnModulePtr amp;
    DataVal av;
    Int4 j;
    Int4 i;
    MedlineEntryPtr mep;
    MedlineEntryPtr lastmep = NULL;
    Int4 retval = 0;

    if (! alreadyInited) {
        amp = AsnAllModPtr();
        if (amp == NULL) {
            return 0;
        }
        alreadyInited = TRUE;
        startAtp = AsnTypeFind(amp, "Pub-set");
        pubSetAtp = AsnTypeFind(amp, "Pub-set.medline");
        pubSetEAtp = AsnTypeFind(amp, "Pub-set.medline.E");
    }

    AsnWrite (aip, startAtp, NULL);
    AsnStartStruct (aip, pubSetAtp);

    mep = s_MedArchPubSetEntryGet (uids[j], lastmep, aip, pubSetEAtp);
    MedlineEntryFree (lastmep);
    lastmep = NULL;

    lastmep = mep;
    if (mep != NULL)
            retval++;
    else
       if (mark_missing)
	  uids[j] *= -1;

    if (lastmep != NULL)
    {
        MedlineEntryAsnWrite (lastmep, aip, pubSetEAtp);
        MedlineEntryFree (lastmep);
    }

    AsnEndStruct (aip, pubSetAtp);

    return retval;
}


/*****************************************************************************
*
*   MedArchGetPub
*                          changed by Tatiana 04-14-94
*****************************************************************************/

ValNodePtr MedArchGetPub (Int4 uid)
{
    ValNodePtr pub;
    CitArtPtr  art, tmp;
    
    if ((tmp = SybaseMedlinePubGet(uid)) != NULL) {
    	pub = ValNodeNew(NULL);
		art = AsnIoMemCopy(tmp, (AsnReadFunc) CitArtAsnRead,
						(AsnWriteFunc) CitArtAsnWrite);
    	pub->choice = PUB_Article;
    	pub->data.ptrvalue = art;
    	return pub;
    } else {
    	/* may be an error message here? Tatiana*/
		return NULL;
    }
}


/*****************************************************************************
*
*   MedArchGetTitles
*
*****************************************************************************/

static Int4
s_MedArchGetTitles (CharPtr PNTR titles_found, Int1Ptr title_types_found, CharPtr title_to_lookup, Int1 request_type, Int1 response_type, Int4 max_titles_to_find)

{
    TitleMsgPtr tmsgp;
    TitleMsgListPtr tmlp;
    Int4 i;

    tmsgp = TitleMsgNew();
    tmsgp->type = response_type;
    tmsgp->title = ValNodeNew(NULL);
    tmsgp->title->choice = request_type;
    tmsgp->title->data.ptrvalue = (Pointer) StringSave(title_to_lookup);

    tmlp = SybaseMedlineGetTitle( tmsgp );

    MemFree( tmsgp->title->data.ptrvalue );
    tmsgp->title->data.ptrvalue = NULL;

    for (i = 0, tmsgp = tmlp->titles; i < max_titles_to_find && i < tmlp->num &&
         tmlp != NULL; i++, tmsgp = tmsgp->next)
    {
        titles_found[i] = (CharPtr) tmsgp->title->data.ptrvalue;
        tmsgp->title->data.ptrvalue = NULL; /* for clean free */
        title_types_found[i] = (Int1) tmsgp->type;
    }

    return i;
}

Int4
MedArchGetTitles (CharPtr PNTR titles_found, Int1Ptr title_types_found, CharPtr title_to_lookup, Int1 request_type, Int1 response_type, Int4 max_titles_to_find)
{
    Int4 retval = 0;

    retval = s_MedArchGetTitles(titles_found, title_types_found,
                                    title_to_lookup, request_type,
                                    response_type, max_titles_to_find);
    return retval;
}


/*****************************************************************************
*
*   MedArchCitMatch
*
*****************************************************************************/

Int4 MedArchCitMatch (ValNodePtr pub)
{
    return( SybaseMedlineCitMatch( (CitArtPtr)pub->data.ptrvalue ) );
}
