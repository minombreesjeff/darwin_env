/*   vsmpriv.h
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
* File Name:  vsmpriv.h
*
* Author:  Jim Ostell
*
* Version Creation Date:   11-29-94
*
* $Revision: 6.1 $
*
* File Description: Internal data structures used by VSM routines
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* ==========================================================================
*/

#ifndef _NCBI_VSMPriv_
#define _NCBI_VSMPriv_

#include <vibrant.h>
#include <vsm.h>
#include <drawingp.h>
#include <panels.h>
#include <document.h>
#include <picture.h>
#include <viewer.h>
#include <sequtil.h>
#include <gather.h>

typedef struct vsmpict {
	Pointer vsmwin;           /* really the VSMWinPtr */
	Uint2 entityID;
	Int4 currx, curry;		  /* position of lower right corner */
	SegmenT s;                /* the picture for this entity */
	Int2 expansion;           /* level of picture expansion */
	Boolean deleted;          /* set when delete message received */
} VSMPict, PNTR VSMPictPtr;

typedef struct vsmwin {
	WindoW w;
	SegmenT picture;
	Uint2 entityID,    /* if 0, is the desktop */
		  itemID,
		  itemtype;
	Uint2 procid;      /* proc for drawing */
	Uint2 proctype;
	Uint2 userkey;
	OMMessageFunc msgfunc;   /* the message func for user objects */
	Pointer vsmp;       /* the VSeqMgrPtr */
	Boolean in_process,
		is_main_VSM,      /* special one which keeps values on quit */
		is_clipboard,     /* special one for clipboard */
		populated,
		shown,
		dblclick,
		shftkey,
		marquee,          /* marquee active */
		dragged;
	RecT rv,              /* viewer rect */
	     r;               /* marquee rect */
	PoinT lastpnt;
	Uint2 entityID1,      /* first thing clicked on */
	      itemID1,
	      itemtype1;
	struct vsmwin PNTR next;
}VSMWin, PNTR VSMWinPtr;

typedef struct vseqmgr {
	CharPtr appname;    /* points to application name for config file */
	ObjMgrPtr omp;
	WindoW w;
	Int2 xoff, yoff;  /* offset of viewer from window */
	Int2 x, y;      /* numbers to subtract from window rect for viewer */
	FonT font;
	Int2 lineheight,
		leading, charw;
	AsnTypePtr set_atp;
	VSMWinPtr wins;
	Uint2 procid;      /* for the main proc */
	Uint2 proctype;
	Uint2 highest_userkey;  /* for UserData */
	Boolean update_all;    /* refresh all pictures.. font change etc */
	Boolean show_clipboard,
		show_cached;
    Char lastASNtype[80];   /* last asn.1 type read in binary mode */
	Uint2 procmenu [OMPROC_MAX+1][20];   /* procids for up to 20 procs of each type to associate with menu */
	CharPtr aboutinfo, title;       /* used for about box */
	Boolean extraLevel;       /* extra level to show unexpanded Seq-annot */
} VSeqMgr, PNTR VSeqMgrPtr;

VSeqMgrPtr LIBCALL VSeqMgrGet PROTO((void)); /* in vsm.c */


#endif
