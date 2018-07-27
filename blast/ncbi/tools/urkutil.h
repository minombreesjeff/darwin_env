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
* File Name: urkutil.h
*
* Author(s): John Kuzio
*
* Version Creation Date: 98-01-01
*
* $Revision: 6.3 $
*
* File Description: urk utilities header
*
* Modifications:
* --------------------------------------------------------------------------
* Date       Name        Description of modification
* --------------------------------------------------------------------------
* $Log: urkutil.h,v $
* Revision 6.3  1999/01/27 15:08:42  kuzio
* ErrorDescString
*
* Revision 6.2  1998/09/16 14:20:10  kuzio
* testing cvs logging on boilerplate (?)
*
*
* ==========================================================================
*/

#ifndef _URKUTIL__
#define _URKUTIL__

#include <ncbi.h>
#include <objseq.h>

#ifdef __cplusplus
extern "C" {
#endif

#define L_T      256
#define L_ID     32
#define L_DEF    220

extern CharPtr FastaTitle (BioseqPtr bsp, CharPtr pretext, CharPtr posttext);
extern CharPtr ErrorDescString (SeqIdPtr sip);

#ifdef __cplusplus
}
#endif

#endif
