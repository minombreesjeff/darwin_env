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
* ===========================================================================*/

/*****************************************************************************

File name: dotmatrx.c

Author: Tom Madden

*****************************************************************************/
#include <dotmatrx.h>
#include <objmgr.h>
#include <dlogutil.h>
#include <vibrant.h>
#include <picture.h>
#include <viewer.h>
#include <objseq.h>
#include <objsset.h>
#include <salsap.h>
#include <blast.h>
#include <salpedit.h>

typedef struct dotmatrixform {
  FEATURE_FORM_BLOCK
  PopuP        scale;
  GrouP        showLabels;
  VieweR       vwr;
  SegmenT      pict;
  DotMatrixAlignmentPtr	dmap;
  int (LIBCALLBACK *user_callback)PROTO((SeqAlignPtr seqalign));
  Boolean      scaleNotCalculated;
} DotMatForm, PNTR DotMatFormPtr;

#define MAXZOOMSCALEVAL 22

static Int4  zoomScaleVal [MAXZOOMSCALEVAL] = {
  1L, 1L, 2L, 3L, 4L, 5L, 6L, 7L, 8L, 9L, 10L, 20L,
  30L, 40L, 50L, 60L, 70L, 80L, 90L, 100L, 200L, 500L
};


static Boolean DeselectSegment (SegmenT seg, PrimitivE prim, Uint2 segID,
                                Uint2 primID, Uint2 primCt, VoidPtr userdata)

{
  DotMatFormPtr  dfp;

  dfp = (DotMatFormPtr) userdata;
  if (dfp == NULL) 
	return FALSE;
  HighlightSegment (dfp->vwr, seg, PLAIN_SEGMENT);
  return TRUE;
}

static SeqAlignPtr FindSeqAlign(DotMatFormPtr dfp, Int4 segID)

{
	DenseDiagPtr ddp, ddp_new;
	DotMatrixAlignmentPtr dmap;
	SeqAlignPtr seqalign, new_seqalign=NULL;
	Int4 index=0;

	dmap = dfp->dmap;
	while (dmap)
	{
		if (dmap->sap->segtype == 2)
		{
			seqalign = dmap->sap;
		}
		else
		{	/* discontinuous */
			seqalign = dmap->sap->segs;
		}
		
	  	while (seqalign != NULL) 
	  	{
	    		index++;
			if (index == segID)
			{
				break;
			}
	    		seqalign = seqalign->next;
		}
		
		if (seqalign)
		{
			if (seqalign->segtype == 1)
			{
				new_seqalign = SeqAlignNew();
				new_seqalign->type = 2;
				new_seqalign->segtype = 1;
				ddp = seqalign->segs;
				new_seqalign->segs = ddp_new = DenseDiagDup(ddp);	
				while (ddp->next)
				{
					ddp_new->next = DenseDiagDup(ddp->next);
					ddp = ddp->next;
					ddp_new = ddp_new->next;
				}
			}
			else if (seqalign->segtype == 2)
			{
				new_seqalign = SeqAlignDup(seqalign);
			}
		}
		dmap = dmap->next;
	}

	return new_seqalign;
}

static void ClickDotMatrix (VieweR vwr, SegmenT seg, PoinT pt)

{
  DotMatFormPtr  dfp;
  Int1           highlight;
  SegmenT        new_seg;
  PrimitivE      prim;
  SeqAlignPtr	 seqalign;
  Uint2          primCT;
  Uint2          primID;
  Uint2          segID;

	dfp = (DotMatFormPtr) GetObjectExtra (vwr);
	new_seg = FindSegment (vwr, pt, &segID, &primID, &primCT);
	if (new_seg == NULL) 
	{
		ExploreSegment(seg, (Pointer) dfp, DeselectSegment);
	} 
	else 
	{
		prim = GetPrimitive (new_seg, primCT);
		GetPrimDrawAttribute (prim, NULL, NULL, NULL, NULL, NULL, &highlight);
		if (highlight != PLAIN_SEGMENT) 
		{
			HighlightSegment (vwr, new_seg, PLAIN_SEGMENT);
		} 
		else 
		{
			HighlightSegment (vwr, new_seg, FILL_CONTENTS);
			seqalign = FindSeqAlign(dfp, segID);
			if (dblClick && dfp->user_callback)
				dfp->user_callback(seqalign);
		}
 	}
}
static SegmenT 
CreateDotMatrixPic (DotMatrixAlignmentPtr dmap, Boolean showLabels)

{
  Char          buffer [50];
  DenseDiagPtr  ddp;
  DenseSegPtr	dsp;
  SeqAlignPtr   sap;
  Int4          index, index1;
  SegmenT       seg;
  Int4          x;
  Int4          y;
  SegmenT 		pict;

  if (dmap == NULL || dmap->sap == NULL) 
	return NULL;
  pict = CreatePicture ();

  while (dmap)
  {
	  sap = dmap->sap;
	  if (sap == NULL)
		break;
	  if (sap->segtype != 2)
	  {	/* discontinuous */
	  	sap = sap->segs;
	  }
	  index = 0;
	  while (sap != NULL) 
	  {
	    seg = CreateSegment (pict, index + 1, 0);
	
	    AddAttribute (seg, COLOR_ATT, dmap->color, 0, 0, 0, 0);
	
	    if (sap->segtype == 1)
	    {
	    	ddp = sap->segs;
	    	while (ddp != NULL) 
	    	{
	    		AddLine (seg, ddp->starts [0], ddp->starts [1], ddp->starts [0] + ddp->len, ddp->starts [1] + ddp->len, FALSE, 0);
		
	      		if (showLabels) 
	      		{
	       	 		x = ddp->starts [0];
	        		y = ddp->starts [1];
				sprintf (buffer, "(%ld, %ld)", (long) x + 1, (long) y + 1);
	       			AddLabel (seg, x, y, buffer, SMALL_TEXT, 0, LOWER_RIGHT, 0);
	
	        		x = ddp->starts [0] + ddp->len;
	        		y = ddp->starts [1] + ddp->len;
	        		sprintf (buffer, "(%ld, %ld)", (long) x, (long) y);
	        		AddLabel (seg, x, y, buffer, SMALL_TEXT, 0, UPPER_RIGHT, 0);
	      		}
	
	      		ddp = ddp->next;
	    	}
	    }
	    else if (sap->segtype == 2)
	    {
		dsp = sap->segs;
		for (index1=0; index1<dsp->numseg; index1++)
		{
			if (dsp->starts[2*index1] != -1 && dsp->starts[2*index1+1] != -1)
			{
	    			AddLine (seg, dsp->starts[2*index1], dsp->starts[2*index1+1], dsp->starts[2*index1] + dsp->lens[index1], dsp->starts[2*index1+1] + dsp->lens[index1], FALSE, 0);
	      			if (showLabels) 
	      			{
					x = dsp->starts[2*index1];
					y = dsp->starts[2*index1+1];
					sprintf (buffer, "(%ld, %ld)", (long) x + 1, (long) y + 1);
	       				AddLabel (seg, x, y, buffer, SMALL_TEXT, 0, LOWER_RIGHT, 0);
					x = dsp->starts[2*index1] + dsp->lens[index1];
					y = dsp->starts[2*index1+1] + dsp->lens[index1];
					sprintf (buffer, "(%ld, %ld)", (long) x + 1, (long) y + 1);
	       				AddLabel (seg, x, y, buffer, SMALL_TEXT, 0, UPPER_RIGHT, 0);
				}
			}
		}
	    }
	    index++;
	    sap = sap->next;
  	}
	dmap = dmap->next;
  }
  return pict;
}

static void MakeDotMatrixPicture (DotMatFormPtr dfp)

{
  Boolean       showLabels;

  if (dfp == NULL || dfp->dmap == NULL) 
	return;

  showLabels = (Boolean) (GetValue (dfp->showLabels) == 1);
  dfp->pict = CreateDotMatrixPic (dfp->dmap, showLabels);

}
static void RepopulateDotMatrixViewer (DotMatFormPtr dfp)

{
  Int2  index;
  Int4  scaleX;
  Int4  scaleY;
  Char  str [16];

  if (dfp == NULL) 
	return;

  Reset (dfp->vwr);
  dfp->pict = DeletePicture (dfp->pict);
  Update ();
  MakeDotMatrixPicture (dfp);
  if (dfp->scaleNotCalculated) 
  {
    SafeHide (dfp->scale);
    Reset (dfp->scale);
    for (index=1; index<=20; index++) 
    {
      sprintf (str, "%ld", (long) (zoomScaleVal [index]));
      PopupItem (dfp->scale, str);
    }
    SetValue (dfp->scale, 5);
    dfp->scaleNotCalculated = FALSE;
  }
  SafeShow (dfp->scale);
  index = GetValue (dfp->scale);
  if (index < MAXZOOMSCALEVAL && index > 0) 
  {
    scaleX = zoomScaleVal [index];
  } 
  else 
  {
    scaleX = 1;
  }
  scaleY = scaleX;
  AttachPicture (dfp->vwr, dfp->pict, INT4_MIN, INT4_MAX, UPPER_LEFT, scaleX, scaleY, NULL);
  SetViewerProcs (dfp->vwr, ClickDotMatrix, NULL, NULL, NULL);
  Update ();
}

static void ChangeScale (PopuP p)

{
  DotMatFormPtr  dfp;

  dfp = (DotMatFormPtr) GetObjectExtra (p);
  if (dfp != NULL) 
  {
    RepopulateDotMatrixViewer (dfp);
  }
}

static void ChangeLabels (GrouP g)

{
  DotMatFormPtr  dfp;

  dfp = (DotMatFormPtr) GetObjectExtra (g);
  if (dfp != NULL) 
  {
    RepopulateDotMatrixViewer (dfp);
  }
}

static void ResizeDotMatrixForm (WindoW w)

{
  DotMatFormPtr  dfp;
  Int2           height;
  RecT           r;
  RecT           s;
  Int2           width;

  dfp = (DotMatFormPtr) GetObjectExtra (w);
  if (dfp != NULL) 
  {
    ObjectRect (w, &r);
    width = r.right - r.left;
    height = r.bottom - r.top;
    if (dfp->vwr != NULL) 
    {
      GetPosition (dfp->vwr, &s);
      s.right = width - s.left;
      s.bottom = height - s.left;
      SetPosition (dfp->vwr, &s);
      AdjustPrnt (dfp->vwr, &s, FALSE);
      if (Visible (dfp->vwr) && AllParentsVisible (dfp->vwr)) 
      {
      		ViewerWasResized (dfp->vwr);
      }
    }
    Update ();
  }
}

static void CleanupDotMatrixForm (GraphiC g, VoidPtr data)

{
  DotMatFormPtr  dfp;
  DotMatrixAlignmentPtr dmap;

  dfp = (DotMatFormPtr) data;
  if (dfp != NULL) 
  {
    dfp->pict = DeletePicture (dfp->pict);
    dmap = dfp->dmap;
    while (dmap)
    {
    	dmap->sap = SeqAlignFree (dmap->sap);
	dmap = dmap->next;
    }
  }
  StdCleanupFormProc (g, data);
}

static void HideDotMatrix (ButtoN i)

{
	WindoW w;

	w = (WindoW)ParentWindow (i);
	Remove(w);
	
}

DotMatrixAlignmentPtr LIBCALL
DotMatrixAlignmentNew (SeqAlignPtr sap, Uint1Ptr color, DotMatrixAlignmentPtr PNTR oldp)

{
	DotMatrixAlignmentPtr new, old;

	new = MemNew(sizeof(DotMatrixAlignment));

	if (new == NULL || oldp == NULL)
		return NULL;

	new->sap = sap;
	new->color = color;

	if (*oldp)
	{
		old = *oldp;
		while (old->next)
			old = old->next;

		old->next = new;
	}
	else
	{
		*oldp = new;
	}
	return new;
}

#define BLAST_DOTMATRIX_EXTENT 600	/* Size of screen. */

ForM LIBCALL CreateDotMatrixForm (SeqAlignPtr sap, int (LIBCALLBACK *callback)PROTO((SeqAlignPtr seqalign)))

{
	DotMatrixAlignmentPtr dmap, tmp;

	tmp = NULL;
	dmap = DotMatrixAlignmentNew(sap, NULL, &tmp);
	return CreateDotMatrixFormEx(dmap, callback);
}

ForM LIBCALL CreateDotMatrixFormEx (DotMatrixAlignmentPtr dmap, int (LIBCALLBACK *callback)PROTO((SeqAlignPtr seqalign)))

{
  DotMatFormPtr  dfp;
  GrouP          g;
  PrompT         p1;
  PrompT         p2;
  GrouP          s, x;
  WindoW         w;

  w = NULL;
  dfp = (DotMatFormPtr) MemNew (sizeof (DotMatForm));
  if (dfp != NULL) 
  {
    dfp->dmap = dmap;
    dfp->user_callback = callback;

    w = DocumentWindow (-40, -33, -10, -10, "Dot Matrix Display",
                        StdCloseWindowProc, ResizeDotMatrixForm);
    SetObjectExtra (w, dfp, CleanupDotMatrixForm);
    dfp->form = (ForM) w;

    x = HiddenGroup (w, 0, 1, NULL);
    StaticPrompt (x, "Dot Matrix showing the BLAST hits between 2 sequences", 0, 0, programFont, 'l');

    s = HiddenGroup (w, 5, 0, NULL);
    SetGroupSpacing (s, 7, 2);

    p1 = StaticPrompt (s, "Labels", 0, 0, programFont, 'l');
    dfp->showLabels = HiddenGroup (s, 3, 0, ChangeLabels);
    SetObjectExtra (dfp->showLabels, dfp, NULL);
    RadioButton (dfp->showLabels, "Show");
    RadioButton (dfp->showLabels, "Hide");
    SetValue (dfp->showLabels, TRUE);

    p2 = StaticPrompt (s, "Scale", 0, popupMenuHeight, programFont, 'l');
    dfp->scale = PopupList (s, TRUE, ChangeScale);
    SetObjectExtra (dfp->scale, dfp, NULL);

    AlignObjects (ALIGN_MIDDLE, (HANDLE) p1, (HANDLE) dfp->showLabels, (HANDLE) p2, (HANDLE) dfp->scale, NULL);

    x = HiddenGroup (w, 0, 2, NULL);
    StaticPrompt (x, "Decrease scale to zoom in", 0, 0, programFont, 'l');
    StaticPrompt (x, "Click on diagonals to launch the alignment editor", 0, 0, programFont, 'l');

    g = HiddenGroup (w, 2, 0, NULL);
    dfp->vwr = CreateViewer (g, BLAST_DOTMATRIX_EXTENT, BLAST_DOTMATRIX_EXTENT, TRUE, TRUE);
    SetObjectExtra (dfp->vwr, dfp, NULL);
    dfp->pict = NULL;
    dfp->scaleNotCalculated = TRUE;

    PushButton (w, "Dismiss", HideDotMatrix );

    RealizeWindow (w);
    RepopulateDotMatrixViewer (dfp);
  }
  return (ForM) w;
}

Int2 LIBCALL DotMatrixSearch(SeqIdPtr sip1, SeqIdPtr sip2, int (LIBCALLBACK *callback)PROTO((SeqAlignPtr seqalign)))

{
	BioseqPtr query_bsp, subject_bsp;
	BLAST_OptionsBlkPtr options;
	Boolean is_na;
	SeqAlignPtr seqalign;
	WindoW  window;


	query_bsp = BioseqLockById(sip1);
	subject_bsp = BioseqLockById(sip2);
	if (query_bsp == NULL || subject_bsp == NULL)
	{
                ErrPostEx(SEV_WARNING, 0, 0, "Unable to obtain sequences");
                return 1;
	}

	if (ISA_na(query_bsp->mol) != ISA_na(subject_bsp->mol))
	{
                ErrPostEx(SEV_WARNING, 0, 0, "Sequences are of different types");
                return 1;
	}

	is_na = ISA_na(query_bsp->mol);

	options = BLASTOptionNew((is_na == TRUE) ? "blastn":"blastp", TRUE);

        seqalign = BlastTwoSequences(query_bsp, subject_bsp, NULL, options);

        if (seqalign == NULL)
        {
                ErrPostEx(SEV_WARNING, 0, 0, "No hits found");
                return 0;
        }

        window = (WindoW) CreateDotMatrixForm(seqalign, callback);

        Show(window);

	BioseqUnlockById(sip1);
	BioseqUnlockById(sip2);

	return 0;
}

Int2 LIBCALLBACK DotMatrixGenFunc (Pointer data)

{
  OMProcControlPtr  ompcp;
  SeqAlignPtr       sap;
  WindoW            w;

  ompcp = (OMProcControlPtr) data;
  if (ompcp == NULL || ompcp->proc == NULL) 
	return OM_MSG_RET_ERROR;

  sap = NULL;
  switch (ompcp->input_itemtype) 
  {
    case OBJ_SEQALIGN :
      sap = (SeqAlignPtr) ompcp->input_data;
      break;
    case 0 :
      return OM_MSG_RET_ERROR;
    default :
      return OM_MSG_RET_ERROR;
  }
  w = (WindoW) CreateDotMatrixForm (sap, NULL);
  Show (w);
  Select (w);
  return OM_MSG_RET_DONE;
}
