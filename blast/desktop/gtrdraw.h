/*
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
* File Name: gtrdraw.h
*
* Author(s): John Kuzio
*
* Version Creation Date: 98-01-01
*
* $Revision: 6.2 $
*
* File Description: sentinel trees header
*
* Modifications:
* --------------------------------------------------------------------------
* Date       Name        Description of modification
* --------------------------------------------------------------------------
* $Log: gtrdraw.h,v $
* Revision 6.2  1998/09/16 19:00:37  kuzio
* cvs logs
*
* ==========================================================================
*/

#ifndef _GTRDRAW_
#define _GTRDRAW_

#include <picture.h>
#include <urktree.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct graphtreesentdata {
  Boolean        flagIsGUI;
  SegmenT        seg;
  PrimitivE      snt;
  Int4Ptr        epa;
  ValNodePtr     vnpnames;
  Int2           fontsize;
  Boolean        flagLabelNodes;
  Int4Ptr        offsetX;
  Int4Ptr        offsetY;
  Int4           numval;
} GraphTreeSentData, PNTR GraphTreeSentPtr;

extern SegmenT DrawTree (TreeNodePtr ptrNode);

#ifdef __cplusplus
}
#endif

#endif
