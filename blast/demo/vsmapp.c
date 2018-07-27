/*   vsmapp.c
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
* File Name:  vsmapp.c
*
* Author:  Jim Ostell
*
* Version Creation Date:   11-29-94
*
* $Revision: 6.0 $
*
* File Description: Demo application which puts up a single window and
*                   enables VSM as a menu choice.
*
*                   Note: SetUpMenus adds VSM as a choice
*                         Procs and datatypes are initialized in main program
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* ==========================================================================
*/

#include <vibrant.h>
#include <vsm.h>
#include <sequtil.h>   /* for sequence load funcs */
#include <objsub.h>

/*** internal menu callback prototypes *****/

static void VSMDrawAbout PROTO((Nlm_PaneL p));
static void SetUpMenus PROTO((WindoW w));
static void QuitVSM PROTO((WindoW w));

/*** Main Program.. just puts up a window ***/

Int2 Main (void)

{
	PaneL         p;
#ifdef WIN_MOTIF
	RecT          r;
#endif
	WindoW        w;
	static CharPtr wtitle = "NCBI DeskTop Demo";

	ErrSetFatalLevel (SEV_MAX);

#ifdef WIN_MAC
	SetUpMenus (NULL);
#endif
	w = FixedWindow (-50, -33, -10, -10, wtitle, QuitVSM);
#ifndef WIN_MAC
	SetUpMenus (w);
#endif
	p = SimplePanel (w, 28 * stdCharWidth, 12 * stdLineHeight, VSMDrawAbout);
	Show (w);
#ifdef WIN_MOTIF
	Select (w);
	ObjectRect (p, &r);
	InsetRect (&r, 3, 3);
	InvalRect (&r);
#endif
	Update ();

	WatchCursor ();

	                      /* initialize data types by calling AsnLoad funcs */

	SetTitle(w, "Loading Parse Trees");
	if (! SeqEntryLoad())
	{
		Message(MSG_ERROR, "SeqEntryLoadFailed");
		ErrShow();
		return 0;
	}
	if (! SubmitAsnLoad())
	{
		Message(MSG_ERROR, "SubmitAsnLoadLoadFailed");
		ErrShow();
		return 0;
	}

			    /* initialize File I/O procs for VSM (vsmfile.h) */
                /* could also add to your menu with VSMAddToMenu() (vsm.h) */
	VSMFileInit();

	SetTitle(w, wtitle);
    ArrowCursor ();

    ProcessEvents ();

	return 0;
}

static void AboutProc (IteM i)
{
	Message(MSG_OK, "About message");
	return;
}

static void QuitVSM (WindoW w)
{
	QuitProgram();
	return;
}

static void QuitProc (IteM i)
{
	QuitProgram();
	return;
}

static void SetUpMenus (WindoW w)
{
	MenU m;
#ifdef WIN_MAC
	m = AppleMenu (NULL);
	CommandItem (m, "About...", AboutProc);
	SeparatorItem (m);
	DeskAccGroup (m);
#endif
	m = PulldownMenu (w, "File");
#ifndef WIN_MAC
	CommandItem (m, "About...", AboutProc);
	SeparatorItem (m);
#endif
	VSMAddToMenu(m, VSM_DESKTOP);    /* add VSM here */
	CommandItem(m, "Quit/Q", QuitProc);

	return;
}

static void Nlm_CenterLine (Nlm_RectPtr rptr, Nlm_CharPtr text, Nlm_FonT fnt)

{
  if (fnt != NULL) {
    Nlm_SelectFont (fnt);
  }
  rptr->bottom = rptr->top + Nlm_LineHeight ();
  Nlm_DrawString (rptr, text, 'c', FALSE);
  rptr->top = rptr->bottom;
}

static void VSMDrawAbout (Nlm_PaneL p)

{
  Nlm_RecT  r;

  Nlm_ObjectRect (p, &r);
  Nlm_InsetRect (&r, 4, 4);
  r.top += 10;
  Nlm_Blue ();
  Nlm_CenterLine (&r, "NCBI DeskTop Demo", Nlm_systemFont);
  r.top += 10;
  Nlm_Red ();
  Nlm_CenterLine (&r, "National Center for Biotechnology Information", Nlm_systemFont);
  r.top += 5;
  Nlm_CenterLine (&r, "National Library of Medicine", Nlm_systemFont);
  r.top += 5;
  Nlm_CenterLine (&r, "National Institutes of Health", Nlm_systemFont);
  r.top += 10;
  Nlm_CenterLine (&r, "(301) 496-2475", Nlm_systemFont);
  r.top += 5;
  Nlm_CenterLine (&r, "info@ncbi.nlm.nih.gov", Nlm_systemFont);
}

