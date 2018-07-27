/*  $Id: seqsrc_multiseq.h,v 1.9 2005/04/06 23:27:53 dondosha Exp $
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
*  Author:  Ilya Dondoshansky
* ===========================================================================*/

/** @file seqsrc_multiseq.h
 * C Implementation of the BlastSeqSrc interface for a list of sequence 
 * locations.
 */

#ifndef MULTISEQ_SRC_H
#define MULTISEQ_SRC_H

#include <algo/blast/core/blast_seqsrc.h>
#include <algo/blast/core/blast_def.h>
#include <algo/blast/core/blast_message.h>
#include <objloc.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup CToolkitAlgoBlast
 *
 * @{
 */

/** Initialize the sequence source structure.
 * @param seqloc_list Linked list of sequence locations [in]
 * @param program Type of BLAST to be performed [in]
 */
BlastSeqSrc* 
MultiSeqBlastSeqSrcInit(SeqLoc* seqloc_list, EBlastProgramType program);

/* @} */

#ifdef __cplusplus
}
#endif

#endif /* MULTISEQ_SRC_H */
