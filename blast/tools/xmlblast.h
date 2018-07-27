/* $Id: xmlblast.h,v 6.9 2000/11/28 20:51:58 shavirin Exp $ */
/**************************************************************************
*                                                                         *
*                             COPYRIGHT NOTICE                            *
*                                                                         *
* This software/database is categorized as "United States Government      *
* Work" under the terms of the United States Copyright Act.  It was       *
* produced as part of the author's official duties as a Government        *
* employee and thus can not be copyrighted.  This software/database is    *
* freely available to the public for use without a copyright notice.      *
* Restrictions can not be placed on its present or future use.            *
*                                                                         *
* Although all reasonable efforts have been taken to ensure the accuracy  *
* and reliability of the software and data, the National Library of       *
* Medicine (NLM) and the U.S. Government do not and can not warrant the   *
* performance or results that may be obtained by using this software,     *
* data, or derivative works thereof.  The NLM and the U.S. Government     *
* disclaim any and all warranties, expressed or implied, as to the        *
* performance, merchantability or fitness for any particular purpose or   *
* use.                                                                    *
*                                                                         *
* In any work or product derived from this material, proper attribution   *
* of the author(s) as the source of the software or data would be         *
* appreciated.                                                            *
*                                                                         *
************************************************************************** 
* File Name:  xmlblast.c
*
* Author:  Sergei B. Shavirin
*   
* Version Creation Date: 05/17/2000
*
* $Revision: 6.9 $
*
* File Description:  Functions to print simplified BLAST output (XML)
*
* 
* $Log: xmlblast.h,v $
* Revision 6.9  2000/11/28 20:51:58  shavirin
* Adopted for usage with mani-iterational XML definition.
*
* Revision 6.8  2000/11/22 21:55:49  shavirin
* Added function BXMLPrintOutputEx() with new parameter iteration_number
* for usage with PSI-Blast.
*
* Revision 6.7  2000/11/07 21:50:52  shavirin
* Added external definition of the function  BXMLSeqAlignToHits().
*
* Revision 6.6  2000/10/24 17:49:42  egorov
* Remove blstxml.h because it makes impossible to use this ASN.1 spec together
* with another ASN.1 spec
*
* Revision 6.5  2000/10/23 19:55:15  dondosha
* Changed prototype of function BXMLPrintOutput
*
* Revision 6.4  2000/10/12 21:35:31  shavirin
* Added support for OOF alignment.
*
* Revision 6.3  2000/10/12 15:46:19  shavirin
* Added definition of the function BXMLGetHspFromSeqAlign().
*
* Revision 6.2  2000/08/10 14:42:33  shavirin
* Added missing comment.
*
* Revision 6.1  2000/08/10 13:58:36  shavirin
* Initial revision.
* *
*
*/

#ifndef XMLBLAST_H
#define XMLBLAST_H

#include <ncbi.h>
#include <readdb.h>
#include <txalign.h>
#include <bxmlobj.h>
#include <blastdef.h>
#include <blastpri.h>

#ifdef __cplusplus
extern "C" { /* } */
#endif

typedef struct PSIXml {
    BlastOutputPtr boutp;
    AsnIoPtr   aip;
    AsnTypePtr atp;
    AsnTypePtr BlastOutput;
    AsnTypePtr BlastOutput_iterations;
} PSIXml, PNTR PSIXmlPtr;

#define BXML_INCLUDE_QUERY 0x1

#define MACRO_atp_find(atp,name)\
        if((atp = AsnTypeFind(amp, #name))==NULL){\
                ErrPostEx(SEV_ERROR,0,0,\
                        "Could not find type <%s>", #name);\
                return NULL; \
        }

Boolean BXMLPrintOutput(AsnIoPtr aip, SeqAlignPtr seqalign, 
                        BLAST_OptionsBlkPtr options, CharPtr program,
                        CharPtr database, BioseqPtr query, 
                        ValNodePtr other_returns, Int4 option, 
                        CharPtr message);

BlastOutputPtr BXMLCreateBlastOutputHead(CharPtr program, CharPtr database, 
                                         BLAST_OptionsBlkPtr options, 
                                         BioseqPtr query, Int4 flags);

IterationPtr BXMLBuildOneIteration(SeqAlignPtr seqalign, 
                                   ValNodePtr other_returns,
                                   Boolean is_ooframe, Boolean ungapped,
                                   Int4 iter_num, CharPtr message);

HspPtr BXMLGetHspFromSeqAlign(SeqAlignPtr sap, Boolean is_aa, Int4 chain,
                              Boolean is_ooframe);

HitPtr BXMLSeqAlignToHits(SeqAlignPtr seqalign, Boolean ungapped, 
                          Boolean is_ooframe);

PSIXmlPtr PSIXmlInit(AsnIoPtr aip, CharPtr program, CharPtr database, 
                     BLAST_OptionsBlkPtr options, 
                     BioseqPtr query, Int4 flags);
    
void PSIXmlClose(PSIXmlPtr psixp);


#ifdef __cplusplus
/* { */ }
#endif

#endif /* XMLBLAST_H */
