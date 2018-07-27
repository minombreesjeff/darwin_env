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
* File Name: blast18p.h
*
* Author:  Jonathan Epstein
*
* Version Creation Date:   06/16/95
*
* $Revision: 1.4 $
*
* File Description: 
*       Header file to patch around constructs which the "ASNCODE" generator
*       is unable to deal with; this file is specific to the BLAST 1.8 ASN.1
*       specification.
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
* ==========================================================================
*
*
* RCS Modification History:
* $Log: blastpat.h,v $
* Revision 1.4  2000/08/28 16:38:09  kans
* included objsset.h
*
* Revision 1.3  2000/08/28 16:30:02  dondosha
* Added one #define
*
* Revision 1.2  1999/08/19 17:39:11  shavirin
* Added typedef for SeqAlign.
*
* Revision 1.1  1997/10/08 19:25:38  madden
* patch header for asncode
*
 * Revision 5.0  1996/05/28  14:09:11  ostell
 * Set to revision 5.0
 *
 * Revision 4.0  1995/07/26  13:55:34  ostell
 * force revision to 4.0
 *
 * Revision 1.2  1995/06/21  17:18:11  epstein
 * use Seq-align set instead of SET OF Seq-align (hack-around via SpecialSeqAlignSetAsn{Read,Write})
 *
 * Revision 1.1  95/06/16  11:26:14  epstein
 * Initial revision
 * 
*/

#include <objalign.h>
#include <objseq.h>
#include <objsset.h>
 
#define NLM_EXTERN_LOADS { SeqAlignAsnLoad(); }
 
#define struct_Score score

#define SeqAlignSetAsnWrite SpecialSeqAlignSetAsnWrite
#define SeqAlignSetAsnRead SpecialSeqAlignSetAsnRead

#define struct_Bioseq    bioseq
#define struct_Seq_align seqalign
#define struct_Bioseq_set seqset

