/*
*
*
* RCS Modification History:
* $Log: spellapi.c,v $
* Revision 6.1  1998/02/03 16:51:12  shavirin
* Removed some memory leaks.
*
* Revision 6.0  1997/08/25 18:40:31  madden
* Revision changed to 6.0
*
* Revision 4.0  1995/07/26 13:55:42  ostell
* force revision to 4.0
*
 * Revision 1.7  1995/05/31  21:12:10  ostell
 * added SpellCheckData() to return user supplied data ptr and eliminate
 * the need for global static variables by calling functions
 *
 * Revision 1.6  1995/05/17  17:59:56  epstein
 * add RCS log revision history
 *
*/

#include <ncbinet.h>
#include "spellapi.h"
#include "objspell.h"

static SpellResponsePtr NetSpellReadAsn PROTO((void));
static Boolean ReestablishNetSpell PROTO((void));
static Boolean NetInit PROTO((void));
static Boolean ForceNetInit PROTO((void));
static Boolean NetFini PROTO((void));
static Boolean GenericReestablishNet PROTO((CharPtr svcName, Boolean showErrs)); 
static NI_HandPtr svcp = NULL;
static AsnIoPtr   asnin = NULL;
static AsnIoPtr   asnout = NULL;
SpellRequestPtr sprp;
SpellResponsePtr spbp;
static Boolean num_attached = 0;
static Boolean reallyFinal = TRUE;
static NI_DispatcherPtr dispatcher;
static Boolean (*myNetInit) PROTO((void));

 
  

/*****************************************************************************
*
*   SpellInit ()
*
*****************************************************************************/

Boolean SpellInit (void)

{
    DataVal    av;

    myNetInit = SpellInit;

    if (! NetInit())
        return FALSE;

    svcp = NI_GenericGetService(dispatcher, NULL, "SPELL", "Spell", TRUE);
    if (svcp == NULL)
    {
        ErrPostEx(SEV_ERROR, 0, 0, "NI_ServiceGet [%s] (%s)", ni_errlist[ni_errno], ni_errtext);
        SpellFini();
        return FALSE;
    }

    asnin = svcp->raip;
    asnout = svcp->waip;

    /**********************************************************/

    sprp = ValNodeNew(NULL);
    sprp->choice = SpellRequest_init;
    SpellRequestAsnWrite (sprp, asnout, NULL);
    AsnIoReset(asnout);
    SpellRequestFree (sprp);

    if ((spbp = NetSpellReadAsn()) == NULL)
    {
        return FALSE;
    }
    else
    {
        spbp->data.ptrvalue = NULL;
        SpellResponseFree (spbp);
        return TRUE;
    }
}

/*****************************************************************************
*
*   SpellFini ()
*
*****************************************************************************/

static Boolean s_SpellFini (void)

{
    Boolean retval = TRUE;

    if (asnout != NULL && asnin != NULL)
    {
        sprp = ValNodeNew(NULL);
        sprp->choice = SpellRequest_fini;
        SpellRequestAsnWrite (sprp, asnout, NULL);
        AsnIoReset(asnout);
        SpellRequestFree (sprp);

        if ((spbp = NetSpellReadAsn()) == NULL)
        {
            retval = FALSE;
        }
        else
        {
            spbp->data.ptrvalue = NULL;
            SpellResponseFree (spbp);
            retval = TRUE;
        }
    }

    NetFini();
    return retval;
}

/* the only thing done here is to suppress errors */

Boolean SpellFini (void)

{
    short erract;
    ErrDesc err;
    Boolean retval;

    ErrGetOpts(&erract, NULL);
    ErrSetOpts(ERR_IGNORE, 0);
    ErrFetch(&err);

    retval = s_SpellFini();

    ErrSetOpts(erract, 0);
    ErrFetch(&err);

    return retval;
}

static ValNodePtr
s_SpellCheck(CharPtr str)
{
    SpellRequestPtr sprp;
    ValNodePtr retval;

    sprp = ValNodeNew(NULL);
    sprp->choice = SpellRequest_spelltext;
    sprp->data.ptrvalue = str;
    SpellRequestAsnWrite(sprp, asnout, NULL);
    AsnIoReset(asnout);
    sprp->data.ptrvalue = NULL;

    if ((spbp = NetSpellReadAsn()) == NULL )
        return NULL;

    if (spbp->choice != SpellResponse_spelltext)
    {
        SpellResponseFree( sprp );
        return NULL;
    }
    retval = (ValNodePtr) spbp->data.ptrvalue;
    spbp->data.ptrvalue = NULL;

    SpellResponseFree(sprp);
    SpellResponseFree(spbp);
 
    return retval;
}

int SpellCheck(CharPtr str, void (*CallBack)(CharPtr))
{
    Int4 i;
    short erract;
    ErrDesc err;
    ValNodePtr vnp = NULL;
    ValNodePtr v;
    int retval = 0;
    CharPtr p;
    CharPtr str2;

    if (str == NULL)
        return 0;

    /* convert the string, converting characters which may not appear in */
    /* a VisibleString to blanks                                         */
    str2 = StringSave(str);
    for (p = str2; *p; p++)
    {
        if (*p < ' ' || *p >= 0x7f)
            *p = ' ';
    }

    for (i = 0; i < SPELL_SERV_RETRIES; i++)
    {
        if (i > 0)
        {
            if (! ReestablishNetSpell())
                break;
        }
    
        ErrGetOpts(&erract, NULL);
        ErrSetOpts(ERR_IGNORE, 0);
        ErrFetch(&err);

        vnp = s_SpellCheck(str2);

        ErrSetOpts(erract, 0);
        if (! ErrFetch(&err))
            break; /* success */
    }

    for (v = vnp; v != NULL; v = v->next)
    {
        if (CallBack != NULL)
            CallBack((CharPtr) v->data.ptrvalue);
        retval++;
    }
    ValNodeFreeData(vnp);

    MemFree(str2);

    return retval;
}

int SpellCheckData(CharPtr str, void (*CallBack)(CharPtr, Pointer), Pointer userdata)
{
    Int4 i;
    short erract;
    ErrDesc err;
    ValNodePtr vnp = NULL;
    ValNodePtr v;
    int retval = 0;
    CharPtr p;
    CharPtr str2;

    if (str == NULL)
        return 0;

    /* convert the string, converting characters which may not appear in */
    /* a VisibleString to blanks                                         */
    str2 = StringSave(str);
    for (p = str2; *p; p++)
    {
        if (*p < ' ' || *p >= 0x7f)
            *p = ' ';
    }

    for (i = 0; i < SPELL_SERV_RETRIES; i++)
    {
        if (i > 0)
        {
            if (! ReestablishNetSpell())
                break;
        }
    
        ErrGetOpts(&erract, NULL);
        ErrSetOpts(ERR_IGNORE, 0);
        ErrFetch(&err);

        vnp = s_SpellCheck(str2);

        ErrSetOpts(erract, 0);
        if (! ErrFetch(&err))
            break; /* success */
    }

    for (v = vnp; v != NULL; v = v->next)
    {
        if (CallBack != NULL)
            CallBack((CharPtr) v->data.ptrvalue, userdata);
        retval++;
    }
    ValNodeFreeData(vnp);

    return retval;
}

int StringInMedline(CharPtr String, void (*CallBack)(CharPtr))
{
    int retval;

    if (! SpellInit())
    {
        return 0;
    }
    retval = SpellCheck(String, CallBack);
    SpellFini();

    return retval;
}


/*****************************************************************************
*
*   NetSpellReadAsn ()
*
*****************************************************************************/

static SpellResponsePtr NetSpellReadAsn(void)
{
    SpellResponsePtr spbp;
    short erract;
    ErrDesc err;

    ErrGetOpts(&erract, NULL);
    ErrSetOpts(ERR_IGNORE, 0);
    ErrFetch(&err); /* clear any pending error */

    spbp = SpellResponseAsnRead(asnin, NULL);

    if (ErrFetch(&err))
    {
        ErrPost(CTX_UNKNOWN, 1, "Null message read from server");
    }
    ErrSetOpts(erract, 0);

    return spbp;
}

/*****************************************************************************
*
*   ReestablishNetSpell ()
*
*****************************************************************************/

static Boolean ReestablishNetSpell(void)
{
    return GenericReestablishNet("Spell", TRUE);
}

/*****************************************************************************
*
*   GenericReestablishNet ()
*
*****************************************************************************/

static Boolean GenericReestablishNet(CharPtr svcName, Boolean showErrs)
{
    Handle mon = NULL;
    Boolean retval;
    CharPtr buf;

    buf = MemNew(2 * StrLen(svcName) + 60);

    if (showErrs) {
        sprintf (buf, "Re-establishing %s Service", svcName);
        mon = MonitorStrNew(buf, 40);
        sprintf (buf, "Requesting %s service", svcName);
        MonitorStrValue(mon, buf);
    }
    NetFini();
    retval = TRUE;

    if (! myNetInit())
    {
        sprintf (buf, "%s get failed; re-contacting dispatcher", svcName);
        MonitorStrValue(mon, buf);
        retval = FALSE;
        if (ForceNetInit())
        { /* successfully established contact w/dispatcher */
            sprintf (buf, "%s get failed; re-requesting %s service",
                     svcName, svcName);
            MonitorStrValue(mon, buf);
            retval = myNetInit();
        }
        else {
            ErrPost(CTX_UNKNOWN, 1, "Unable to re-contact dispatcher");
            if (showErrs) {
                ErrShow();
            }
        }
    }

    MonitorFree(mon);

    if (! retval )
    {
        sprintf (buf, "Unable to re-establish %s service", svcName);
        ErrPost(CTX_UNKNOWN, 1, buf);
        if (showErrs) {
            ErrShow();
        }
    }

    MemFree(buf);
    return retval;
}

/*****************************************************************************
*
*   NetInit ()
*
*****************************************************************************/

static Boolean
NetInit(void)
{
    if (num_attached++ > 0)
        return TRUE;

    return ((dispatcher = NI_GenericInit(NULL, NULL, TRUE, NULL, 0)) != NULL);
}


/*****************************************************************************
*
*   ForceNetInit ()
*
*****************************************************************************/

static Boolean ForceNetInit(void)
{
    Boolean retval;

    reallyFinal = FALSE;
    num_attached = 0; /* force re-attempt to contact dispatcher */
    retval = NetInit();
    reallyFinal = TRUE;

    return retval;
}

/*****************************************************************************
*
*   NetFini ()
*
*****************************************************************************/

static Boolean NetFini(void)
{
   if (num_attached > 0)
        num_attached--;

    if (num_attached == 0)
    {
        NI_ServiceDisconnect(svcp);
        svcp = NULL;
        NI_EndServices (dispatcher);
        dispatcher = NULL;
    }

    return TRUE;
}
