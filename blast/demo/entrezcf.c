/*   entrezcf.c
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
* $Id: entrezcf.c,v 6.1 1998/03/31 21:39:19 vakatov Exp $
*
* File Name:  entrezcf.c
*
* Author:  Gregory Schuler
*
* Version Creation Date:   10-01-94
*
* File Description:
*     Standalone configuration program used to configure the complex
*     configuration mechanism for the Entrez application and the data access
*     library for the Molecular Sequence Data CD-ROM library produced by the NCBI.
*
* Modifications:
* --------------------------------------------------------------------------
* Rev   Date      Name      Description of modification
* ----  --------  --------  ------------------------------------------------
* 1.76  10-01-94  Schuler   Complete re-write of old program for multi-CDs.
* 1.77  10-25-94  Schuler   Fix to handle UPPLER/lower case and ;1.
* 1.78  10-25-94  Schuler   Fix syntax error in Mac-specific portion.
* 1.79  11-08-94  Schuler   Changed default height of editboxes.
* "     "         "         Changed Prompt_New to not use inital size of (0,0).
* "     "         "         Re-enabled final MessageBox to indicate success.
* 1.80  11-08-94  Schuler   Additional changes to avoid zero-width text objects.
* 1.81  11-18-94  Schuler   Fixed bug with not uppercasing directory name if
*                           required by platform.
* "     "         "         Added many log messages to aid debugging efforts.
* 1.82  11-28-94  Schuler   Don't generate default AsnLoad path if not needed.
* 1.83  11-30-94  Schuler   Don't create zero-width EditBoxes (for X warning)
* "     "         "         Workaround for 4 CD-ROM scenario
* "     "         "         Added "For your information" section on last page
* 1.84  12-01-94  Kans      Added function prototypes as needed
* "     "         "         Fixed bug in "For your information section"
* 1.85  12-01-94  Schuler   Adjusted some line heights to fix visual glitch
* 1.86  12-09-94  Schuler   Started adding screens to configure network
* 1.89  12-15-94  Kans      Replaced table with document object
* 1.90  12-22-94  Epstein   Completed most of network configuration
* 1.91  12-22-94  Epstein   Ran through 'indent' to cleanup white-space
* 1.92  12-23-94  Epstein   Completed network configuration
*
* $Log: entrezcf.c,v $
* Revision 6.1  1998/03/31 21:39:19  vakatov
* Check for the old-fashioned NCBI dispatcher interface availability
* and forcibly set networking interface to "eNII_Dispatcher"
*
* Revision 6.0  1997/08/25 18:19:44  madden
* Revision changed to 6.0
*
* Revision 5.4  1997/08/25 16:20:44  kans
* checks for .l60 through .l69 parse tables
*
* Revision 5.3  1996/07/23 21:28:09  epstein
* change button size to accomodate (relatively) recent Vibrant sizing changes
*
 * Revision 5.2  1996/06/07  20:52:17  vakatov
 * Moved the RealizeWindow() call to the very end of the ControlPanel_Construct()
 *
 * Revision 5.1  1996/06/07  15:23:08  kans
 * uselocalasnload now looks for .l50 through .l59
 *
 * Revision 5.0  1996/05/28  14:06:48  ostell
 * Set to revision 5.0
 *
 * Revision 4.14  1996/04/17  13:38:09  epstein
 * switch to 6 CDs (change per Schuler)
 *
 * Revision 4.13  1996/04/11  13:40:25  kans
 * *** empty log message ***
 *
 * Revision 4.12  1996/04/10  15:58:23  kans
 * changed page2 release compare value to 22
 *
 * Revision 4.11  1996/04/09  17:25:09  kans
 * _dkey was being indexed by DType_MAX, which is too large
 *
 * Revision 4.10  1996/03/28  21:25:07  epstein
 * conversion to 6 CD-ROMs
 *
 * Revision 4.9  1996/03/08  21:47:57  epstein
 * added workaround for brain-damaged OSF/1 compiler/system
 *
 * Revision 4.8  1996/02/21  23:31:33  kans
 * changes to allow nentrezcf compilation on NT (DV)
 *
 * Revision 4.7  1996/02/15  22:03:18  kans
 * changed platform symbol back to OS_NT
 *
 * Revision 4.6  1996/02/14  16:54:59  kans
 * calls Nlm_GetDriveType function hidden in vibutils.c
 *
 * Revision 4.5  1995/12/13  17:19:46  kans
 * final revisions for 5 CDs (GS)
 *
*
* ==========================================================================
*/

#define REVISION_STR  "$Revision: 6.1 $"



#ifdef NETWORK_SUPPORTED
char            _program[] = "NetEntCf";

char            _email_contact[] = "net-info@ncbi.nlm.nih.gov";

#else
char            _program[] = "EntrezCf";

char            _email_contact[] = "info@ncbi.nlm.nih.gov";

#endif

char            _this_file[] = __FILE__;

#define THIS_MODULE _program
#define THIS_FILE _this_file

#include <ncbi.h>
#include <vibrant.h>
#include <document.h>

#ifdef  _NEW_CdEntrez_
#define _NEW_CdEntrez_
#endif

#include <accentr.h>
#include <cdromlib.h>           /* shouldn't need this (?) */
#include <cdnewlib.h>


#ifdef WIN_MSWIN
/* undef some things from Vibrant that conflict with windows.h  */
#undef HANDLE
#undef Beep
#undef EqualRgn
#undef FrameRgn
#undef InvertRgn
#undef LineTo
#undef PaintRgn
#undef StartPage
#undef EndPage
#undef DrawText
#undef FrameRect
#undef InvertRect
#undef UnionRect
#undef OffsetRect
#undef EqualRect
#undef PtInRect
#undef MoveTo
#undef CreateFont
#undef ChooseFont

#ifndef NETWORK_SUPPORTED
#ifdef WIN16
extern Nlm_Uint2 Nlm_GetDriveType (int driveNumber);
#else
extern Nlm_Uint2 Nlm_GetDriveType (char* path);
#endif
#ifndef DRIVE_REMOTE
#define DRIVE_REMOTE 4
#endif
#ifndef DRIVE_CDROM
#define DRIVE_CDROM 5
#endif
#endif

#endif                          /* WIN_MSWIN */

#ifdef NETWORK_SUPPORTED
#include <ncbinet.h>
#include <ni_net.h>
#include <netentr.h>

#ifdef NETP_INET_NEWT
#define SIN_ADDR        sin_addr.S_un.S_addr
#else
#define SIN_ADDR        sin_addr
#endif

#endif                          /* NETWORK_SUPPORTED */





/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*\
	     MACROS
\*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

#ifdef WIN_MAC
#define LARGE_NAME "Geneva"
#define LARGE_SIZE 14
#define SMALL_NAME "Monaco"
#define SMALL_SIZE 9
#else

#ifdef WIN_MSWIN
#define LARGE_NAME "Helvetica"
#define LARGE_SIZE -14
#define SMALL_NAME "MS Sans Serif"
#define SMALL_SIZE -10
#else
#define LARGE_NAME "Helvetica"
#define LARGE_SIZE 14
#define SMALL_NAME "Helvetica"
#define SMALL_SIZE 11
#endif

#endif

#define Y _yunit
#define X _xunit

#define Volume_MAX  6
#define Device_MAX  6
#define DType_MAX   6

#define WIN_ySIZE  (50*Y)
#define WIN_xSIZE  (54*X)
#define DFLT_xBUTTON (20*X)
#define DFLT_yBUTTON (6*Y)
#define DFLT_yEDIT (int)(Nlm_dialogTextHeight)
#define dyEDIT ((DFLT_yEDIT - 2*Y)/2)

#ifndef MsgBeep
#define MsgBeep Nlm_Beep
#endif

#ifdef OS_MAC
#define CD_InsVolname  TRUE
#else
#define CD_InsVolname  FALSE
#endif

#define FORWARD  (+1)
#define BACKWARD (-1)

#define SERVICE_TYPE "Entrez"

enum {
    Dev_CD, Dev_HD, Dev_ND
};


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*\
	     TYPE DEFINTIONS AND FUNCTION PROTOTYPES
\*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

typedef Nlm_Handle Control;

/*---------- CtrlGrp ----------*/
typedef struct CtrlGrpItem {
    Control         ctrl;
		    Boolean vis;
}

		CtrlGrpItem;

typedef struct CtrlGrp {
    CtrlGrpItem    *list;
    int             count;
    int             slots;
    Boolean         vis;
}

		CtrlGrp;

CtrlGrp        *CtrlGrp_Construct (CtrlGrp * grp);

CtrlGrp        *CtrlGrp_Destruct (CtrlGrp * grp);

#define CtrlGrp_New()  CtrlGrp_Construct((CtrlGrp*)MemNew(sizeof(CtrlGrp)))
#define CtrlGrp_Free(x) MemFree((void*)CtrlGrp_Destruct(x))
void            CtrlGrp_AddControl (CtrlGrp * grp, Control ctrl);

void            CtrlGrp_ShowControl (CtrlGrp * grp, Control ctrl, Boolean vis);

void            CtrlGrp_ShowGroup (CtrlGrp * grp, Boolean vis);

/*---------- PropPage ----------*/

typedef enum PropPageMsg {
    MSG_none = 0,
    MSG_Create,
    MSG_Destroy,
    MSG_EnterPage,
    MSG_LeavePage,
    MSG_IsActive,
    MSG_Commit
}

		PropPageMsg;

struct PropPage;

typedef int     (*PropPageProc) (struct PropPage *, PropPageMsg msg, int param);

int             PropPage_DefaultProc (struct PropPage *, PropPageMsg msg, int param);

typedef struct PropPage {
    PrompT          title_prompt;
    PropPageProc    proc;
    CtrlGrp        *grp;
    char           *section;
}

		PropPage;

PropPage       *PropPage_Construct (PropPage * ppage, const char *title, const char *section, PropPageProc proc);

PropPage       *PropPage_Destruct (PropPage * ppage);

int             PropPage_SendMessage (PropPage * ppage, PropPageMsg msg, int param);

void            PropPage_RegisterControl (PropPage * ppage, Control ctrl);

int             Page0_PageProc (PropPage * ppage, PropPageMsg msg, int param);

int             Page1_PageProc (PropPage * ppage, PropPageMsg msg, int param);

int             Page2_PageProc (PropPage * ppage, PropPageMsg msg, int param);

int             Page3_PageProc (PropPage * ppage, PropPageMsg msg, int param);

int             Page4_PageProc (PropPage * ppage, PropPageMsg msg, int param);

int             Page5_PageProc (PropPage * ppage, PropPageMsg msg, int param);

int             Page6_PageProc (PropPage * ppage, PropPageMsg msg, int param);

int             Page7_PageProc (PropPage * ppage, PropPageMsg msg, int param);

int             Page8_PageProc (PropPage * ppage, PropPageMsg msg, int param);

int             Page9_PageProc (PropPage * ppage, PropPageMsg msg, int param);

int             Page10_PageProc (PropPage * ppage, PropPageMsg msg, int param);

int             Page11_PageProc (PropPage * ppage, PropPageMsg msg, int param);

int             Page12_PageProc (PropPage * ppage, PropPageMsg msg, int param);


/*---------- ControlPanel ----------*/
typedef struct ControlPanel {
    WindoW          win;
    PaneL           panel;
    RecT            panel_rect;
}

		ControlPanel;

#define ControlPanel_New()    ControlPanel_Construct(MemNew(sizeof(ControlPanel)))
#define ControlPanel_Free(x)  MemFree(ControlPanel_Destruct(x))
ControlPanel   *ControlPanel_Construct (ControlPanel * cp);

void           *ControlPanel_Destruct (ControlPanel * cp);

void            ControlPanel_OnClose (ControlPanel * cp);

void            ControlPanel_OnOkay (ControlPanel * cp);

void            ControlPanel_OnCancel (ControlPanel * cp);

Boolean         ControlPanel_GotoPageNum (ControlPanel * cp, int pnum);

Boolean         ControlPanel_GotoNextPage (ControlPanel * cp);

Boolean         ControlPanel_GotoPrevPage (ControlPanel * cp);

PropPage       *ControlPanel_GetCurrentPage (ControlPanel * cp);

int             ControlPanel_GetNextPageNum (ControlPanel * cp);

int             ControlPanel_GetPrevPageNum (ControlPanel * cp);

int             DoTestPath (const char *path, int vol_expected);

Boolean         DoCdEntrezInit (void);

Boolean         DoCdEntrezFini (void);

/*---------- FileToCopy ----------*/
typedef struct EntrezRelInfo {
    int             rel_minor;
    int             rel_major;
    CdDate          rel_date;
    int             types;
    long            docs[4];
}

		EntrezRelInfo;

static EntrezRelInfo _release_cd;

static EntrezRelInfo _release_net;

static CharPtr  _adminInfo;


/*---------- FileToCopy ----------*/
typedef struct FileToCopy {
    char            filename[16];
    long            filesize;
    int             cd_num;
    int             dir_num;
}

		FileToCopy;

#define FileToCopy_New(a,b,c,d) FileToCopy_Construct(MemGet(sizeof(FileToCopy),MGET_ERRPOST),a,b,c,d)
FileToCopy     *FileToCopy_Construct (FileToCopy * ftc, const char *name, long size, int cd, int dir);

#define FileToCopy_Free(a) (FileToCopy*)MemFree((void*)(a))
FileToCopy     *FileToCopy_Destruct (FileToCopy * ftc);

Boolean         FileToCopy_RemoveOldCopy (FileToCopy * ftc, const char *dst_folder);

Boolean         FileToCopy_PerformCopy (FileToCopy * ftc, CdRomInfo * info, const char *src, const char *dst, Monitor * mon);

#define VBUFSIZE  (16L * (long)KBYTE)
static char    *_vbuf_in;

static char    *_vbuf_out;

static char     _logfile[256];

/*---------- Control Utilties ----------*/

ButtoN          Button_New (CtrlGrp * grp, int x, int y, int cx, int cy, char *text, BtnActnProc proc);

ButtoN          CheckBox_New (CtrlGrp * grp, int x, int y, int cx, int cy, char *text, BtnActnProc proc);

#define CheckBox_Check(x,y)     SetStatus(x,y)
#define CheckBox_IsChecked(x)   GetStatus(x)



typedef GrouP   GroupBox;

GroupBox        GroupBox_New (CtrlGrp * grp, int x, int y, int cx, int cy, char *title, GrpActnProc proc);

GroupBox        HiddenGroupBox_New (CtrlGrp * grp, int x, int y, int cx, int cy, GrpActnProc proc);

typedef Nlm_ButtoN Radio;

Radio           Radio_New (CtrlGrp * grp, int x, int y, int cx, int cy, char *title, GroupBox gbox);

typedef Nlm_TexT EditBox;

EditBox         EditBox_New (CtrlGrp * grp, int x, int y, int cx, int cy, char *text, TxtActnProc proc);

#define EditBox_GetText(a,b,c) GetTitle(a,b,c)
#define EditBox_SetText(a,b)   SetTitle(a,(char*)(b))
int             EditBox_SaveText (EditBox edit, char **pstr);

int             EditBox_GetInt (EditBox edit);

void            EditBox_SetInt (EditBox edit, int value);

typedef Nlm_LisT ListBox;

ListBox         ListBox_New (CtrlGrp * grp, int x, int y, int cx, int cy, LstActnProc proc);

#define ListBox_GetCount(x)  (int)CountItems(x)
int             ListBox_AddString (ListBox lbox, char *str);

#define ListBox_ResetContent(x)  Reset(x)
#define ListBox_GetCurSel(a) (int)(GetValue(a) -1)
#define ListBox_SetCurSel(a,b) SetValue(a,(Int2)((b)+1))
#define LB_ERR  (-1)

typedef Nlm_PopuP ComboBox;

ComboBox        ComboBox_New (CtrlGrp * grp, int x, int y, int cx, int cy, PupActnProc proc);

#define ComboBox_Delete(x)  Remove(x)
int             ComboBox_AddString (ComboBox, const char *str);

#define ComboBox_ResetContent(x)  Reset(x)
#define ComboBox_GetCurSel(a) (int)(GetValue(a) -1)
#define ComboBox_SetCurSel(a,b) SetValue(a,(Int2)((b)+1))
#define CB_ERR  (-1)

PrompT          Prompt_New (CtrlGrp * grp, int x, int y, int cx, int cy, const char *str, int just);

#define LPrompt_New(a,b,c,d,e,f)  Prompt_New(a,b,c,d,e,f,'l')
#define RPrompt_New(a,b,c,d,e,f)  Prompt_New(a,b,c,d,e,f,'r')

void            MultiPrompt_New (CtrlGrp * grp, int x, int y, const char **slist, int count);

/*---------- Misc. Utilties ----------*/

#define FileExists(x) (FileLength((char*)(x)) >0)


/* ----- About box stuff ----- */
static void     DrawAbout (PaneL p);

static void     QuitItemProc (IteM i);

static void     AboutProc (IteM i);

static void     CloseWindowProc (WindoW w);

static void     CloseAboutPanelProc (PaneL p, PoinT pt);

static void     CenterString (RectPtr rptr, CharPtr text, FonT fnt, Int2 inc);


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*\
	     VARIABLES
\*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

static EntrezInfo *_entrez_info;

static int      _xunit;

static int      _yunit;

static FonT     _font1;

static FonT     _font2;

static ControlPanel *_cp;

static WindoW   _win;

static char     _rcfile[] = "ncbi";

static char     _rcfile2[] = "entrezcf";

static char     _cdentrez[] = "CdEntrez";

static char     _device1[] = "CdEntrez.Device.1";

static char     _device2[] = "CdEntrez.Device.2";

static char     _device3[] = "CdEntrez.Device.3";

static char     _device4[] = "CdEntrez.Device.4";

static char     _device5[] = "CdEntrez.Device.5";

static char     _device6[] = "CdEntrez.Device.6";

static char    *_cddev[] = {_device1, _device2, _device3, _device4, _device5, _device6};

static char     _default[] = "(default)";

char            _cd_drive[] = "CD-ROM drive";

#ifdef OS_MAC
static char     _folder[] = "folder";

#else
static char     _folder[] = "directory";

#endif

static char    *_devtype_val[] = {"CD", "HARDDISK", "NET"};

int             _volume_ct;     /* number of volumes (CD-ROMs) */

int             _vol_dtyp[Volume_MAX];  /* type of data source for each
					 * volume */

int             _dtyp_vols[4];  /* number of volumes for each device type */

int             _device_ct;     /* total number of devices */

static Boolean  _config_ok;

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*\
	     Main Entry Point
\*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

Int2 Main ()
{
    char           *p;

#ifdef OS_UNIX_OSF1
    /* incredibly enough, if you don't do the following command then we'll
       die on CacheAppParam().  Lots of other things will work here, e.g.
       sleep(1), MemNew(16), etc.  This problem was observed under OSF1
       2.0; the compiler version number is unknown */
    MemNew(1);
#endif

    /* cache writes to parameter files, for faster performance */
    CacheAppParam(TRUE);

/** TO DO: ensure that the user has write priveleges for the directory in which
	the entrezcf.log is being written ... **/
    ProgramPath (_logfile, sizeof _logfile);
    if ((p = strrchr (_logfile, DIRDELIMCHR)) != NULL)
	++p;
    else
	p = _logfile;
    strcpy (p, "entrezcf.log");
    ErrSetLogfile (_logfile, 0);
    ErrSetLogLevel (SEV_WARNING);       /* to avoid FileOpen info messages */
    ErrSetOptFlags (EO_LOGTO_USRFILE);
    TransientSetAppParam (_rcfile, _cdentrez, "LogVerbose", "YES");

    ErrLogPrintf ("%s Log File\n\n", _program);
    ErrLogPrintf ("    If you encounter difficulties configuring Entrez, please send this file\n");
    ErrLogPrintf ("    by e-mail to \"%s\".  Also include your name, phone\n", _email_contact);
    ErrLogPrintf ("    number, and a description of your computer system (type of computer,\n");
    ErrLogPrintf ("    operating system, types of peripherals, network setup, etc.).\n\n");
    ErrLogPrintf ("    [%s]\n", REVISION_STR);
    ErrLogPrintf ("-----------------------------------------------------------------------------\n\n");

    ErrSetFatalLevel (SEV_MAX);
    ErrSetMessageLevel (SEV_ERROR);

    _font1 = GetFont (LARGE_NAME, LARGE_SIZE, 1, 0, 0, 0);
    _font2 = GetFont (SMALL_NAME, SMALL_SIZE, 0, 0, 0, 0);
    /**SelectFont(_font2);**/
    _xunit = CharWidth ('0');
    _yunit = LineHeight () / 2;
    _font1 = _font1;
    _font2 = _font2;

#ifdef NETWORK_SUPPORTED
    /* It is valid for the old-fashioned NCBI dispatcher only... */
    if ( !NI_IsInterfaceSupported(eNII_Dispatcher) ) {
      ErrPostEx(SEV_FATAL, 0, 0,
                "The eNII_Dispatcher interface is not supported.\n"
                "Please quit the configuration program and report.");
    }
    NI_SetInterface(eNII_Dispatcher);
#endif /* NETWORK_SUPPORTED */


#ifdef WIN_MAC                  /* Add About... command to apple menu */
    {
	char            buffer[32];

	MenU            menu;

	menu = AppleMenu (NULL);
	sprintf (buffer, "About %s...", _program);
	CommandItem (menu, buffer, AboutProc);
	SeparatorItem (menu);
	DeskAccGroup (menu);
	menu = PulldownMenu (NULL, "File");
	CommandItem (menu, "Quit/Q", QuitItemProc);
    }
#endif

#ifdef WIN_MSWIN                /* Use CoreLib alerts instead of Vibrant */
    SetMessageHook (NULL);
#endif

    _cp = ControlPanel_New ();
    ProcessEvents ();
    ControlPanel_Free (_cp);

    return 0;
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*\
	     ControlPanel
\*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

static void     _OnClose (WindoW win);

static void     _OnOkay (ButtoN);

static void     _OnCancel (ButtoN);

static void     _OnNext (ButtoN btn);

static void     _OnPrev (ButtoN btn);

static PropPage _page[16];

static int      _page_ct;

static int      _page_num = -1;

static GrouP    _group;

static ButtoN   _btn_prev;

static ButtoN   _btn_next;

static ButtoN   _btn_okay;

static ButtoN   _btn_cancel;

struct PPInfo {
    PropPageProc    proc;
    const char     *sect;
    const char     *title;
};

struct PPInfo   _ppinfo[] = {
    Page0_PageProc, "Start", "Entrez Configuration Utility",
    Page1_PageProc, "Paths", "NCBI System Files",
    Page2_PageProc, "Cd-Sources", "Entrez CD-ROM Data Sources",
    Page3_PageProc, "Cd-CDROM", "Setup for CD-ROM Drives",
    Page4_PageProc, "Cd-Hard", "Setup for Hard Disks",
    Page5_PageProc, "Cd-LAN", "Setup for LAN Fileserver",
    Page6_PageProc, "Cd-Copy", "Copy Index Files to Hard Disk",
    Page7_PageProc, "Cd-Copy2", "Copy Files to Hard Disk",
    Page8_PageProc, "Cd-Opt", "Entrez Optimization",
    Page9_PageProc, "Net-Prefs", "Entrez for Internet Setup",
    Page10_PageProc, "Net-Disp", "Dispatcher Internet Address",
    Page11_PageProc, "Net-Serv", "Entrez Service Selection",
    Page12_PageProc, "Finish", "Configuration Complete!"
};

ControlPanel   *
ControlPanel_Construct (ControlPanel * cp)
{
    if (cp != NULL) {
	int             cx = WIN_xSIZE;

	int             cy = WIN_ySIZE;

	_cp = cp;
	_win = FixedWindow (-50, -33, (Int2) WIN_xSIZE, (Int2) WIN_ySIZE, _program, _OnClose);
	if (_win != NULL) {
	    PropPage       *ppage = _page;

	    struct PPInfo  *p;

	    int             y, i, k;

	    int             cy = DFLT_yBUTTON;

	    int             cx = DFLT_xBUTTON;

	    y = WIN_ySIZE - DFLT_yBUTTON - 2 * Y;
	    _btn_cancel = Button_New (NULL, 4 * X, y, 12 * X, cy, "Cancel", _OnCancel);
	    _btn_prev = Button_New (NULL, 31 * X, y, 9 * X, cy, "<< Prev", _OnPrev);
	    _btn_next = Button_New (NULL, 41 * X, y, 9 * X, cy, "Next >>", _OnNext);
	    SelectFont (_font1);
	    for (i = k = 0, p = _ppinfo; i < DIM (_ppinfo); ++i, ++p) {
		if (PropPage_Construct (&_page[k], p->title, p->sect, p->proc))
		    k++;
	    }
	    _page_ct = k;

	    ControlPanel_GotoPageNum (cp, 0);
            RealizeWindow (_win);
	    Show (_win);
	    cp->win = _win;
	}
    }
    return cp;
}


void           *
ControlPanel_Destruct (ControlPanel * cp)
{
    if (cp != NULL) {
	Remove (cp->win);
    }
    return (void *) cp;
}


void 
ControlPanel_OnClose (ControlPanel * cp)
{
    ControlPanel_OnCancel (NULL);
}


#ifdef NETWORK_SUPPORTED
static void     HandleNetQuit (void);

#endif                          /* NETWORK_SUPPORTED */


static void
MyQuitProgram (void)
{

#ifdef NETWORK_SUPPORTED
    HandleNetQuit ();
#endif                          /* NETWORK_SUPPORTED */

    QuitProgram ();
}

void 
ControlPanel_OnOkay (ControlPanel * cp)
{
    int             i;

    _config_ok = TRUE;

    WatchCursor ();
    for (i = 0; i < _page_ct; ++i) {
	if (!PropPage_SendMessage (&_page[i], MSG_Commit, 0)) {
	    ErrPostEx (SEV_INFO, 0, 0, "Error: Page %d Commit\n", i);
	    _config_ok = FALSE;
	    /* break; */
	}
    }

    ErrSetLogfile (NULL, 0);

#ifndef _DEBUG
    if (_config_ok) {
	if (!FileRemove (_logfile))
	    ErrPostEx (SEV_INFO, 0, 0, "Attempt to remove %s failed", _logfile);
    }
#endif

    ArrowCursor ();
    MyQuitProgram ();
}

void 
ControlPanel_OnCancel (ControlPanel * cp)
{
    ErrLogPrintf ("\n[[ CANCEL ]]\n");
    MyQuitProgram ();
}

Boolean 
ControlPanel_GotoPageNum (ControlPanel * cp, int pnum)
{
    char            buffer[48];

    if (pnum < 0 || pnum == _page_num)
	return FALSE;

    if (_page_num >= 0) {
	PropPage       *ppage = &_page[_page_num];

	Hide (ppage->title_prompt);
	CtrlGrp_ShowGroup (ppage->grp, FALSE);
    }
    if (pnum >= 0) {
	PropPage       *ppage = &_page[pnum];

	GetTitle (ppage->title_prompt, buffer, sizeof buffer);
	ErrLogPrintf ("\n[[%s]]\n", buffer);
	(*ppage->proc) (ppage, MSG_EnterPage, (pnum > _page_num) ? FORWARD : BACKWARD);
	Show (ppage->title_prompt);
	CtrlGrp_ShowGroup (ppage->grp, TRUE);
    }
    _page_num = pnum;

    if (ControlPanel_GetPrevPageNum (cp) >= 0)
	Enable (_btn_prev);
    else
	Disable (_btn_prev);

    if (ControlPanel_GetNextPageNum (cp) > 0)
	Enable (_btn_next);
    else
	Disable (_btn_next);
    return TRUE;
}

PropPage       *
ControlPanel_GetCurrentPage (ControlPanel * cp)
{
    return (_page_num < 0) ? NULL : &_page[_page_num];
}

int 
ControlPanel_GetNextPageNum (ControlPanel * cp)
{
    int             i;

    for (i = _page_num + 1; i < _page_ct; ++i) {
	if (_page[i].proc (&_page[i], MSG_IsActive, 0))
	    return i;
    }
    return -1;
}

int 
ControlPanel_GetPrevPageNum (ControlPanel * cp)
{
    int             i;

    for (i = _page_num - 1; i >= 0; --i) {
	if (_page[i].proc (&_page[i], MSG_IsActive, 0))
	    return i;
    }
    return -1;
}

Boolean 
ControlPanel_GotoPrevPage (ControlPanel * cp)
{
    Boolean         success;

    WatchCursor ();
    if (_page_num >= 0) {
	PropPage       *ppage = &_page[_page_num];

	if (!(*ppage->proc) (ppage, MSG_LeavePage, BACKWARD)) {
	    ArrowCursor ();
	    return FALSE;
	}
    }
    success = ControlPanel_GotoPageNum (cp, ControlPanel_GetPrevPageNum (cp));
    ArrowCursor ();
    return success;
}

Boolean 
ControlPanel_GotoNextPage (ControlPanel * cp)
{
    Boolean         success;

    WatchCursor ();
    if (_page_num >= 0) {
	PropPage       *ppage = &_page[_page_num];

	if (!(*ppage->proc) (ppage, MSG_LeavePage, FORWARD)) {
	    ArrowCursor ();
	    return FALSE;
	}
    }
    success = ControlPanel_GotoPageNum (cp, ControlPanel_GetNextPageNum (cp));
    ArrowCursor ();
    return success;
}


/* Vibrant Hook Procs */

static void 
_OnClose (WindoW win)
{
    ControlPanel_OnCancel (_cp);
}

static void 
_OnOkay (ButtoN btn)
{
    ControlPanel_OnOkay (_cp);
}

static void 
_OnCancel (ButtoN btn)
{
    ControlPanel_OnCancel (_cp);
}

static void 
_OnNext (ButtoN btn)
{
    ControlPanel_GotoNextPage (_cp);
}

static void 
_OnPrev (ButtoN btn)
{
    ControlPanel_GotoPrevPage (_cp);
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*\
	     PAGE 0 --  Title
\*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

static const char *_p0_text1[] = {
    "National Center for Biotechnology Information",
    "National Library of Medicine, NIH",
    "8600 Rockville Pike,  Bethesda, MD 20894",
    "301-496-2475     FAX: 301-480-9241",
    "entrez@ncbi.nlm.nih.gov"
};

static const char *_p0_text2[] = {
    "Use this program to configure the Entrez application."
};

static const char *_p0_text3[] = {
    "Use this program to configure either the CD-ROM",
    "or Internet versions of the Entrez application."
};

static Boolean  _config_cd;

static Boolean  _config_net;

static Boolean  _advanced_settings;

static ButtoN   _check_cd;

static ButtoN   _check_net;

static ButtoN   _check_advanced;

static Boolean  Page0_Create (PropPage * p);

static Boolean  Page0_LeavePage (PropPage * p, int direction);

static Boolean  Page0_Commit (PropPage * p);

static void     Page0_OnConfigOpt (GrouP grp);


int 
Page0_PageProc (PropPage * ppage, PropPageMsg msg, int param)
{
    switch (msg) {
	case MSG_Create:
	return Page0_Create (ppage);
    case MSG_LeavePage:
	return Page0_LeavePage (ppage, param);
    }
    return PropPage_DefaultProc (ppage, msg, param);
}


static Boolean 
Page0_Create (PropPage * p)
{

#ifdef NETWORK_SUPPORTED
    GroupBox        gbox;

    MultiPrompt_New (p->grp, 4 * X, 6 * Y, _p0_text3, DIM (_p0_text3));
    gbox = GroupBox_New (p->grp, 10 * X, 14 * Y, 30 * X, 10 * Y, "Configure:", NULL);
    _check_cd = CheckBox_New (p->grp, 12 * X, 17 * Y, 20 * X, 2 * Y, "Entrez for CD-ROM", NULL);
    _check_net = CheckBox_New (p->grp, 12 * X, 20 * Y, 20 * X, 2 * Y, "Entrez for Internet", NULL);
    _config_cd = GetAppParamBoolean (_rcfile2, p->section, "CDROM", FALSE);
    CheckBox_Check (_check_cd, _config_cd);
    _config_net = GetAppParamBoolean (_rcfile2, p->section, "Internet", TRUE);
    CheckBox_Check (_check_net, _config_net);
#else
		    MultiPrompt_New (p->grp, 4 * X, 6 * Y, _p0_text1, DIM (_p0_text1));
    MultiPrompt_New (p->grp, 4 * X, 18 * Y, _p0_text2, DIM (_p0_text2));
    _config_cd = TRUE;
    _config_net = FALSE;
#endif

    _advanced_settings = GetAppParamBoolean (_rcfile2, p->section, "Advanced", FALSE);
    _check_advanced = CheckBox_New (p->grp, 10 * X, 27 * Y, 30 * X, 3 * Y, "Show advanced settings", NULL);
    CheckBox_Check (_check_advanced, _advanced_settings);

    LPrompt_New (p->grp, 4 * X, 32 * Y, -1, 2 * Y, "Press the \"Next\" button to continue");
    return TRUE;
}

static Boolean 
Page0_LeavePage (PropPage * p, int direction)
{

#ifdef NETWORK_SUPPORTED
    _config_cd = CheckBox_IsChecked (_check_cd);
    _config_net = CheckBox_IsChecked (_check_net);
    if (!(_config_cd || _config_net)) {
	MsgAlert (KEY_OK, SEV_ERROR, _program, "To continue, you must indicate which "
		  "version(s) of Entrez you would like to configure");
	return FALSE;
    }
    SetAppParamBoolean (_rcfile2, p->section, "CDROM", _config_cd);
    SetAppParamBoolean (_rcfile2, p->section, "Internet", _config_net);
#endif

    _advanced_settings = CheckBox_IsChecked (_check_advanced);

    SetAppParamBoolean (_rcfile2, p->section, "Advanced", _advanced_settings);
    return TRUE;
}

static Boolean 
Page0_Commit (PropPage * p)
{
    return TRUE;
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*\
	     PAGE 1 --  NCBI System Files
\*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

static const char *_p1_text[] = {
    "Entrez needs to use some system files, which are normally ",
    "located in folders called \"asnload\" and \"data\" within ",
    "the Entrez software folder.",
};

#if (defined(OS_DOS) || defined(OS_MAC))
#define AsnLoad_RequiredByPlatform 1
#else
#define AsnLoad_RequiredByPlatform 0
#endif

static Boolean  _p1_required;

static EditBox  _editDataPath;

static EditBox  _editAsnLoadPath;

static Boolean  Page1_Create (PropPage * p);

static Boolean  Page1_LeavePage (PropPage * p, int direction);

static Boolean  Page1_Commit (PropPage * p);

typedef int     (*PathOkayProc) (const char *path);

static Boolean  InitPathSetting (EditBox edit, const char *dirname, PathOkayProc isokay);

static int      IsAsnloadPathOkay (const char *path);

static int      IsDataPathOkay (const char *path);

static Boolean  ToolkitPathsOkay (void);


int 
Page1_PageProc (PropPage * ppage, PropPageMsg msg, int param)
{
    switch (msg) {
	case MSG_Create:
	return Page1_Create (ppage);
    case MSG_LeavePage:
	return Page1_LeavePage (ppage, param);
    case MSG_IsActive:
	return (_advanced_settings || _p1_required);
    case MSG_Commit:
	return Page1_Commit (ppage);
    }
    return PropPage_DefaultProc (ppage, msg, param);
}


static Boolean 
Page1_Create (PropPage * p)
{
    MultiPrompt_New (p->grp, 4 * X, 6 * Y, _p1_text, DIM (_p1_text));

#if AsnLoad_RequiredByPlatform
    LPrompt_New (p->grp, 4 * X, 14 * Y, -1, 2 * Y, "Folder for ASN.1 parser files:");
#else
    LPrompt_New (p->grp, 4 * X, 14 * Y, -1, 2 * Y, "Folder for ASN.1 parser files (optional):");
#endif

    _editAsnLoadPath = EditBox_New (p->grp, 4 * X, 17 * Y, 46 * X, 3 * Y, NULL, NULL);
    if (!InitPathSetting (_editAsnLoadPath, "asnload", IsAsnloadPathOkay))
	_p1_required = AsnLoad_RequiredByPlatform;
    LPrompt_New (p->grp, 4 * X, 22 * Y, -1, 2 * Y, "Folder for common data files:");
    _editDataPath = EditBox_New (p->grp, 4 * X, 25 * Y, 46 * X, 3 * Y, NULL, NULL);
    if (!InitPathSetting (_editDataPath, "data", IsDataPathOkay))
	_p1_required = TRUE;
    return TRUE;
}

static Boolean 
Page1_LeavePage (PropPage * p, int direction)
{
    if (direction == FORWARD) {
	if (!ToolkitPathsOkay ())
	    return FALSE;       /* If paths are not valid, don't allow to
				 * leave page */
	_p1_required = FALSE;   /* If paths are valid, this page no longer
				 * required */
    }
    return TRUE;
}

static Boolean 
Page1_Commit (PropPage * p)
{
    char            buffer[256];

    EditBox_GetText (_editAsnLoadPath, buffer, sizeof buffer);
    SetAppParam (_rcfile, _rcfile, "AsnLoad", buffer);
    EditBox_GetText (_editDataPath, buffer, sizeof buffer);
    SetAppParam (_rcfile, _rcfile, "Data", buffer);
    return TRUE;
}


static Boolean 
InitPathSetting (EditBox edit, const char *dirname, PathOkayProc isokay)
{
    char            path[256], *p;

    path[0] = '\0';

    /*** 1st attempt: current setting from config file ***/
    if (GetAppParam (_rcfile, _rcfile, (char *) dirname, NULL, path, sizeof path)) {
	if (isokay (path))
	    goto good_path;
    }
    if (strcmp (dirname, "asnload") == 0 && !AsnLoad_RequiredByPlatform)
	goto good_path;

    /*** 2nd attempt: program path + dirname ***/
    ProgramPath (path, sizeof path);
    if ((p = strrchr (path, DIRDELIMCHR)) != NULL) {
	*(p + 1) = '\0';
	FileBuildPath (path, (char *) dirname, NULL);
	if (isokay (path))
	    goto good_path;
    }
    EditBox_SetText (edit, path);
    return FALSE;

good_path:
    EditBox_SetText (edit, path);
    return TRUE;
}


static int 
IsAsnloadPathOkay (const char *path)
{
    int             i;

    char            fname[16];

    char            fpath[256];

    Boolean         ok = FALSE;

    ASSERT (path != NULL);

    for (i = 60; i <= 69; ++i) {
	sprintf (fname, "cdrom.l%02d", i);
	FileBuildPath (strcpy (fpath, path), NULL, fname);
	if (FileExists (fpath)) {
	    ok = TRUE;
	    break;
	}
    }

#ifdef NETWORK_SUPPORTED
    if (!ok)
	return FALSE;
    ok = FALSE;
    for (i = 60; i <= 69; ++i) {
	sprintf (fname, "ni_asn.l%02d", i);
	FileBuildPath (strcpy (fpath, path), NULL, fname);
	if (FileExists (fpath)) {
	    ok = TRUE;
	    break;
	}
    }

    if (!ok)
	return FALSE;
    ok = FALSE;
    for (i = 60; i <= 69; ++i) {
	sprintf (fname, "asnneten.l%02d", i);
	FileBuildPath (strcpy (fpath, path), NULL, fname);
	if (FileExists (fpath)) {
	    ok = TRUE;
	    break;
	}
    }
#endif                          /* NETWORK_SUPPORTED */

    if (ok) {
	TransientSetAppParam (_rcfile, _rcfile, "AsnLoad", (char *) path);
    }
    return ok;
}


static int 
IsDataPathOkay (const char *path)
{
    char            fpath[256];

    ASSERT (path != NULL);

    FileBuildPath (strcpy (fpath, path), NULL, "seqcode.val");
    if (!FileExists (fpath))
	return FALSE;
    FileBuildPath (strcpy (fpath, path), NULL, "gc.val");
    if (!FileExists (fpath))
	return FALSE;

    TransientSetAppParam (_rcfile, _rcfile, "Data", (char *) path);
    return TRUE;
}


static Boolean 
ToolkitPathsOkay (void)
{
    char            a_path[256];

    char            d_path[256];

    /* Check the ASNLOAD path */
    EditBox_GetText (_editAsnLoadPath, a_path, sizeof a_path);
    if (a_path[0] == '\0') {
	/*
	 * ASNLOAD can be omitted on some platforms.  But if anything was
	 * entered, we check to make sure it is valid
	 */
	if (AsnLoad_RequiredByPlatform) {
	    MsgAlert (KEY_OK, SEV_ERROR, _program, "You must enter a valid path for"
		      "the ASNLOAD setting in order to continue");
	    return FALSE;
	}
    } else if (!IsAsnloadPathOkay (a_path)) {
	{
	    MsgAlert (KEY_OK, SEV_ERROR, _program, "The path you have entered for the "
	       "ASNLOAD setting (%s) does not contain the expected files.  "
		      "Please try again.", a_path);
	    return FALSE;
	}
    }
    /* Check the DATA path */
    EditBox_GetText (_editDataPath, d_path, sizeof d_path);
    if (!IsDataPathOkay (d_path)) {
	MsgAlert (KEY_OK, SEV_ERROR, _program, "The path you have entered for the "
		  "DATA setting (%s) does not contain the expected files.  "
		  "Please try again.", d_path);
	return FALSE;
    }
    /*
     * Now that we know both paths are correct, we add them to transient
     * param table
     */
    TransientSetAppParam (_rcfile, _rcfile, "AsnLoad", a_path);
    TransientSetAppParam (_rcfile, _rcfile, "Data", d_path);
    return TRUE;
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*\
	     PAGE 2 --  Entrez CD-ROM Data Sources
\*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

static const char *_p2_text[] = {
    "The Entrez application normally reads information from the",
    "CD-ROMs.  Alternatively, you may copy the contents of one or",
    "more Entrez CD-ROMs to a hard disk or LAN fileserver.  For",
    "each Entrez volume, please specify the desired option."
};

ComboBox        _p2_cbox[Volume_MAX];

PrompT          _p2_prmt[Volume_MAX];

#define DeviceCount() ((_dtyp_vols[0]?_cdrom_ct:0) + _dtyp_vols[1] + _dtyp_vols[2])
int             _cdrom_ct = 1;

static Boolean  Page2_Create (PropPage * p);

static Boolean  Page2_EnterPage (PropPage * p, int direction);

static Boolean  Page2_LeavePage (PropPage * p, int direction);

static Boolean  Page2_Commit (PropPage * p);


int 
Page2_PageProc (PropPage * ppage, PropPageMsg msg, int param)
{
    switch (msg) {
	case MSG_Create:
	return Page2_Create (ppage);
    case MSG_IsActive:
	return (int) _config_cd;
    case MSG_EnterPage:
	return Page2_EnterPage (ppage, param);
    case MSG_LeavePage:
	return Page2_LeavePage (ppage, param);
    case MSG_Commit:
	return Page2_Commit (ppage);
    }
    return PropPage_DefaultProc (ppage, msg, param);
}


static Boolean 
Page2_Create (PropPage * p)
{
    int             i, n, x, y;

    char            buffer[16];

    _volume_ct = GetAppParamInt (_rcfile2, p->section, "Count", Volume_MAX);
	if (GetAppParamInt(_rcfile2,"CdEntrezX","release",0) < 22)
	{
		_volume_ct = 6;
	}

    MultiPrompt_New (p->grp, 4 * X, 6 * Y, _p2_text, DIM (_p2_text));

    x = 16 * X;
    y = 18 * Y;
    for (i = 0; i < Volume_MAX; ++i, y += 4 * Y) {
	_p2_cbox[i] = ComboBox_New (p->grp, x, y, 16 * X, 3 * Y, NULL);
	ComboBox_AddString (_p2_cbox[i], "CD-ROM Drive");
	ComboBox_AddString (_p2_cbox[i], "Hard Disk");
	ComboBox_AddString (_p2_cbox[i], "LAN Fileserver");
	sprintf (buffer, "Entrez%d", i + 1);
	n = GetAppParamInt (_rcfile2, p->section, buffer, 0);
	ComboBox_SetCurSel (_p2_cbox[i], n);
	strcat (buffer, ":");
	_p2_prmt[i] = LPrompt_New (p->grp, 8 * X, y, 6 * X, 3 * Y, buffer);
    }
    return TRUE;
}

static Boolean 
Page2_EnterPage (PropPage * p, int direction)
{
    if (direction == FORWARD) {
	Boolean         vis;

	int             i;

	for (i = 0; i < Volume_MAX; ++i) {
	    vis = (i < _volume_ct);
	    CtrlGrp_ShowControl (p->grp, _p2_cbox[i], vis);
	    CtrlGrp_ShowControl (p->grp, _p2_prmt[i], vis);
	}
    }
    return TRUE;
}

static Boolean 
Page2_LeavePage (PropPage * p, int direction)
{
    if (direction == FORWARD) {
	int             i, n;

	char            buffer[16];

	memset ((void *) _dtyp_vols, 0, sizeof _dtyp_vols);
	for (i = 0; i < _volume_ct; ++i) {
	    n = ComboBox_GetCurSel (_p2_cbox[i]);
	    sprintf (buffer, "Entrez%d", i + 1);
	    SetAppParamInt (_rcfile2, p->section, buffer, n);
	    _vol_dtyp[i] = n;
	    _dtyp_vols[n]++;
	}
	if (_dtyp_vols[0] == 0) {
	    sprintf (buffer, "%d", _volume_ct);
	    TRACE ("Transiently setting DeviceCount=%s\n", buffer);
	    TransientSetAppParam (_rcfile, _cdentrez, "DeviceCount", buffer);
	}
    }
    return TRUE;
}

static Boolean 
Page2_Commit (PropPage * p)
{
    if (_config_cd)
	SetAppParamInt (_rcfile, _cdentrez, "DeviceCount", DeviceCount ());
    return TRUE;
}



/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*\
	     PAGE 3 --  CD-ROM Drive Setup
\*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

static const char *_p3_text[] = {
    "You have specified the \"CD-ROM\" option for one or more",
    "Entrez volumes.  Please supply the requested information",
    "about your CD-ROM setup."
};

GroupBox        _p3_gbox;

Radio           _p3_rad[Volume_MAX];

PrompT          _p3_prmt[Volume_MAX];

EditBox         _p3_ebox[Volume_MAX];

char            _first_cd;

#if defined(WIN_MSWIN) & !defined(NETWORK_SUPPORTED)
char            DeduceFirstCdDriveLetter ();
#else
#define DeduceFirstCdDriveLetter()  0
#endif

static Boolean  Page3_Create (PropPage * p);

static Boolean  Page3_EnterPage (PropPage * p, int direction);

static Boolean  Page3_LeavePage (PropPage * p, int direction);

static Boolean  Page3_Commit (PropPage * p);

static void     Page3_OnCdCount (PropPage * p, int count);

static void     CdCountGroupProc (GrouP grp);



int 
Page3_PageProc (PropPage * ppage, PropPageMsg msg, int param)
{
    switch (msg) {
	case MSG_Create:
	return Page3_Create (ppage);
    case MSG_IsActive:
	return (_config_cd && _dtyp_vols[0] != 0);
    case MSG_EnterPage:
	return Page3_EnterPage (ppage, param);
    case MSG_LeavePage:
	return Page3_LeavePage (ppage, param);
    case MSG_Commit:
	return Page3_Commit (ppage);
    }
    return PropPage_DefaultProc (ppage, msg, param);
}


static Boolean 
Page3_Create (PropPage * p)
{
    int             i, x, y;

    char            buffer[32];

#ifndef OS_MAC
    char            dflt[32];

    char            key[16];
#endif

    MultiPrompt_New (p->grp, 4 * X, 6 * Y, _p3_text, DIM (_p3_text));
    LPrompt_New (p->grp, 4 * X, 13 * Y, -1, 2 * Y, "Number of CD-ROM drives");
    _p3_gbox = GroupBox_New (p->grp, 4 * X, 15 * Y, 13 * X, 7 * (DFLT_yEDIT + Y) + Y, "", CdCountGroupProc);
    _first_cd = DeduceFirstCdDriveLetter ();
    x = 8 * X;
    y = 18 * Y;
    for (i = 0; i < Volume_MAX; ++i, y += (DFLT_yEDIT + Y)) {
	sprintf (buffer, "%d", i + 1);
	_p3_rad[i] = Radio_New (p->grp, x, y, 6 * X, 3 * Y, buffer, _p3_gbox);
	sprintf (buffer, "Path %d:", i + 1);
	_p3_prmt[i] = LPrompt_New (p->grp, 20 * X, y + Y / 2, 6 * X, 2 * Y, buffer);
	_p3_ebox[i] = EditBox_New (p->grp, 26 * X, y, 24 * X, 3 * Y, "", NULL);

#ifndef OS_MAC

#if defined(OS_DOS) || defined(OS_NT)
	sprintf (dflt, "%c:\\", (char) (_first_cd + i));
#else
	sprintf (dflt, "/cdrom/cdrom%d", i);
#endif

	sprintf (key, "Path%d", i + 1);
	GetAppParam (_rcfile2, p->section, key, dflt, buffer, sizeof buffer);
	EditBox_SetText (_p3_ebox[i], buffer);
#endif

#ifndef OS_MAC
	if (i > 0)
#endif

	{
	    CtrlGrp_ShowControl (p->grp, _p3_prmt[i], FALSE);
	    CtrlGrp_ShowControl (p->grp, _p3_ebox[i], FALSE);
	}
    }
    _cdrom_ct = GetAppParamInt (_rcfile2, p->section, "Count", 1);
    SetValue (_p3_gbox, (Int2) _cdrom_ct);
    return TRUE;
}


static Boolean 
Page3_EnterPage (PropPage * p, int direction)
{
    if (direction == FORWARD) {
	int             i;

	for (i = 0; i < Volume_MAX; ++i) {
	    if (i >= _dtyp_vols[0])
		Disable (_p3_rad[i]);
	    else
		Enable (_p3_rad[i]);
	}
    } else {
	DoCdEntrezFini ();
    }
    return TRUE;
}


static char    *_ordinal[] = {"1st", "2nd", "3rd", "4th", "5th", "6th"};

static Boolean 
Page3_LeavePage (PropPage * p, int direction)
{
    if (direction == FORWARD) {
	char            buffer[128];

	int             i;

#ifndef OS_MAC
	char            key[16];
#endif

	SetAppParamInt (_rcfile2, p->section, "Count", _cdrom_ct);

	for (i = 0; i < _cdrom_ct; ++i) {
	    TransientSetAppParam (_rcfile, _cddev[i], NULL, NULL);      /* clear */
	    if (_cdrom_ct == 1)
		strcpy (buffer, "CD-ROM drive");
	    else
		sprintf (buffer, "%s CD-ROM drive", _ordinal[i]);
	    SetAppParam (_rcfile, _cddev[i], "Formal_Name", buffer);

#ifdef OS_MAC
	    TransientSetAppParam (_rcfile, _cddev[i], "Root", NULL);
	    TransientSetAppParam (_rcfile, _cddev[i], "Insert_VolName", "Yes");
#else
	    EditBox_GetText (_p3_ebox[i], buffer, sizeof buffer);
	    if (!DoTestPath (buffer, 0))
		return FALSE;
	    TransientSetAppParam (_rcfile, _cddev[i], "Root", buffer);
	    sprintf (key, "Path%d", i + 1);
	    SetAppParam (_rcfile2, p->section, key, buffer);
#endif

	    TransientSetAppParam (_rcfile, _cddev[i], "Type", "CD");
	    TransientSetAppParam (_rcfile, _cddev[i], "Ejectable", "Yes");
	}

	sprintf (buffer, "%d", DeviceCount ());
	TRACE ("Transiently setting DeviceCount=%s\n", buffer);
	TransientSetAppParam (_rcfile, _cdentrez, "DeviceCount", buffer);

	if (_dtyp_vols[1] + _dtyp_vols[2] == 0) {       /* i.e., all-CDROM */
	    if (!DoCdEntrezInit ())
		return FALSE;
	}
    }
    return TRUE;
}


static Boolean 
Page3_Commit (PropPage * p)
{
    if (_config_cd) {
	char            buffer[128];

	int             i;

	for (i = 0; i < _cdrom_ct; ++i) {
	    SetAppParam (_rcfile, _cddev[i], NULL, NULL);       /* clear */

	    if (_cdrom_ct == 1)
		strcpy (buffer, "CD-ROM drive");
	    else
		sprintf (buffer, "%s CD-ROM drive", _ordinal[i]);
	    SetAppParam (_rcfile, _cddev[i], "Formal_Name", buffer);

#ifdef OS_MAC
	    SetAppParam (_rcfile, _cddev[i], "Root", NULL);
	    SetAppParam (_rcfile, _cddev[i], "Insert_VolName", "Yes");
#else
	    EditBox_GetText (_p3_ebox[i], buffer, sizeof buffer);
	    SetAppParam (_rcfile, _cddev[i], "Root", buffer);
#endif

	    SetAppParam (_rcfile, _cddev[i], "Type", "CD");
	    SetAppParam (_rcfile, _cddev[i], "Ejectable", "Yes");
	}

	SetAppParamBoolean (_rcfile, _cdentrez, "NewStyle", TRUE);
	SetAppParamInt (_rcfile, _cdentrez, "DeviceCount", DeviceCount ());
	if (!GetAppParamInt ("entrez", "Preferences", "MaxLoad", 0))
	    SetAppParamInt ("entrez", "Preferences", "MaxLoad", 1000);
    }
    return TRUE;
}


static void 
Page3_OnCdCount (PropPage * p, int count)
{
    _cdrom_ct = count;

#ifndef OS_MAC
    {
	int             i;

	Boolean         vis;

	for (i = 0; i < _volume_ct; ++i) {
	    vis = (i < _cdrom_ct);
	    CtrlGrp_ShowControl (p->grp, _p3_prmt[i], vis);
	    CtrlGrp_ShowControl (p->grp, _p3_ebox[i], vis);
	}
    }
#endif
}

static void 
CdCountGroupProc (GrouP grp)
{
    Page3_OnCdCount (ControlPanel_GetCurrentPage (_cp), GetValue (grp));
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*\
	     PAGE 4 --  Hard Drive Setup
\*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

static const char *_p4_text[] = {
    "You have specified the \"Hard Disk\" option for one or",
    "more Entrez volumes.  You should have copied the CD-ROM",
    "contents to the hard disk prior to running this program.",
    "Please supply the paths you used when you did this."
};

PrompT          _p4_prmt[Volume_MAX];

EditBox         _p4_ebox[Volume_MAX];

static Boolean  Page4_Create (PropPage * p);

static Boolean  Page4_EnterPage (PropPage * p, int direction);

static Boolean  Page4_LeavePage (PropPage * p, int direction);

static Boolean  Page4_Commit (PropPage * p);


int 
Page4_PageProc (PropPage * ppage, PropPageMsg msg, int param)
{
    switch (msg) {
	case MSG_Create:
	return Page4_Create (ppage);
    case MSG_IsActive:
	return (_config_cd && _dtyp_vols[1] != 0);
    case MSG_EnterPage:
	return Page4_EnterPage (ppage, param);
    case MSG_LeavePage:
	return Page4_LeavePage (ppage, param);
    case MSG_Commit:
	return Page4_Commit (ppage);
	break;
    }
    return PropPage_DefaultProc (ppage, msg, param);
}


static Boolean 
Page4_Create (PropPage * p)
{
    int             i, y;

    MultiPrompt_New (p->grp, 4 * X, 6 * Y, _p4_text, DIM (_p4_text));
    for (i = 0, y = 18 * Y; i < Volume_MAX; ++i, y += (DFLT_yEDIT + Y)) {
	_p4_prmt[i] = LPrompt_New (p->grp, 4 * X, y + Y / 2, -1, 2 * Y, "Entrez# path:");
	_p4_ebox[i] = EditBox_New (p->grp, 16 * X, y, 34 * X, 3 * Y, NULL, NULL);
    }
    return TRUE;
}


static Boolean 
Page4_EnterPage (PropPage * p, int direction)
{
    if (direction == FORWARD) {
	char            buffer[32];

	char            path[128];

	char            key[16];

	int             i, k;

	for (i = k = 0; i < _volume_ct; ++i) {
	    if (_vol_dtyp[i] == Dev_HD) {
		sprintf (buffer, "Entrez%d path:", i + 1);
		SetTitle (_p4_prmt[k], buffer);
		CtrlGrp_ShowControl (p->grp, _p4_prmt[k], TRUE);
		sprintf (key, "Path%d", i + 1);
		GetAppParam (_rcfile2, p->section, key, NULL, path, sizeof path);
		EditBox_SetText (_p4_ebox[k], path);
		CtrlGrp_ShowControl (p->grp, _p4_ebox[k], TRUE);
		k++;
	    }
	}
	for (; k < Volume_MAX; ++k) {
	    CtrlGrp_ShowControl (p->grp, _p4_prmt[k], FALSE);
	    CtrlGrp_ShowControl (p->grp, _p4_ebox[k], FALSE);
	}
    } else {
	DoCdEntrezFini ();
    }
    return TRUE;
}


static Boolean 
Page4_LeavePage (PropPage * p, int direction)
{
    if (direction == FORWARD) {
	char            buffer[256];

	char            key[16];

	int             d = (_dtyp_vols[0] == 0) ? 0 : _cdrom_ct;

	int             i, k;

	for (i = k = 0; i < _volume_ct; ++i) {
	    if (_vol_dtyp[i] == Dev_HD) {
		TransientSetAppParam (_rcfile, _cddev[d], NULL, NULL);  /* clear */

		EditBox_GetText (_p4_ebox[k], buffer, sizeof buffer);
		if (!DoTestPath (buffer, i + 1))
		    return FALSE;
		sprintf (key, "Path%d", i + 1);
		SetAppParam (_rcfile2, p->section, key, buffer);
		TransientSetAppParam (_rcfile, _cddev[d], "Root", buffer);
		TransientSetAppParam (_rcfile, _cddev[d], "Type", "HardDisk");
		sprintf (buffer, "Entrez%d", i + 1);
		TransientSetAppParam (_rcfile, _cddev[d], "Bind", buffer);
		TransientSetAppParam (_rcfile, _cddev[d], "Ejectable", "No");
		k++;
		d++;
	    }
	}

	if (_dtyp_vols[2] == 0) {       /* i.e., no fileservers in use */
	    if (!DoCdEntrezInit ())
		return FALSE;
	}
    }
    return TRUE;
}


static Boolean 
Page4_Commit (PropPage * p)
{
    if (_config_cd) {
	char            buffer[256];

	int             d = (_dtyp_vols[0] == 0) ? 0 : _cdrom_ct;

	int             i, k;

	for (i = k = 0; i < _volume_ct; ++i) {
	    if (_vol_dtyp[i] == Dev_HD) {
		SetAppParam (_rcfile, _cddev[d], NULL, NULL);   /* clear */

		EditBox_GetText (_p4_ebox[k], buffer, sizeof buffer);
		SetAppParam (_rcfile, _cddev[d], "Root", buffer);
		sprintf (buffer, "Entrez%d (hard disk image)", i + 1);
		SetAppParam (_rcfile, _cddev[d], "Formal_Name", buffer);
		SetAppParam (_rcfile, _cddev[d], "Type", "HardDisk");
		SetAppParam (_rcfile, _cddev[d], "Ejectable", "No");
		sprintf (buffer, "Entrez%d", i + 1);
		SetAppParam (_rcfile, _cddev[d], "Bind", buffer);
		k++;
		d++;
	    }
	}
    }
    return TRUE;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*\
	     PAGE 5 -- LAN Fileserver Setup
\*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

static const char *_p5_text[] = {
    "You have specified the \"LAN fileserver\" option for one or",
    "more Entrez volumes.  Please enter the appropriate fileserver",
    "paths (ask your LAN administrator for details)."
};

PrompT          _p5_prmt[Volume_MAX];

EditBox         _p5_ebox[Volume_MAX];

static Boolean  Page5_Create (PropPage * p);

static Boolean  Page5_EnterPage (PropPage * p, int direction);

static Boolean  Page5_LeavePage (PropPage * p, int direction);

static Boolean  Page5_Commit (PropPage * p);


int 
Page5_PageProc (PropPage * ppage, PropPageMsg msg, int param)
{
    switch (msg) {
	case MSG_Create:
	return Page5_Create (ppage);
    case MSG_IsActive:
	return (_config_cd && _dtyp_vols[2] != 0);
    case MSG_EnterPage:
	return Page5_EnterPage (ppage, param);
    case MSG_LeavePage:
	return Page5_LeavePage (ppage, param);
    case MSG_Commit:
	return Page5_Commit (ppage);
    }
    return PropPage_DefaultProc (ppage, msg, param);
}


static Boolean 
Page5_Create (PropPage * p)
{
    int             i, y;

    MultiPrompt_New (p->grp, 4 * X, 6 * Y, _p5_text, DIM (_p5_text));
    for (i = 0, y = 18 * Y; i < Volume_MAX; ++i, y += (DFLT_yEDIT) + Y) {
	_p5_prmt[i] = LPrompt_New (p->grp, 4 * X, y + Y / 2, -1, 2 * Y, "Entrez# path:");
	_p5_ebox[i] = EditBox_New (p->grp, 16 * X, y, 34 * X, 3 * Y, NULL, NULL);
    }
    return TRUE;
}

static Boolean 
Page5_EnterPage (PropPage * p, int direction)
{
    if (direction == FORWARD) {
	char            buffer[32];

	char            path[128];

	char            key[16];

	int             i, k;

	for (i = k = 0; i < _volume_ct; ++i) {
	    if (_vol_dtyp[i] == Dev_ND) {
		sprintf (buffer, "Entrez%d path:", i + 1);
		SetTitle (_p5_prmt[k], buffer);
		CtrlGrp_ShowControl (p->grp, _p5_prmt[k], TRUE);
		sprintf (key, "Path%d", i + 1);
		GetAppParam (_rcfile2, p->section, key, NULL, path, sizeof path);
		EditBox_SetText (_p5_ebox[k], path);
		CtrlGrp_ShowControl (p->grp, _p5_ebox[k], TRUE);
		k++;
	    }
	}
	for (; k < Volume_MAX; ++k) {
	    CtrlGrp_ShowControl (p->grp, _p5_prmt[k], FALSE);
	    CtrlGrp_ShowControl (p->grp, _p5_ebox[k], FALSE);
	}
    } else {
	DoCdEntrezFini ();
    }
    return TRUE;
}


static Boolean 
Page5_LeavePage (PropPage * p, int direction)
{
    if (direction == FORWARD) {
	char            buffer[256];

	char            key[16];

	int             d = ((_dtyp_vols[0] == 0) ? 0 : _cdrom_ct) + _dtyp_vols[1];

	int             i, k;

	for (i = k = 0; i < _volume_ct; ++i) {
	    if (_vol_dtyp[i] == Dev_ND) {
		TransientSetAppParam (_rcfile, _cddev[d], NULL, NULL);  /* clear */

		EditBox_GetText (_p5_ebox[k], buffer, sizeof buffer);
		if (!DoTestPath (buffer, i + 1))
		    return FALSE;
		sprintf (key, "Path%d", i + 1);
		SetAppParam (_rcfile2, p->section, key, buffer);
		TransientSetAppParam (_rcfile, _cddev[d], "Root", buffer);
		sprintf (buffer, "Entrez%d", i + 1);
		TransientSetAppParam (_rcfile, _cddev[d], "Bind", buffer);
		TransientSetAppParam (_rcfile, _cddev[d], "Type", "Net");
		TransientSetAppParam (_rcfile, _cddev[d], "Ejectable", "No");
		k++;
		d++;
	    }
	}
	return DoCdEntrezInit ();
    }
    return TRUE;
}


static Boolean 
Page5_Commit (PropPage * p)
{
    if (_config_cd) {
	char            buffer[256];

	int             d = ((_dtyp_vols[0] == 0) ? 0 : _cdrom_ct) + _dtyp_vols[1];

	int             i, k;

	for (i = k = 0; i < _volume_ct; ++i) {
	    if (_vol_dtyp[i] == Dev_ND) {
		SetAppParam (_rcfile, _cddev[d], NULL, NULL);   /* clear */

		EditBox_GetText (_p5_ebox[k], buffer, sizeof buffer);
		SetAppParam (_rcfile, _cddev[d], "Root", buffer);
		sprintf (buffer, "Entrez%d (fileserver image)", i + 1);
		SetAppParam (_rcfile, _cddev[d], "Formal_Name", buffer);
		SetAppParam (_rcfile, _cddev[d], "Type", "Net");
		SetAppParam (_rcfile, _cddev[d], "Ejectable", "No");
		sprintf (buffer, "Entrez%d", i + 1);
		SetAppParam (_rcfile, _cddev[d], "Bind", buffer);
		k++;
		d++;
	    }
	}
    }
    return TRUE;
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*\
	     PAGE 6 --  Copy index files (dummy version)
\*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

static const char *_p6_text[] = {
    "If you have sufficient space, you may want to copy some Entrez",
    "index files to your hard disk in order to improve performance.",
};

static ButtoN   _p6_chk[DType_MAX];

static PrompT   _p6_prmt[DType_MAX];

static PrompT   _p6_prmt_tsize;
static char    *_dtyp[] = {"MEDLINE", "protein", "nucleotide", "structure", "genomes"};
static char    *_dkey[] = {"IDX-ml", "IDX-aa", "IDX-nt", "IDX-st", "IDX-ch"};

static int      _dtypes;

static int      _index_types;

static char     _index_folder[192];

static long     _tsize;         /* total size in bytes */

static int      _tsize_kb;      /* total size in Kbytes */

static FileToCopy _p6_ftc[] = {
    "ml.ofs", -1, 1, CdDir_idx,
    "ml.oix", 0, 1, CdDir_idx,
    "aa.ofs", -1, 1, CdDir_idx,
    "aa.oix", 0, 1, CdDir_idx,
    "nt.ofs", -1, 1, CdDir_idx,
    "nt.oix", 0, 1, CdDir_idx,
    "st.ofs", -1, 1, CdDir_idx,
    "st.oix", 0, 1, CdDir_idx,
    "ch.ofs", -1, 1, CdDir_idx,
    "ch.oix", 0, 1, CdDir_idx
};

static Boolean  Page6_Create (PropPage * p);

static Boolean  Page6_EnterPage (PropPage * p, int direction);

static Boolean  Page6_LeavePage (PropPage * p, int direction);

static Boolean  Page6_Commit (PropPage * p);

static Boolean  GetIndexFileSizes (void);

static int      FindAnyEntrezImage (CdRomInfo * info, char *path, size_t pathsize);

static void     _OnIndexCheck (ButtoN btn);

static Boolean  FolderExists (const char *folder);

int 
Page6_PageProc (PropPage * ppage, PropPageMsg msg, int param)
{
    switch (msg) {
	case MSG_Create:
	return Page6_Create (ppage);
    case MSG_EnterPage:
	return Page6_EnterPage (ppage, param);
    case MSG_LeavePage:
	return Page6_LeavePage (ppage, param);
    case MSG_IsActive:
	/***return (_advanced_settings == 0);***/
	/* Always use page 6 because page 7 is not fully implemented */
	return (int) _config_cd;
    case MSG_Commit:
	if (_config_cd)
	    Page6_Commit (ppage);
	break;
    }
    return PropPage_DefaultProc (ppage, msg, param);
}

static Boolean 
Page6_Create (PropPage * p)
{
    int             i, x, y;

    char            buff[4];

    MultiPrompt_New (p->grp, 4 * X, 6 * Y, _p6_text, DIM (_p6_text));
    LPrompt_New (p->grp, 4 * X, 36 * Y, -1, 2 * Y, "The files will be copied upon completion of the setup process.");
    x = 4 * X;
    y = 12 * Y;
    for (i = 0; i < DIM(_dkey); ++i, y += 4 * Y) {
	_p6_chk[i] = CheckBox_New (p->grp, x, y, 28 * X, 3 * Y, "---", _OnIndexCheck);
	if (GetAppParam (_rcfile, "CdEntrez.Paths", _dkey[i], NULL, buff, sizeof buff))
	    CheckBox_Check (_p6_chk[i], TRUE);
	_p6_prmt[i] = RPrompt_New (p->grp, 40 * X, y + 2, 10 * X, 3 * Y, "0  Kbytes");
    }
    LPrompt_New (p->grp, 12 * X, y + Y / 2, -1, 3 * Y, "Total size of selected files:");
    _p6_prmt_tsize = RPrompt_New (p->grp, 40 * X, y + Y / 2, 10 * X, 3 * Y, "0  Kbytes");
    return TRUE;
}

static Boolean 
Page6_EnterPage (PropPage * p, int direction)
{
    if (direction == FORWARD) {
	char            buffer[64];

	int             i;

	long            bytes;

	int             kbytes;

	RecT            r;

	if (_entrez_info == NULL) {
	    ErrLogPrintf ("Page6_EnterPage:  cdnewlib not initialized\n");
	    return FALSE;
	}
	if (!GetIndexFileSizes ())
	    return FALSE;

	SelectFont (systemFont);

	for (i = 0; i < _dtypes; ++i) {
	    sprintf (buffer, "Index files for %s records", _dtyp[i]);
	    SetTitle (_p6_chk[i], buffer);
	    ObjectRect (_p6_chk[i], &r);
	    r.right = r.left + StringWidth (buffer) + 20;
	    SetPosition (_p6_chk[i], &r);
	    bytes = _p6_ftc[i * 2].filesize + _p6_ftc[1 + i * 2].filesize;
	    kbytes = (int) (bytes / (long) KBYTE);
	    sprintf (buffer, "%d  Kbytes", kbytes);
	    SetTitle (_p6_prmt[i], buffer);

	    CtrlGrp_ShowControl (p->grp, _p6_chk[i], TRUE);
	    CtrlGrp_ShowControl (p->grp, _p6_prmt[i], TRUE);
	}

	for (; i < DType_MAX; ++i) {
	    CtrlGrp_ShowControl (p->grp, _p6_chk[i], FALSE);
	    CtrlGrp_ShowControl (p->grp, _p6_prmt[i], FALSE);
	}

    }
    return TRUE;
}


static Boolean 
Page6_LeavePage (PropPage * p, int direction)
{
    if (direction == FORWARD) {
	int             i;

	_index_types = 0;
	for (i = 0, _tsize = 0; i < _dtypes; ++i) {
	    if (CheckBox_IsChecked (_p6_chk[i])) {
		_index_types++;
		_tsize += _p6_ftc[i * 2].filesize;
	    }
	}

	/* if (_index_folder[0] ==0) */
	{
	    char           *p;

	    EditBox_GetText (_editDataPath, _index_folder, sizeof _index_folder);
	    p = strchr (_index_folder, '\0');
	    if (p == _index_folder) {
		ErrPostEx (SEV_FATAL, 1, 0, "Unknown error (%s, line %d)", __FILE__, __LINE__);
		return FALSE;
	    }
	    if (*(p - 1) == DIRDELIMCHR)
		p--;
	    p -= 4;
	    if (StrNICmp (p, "data", 4) == 0)
		*p = '\0';
	    FileBuildPath (_index_folder, "index", NULL);
	}

	if (_index_types > 0) {
	    if (!FolderExists (_index_folder)) {
		if (CreateDir (_index_folder))
		    ErrLogPrintf ("Folder %s created\n", _index_folder);
		else
		    ErrPostEx (SEV_INFO, 0, 0, "CreateDir(%s) failed", _index_folder);
	    }
	    if (!FolderExists (_index_folder)) {
		MsgAlert (KEY_OK, SEV_ERROR, _program, "The %s to be used to store "
			  "index files [%s] either cannot be create or is "
			  "not write-enabled", _folder, _index_folder);
		return FALSE;
	    }
	}
    }
    return TRUE;
}

static Boolean file_copying_cancelled;

static Boolean 
Page6_Commit (PropPage * p)
{
    if (_config_cd) {
	Monitor        *mon = NULL;

	Boolean         b = TRUE;

	char            folder[256];

	CdRomInfo       info;

	int             i;

	if (_index_types > 0) {
	    if (!FindAnyEntrezImage (&info, folder, sizeof folder)) {
		ErrPostEx (SEV_INFO, 1, 0, "Can't find any Entrez volume!");
		return FALSE;
	    }
	    _vbuf_in = (char *) Malloc (VBUFSIZE);
	    _vbuf_out = (char *) Malloc (VBUFSIZE);
	    mon = MonitorIntNew ("Copying index files", 0, (long) (_tsize / 2));
	    ErrLogPrintf ("Copying index files ...\n");
	}
	file_copying_cancelled = FALSE;
	for (i = 0; i < _dtypes; ++i) {
	    int             n = i * 2;

	    if (CheckBox_IsChecked (_p6_chk[i])) {
		if (!FileToCopy_PerformCopy (&_p6_ftc[n], &info, folder, _index_folder, mon)) {
		    b = FALSE;
		    break;
		}
		if (!FileToCopy_PerformCopy (&_p6_ftc[n + 1], &info, folder, _index_folder, mon)) {
		    b = FALSE;
		    break;
		}
		SetAppParam (_rcfile, "CdEntrez.Paths", _dkey[i], _index_folder);
	    } else {
		SetAppParam (_rcfile, "CdEntrez.Paths", _dkey[i], NULL);
		FileToCopy_RemoveOldCopy (&_p6_ftc[n], _index_folder);
		FileToCopy_RemoveOldCopy (&_p6_ftc[n + 1], _index_folder);
	    }
	}

	if (_vbuf_in)
	    Free ((void *) _vbuf_in);
	if (_vbuf_out)
	    Free ((void *) _vbuf_out);

	if (_index_types > 0) {
	    if (file_copying_cancelled) {
		MsgAlert (KEY_OK, SEV_INFO, _program, "Index files copying was cancelled");
	    } else if (b == FALSE) {
		MsgAlert (KEY_OK, SEV_ERROR, _program, "An error occured while copying the "
			  "index files.  This may mean that you do not have sufficient "
			  "disk space.  You can still run Entrez, but the program will use "
			  "the index files on the CD-ROM instead.");
		_config_ok = FALSE;
	    } else {
		MsgAlert (KEY_OK, SEV_INFO, _program, "The index files were copied successfully");
	    }
	}
	if (mon != NULL)
	    MonitorFree (mon);

    }
    return TRUE;
}


static int 
FindAnyEntrezImage (CdRomInfo * info, char *path, size_t pathsize)
{
    /*
     * There _should_ be one on-line since we just did an CdInit, but we have
     * to figure out which one and return the path to it
     */

    int             i, ct = GetAppParamInt (_rcfile, _cdentrez, "DeviceCount", 0);

    ASSERT (path != NULL);
    *path = '\0';

    for (i = 0; i < ct; ++i) {
	if (GetAppParam (_rcfile, _cddev[i], "Root", NULL, path, (Int2) pathsize)) {
	    if (CdTestPath (path, info))
		break;
	    *path = '\0';
	}
    }

#ifdef OS_MAC
    if (*path == '\0') {
	/*
	 * On the Mac, if all devices are ejectable we will not have found
	 * any path yet.  So we try the volume labels for all three CD-ROMs
	 */
	for (i = 0; i < _volume_ct; ++i) {
	    sprintf (path, "Entrez%d", i + 1);
	    if (CdTestPath (path, info))
		break;
	    *path = '\0';
	}
    }
#endif

    return strlen (path);
}


static Boolean 
FolderExists (const char *folder)
{
    /* or more precisely, FolderExistsAndIsWritable ! */
    char            fpath[256];

    FILE           *fd;

    ASSERT (folder != NULL);

    FileBuildPath (strcpy (fpath, folder), NULL, "foo.tmp");
    if ((fd = fopen (fpath, "w")) == NULL)
	return FALSE;
    fclose (fd);
    FileRemove (fpath);
    return TRUE;
}


static Boolean 
GetIndexFileSizes (void)
{
    long            bytes;
    int             i;

    ASSERT(_entrez_info != NULL);
    _dtypes = _entrez_info->type_count;

    for (i = 0; i < _dtypes; ++i) {
	bytes = CdEnumFiles (CdDir_idx, (DocType) i, NULL, NULL, NULL);
	/**if (bytes == 0)
		return FALSE;**/
	_p6_ftc[i * 2].filesize = bytes;

    }
    return TRUE;
}


static void 
_OnIndexCheck (ButtoN btn)
{
    char            buffer[32];

    int             i;

    for (i = 0, _tsize = 0; i < _dtypes; ++i) { 
	if (CheckBox_IsChecked (_p6_chk[i]))
	    _tsize += _p6_ftc[i * 2].filesize;
    }
    _tsize_kb = (int) (_tsize / (long) KBYTE);
    sprintf (buffer, "%d  Kbytes", _tsize_kb);
    SetTitle (_p6_prmt_tsize, buffer);
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*\
	     PAGE 7 --  Copy index files (advanced version)
\*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

static const char *_p7_text[] = {
    "To improve performance, you may want to keep certain",
    "groups of frequently used files on your hard disk."
};

static void     _OnFGroup (ListBox lbox);

/* static CtrlGrp *_p7_grp; */
/* static Control _p7_ctrl[8]; */
static ListBox  _list_FileClass;

static ButtoN   _chk_Copy;

static EditBox  _edit_Path;

int 
Page7_PageProc (PropPage * ppage, PropPageMsg msg, int param)
{
    switch (msg) {
	case MSG_Create:
	{
	    int             i = 0;

	    MultiPrompt_New (ppage->grp, 4 * X, 6 * Y, _p7_text, DIM (_p7_text));

	    i = 0;
	    _list_FileClass = ListBox_New (ppage->grp, 4 * X, 11 * Y, 46 * X, 8 * Y + 2, _OnFGroup);
	    ListBox_AddString (_list_FileClass, "Index files, all");
	    ListBox_AddString (_list_FileClass, "  Index files, medline");
	    ListBox_AddString (_list_FileClass, "  Index files, protein");
	    ListBox_AddString (_list_FileClass, "  Index files, nucleotide");
	    ListBox_AddString (_list_FileClass, "Summary files, all");
	    ListBox_AddString (_list_FileClass, "  Summary files, medline");
	    ListBox_AddString (_list_FileClass, "  Summary files, protein");
	    ListBox_AddString (_list_FileClass, "  Summary files, nucleotide");
	    _chk_Copy = CheckBox_New (ppage->grp, 4 * X, 22 * Y, 10 * X, 3 * Y, "Copy to:", NULL);
	    _edit_Path = EditBox_New (ppage->grp, 14 * X, 22 * Y, 36 * X, DFLT_yEDIT, NULL, NULL);
	    return TRUE;
	}
	break;
    case MSG_IsActive:
	{
	    /**return (_advanced_settings != 0);**/

	    /* Let's disable this page since it is not fully implemented */
	    return FALSE;
	}
	break;
    }
    return PropPage_DefaultProc (ppage, msg, param);
}


static void 
_OnFGroup (ListBox lbox)
{
    /**** NOT IMPLEMENTED ****/
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*\
	     PAGE 8 --  CdEntrez Optimization
\*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

static const char *_p8_text[] = {
    "If your computer is equipped with a large amount of ",
    "memory, increasing these values may boost performance."
};

#define IdxCacheSize_DFLT 32
#define IdxCacheSize_MIN  2
#define IdxCacheSize_MAX  256
#define TrmCacheSize_DFLT 16
#define TrmCacheSize_MIN  2
#define TrmCacheSize_MAX  128

static CtrlGrp *_p8_grp;

static EditBox  _editIdxCacheSize;

static EditBox  _editTrmCacheSize;

static void     WriteCacheSetting (const char *key, int dflt, int min, int max, EditBox edit);

static void     ReadCacheSetting (const char *key, int dflt, EditBox edit);


int 
Page8_PageProc (PropPage * ppage, PropPageMsg msg, int param)
{
    switch (msg) {
	case MSG_Create:
	{
	    _p8_grp = ppage->grp;
	    MultiPrompt_New (_p8_grp, 4 * X, 6 * Y, _p8_text, DIM (_p8_text));
	    LPrompt_New (_p8_grp, 4 * X, 13 * Y, -1, 2 * Y, "Cache size for Index files:");
	    LPrompt_New (_p8_grp, 4 * X, 17 * Y, -1, 2 * Y, "Cache size for Terms files:");
	    LPrompt_New (_p8_grp, 33 * X, 13 * Y, -1, 2 * Y, "Kbytes");
	    LPrompt_New (_p8_grp, 33 * X, 17 * Y, -1, 2 * Y, "Kbytes");

	    _editIdxCacheSize = EditBox_New (_p8_grp, 24 * X, 13 * Y - Y / 2, 8 * X, 3 * Y, NULL, NULL);
	    ReadCacheSetting ("IdxCacheSize", IdxCacheSize_DFLT, _editIdxCacheSize);
	    _editTrmCacheSize = EditBox_New (_p8_grp, 24 * X, 17 * Y - Y / 2, 8 * X, 3 * Y, NULL, NULL);
	    ReadCacheSetting ("TrmCacheSize", TrmCacheSize_DFLT, _editTrmCacheSize);
	}
	break;
    case MSG_IsActive:
	{
	    return (_config_cd && _advanced_settings != 0);
	}
	break;
    case MSG_Commit:
	{
	    if (_config_cd && _advanced_settings) {
		WriteCacheSetting ("IdxCacheSize", IdxCacheSize_DFLT,
		     IdxCacheSize_MIN, IdxCacheSize_MAX, _editIdxCacheSize);
		WriteCacheSetting ("TrmCacheSize", TrmCacheSize_DFLT,
		     TrmCacheSize_MIN, TrmCacheSize_MAX, _editTrmCacheSize);
	    }
	}
    }
    return PropPage_DefaultProc (ppage, msg, param);
}

static void 
ReadCacheSetting (const char *key, int dflt, EditBox edit)
{
    int             kbytes = GetAppParamInt (_rcfile, _cdentrez, (char *) key, dflt);

    EditBox_SetInt (edit, kbytes);
}

static void 
WriteCacheSetting (const char *key, int dflt, int min, int max, EditBox edit)
{
    int             kbytes;

    char            buffer[32];

    EditBox_GetText (edit, buffer, sizeof buffer);
    kbytes = atoi (buffer);
    if (kbytes < min || kbytes > max) {
	ErrPostEx (SEV_INFO, 0, 0, "%s=%s;  Value out of range, using %d instead",
		   key, buffer, dflt);
	kbytes = dflt;
    }
    SetAppParamInt (_rcfile, _cdentrez, (char *) key, kbytes);
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*\
	     PAGE 9 -- Entrez for Internet Setup
\*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

#ifndef NETWORK_SUPPORTED
int 
Page9_PageProc (PropPage * ppage, PropPageMsg msg, int param)
{
    return 0;
}

#else

static const char *_p9_text[] = {
    "Enter a username to identify this user or this site; this",
    "information is not used for validation, but helps to track usage.",
    "Also indicate what action your client software should perform",
    "when it is unable to contact the primary Dispatcher computer."
};


static Boolean  Page9_Create (PropPage * p);

static Boolean  Page9_EnterPage (PropPage * p, int direction);

static Boolean  Page9_LeavePage (PropPage * p, int direction);

static Boolean  Page9_Commit (PropPage * p);


int 
Page9_PageProc (PropPage * ppage, PropPageMsg msg, int param)
{
    switch (msg) {
	case MSG_Create:
	Page9_Create (ppage);
	break;
    case MSG_IsActive:
	return (int) _config_net;
    case MSG_EnterPage:
	Page9_EnterPage (ppage, param);
	break;
    case MSG_LeavePage:
	if (param == FORWARD && !Page9_LeavePage (ppage, param))
	    return FALSE;
	break;
    case MSG_Commit:
	if (_config_net)
	    Page9_Commit (ppage);
	break;
    }
    return PropPage_DefaultProc (ppage, msg, param);
}

static char     _NetServ[] = "NET_SERV";

static EditBox  _edit_UserName;

static GroupBox _gbox_DispFail;

static ButtoN   _chk_WantEncryption;

static ButtoN   _chk_WantOutgoing;


static Boolean 
Page9_Create (PropPage * p)
{
    char            buffer[64];

    MultiPrompt_New (p->grp, 4 * X, 6 * Y, _p9_text, DIM (_p9_text));
    LPrompt_New (p->grp, 4 * X, 13 * Y, -1, 2 * Y, "Username:");
    GetAppParam (_rcfile, "NET_SERV", "DISP_USERNAME", NULL, buffer, sizeof buffer);

    /*
     * for UNIX and VMS systems (or, for the future, any system where the
     * user
     */
    /* user name can be determined), use the user's login name as the default  */

#ifdef OS_UNIX
    if (buffer[0] == '\0') {
	StringCpy (buffer, getlogin ());
    }
#endif

#ifdef OS_VMS
    if (buffer[0] == '\0') {
	StringCpy (buffer, getenv ("USER"));
    }
#endif

    _edit_UserName = EditBox_New (p->grp, 16 * X, 13 * Y - Y / 2, 28 * X, 2 * Y, buffer, NULL);

    _gbox_DispFail = GroupBox_New (p->grp, 4 * X, 17 * Y, 40 * X, 4 * DFLT_yEDIT + Y, "When cannot find primary dispatcher", NULL);
    GetAppParam (_rcfile, "NET_SERV", "DISP_RECONN_ACTION", "CONT", buffer, sizeof buffer);
    Radio_New (p->grp, 6 * X, 17 * Y + 3 * DFLT_yEDIT, 0, 0, "Give up", _gbox_DispFail);
    Radio_New (p->grp, 6 * X, 17 * Y + DFLT_yEDIT, 0, 0, "Try alternate dispatchers", _gbox_DispFail);
    Radio_New (p->grp, 6 * X, 17 * Y + 2 * DFLT_yEDIT, 0, 0, "Ask the user what to do", _gbox_DispFail);
    if (StrCmp (buffer, "ASK") == 0) {
	SetValue (_gbox_DispFail, 3);
    } else if (StrCmp (buffer, "QUIT") == 0) {
	SetValue (_gbox_DispFail, 1);
    } else {
	SetValue (_gbox_DispFail, 2);
    }

    _chk_WantOutgoing = CheckBox_New (p->grp, 4 * X, 34 * Y, 30 * X, 2 * Y, "Outgoing connections only", NULL);
    GetAppParam (_rcfile, "NET_SERV", "DIRECT_SVC_CON", "FALSE", buffer, sizeof buffer);
    if (StrICmp (buffer, "TRUE") == 0) {
	CheckBox_Check (_chk_WantOutgoing, TRUE);
    }
    if (NI_EncrAvailable ()) {
	_chk_WantEncryption = CheckBox_New (p->grp, 35 * X, 34 * Y, 30 * X, 2 * Y, "Use encryption", NULL);
    }
    GetAppParam (_rcfile, "NET_SERV", "ENCRYPTION_DESIRED", "FALSE", buffer, sizeof buffer);
    if (StrICmp (buffer, "TRUE") == 0) {
	CheckBox_Check (_chk_WantEncryption, TRUE);
    }
    return TRUE;
}

static Boolean 
Page9_EnterPage (PropPage * p, int direction)
{
    return TRUE;
}

static Boolean 
Page9_LeavePage (PropPage * p, int direction)
{
    if (direction == FORWARD) {
	char            buffer[64];

	EditBox_GetText (_edit_UserName, buffer, sizeof buffer);
	TransientSetAppParam (_rcfile, "NET_SERV", "DISP_USERNAME", buffer);
    }
    return TRUE;
}

static Boolean 
Page9_Commit (PropPage * p)
{
    char            buffer[64];

    EditBox_GetText (_edit_UserName, buffer, sizeof buffer);
    SetAppParam (_rcfile, "NET_SERV", "DISP_USERNAME", buffer);



    if (NI_EncrAvailable ()) {
	SetAppParam (_rcfile, "NET_SERV", "ENCRYPTION_DESIRED", CheckBox_IsChecked (_chk_WantEncryption) ? "TRUE" : "FALSE");
    }
    SetAppParam (_rcfile, "NET_SERV", "DIRECT_SVC_CON", CheckBox_IsChecked (_chk_WantOutgoing) ? "TRUE" : "FALSE");
    switch (GetValue (_gbox_DispFail)) {
    case 3:
	StrCpy (buffer, "ASK");
	break;
    case 1:
	StrCpy (buffer, "QUIT");
	break;
    case 2:
    default:
	StrCpy (buffer, "CONT");
	break;
    }

    SetAppParam (_rcfile, "NET_SERV", "DISP_RECONN_ACTION", buffer);


    return TRUE;
}

#endif


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*\
	     PAGE 10 -- Dispatcher Addresses
\*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

#ifndef NETWORK_SUPPORTED
int 
Page10_PageProc (PropPage * ppage, PropPageMsg msg, int param)
{
    return 0;
}

#else

static const char *_p10_text[] = {
    "The client software needs to know either the fully qualified",
    "domain name (FQDN) or the dotted Internet address of an NCBI",
    "dispatcher.  Please select one of the possibilities below",
    "by clicking on that address, or enter an address manually."
};

struct DispAddr {
    const char     *fqdn;
    const char     *addr;
};

struct DispAddr _dispAddr[] = {
    "dispatch1.nlm.nih.gov", "130.14.25.211",
    "dispatch2.nlm.nih.gov", "130.14.25.47",
    "dispatch3.nlm.nih.gov", "130.14.25.1"
};


static NI_DispatcherPtr dispatcher = NULL;

static NI_DispInfoPtr dispInfoPtr = NULL;

CharPtr         svname;

CharPtr         resname;


static Boolean  Page10_Create (PropPage * p);

static Boolean  Page10_EnterPage (PropPage * p, int direction);

static Boolean  Page10_LeavePage (PropPage * p, int direction);

static Boolean  Page10_Commit (PropPage * p);



int 
Page10_PageProc (PropPage * ppage, PropPageMsg msg, int param)
{
    switch (msg) {
	case MSG_Create:
	return Page10_Create (ppage);
    case MSG_IsActive:
	return (int) _config_net;
    case MSG_EnterPage:
	return Page10_EnterPage (ppage, param);
    case MSG_LeavePage:
	return Page10_LeavePage (ppage, param);
    case MSG_Commit:
	return Page10_Commit (ppage);
    }
    return PropPage_DefaultProc (ppage, msg, param);
}


static EditBox  _edit_dispADDR;

static ButtoN   _chk_TestDispatcher;

#ifdef OS_MAC
extern struct hostent PNTR gethostbyname (CharPtr);

extern CharPtr  inet_ntoa (struct in_addr);

#endif

static DoC      dispDoc;

static Int2     currentDispRow;

static Int2     currentDispCol;

static Int2     clickedRow;

static Int2     clickedCol;


static ColData  dispColFmt[2] = {
    {0, 0, 0, 0, NULL, 'l', FALSE, FALSE, FALSE, FALSE, FALSE},
    {0, 5, 0, 0, NULL, 'l', FALSE, TRUE, FALSE, FALSE, TRUE}
};

static ParData  dispParFmt = {
    FALSE, FALSE, FALSE, FALSE, FALSE, 0, 0
};

static void 
dispClick (DoC d, PoinT pt)
{
    MapDocPoint (d, pt, &clickedRow, NULL, &clickedCol, NULL);
}

static void
dispAction (DoC d, PoinT pt)
{
    char            dname[64];

    Int2            col;

    Int2            row;

    Int2            oldrow;

    Int2            oldcol;

    Int2            top;

    Int2            bottom;

    Int2            left;

    Int2            right;

    CharPtr         str;

    RecT            r;

    MapDocPoint (d, pt, &row, NULL, &col, NULL);
    if (clickedRow != row || clickedCol != col) {
	return;
    }
    if (row != currentDispRow || col != currentDispCol) {
	oldrow = currentDispRow;
	oldcol = currentDispCol;
	currentDispRow = row;
	currentDispCol = col;
	if (oldrow != 0) {
	    ObjectRect (d, &r);
	    InsetRect (&r, 4, 4);
	    if (RowIsVisible (d, oldrow, 1, &top, &bottom)) {
		if (oldcol == 1) {
		    left = r.left;
		    right = r.left + dispColFmt[0].pixWidth;
		} else {
		    left = r.left + dispColFmt[0].pixWidth;
		    right = r.right;
		}
		LoadRect (&r, left, top, right, bottom);
		InsetRect (&r, -1, -1);
		InvalRect (&r);
	    }
	    /*
	     * InvalDocRows (dispDoc, oldrow, 1, 1);
	     */
	}
	ObjectRect (d, &r);
	InsetRect (&r, 4, 4);
	if (RowIsVisible (d, currentDispRow, 1, &top, &bottom)) {
	    if (currentDispCol == 1) {
		left = r.left;
		right = r.left + dispColFmt[0].pixWidth;
	    } else {
		left = r.left + dispColFmt[0].pixWidth;
		right = r.right;
	    }
	    LoadRect (&r, left, top, right, bottom);
	    InsetRect (&r, -1, -1);
	    InvalRect (&r);
	}
	/*
	 * InvalDocRows (dispDoc, currentDispRow, 1, 1);
	 */
    }
    Update ();

    str = GetDocText (dispDoc, row, 1, col);
    StringNCpy (dname, str, sizeof (dname));
    MemFree (str);
    EditBox_SetText (_edit_dispADDR, dname);
}

static Boolean 
dispInvert (DoC d, Int2 item, Int2 row, Int2 col)
{
    if (currentDispRow == item && currentDispCol == col) {
	return TRUE;
    } else {
	return FALSE;
    }
}


static Boolean 
Page10_Create (PropPage * p)
{
    int             y = 8 * Y + 3 * DFLT_yEDIT;

    struct hostent PNTR dispHost;

    struct sockaddr_in serv_addr;

    Int2            i;

    char            buf[100];

    RecT            r;

    Int2            width;

    MultiPrompt_New (p->grp, 4 * X, 6 * Y, _p10_text, DIM (_p10_text));
    dispDoc = DocumentPanel (_win, 28, MIN ((Int2) 3, DIM (_dispAddr)));
    {
	RecT            r;

	LoadRect (&r, 4 * X, 13 * Y, 50 * X + 8,
	    13 * Y + (MIN (3, (Int2) DIM (_dispAddr))) * stdLineHeight + 8);
	SetPosition (dispDoc, &r);
	CtrlGrp_AddControl (p->grp, dispDoc);
    }
    ObjectRect (dispDoc, &r);
    InsetRect (&r, 4, 4);
    width = r.right - r.left;
    currentDispRow = 0;
    currentDispCol = 0;
    dispColFmt[0].pixWidth = 2 * width / 3 - 2;
    dispColFmt[1].pixWidth = width - dispColFmt[0].pixWidth - 4;
    SetDocProcs (dispDoc, dispClick, NULL, dispAction, NULL);
    SetDocShade (dispDoc, NULL, NULL, dispInvert, NULL);

    serv_addr.sin_family = AF_INET;
    for (i = 0; i < (Int2) DIM (_dispAddr); i++) {
	if ((dispHost = gethostbyname ((CharPtr) _dispAddr[i].fqdn)) != NULL) {
	    MemCopy (&serv_addr.sin_addr, dispHost->h_addr, dispHost->h_length);
	    sprintf (buf, "%s\t%s\n", _dispAddr[i].fqdn, inet_ntoa (serv_addr.SIN_ADDR));
	} else {
	    sprintf (buf, "%s\t%s\n", _dispAddr[i].fqdn, _dispAddr[i].addr);
	}
	AppendText (dispDoc, buf, &dispParFmt, dispColFmt, systemFont);
    }
    LPrompt_New (p->grp, 4 * X, 24 * Y, -1, -1, "Dispatcher FQDN or Address:");
    GetAppParam (_rcfile, "NET_SERV", "DISPATCHER", "", buf, sizeof buf);
    _edit_dispADDR = EditBox_New (p->grp, 26 * X, 24 * Y - dyEDIT, 25 * X, -1, buf, NULL);
    _chk_TestDispatcher = CheckBox_New (p->grp, 4 * X, 33 * Y, 30 * X, 2 * Y, "Test connection during configuration", NULL);
    if (GetAppParamBoolean (_rcfile2, p->section, "Connect", TRUE))
	CheckBox_Check (_chk_TestDispatcher, TRUE);
    return TRUE;
}

static Boolean 
Page10_EnterPage (PropPage * p, int direction)
{
    if (direction == FORWARD) {
    } else {
	NI_EndServices (dispatcher);
	dispatcher = NULL;
    }
    return TRUE;
}

static void 
HandleNetQuit (void)
{
    Int2            sockCount;

    if (dispatcher != NULL) {
	NI_EndServices (dispatcher);
	dispatcher = NULL;
    }
    if ((sockCount = NI_SocketsOpen ()) > 0) {
	TRACE ("At termination time, %d open sockets\n", sockCount);
    }
}

static Boolean 
EstabAnonConnection (void)
{
    char            buffer[64];

    EditBox_GetText (_edit_dispADDR, buffer, sizeof buffer);
    dispatcher = NI_SetDispatcher (NULL, buffer, NULL, 0, 0, NULL, FALSE);
    EditBox_GetText (_edit_UserName, buffer, sizeof buffer);
    return NI_InitServices (dispatcher, buffer, "ANONYMOUS", NULL, &dispInfoPtr) >= 0;
}

static Boolean 
Page10_LeavePage (PropPage * p, int direction)
{
    char            buffer[64];

    if (direction == FORWARD) {
	EditBox_GetText (_edit_dispADDR, buffer, sizeof buffer);
	if (buffer[0] == '\0') {
	    MsgAlert (KEY_OK, SEV_ERROR, _program, "No Dispatcher has been specified");
	    return FALSE;
	}
	if (CheckBox_IsChecked(_chk_TestDispatcher))
	{
	    if (! EstabAnonConnection ()) {
		MsgAlert (KEY_OK, SEV_ERROR, _program, "Unable to connect to dispatcher");
		NI_EndServices (dispatcher);
		dispatcher = NULL;
		return FALSE;
	    }
	}
    } else {
	/* TODO: ? */


    }
    return TRUE;
}

static Boolean 
Page10_Commit (PropPage * p)
{
    char            buffer[64];

    if (_config_net) {
	EditBox_GetText (_edit_dispADDR, buffer, sizeof buffer);
	if (dispInfoPtr == NULL) {
	    SetAppParam (_rcfile, "NET_SERV", "DISPATCHER", buffer);
	} else {
	    NI_SetDispConfig (&dispInfoPtr, buffer, sizeof buffer);
	}
    }
    return TRUE;
}

#endif


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*\
	     PAGE 11 --  NetEntrez Service Selection
\*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

#ifndef NETWORK_SUPPORTED
int 
Page11_PageProc (PropPage * ppage, PropPageMsg msg, int param)
{
    return 0;
}

#else

static const char *_p11_text[] = {
    "Please select the desired Entrez service from",
    "the list below."
};

static ColData  catColFmt[5] = {
    {0, 0, 0, 0, NULL, 'l', FALSE, FALSE, FALSE, FALSE, TRUE},  /* name */
};

static DoC      catDoc;

static Int2     currentCatRow;

static Int2     currentCatCol;

static Int2     clickedCatRow;

static Int2     clickedCatCol;


static EditBox  _edit_Service;

static ListBox  _list_Services;

static GroupBox _gbox_SvcDescr;

static GroupBox _gbox_RscDescr;

static PrompT   _txt_SvcDescr;

static PrompT   _txt_RscDescr;

static ButtoN   _chk_TestService;

static NICatalog *_catalog;

static NIToolset *_toolset[16];

static NIService *_svc[16];

static NIResource *_rsc;

static int      _svc_ct;

static int      _svc_sel;

static int      _verSvcMin = 1;

static int      _verSvcMax = 0;

static int      _verRscMin = 1;

static int      _verRscMax = 0;

static Boolean  Page11_Create (PropPage * p);

static Boolean  Page11_EnterPage (PropPage * p, int direction);

static Boolean  Page11_LeavePage (PropPage * p, int direction);

static Boolean  Page11_Commit (PropPage * p);

extern Int2     NI_DestroyMsgCatalog (NICatalogPtr cp);

static void     Page11_InitList (PropPage * p, CharPtr lastService);

static void     Page11_SelectListItem (PropPage * p, ListBox list, int n);

static NICatalog *RestoreCatalog (void);

static NICatalog *FabricateCatalog (void);

static NICatalog *Catalog_New (const char *motd);

static void     Catalog_AddToolset (NICatalog * cat, NIToolset * tset);

static NIToolset *Toolset_New (const char *name, int minv, int maxv, const char *descr);

static void     Toolset_AddResource (NIToolset * tset, NIResource * rsrc);

static NIResource *Resource_New (const char *name, int minv, int maxv, const char *descr);


int 
Page11_PageProc (PropPage * ppage, PropPageMsg msg, int param)
{
    switch (msg) {
	case MSG_Create:
	return Page11_Create (ppage);
    case MSG_IsActive:
	return (int) _config_net;
    case MSG_EnterPage:
	return Page11_EnterPage (ppage, param);
    case MSG_LeavePage:
	return Page11_LeavePage (ppage, param);
    case MSG_Commit:
	return Page11_Commit (ppage);
    }
    return PropPage_DefaultProc (ppage, msg, param);
}

static void 
catClick (DoC d, PoinT pt)
{
    MapDocPoint (d, pt, &clickedCatRow, NULL, &clickedCatCol, NULL);
}

static void
catAction (DoC d, PoinT pt)
{
    Int2            col;

    Int2            row;

    Int2            oldrow;

    Int2            oldcol;

    Int2            top;

    Int2            bottom;

    Int2            left;

    Int2            right;

    CharPtr         str;

    RecT            r;

    MapDocPoint (d, pt, &row, NULL, &col, NULL);
    if (clickedCatRow != row || clickedCatCol != col) {
	return;
    }
    if (row != currentCatRow || col != currentCatCol) {
	oldrow = currentCatRow;
	oldcol = currentCatCol;
	currentCatRow = row;
	currentCatCol = col;
	if (oldrow != 0) {
	    ObjectRect (d, &r);
	    InsetRect (&r, 4, 4);
	    if (RowIsVisible (d, oldrow, 1, &top, &bottom)) {
		if (oldcol == 1) {
		    left = r.left;
		    right = r.left + catColFmt[0].pixWidth;
		} else {
		    left = r.left + catColFmt[0].pixWidth;
		    right = r.right;
		}
		LoadRect (&r, left, top, right, bottom);
		InsetRect (&r, -1, -1);
		InvalRect (&r);
	    }
	    /*
	     * InvalDocRows (catDoc, oldrow, 1, 1);
	     */
	}
	ObjectRect (d, &r);
	InsetRect (&r, 4, 4);
	if (RowIsVisible (d, currentCatRow, 1, &top, &bottom)) {
	    if (currentCatCol == 1) {
		left = r.left;
		right = r.left + catColFmt[0].pixWidth;
	    } else {
		left = r.left + catColFmt[0].pixWidth;
		right = r.right;
	    }
	    LoadRect (&r, left, top, right, bottom);
	    InsetRect (&r, -1, -1);
	    InvalRect (&r);
	}
	/*
	 * InvalDocRows (catDoc, currentCatRow, 1, 1);
	 */
    }
    Update ();

    str = GetDocText (catDoc, row, 1, col);
    TransientSetAppParam(_rcfile, "ENTREZ_NET", "SERVICE_NAME", str);
    Page11_SelectListItem (NULL, _list_Services, row - 1);
}

static Boolean 
catInvert (DoC d, Int2 item, Int2 row, Int2 col)
{
    if (currentCatRow == item && currentCatCol == col) {
	return TRUE;
    } else {
	return FALSE;
    }
}

static Boolean 
Page11_Create (PropPage * p)
{
    RecT            r;

    Int2            width;

    catDoc = DocumentPanel (_win, 28, 3);
    {
	RecT            r;

	LoadRect (&r, 4 * X, 13 * Y, 50 * X + 8,
		  13 * Y + 3 * stdLineHeight + 8);
	SetPosition (catDoc, &r);
	CtrlGrp_AddControl (p->grp, catDoc);
    }
    ObjectRect (catDoc, &r);
    InsetRect (&r, 4, 4);
    width = r.right - r.left;
    currentCatRow = 0;
    currentCatCol = 0;
    catColFmt[0].pixWidth = width - 2;
    SetDocProcs (catDoc, catClick, NULL, catAction, NULL);
    SetDocShade (catDoc, NULL, NULL, catInvert, NULL);


    MultiPrompt_New (p->grp, 4 * X, 6 * Y, _p11_text, DIM (_p11_text));
    _gbox_SvcDescr = GroupBox_New (p->grp, 4 * X, 20 * Y, 46 * X, 4 * Y, "Service Description", NULL);
    _txt_SvcDescr = LPrompt_New (p->grp, 7 * X, 22 * Y, 40 * X, -1, "");
    _gbox_RscDescr = GroupBox_New (p->grp, 4 * X, 26 * Y, 46 * X, 4 * Y, "Default Dataset Description", NULL);
    _txt_RscDescr = LPrompt_New (p->grp, 7 * X, 28 * Y, 40 * X, -1, "");
    _chk_TestService = CheckBox_New (p->grp, 4 * X, 33 * Y, 30 * X, 2 * Y, "Test connection during configuration", NULL);
    if (GetAppParamBoolean (_rcfile2, p->section, "Connect", TRUE))
	CheckBox_Check (_chk_TestService, TRUE);

    return TRUE;
}

static Boolean 
Page11_EnterPage (PropPage * p, int direction)
{
    char buffer[50];

    if (direction == BACKWARD && CheckBox_IsChecked(_chk_TestDispatcher)) {
	if (! EstabAnonConnection()) {
	    MsgAlert (KEY_OK, SEV_ERROR, _program, "Unable to connect to dispatcher");
	    NI_EndServices (dispatcher);
	    dispatcher = NULL;
	    return FALSE;
	}
    }

    if (dispatcher != NULL) {
	_catalog = NI_GetCatalog (dispatcher);
	if (_catalog == NULL) {
	    ErrPostEx (SEV_ERROR, 0, 0, "Unable to retrieve service catalog from the dispatcher");
	    Disable (_chk_TestService);
	} else {
	    Enable (_chk_TestService);
	}
    } else {
	CheckBox_Check (_chk_TestService, FALSE);
	Disable (_chk_TestService);
    }
    if (_catalog == NULL) {
	if ((_catalog = RestoreCatalog ()) == NULL)
	{
	    _catalog = FabricateCatalog ();
	    MsgAlert (KEY_OK, SEV_WARNING, _program, "Using a fabricated service catalog since no Dispatcher connection is currently in-place");
	}
    }
    GetAppParam(_rcfile, "ENTREZ_NET", "SERVICE_NAME", "", buffer, sizeof buffer);
    Page11_InitList (p, buffer);
    if (_svc_ct <= 0) {
	if (_catalog != NULL)
	{
	    NI_DestroyMsgCatalog(_catalog);
	}
	_catalog = FabricateCatalog ();
	MsgAlert (KEY_OK, SEV_WARNING, _program, "Using a fabricated service catalog since no suitable services are currently available");
	Page11_InitList (p, buffer);
    }
    return TRUE;
}

static Boolean 
Page11_LeavePage (PropPage * p, int direction)
{
    char            buffer[64];

    NI_HandPtr      shSvc;

    NIService      *svc;

    Boolean         outgoingConns;


    if (direction == FORWARD) {
	if (_svc_sel < 0 || _svc_sel >= _svc_ct) {
	    MsgAlert (KEY_OK, SEV_ERROR, _program, "No service has been specified\n");
	    return FALSE;
	}

	svc = _svc[_svc_sel];
	MemFree (svname);
	svname = StringSave (svc->name);
	TransientSetAppParam(_rcfile, "ENTREZ_NET", "SERVICE_NAME", svname);
	_verSvcMin = svc->minVersion;
	_verSvcMax = svc->maxVersion;
	MemFree (resname);
	resname = StringSave ("Entrez");
	_verRscMin = 1;
	_verRscMax = 0;
	if (_rsc != NULL) {
	    MemFree (resname);
	    resname = StringSave (_rsc->name);
	    _verRscMin = _rsc->minVersion;
	    _verRscMax = _rsc->maxVersion;
	}

	if (CheckBox_IsChecked (_chk_TestService)) {
	    WatchCursor ();
	    /* must disconnect as anonymous and reconnect as GUEST */
	    NI_EndServices (dispatcher);
	    EditBox_GetText (_edit_dispADDR, buffer, sizeof buffer);
	    outgoingConns = CheckBox_IsChecked (_chk_WantOutgoing);
	    dispatcher = NI_SetDispatcher (NULL, buffer, NULL, 0, 0, NULL,
					   outgoingConns);
	    EditBox_GetText (_edit_UserName, buffer, sizeof buffer);
	    if (NI_InitServices (dispatcher, buffer, "GUEST", NULL, &dispInfoPtr) < 0) {
		ArrowCursor ();
		ErrShow ();
		NI_DestroyMsgCatalog (_catalog);        /* JAE ??? */
		_catalog = NULL;
		_svc_ct = 0;
		NI_EndServices (dispatcher);
		dispatcher = NULL;
		EstabAnonConnection ();
		return FALSE;
	    }
	    WatchCursor ();
	    if (dispatcher->adminInfo != NULL && dispatcher->adminInfo[0] != NULLB) {
		_adminInfo = StringSave (dispatcher->adminInfo);
	    }
	    if ((shSvc = NI_ServiceGet (dispatcher, svname, _verSvcMin, _verSvcMax, resname, SERVICE_TYPE, _verRscMin, _verRscMax)) == NULL) {
		ArrowCursor ();
		MsgAlert (KEY_OK, SEV_ERROR, _program, "Unable to get service [%s]: %s", svname, ni_errlist[ni_errno]);
		return FALSE;
	    } else {
		NI_DestroyMsgCatalog (_catalog);
		_catalog = NULL;
		_svc_ct = 0;
		NI_ServiceDisconnect (shSvc);
		NI_EndServices (dispatcher);
		dispatcher = NULL;
		ArrowCursor ();
		return TRUE;
	    }
	} else {
	    NI_DestroyMsgCatalog (_catalog);
	    _catalog = NULL;
	    _svc_ct = 0;
	    NI_EndServices (dispatcher);
	    dispatcher = NULL;
	    return TRUE;
	}
    } else { /* backward */
	NI_EndServices (dispatcher);
	dispatcher = NULL;
	NI_DestroyMsgCatalog (_catalog);
	_catalog = NULL;
	_svc_ct = 0;
	return TRUE;
    }
}

static Boolean 
Page11_Commit (PropPage * p)
{

    SetAppParam (_rcfile, _rcfile, "MEDIA", "ENTREZ_NET");
    SetAppParam (_rcfile, "REFERENCE_FROM_NET", "MEDIA", "ENTREZ_NET");
    SetAppParam (_rcfile, "SEQUENCE_FROM_NET", "MEDIA", "ENTREZ_NET");
    SetAppParam (_rcfile, "LINKS_FROM_NET", "MEDIA", "ENTREZ_NET");
    SetAppParamInt (_rcfile, "LINKS_FROM_NET", "ENTR_SEQ__ENTR_SEQ", 1);
    SetAppParamInt (_rcfile, "LINKS_FROM_NET", "ENTR_REF__ENTR_SEQ", 1);
    SetAppParamInt (_rcfile, "LINKS_FROM_NET", "ENTR_SEQ__ENTR_REF", 1);
    SetAppParamInt (_rcfile, "LINKS_FROM_NET", "ENTR_REF__ENTR_REF", 1);
    SetAppParam (_rcfile, "ENTR_LINK", "CHANNELS", "LINKS_FROM_NET");
    SetAppParam (_rcfile, "ENTR_REF", "CHANNELS", "REFERENCE_FROM_NET");
    SetAppParam (_rcfile, "ENTR_SEQ", "CHANNELS", "SEQUENCE_FROM_NET");
    SetAppParam (_rcfile, "ENTREZ_NET", "TYPE", "NET");

    SetAppParam (_rcfile, "ENTREZ_NET", "SERVICE_NAME", svname);
    SetAppParam (_rcfile, "ENTREZ_NET", "RESOURCE_NAME", resname);
    SetAppParam (_rcfile, "ENTREZ_NET", "RESOURCE_TYPE", SERVICE_TYPE);
    SetAppParamInt (_rcfile, "ENTREZ_NET", "SERV_VERS_MIN", _verSvcMin);
    SetAppParamInt (_rcfile, "ENTREZ_NET", "SERV_VERS_MAX", _verSvcMax);
    SetAppParamInt (_rcfile, "ENTREZ_NET", "RES_VERS_MIN", _verRscMin);
    SetAppParamInt (_rcfile, "ENTREZ_NET", "RES_VERS_MAX", _verRscMax);



    return TRUE;
}

static void 
Page11_InitList (PropPage * p, CharPtr lastService)
{
    NIToolset      *tset;

    Node           *tn;

    char            buf[50];

    int             localSelService = -1;

    ASSERT (_catalog != NULL);

    _svc_ct = 0;
    Reset (catDoc);
    currentCatRow = 0;
    currentCatCol = 0;

    for (tn = _catalog->toolsetL; tn; tn = tn->next) {
	if ((tset = (NIToolset *) tn->elem) != NULL) {
	    NIService      *svc;

	    Node           *sn;

	    for (sn = tset->services; sn; sn = sn->next) {
		svc = (NIService *) sn->elem;
		if (svc->typeL && strcmp ((char *) svc->typeL->elem, SERVICE_TYPE) == 0) {
		    int             k = _svc_ct++;

		    _toolset[k] = tset;
		    _svc[k] = svc;
		    sprintf (buf, "%s\n", svc->name);
		    AppendText (catDoc, buf, &dispParFmt, catColFmt, systemFont);
		    if (lastService != NULL && StrCmp(svc->name, lastService) == 0) {
			localSelService = k;
		    }

		}
	    }
	}
    }

    if (localSelService >= 0)
    {
	Page11_SelectListItem (NULL, _list_Services, localSelService);
	currentCatRow = localSelService + 1;
	currentCatCol = 1;
	UpdateDocument(catDoc, 0, 0);
    } else {
	Page11_SelectListItem (NULL, _list_Services, -1);
    }
}

static Boolean
ResourceCompatWService (NIResPtr resp, NISvcPtr service)
{
    NodePtr         np = service->typeL;

    if (np == NULL)
	return FALSE;

    do {
	np = np->next;
	if (StrICmp (np->elem, MATCHES_ANY_TYPE) == 0 ||
		StrICmp (np->elem, resp->type) == 0)
	    return TRUE;
    } while (np != NULL && np != service->typeL);

    return FALSE;
}


static void 
Page11_SelectListItem (PropPage * p, ListBox list, int n)
{
    _rsc = NULL;
    if (n >= 0 && n < _svc_ct) {
	NIToolset      *tset = _toolset[n];

	NIService      *svc = _svc[n];

	NIResource     *rsc;

	Node           *rn;

	/* find the "best" resource for this service */
	for (rn = tset->resources; rn; rn = rn->next) {
	    rsc = (NIResource *) rn->elem;
	    if (!ResourceCompatWService (rsc, svc))
		continue;
	    if (_rsc == NULL)
		_rsc = rsc;
	    else if (_rsc->maxVersion != 0) {
		if (rsc->maxVersion == 0)
		    _rsc = rsc;
	    }
	}

	EditBox_SetText (_edit_Service, svc->name);
	SetTitle (_txt_SvcDescr, svc->descrip);
	SetTitle (_txt_RscDescr, _rsc->descrip);
    } else {
	n = -1;
	SetTitle (_txt_SvcDescr, NULL);
	SetTitle (_txt_RscDescr, NULL);
    }

    _svc_sel = n;
}

static void 
SaveCatalog (NICatalog * cat)
{
    /*** not implemented ***/

    /*** save the current catalog to entrezcf.ini ***/
}



static NICatalog *
RestoreCatalog (void)
{
    NICatalog      *cat = NULL;

    /*** not implemented ***/


    /*** read the most recent catalog from entrezcf.ini ***/


    return cat;
}

static NICatalog *
FabricateCatalog (void)
{
    NICatalog      *cat = Catalog_New (NULL);

    ErrLogPrintf ("Using fabricated service catalog\n");
    if (cat != NULL) {
	NIToolset      *tset1 = Toolset_New ("Entrez", 1, 0, "Network Entrez");

	NIToolset      *tset2 = Toolset_New ("EntrezBigMed", 1, 0, "Entrez with expanded MEDLINE subset");

	Toolset_AddResource (tset1, Resource_New ("Entrez", 130, 130, "Entrez 13.0"));
	Toolset_AddResource (tset1, Resource_New ("Entrez", 140, 140, "Entrez 14.0"));
	Toolset_AddResource (tset1, Resource_New ("Entrez", 1, 0, "Most recent Entrez release"));
	Toolset_AddResource (tset2, Resource_New ("Entrez", 1, 0, "Entrez plus Genetics subset of MEDLINE"));
	Catalog_AddToolset (cat, tset2);
	Catalog_AddToolset (cat, tset1);
	SaveCatalog (cat);
    }
    return cat;
}




static NICatalog *
Catalog_New (const char *motd)
{
    NICatalog      *cat = (NICatalog *) MemNew (sizeof (NICatalog));

    if (cat != NULL) {
	cat->motd = motd ? StrSave (motd) : NULL;
    }
    return cat;
}

static void 
Catalog_AddToolset (NICatalog * cat, NIToolset * tset)
{
    Node           *node = (Node *) MemNew (sizeof (Node));

    if (node != NULL) {
	ASSERT (cat != NULL);
	ASSERT (tset != NULL);

	node->next = cat->toolsetL;
	if (node->next)
	    node->next->last = node;
	cat->toolsetL = node;
	node->elem = (void *) tset;
    }
}

static NIToolset *
Toolset_New (const char *name, int minv, int maxv, const char *descr)
{
    NIToolset      *tset = (NIToolset *) MemNew (sizeof (NIToolset));

    if (tset != NULL) {
	NIService      *svc = MemNew (sizeof (NIService));

	if (svc == NULL) {
	    MemFree ((void *) tset);
	    tset = NULL;
	} else {
	    svc->name = name ? StrSave (name) : NULL;
	    svc->minVersion = (Uint2) minv;
	    svc->maxVersion = (Uint2) maxv;
	    svc->descrip = descr ? StrSave (descr) : NULL;
	    svc->typeL = (Node *) MemNew (sizeof (Node));
	    svc->typeL->elem = (void *) StrSave (SERVICE_TYPE);
	    svc->typeL->next = svc->typeL; /* this is a single-element circular list */
	    svc->typeL->last = svc->typeL; /* this is a single-element circular list */
	}
	tset->services = (Node *) MemNew (sizeof (Node));
	tset->services->elem = (void *) svc;
    }
    return tset;
}

static void 
Toolset_AddResource (NIToolset * tset, NIResource * rsrc)
{
    Node           *node = (Node *) MemNew (sizeof (Node));

    if (node != NULL) {
	ASSERT (tset != NULL);
	ASSERT (rsrc != NULL);

	node->next = tset->resources;
	if (node->next)
	    node->next->last = node;
	tset->resources = node;
	node->elem = (void *) rsrc;
    }
}

static NIResource *
Resource_New (const char *name, int minv, int maxv, const char *descr)
{
    NIResource     *rsrc = (NIResource *) MemNew (sizeof (NIResource));

    if (rsrc != NULL) {
	rsrc->name = name ? StrSave (name) : NULL;
	rsrc->minVersion = (Uint2) minv;
	rsrc->maxVersion = (Uint2) maxv;
	rsrc->descrip = descr ? StrSave (descr) : NULL;
	rsrc->type = StringSave(SERVICE_TYPE);
    }
    return rsrc;
}

#endif


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*\
	     PAGE 12 --  Finish
\*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

static const char *_p12_text[] = {
    "You have entered all the necessary information.  The",
    "settings may now be saved to the configuration file."
};

static PrompT   _txt_WillCopyIndex;

static PrompT   _fyi[6];

static ButtoN   _p12_accept;

static Boolean  Page12_Create (PropPage * p);

static Boolean  Page12_EnterPage (PropPage * p, int direction);

static void     Page12_SetFyiDouble (PropPage * p, EntrezRelInfo * cd, CharPtr net);

static void     Page12_SetFyiCd (PropPage * p, EntrezRelInfo * inf);

static void     Page12_SetFyiNet (PropPage * p, CharPtr inf);


int 
Page12_PageProc (PropPage * ppage, PropPageMsg msg, int param)
{
    switch (msg) {
	case MSG_Create:
	return Page12_Create (ppage);
	case MSG_EnterPage:
	return Page12_EnterPage (ppage, param);
    }
    return PropPage_DefaultProc (ppage, msg, param);
}

static Boolean 
Page12_Create (PropPage * p)
{
    int             i, y, dy;

    MultiPrompt_New (p->grp, 4 * X, 6 * Y, _p12_text, DIM (_p12_text));

    SelectFont (_font2);
    dy = LineHeight ();
    GroupBox_New (p->grp, 4 * X, 12 * Y, 46 * X, 5 * Y + 6 * dy, "For your information", NULL);
    for (i = 0, y = 15 * Y; i < DIM (_fyi); ++i, y += dy) {
	_fyi[i] = LPrompt_New (p->grp, 6 * X, y, 43 * X, dy, "some text goes here");
    }

    _txt_WillCopyIndex = LPrompt_New (p->grp, 4 * X, 28 * Y, -1, 2 * Y, "Index files will be copied automatically.");
    CtrlGrp_ShowControl (p->grp, _txt_WillCopyIndex, FALSE);
    y = WIN_ySIZE - 2 * DFLT_yBUTTON - 3 * Y;
    _p12_accept = Button_New (p->grp, 31 * X, y, 19 * X, DFLT_yBUTTON, "Save settings", _OnOkay);
    return TRUE;
}

static Boolean 
Page12_EnterPage (PropPage * p, int direction)
{
    if (direction == FORWARD) {
	if (_config_cd) {
	    if (_config_net)
		Page12_SetFyiDouble (p, &_release_cd, _adminInfo);
	    else
		Page12_SetFyiCd (p, &_release_cd);
	} else if (_config_net) {
	    Page12_SetFyiNet (p, _adminInfo);
	}
	CtrlGrp_ShowControl (p->grp, _txt_WillCopyIndex, (Boolean) (_index_types > 0));
    }
    return TRUE;
}

static char    *_mo[] = {"???", "JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"};

static void 
Page12_SetFyiNet (PropPage * p, char *inf)
{
    int             k = 0;

    char            buffer[100];

    char            buf2[100];

    CharPtr         ptr;

    if (inf != NULL) {
	SetTitle (_fyi[k++], "Your Network Entrez administrator is:");
	sprintf (buffer, "  %s", inf);
	if ((ptr = StrStr(buffer, " Email:")) != NULL)
	{ /* parse into two lines */
	    *ptr = NULLB;
	    SetTitle (_fyi[k++], buffer);
	    sprintf (buf2, "      %s", ++ptr);
	    SetTitle (_fyi[k++], buf2);
	} else {
	    SetTitle (_fyi[k++], buffer);
	}
    } else {
	SetTitle (_fyi[k++], "Your Network Entrez administrator is unidentified");
    }
    while (k < DIM (_fyi))
	SetTitle (_fyi[k++], NULL);
}


static void 
Page12_SetFyiCd (PropPage * p, EntrezRelInfo * inf)
{
    int             i, k = 0;

    char            buffer[64];

	SetAppParamInt(_rcfile2,"CdEntrezX","release",inf->rel_major);

    sprintf (buffer, "Entrez %d.%d", inf->rel_major, inf->rel_minor);
    if (inf->rel_date.year != 0) {
	sprintf (strchr (buffer, 0), ",  %d-%s-%d", inf->rel_date.day,
		 _mo[inf->rel_date.month], inf->rel_date.year);
    }
    SetTitle (_fyi[k++], buffer);
    SetTitle (_fyi[k++], NULL);
    for (i = 0; i < inf->types; ++i) {
	sprintf (buffer, "%8ld %s documents", inf->docs[i], _dtyp[i]);
	SetTitle (_fyi[k++], buffer);
    }
    while (k < DIM (_fyi))
	SetTitle (_fyi[k++], NULL);
}

static void 
Page12_SetFyiDouble (PropPage * p, EntrezRelInfo * cd, char *net)
{
    int             k = 0;

    char            buffer[100];

    CharPtr         ptr;

    char            buf2[100];

	SetAppParamInt(_rcfile2,"CdEntrezX","release",cd->rel_major);

    sprintf (buffer, "Entrez for CD-ROM %d.%d", cd->rel_major, cd->rel_minor);
    if (cd->rel_date.year != 0) {
	sprintf (strchr (buffer, 0), ",  %d-%s-%d", cd->rel_date.day,
		 _mo[cd->rel_date.month], cd->rel_date.year);
    }
    SetTitle (_fyi[k++], buffer);
    SetTitle (_fyi[k++], NULL);

    if (net != NULL) {
	SetTitle (_fyi[k++], "Your Network Entrez administrator is:");
	sprintf (buffer, "  %s", net);
	if ((ptr = StrStr(buffer, " Email:")) != NULL)
	{ /* parse into two lines */
	    *ptr = NULLB;
	    SetTitle (_fyi[k++], buffer);
	    ptr++;
	    sprintf (buf2, "      %s", ++ptr);
	    SetTitle (_fyi[k++], buf2);
	} else {
	    SetTitle (_fyi[k++], buffer);
	}
    } else {
	SetTitle (_fyi[k++], "Your Network Entrez administrator is unidentified");
    }
    while (k < DIM (_fyi))
	SetTitle (_fyi[k++], NULL);
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*\
	     PropPage
\*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/


PropPage       *
PropPage_Construct (PropPage * ppage, const char *title, const char *section, PropPageProc proc)
{
    if (ppage != NULL) {
	RecT            r;

	ASSERT (title != NULL);
	ASSERT (proc != NULL);

	ppage->proc = proc;
	ppage->section = (char *) section;
	ppage->grp = CtrlGrp_New ();

	if ((*proc) (ppage, MSG_Create, 0)) {
	    CtrlGrp_ShowGroup (ppage->grp, FALSE);
	} else {
	    CtrlGrp_Free (ppage->grp);
	    ppage->grp = NULL;
	    return NULL;
	}
	ppage->title_prompt = StaticPrompt (_win, (char *) title, (Int2) (40 * X), (Int2) (3 * Y), _font1, 'l');
	ObjectRect (ppage->title_prompt, &r);
	Nlm_OffsetRect (&r, (Int2) ((4 * X) - r.left), (Int2) ((2 * Y) - r.top));
	SetPosition (ppage->title_prompt, &r);
	Hide (ppage->title_prompt);
    }
    return ppage;
}

PropPage       *
PropPage_Destruct (PropPage * ppage)
{
    if (ppage != NULL) {
	CtrlGrp_Free (ppage->grp);
    }
    return ppage;
}



int 
PropPage_SendMessage (PropPage * ppage, PropPageMsg msg, int param)
{
    if (ppage != NULL) {
	return (*ppage->proc) (ppage, msg, param);
    }
    return 0;
}

int 
PropPage_DefaultProc (PropPage * ppage, PropPageMsg msg, int param)
{
    int             result = 0;

    switch (msg) {
    case MSG_Create:
    case MSG_Destroy:
    case MSG_IsActive:
    case MSG_Commit:
	result = 1;
	break;

    case MSG_EnterPage:
	result = 1;
	/* CtrlGrp_ShowGroup(ppage->grp,TRUE); */
	break;

    case MSG_LeavePage:
	result = 1;
	/* CtrlGrp_ShowGroup(ppage->grp,FALSE); */
	break;
    }
    return result;
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*\
	     FileToCopy
\*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

FileToCopy     *
FileToCopy_Construct (FileToCopy * ftc, const char *name, long size, int cd, int dir)
{
    if (ftc != NULL) {
	ASSERT (name != NULL);
	ftc->filename[0] = '\0';
	strncat (ftc->filename, name, sizeof ftc->filename);
	ftc->filesize = size;
	ftc->cd_num = cd;
	ftc->dir_num = dir;
    }
    return ftc;
}

FileToCopy     *
FileToCopy_Destruct (FileToCopy * ftc)
{
    if (ftc != NULL)
	memset ((void *) ftc->filename, 0, sizeof (FileToCopy));
    return ftc;
}


Boolean 
FileToCopy_RemoveOldCopy (FileToCopy * ftc, const char *dst_folder)
{
    char            path[256];

    ASSERT (ftc != NULL);
    ASSERT (dst_folder != NULL);

    FileBuildPath (strcpy (path, dst_folder), NULL, ftc->filename);
    if (FileRemove (path))
	ErrLogPrintf ("   %s REMOVED\n", ftc->filename);
    return TRUE;
}


static char    *_dirname[] = {SYS_DIRNAME, IDX_DIRNAME, SUM_DIRNAME,
TRM_DIRNAME, LNK_DIRNAME, REC_DIRNAME};

Boolean 
FileToCopy_PerformCopy (FileToCopy * ftc, CdRomInfo * info, const char *src_folder,
			const char *dst_folder, Monitor * mon)
{
    char            path[256];

    FILE           *fin;

    FILE           *fout;

    long            mon_val;

    size_t          bytes_remaining, bytes;

    char            buffer[512], *p;

    ASSERT (ftc != NULL);
    ASSERT (info != NULL);
    ASSERT (src_folder != NULL);
    ASSERT (dst_folder != NULL);

    ErrLogPrintf ("   %s\n", ftc->filename);

    strcpy (path, src_folder);
    p = strchr (path, '\0');
    FileBuildPath (path, _dirname[ftc->dir_num], ftc->filename);        /****/
    if (info->upper_case)
	StrUpper (p);
    if (info->semicolon_one)
	strcat (path, ";1");
    if ((fin = FileOpen (path, "rb")) == NULL) {
	ErrPostEx (SEV_WARNING, 1, 0, "Unable to open file for reading: %s", path);
	return FALSE;
    }
    bytes_remaining = FileLength (path);

    FileBuildPath (strcpy (path, dst_folder), NULL, ftc->filename);
    if ((fout = FileOpen (path, "wb")) == NULL) {
	FileClose (fin);
	ErrPostEx (SEV_WARNING, 1, 0, "Unable to open file for writing: %s", path);
	return FALSE;
    }
    if (_vbuf_in)
	setvbuf (fin, _vbuf_in, _IOFBF, VBUFSIZE);
    if (_vbuf_out)
	setvbuf (fout, _vbuf_out, _IOFBF, VBUFSIZE);

    mon_val = mon->intValue;
    while ((bytes = fread (buffer, 1, sizeof buffer, fin)) > 0) {
	if (fwrite (buffer, 1, bytes, fout) != bytes) {
	    FileClose (fin);
	    FileClose (fout);
	    FileRemove (path);
	    ErrPostEx (SEV_WARNING, 1, 0, "Error writing file: %s", path);
	    return FALSE;
	}
	mon_val += (long) (bytes / 2);
	if (! MonitorIntValue (mon, mon_val)) {
	  file_copying_cancelled = TRUE;
      FileClose (fin);
      FileClose (fout);
      return FALSE;
	}
	/* bytes_remaining -= bytes; */
    }

    FileClose (fin);
    FileClose (fout);
    return TRUE;
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*\
	     MISC. UTILITY FUNCTIONS
\*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

int 
DoTestPath (const char *path, int vol_expected)
{
    CdRomInfo       info;

    /* Test for valid Entrez CD image */
    if (!CdTestPath (path, &info)) {
	MsgAlert (KEY_OK, SEV_ERROR, _program, "The path [%s] is invalid or does not point "
		  "to an Entrez CD-ROM image", path);
	return 0;
    }
    if (info.cd_count != _volume_ct) {
	SetAppParamInt (_rcfile2, "Cd-Sources", "Count", info.cd_count);
	MsgAlert (KEY_OK, SEV_WARNING, _program, "This Entrez release consists of %d CD-ROMs "
	       "and not %d as expected.  This fact has been stored and the "
		"program will now close.  Please run this program again to "
		  "complete the configuration process.",
		  info.cd_count, _volume_ct);
	ErrLogPrintf ("\n[[ HALT ]]\n");
	MyQuitProgram ();
	return 0;
    }
    /**** INSERT CODE HERE TO CHECK RELEASE NUMBER ****/
    /* check against what? */


    /* Make sure it is the requested volume */
    if (vol_expected != 0 && info.cd_num != vol_expected) {
	MsgAlert (KEY_OK, SEV_ERROR, _program, "The path [%s] points to an image of Entrez%d "
	   "and not Entrez%d as expected", path, info.cd_num, vol_expected);
	return 0;
    }
    return info.cd_count;
}

Boolean 
DoCdEntrezInit (void)
{
    if (_entrez_info == NULL) {
	int             i;

	if (!cd3_CdInit ())
	    return FALSE;
	if ((_entrez_info = cd3_CdGetInfo ()) == NULL)
	    return FALSE;
	_release_cd.rel_major = _entrez_info->version;
	_release_cd.rel_minor = _entrez_info->issue;
	_release_cd.rel_date = _entrez_info->release_date;
	_release_cd.types = _entrez_info->type_count;
	for (i = 0; i < _entrez_info->type_count; ++i)
	    _release_cd.docs[i] = _entrez_info->types[i].num;

	if (_entrez_info->cd_count != _volume_ct) {
	    SetAppParamInt (_rcfile2, "Cd-Sources", "Count", _entrez_info->cd_count);
	    MsgAlert (KEY_OK, SEV_WARNING, _program, "This Entrez release consists of %d CD-ROMs "
	       "and not %d as expected.  This fact has been stored and the "
		"program will now close.  Please run this program again to "
		      "complete the configuration process.",
		      _entrez_info->cd_count, _volume_ct);

	    ErrLogPrintf ("\n[[ HALT ]]\n");
	    MyQuitProgram ();
	}
    }
    return TRUE;
}

Boolean 
DoCdEntrezFini (void)
{
    if (_entrez_info != NULL) {
	cd3_CdFini ();
	_entrez_info = NULL;
    }
    return TRUE;
}


ButtoN 
Button_New (CtrlGrp * grp, int x, int y, int cx, int cy, char *text, BtnActnProc proc)
{
    ButtoN          btn = PushButton (_win, text, proc);

    if (btn != NULL) {
	RecT            r;

	LoadRect (&r, (Int2) x, (Int2) y, (Int2) (x + cx), (Int2) (y + cy));
	SetPosition (btn, &r);
	CtrlGrp_AddControl (grp, btn);
    }
    return btn;
}

ButtoN 
CheckBox_New (CtrlGrp * grp, int x, int y, int cx, int cy, char *text, BtnActnProc proc)
{
    ButtoN          btn = CheckBox (_win, text, proc);

    if (btn != NULL) {
	RecT            r;

	ObjectRect (btn, &r);
	Nlm_OffsetRect (&r, (Int2) (x - r.left), (Int2) (y - r.top));
	SetPosition (btn, &r);
	CtrlGrp_AddControl (grp, btn);
    }
    return btn;
}

EditBox 
EditBox_New (CtrlGrp * grp, int x, int y, int cx, int cy, char *text, TxtActnProc proc)
{
    /* NOTE: cy is no longer used */
    EditBox         edit = DialogText (_win, text, (Int2) (6 * X), proc);

    if (edit != NULL) {
	RecT            r;

	LoadRect (&r, (Int2) x, (Int2) y, (Int2) (x + cx), (Int2) (y + DFLT_yEDIT));
	SetPosition (edit, &r);
	CtrlGrp_AddControl (grp, edit);
    }
    return edit;
}

int 
EditBox_SaveText (EditBox edit, char **pstr)
{
    char            buffer[256];

    MemFree (*pstr);
    EditBox_GetText (edit, buffer, sizeof buffer);
    *pstr = (buffer[0]) ? StrSave (buffer) : NULL;
    return strlen (buffer);
}


void 
EditBox_SetInt (EditBox edit, int value)
{
    char            buffer[32];

    sprintf (buffer, "%d", value);
    EditBox_SetText (edit, buffer);
}

int 
EditBox_GetInt (EditBox edit)
{
    char            buffer[32];

    EditBox_GetText (edit, buffer, sizeof buffer);
    return atoi (buffer);
}

ListBox 
ListBox_New (CtrlGrp * grp, int x, int y, int cx, int cy, LstActnProc proc)
{
    ListBox         list = SingleList (_win, (Int2) cx, (Int2) cy, proc);

    if (list != NULL) {
	RecT            r;

	LoadRect (&r, (Int2) x, (Int2) y, (Int2) (x + cx), (Int2) (y + cy));
	SetPosition (list, &r);
	CtrlGrp_AddControl (grp, list);
    }
    return list;
}

int 
ListBox_AddString (ListBox lb, char *str)
{
    ListItem (lb, str);
    return TRUE;
}

GroupBox 
GroupBox_New (CtrlGrp * grp, int x, int y, int cx, int cy, char *title, GrpActnProc proc)
{
    GroupBox        gbox = NormalGroup (_win, (Int2) cx, (Int2) cy, title, _font2, proc);

    if (gbox != NULL) {
	RecT            r;

	LoadRect (&r, (Int2) x, (Int2) y, (Int2) (x + cx), (Int2) (y + cy));
	SetPosition (gbox, &r);
	CtrlGrp_AddControl (grp, gbox);
    }
    return gbox;
}

GroupBox 
HiddenGroupBox_New (CtrlGrp * grp, int x, int y, int cx, int cy, GrpActnProc proc)
{
    GroupBox        gbox = HiddenGroup (_win, (Int2) cx, (Int2) cy, proc);

    if (gbox != NULL) {
	RecT            r;

	LoadRect (&r, (Int2) x, (Int2) y, (Int2) (x + cx), (Int2) (y + cy));
	SetPosition (gbox, &r);
	CtrlGrp_AddControl (grp, gbox);
    }
    return gbox;
}

Radio 
Radio_New (CtrlGrp * grp, int x, int y, int cx, int cy, char *title, GroupBox gbox)
{
    RecT            r;

    Radio           rad;

    GetPosition (gbox, &r);
    rad = RadioButton (gbox, title);
    SetPosition (gbox, &r);
    if (rad != NULL) {
	ObjectRect (rad, &r);
	Nlm_OffsetRect (&r, (Int2) (x - r.left), (Int2) (y - r.top));
	SetPosition (rad, &r);
	CtrlGrp_AddControl (grp, rad);
    }
    return rad;
}


ComboBox 
ComboBox_New (CtrlGrp * grp, int x, int y, int cx, int cy, PupActnProc proc)
{
    ComboBox        combo = Nlm_PopupList (_win, TRUE, proc);

    if (combo != NULL) {
	RecT            r;

	ObjectRect (combo, &r);
	Nlm_OffsetRect (&r, (Int2) (x - r.left), (Int2) (y - r.top));
	SetPosition (combo, &r);
	CtrlGrp_AddControl (grp, combo);
    }
    return combo;
}


int 
ComboBox_AddString (ComboBox combo, const char *str)
{
    PopupItem (combo, (char *) str);
    return 0;
}


PrompT 
Prompt_New (CtrlGrp * grp, int x, int y, int cx, int cy, const char *str, int just)
{
    PrompT          prompt;

    PoinT           pt;

    int             ht;

    SelectFont (_font2);
    if (cx <= 0) {
	cx = StringWidth ((char *) str) + 2;
    }
    if (cy < (ht = LineHeight ())) {
	cy = ht;
    }
    pt.x = x;
    pt.y = y;
    SetNextPosition(_win, pt);
    prompt = Nlm_StaticPrompt (_win, (char *) str, (Int2) cx, (Int2) cy, _font2, (char) just);
    if (prompt != NULL) {
	CtrlGrp_AddControl (grp, prompt);
    }
    return prompt;
}


void 
MultiPrompt_New (CtrlGrp * grp, int x, int y, const char **slist, int count)
{
    int             i, ypos, cy, cx;

    const char    **p;

    SelectFont (_font2);
    cy = LineHeight ();
    for (i = 0, p = slist, ypos = y; i < count; ++i, ypos += cy, ++p) {
	cx = StringWidth ((char *) *p) + 2;
	Prompt_New (grp, x, ypos, cx, cy, *p, 'l');
    }
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*\
	     ControlGroup
\*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

#define CHUNK 4


CtrlGrp        *
CtrlGrp_Construct (CtrlGrp * grp)
{
    if (grp != NULL) {
	grp->list = MemNew (CHUNK * sizeof (CtrlGrpItem));
	grp->slots = CHUNK;
	grp->vis = TRUE;
    }
    return grp;
}

CtrlGrp        *
CtrlGrp_Destruct (CtrlGrp * grp)
{
    if (grp != NULL) {
	MemFree ((void *) grp->list);
    }
    return grp;
}

void 
CtrlGrp_AddControl (CtrlGrp * grp, Control ctrl)
{
    if (grp != NULL) {
	int             k = grp->count;

	if (k == grp->slots) {
	    void           *ptr = MemMore ((void *) grp->list, (CHUNK + k) * sizeof (CtrlGrpItem));

	    if (!ptr)
		return;
	    grp->slots += CHUNK;
	    grp->list = (CtrlGrpItem *) ptr;
	}
	grp->list[k].ctrl = ctrl;
	grp->list[k].vis = TRUE;
	grp->count = k + 1;
    }
}

void 
CtrlGrp_ShowGroup (CtrlGrp * grp, Boolean vis)
{
    if (vis != grp->vis) {
	int             i;

	CtrlGrpItem    *p = grp->list;

	for (i = 0; i < grp->count; ++i, ++p) {
	    if (vis && p->vis) {
		Show (p->ctrl);
	    } else {
		Hide (p->ctrl);
	    }
	}
	grp->vis = vis;
    }
}

void 
CtrlGrp_ShowControl (CtrlGrp * grp, Control ctrl, Boolean vis)
{
    int             i;

    CtrlGrpItem    *p = grp->list;

    for (i = 0; i < grp->count; ++i, ++p) {
	if (p->ctrl == ctrl) {
	    if (vis) {
		if (grp->vis)
		    Show (ctrl);
	    } else {
		Hide (ctrl);
	    }
	    p->vis = vis;
	    break;
	}
    }
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*\
	     ABOUT BOX
\*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

static void 
QuitItemProc (IteM i)
{
    MyQuitProgram ();
}

static void 
CloseWindowProc (WindoW w)
{
    Remove (w);
}

static void 
CloseAboutPanelProc (PaneL p, PoinT pt)
{
    WindoW          w;

    w = ParentWindow (p);
    Remove (w);
}

static void 
CenterString (RectPtr rptr, CharPtr text, FonT fnt, Int2 inc)
{
    if (fnt != NULL) {
	SelectFont (fnt);
    }
    rptr->bottom = rptr->top + LineHeight ();
    DrawString (rptr, text, 'c', FALSE);
    rptr->top = rptr->bottom + inc;
}

static void 
DrawAbout (PaneL p)
{
    RecT            r;

    char            buf[50];

    ObjectRect (p, &r);
    InsetRect (&r, 4, 4);
    r.top += 10;
    CenterString (&r, "Entrez Configuration Program", _font1, 5);
    sprintf (buf, "%s Application Version 4.026", _program);
    CenterString (&r, buf, _font2, 10);
    CenterString (&r, "National Center for Biotechnology Information", systemFont, 5);
    CenterString (&r, "National Library of Medicine", systemFont, 5);
    CenterString (&r, "National Institutes of Health", systemFont, 10);
    CenterString (&r, "(301) 496-2475", systemFont, 5);
    CenterString (&r, _email_contact, systemFont, 0);
}

static void 
AboutProc (IteM i)
{
    PaneL           p;

    WindoW          w;

    w = ModalWindow (-50, -33, -1, -1, CloseWindowProc);
    p = SimplePanel (w, 30 * stdCharWidth, 12 * stdLineHeight, DrawAbout);
    SetPanelClick (p, NULL, NULL, NULL, CloseAboutPanelProc);
    Show (w);
}



/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*\
	     MS-Windows specific code
\*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

#if defined(WIN_MSWIN) && !defined(NETWORK_SUPPORTED) 

char 
DeduceFirstCdDriveLetter ()
{

    /*----- First, look in old config settings for first letter used
		    for either SEQDATA or REFDATA -----*/

    char            drive1, drive2;

    char            path[8];

    GetAppParam (_rcfile, "Entrez_Seq_CD", "root", "|", path, sizeof path);
    drive1 = path[0];
    GetAppParam (_rcfile, "Entrez_Ref_CD", "root", "|", path, sizeof path);
    drive2 = path[0];
    if (isalpha (drive1) || isalpha (drive2)) {
	return (char) MIN (toupper (drive1), toupper (drive2));
    }
    /*----- If that didnt work, try lookinf at the actual devices -----*/

#ifdef WIN16
    {
	/*
	 * With Win16 we can't really get what we want here.  The
	 * GetDriveType function returns DRIVE_REMOTE for both CD-ROMs and
	 * network drives.
	 */
	int             dr;

	for (dr = 'C'; dr <= 'Z'; ++dr) {
	    if (Nlm_GetDriveType (dr - 'A') == DRIVE_REMOTE)
		return dr;
	}
    }
#else
    {
	int             dr;

	for (dr = 'C'; dr <= 'Z'; ++dr) {
	    sprintf (path, "%c:\\", dr);
	    if (Nlm_GetDriveType (path) == DRIVE_CDROM)
		return dr;
	}
    }
#endif

    return 'D';
}

#endif

