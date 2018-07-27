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
* File Name: urksigu.h
*
* Author(s): John Kuzio
*
* Version Creation Date: 98-01-01
*
* $Revision: 6.4 $
*
* File Description: sigme utilities header
*
* Modifications:
* --------------------------------------------------------------------------
* Date       Name        Description of modification
* --------------------------------------------------------------------------
* $Log: urksigu.h,v $
* Revision 6.4  1998/11/24 15:40:54  kuzio
* refine boundary condition for multiple potential leader pepides
*
* Revision 6.3  1998/11/16 14:29:55  kuzio
* flagBoundaryCondition
*
* Revision 6.2  1998/09/16 17:46:47  kuzio
* cvs logging
*
*
* ==========================================================================
*/

#ifndef _URKSIGU__
#define _URKSIGU__

#include <ncbi.h>
#include <seqport.h>
#include <urkptpf.h>

#ifdef __cplusplus
extern "C" {
#endif

extern SeqLocPtr CheckOverlap (SeqLocPtr slp, Int4 ctermsig);
extern Int4 EndOfSig (SeqLocPtr slp);

extern SeqLocPtr FilterSigSeq (BioseqPtr bsp,
                               ComProfPtr pppl, ComProfPtr pppc,
                               FloatHi leadcutoff, FloatHi cutcutoff,
                               Int4 range, SeqIdPtr sip,
                               Boolean flagBoundaryCondition,
                               Boolean flagReportIfAllFuzzyOnly);

#ifdef __cplusplus
}
#endif

#endif
