/* $Id: xmlblast.h,v 6.14 2004/04/29 19:55:35 dondosha Exp $ */
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
* $Revision: 6.14 $
*
* File Description:  Functions to print simplified BLAST output (XML)
*
* 
* $Log: xmlblast.h,v $
* Revision 6.14  2004/04/29 19:55:35  dondosha
* Mask filtered locations in query sequence lines
*
* Revision 6.13  2004/03/31 17:58:23  dondosha
* Added PSIXmlReset function to allow keeping the AsnIoPtr between outputs for multiple queries in blastpgp
*
* Revision 6.12  2003/01/06 23:01:40  dondosha
* Added function to create a multi-query XML output for web megablast
*
* Revision 6.11  2002/11/14 15:37:18  dondosha
* Added functions to extract all hit information from seqalign that can be extracted without loading sequences
*
* Revision 6.10  2002/07/17 22:28:13  dondosha
* Added support for megablast XML output
*
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
   AsnTypePtr BlastOutput_mbstat;
} PSIXml, MBXml, PNTR PSIXmlPtr, PNTR MBXmlPtr;

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
                        CharPtr message, ValNodePtr mask_loc);

Boolean BXMLPrintMultiQueryOutput(AsnIoPtr aip, SeqAlignPtr seqalign, 
           BLAST_OptionsBlkPtr options, CharPtr program, CharPtr database, 
           BioseqSetPtr query_set, ValNodePtr other_returns, Int4 flags,
           CharPtr message, ValNodePtr mask_loc);

StatisticsPtr BXMLBuildStatistics(ValNodePtr other_returns, Boolean ungapped);
BlastOutputPtr BXMLCreateBlastOutputHead(CharPtr program, CharPtr database, 
                                         BLAST_OptionsBlkPtr options, 
                                         BioseqPtr query, Int4 flags);

IterationPtr BXMLBuildOneIteration(SeqAlignPtr seqalign, 
                                   ValNodePtr other_returns,
                                   Boolean is_ooframe, Boolean ungapped,
                                   Int4 iter_num, CharPtr message, 
                                   ValNodePtr mask_loc);

IterationPtr BXMLBuildOneQueryIteration(SeqAlignPtr seqalign, 
                                   ValNodePtr other_returns,
                                   Boolean is_ooframe, Boolean ungapped,
                                   Int4 iter_num, CharPtr message, 
                                   BioseqPtr query, ValNodePtr mask_loc);

HspPtr BXMLGetHspFromSeqAlign(SeqAlignPtr sap, Boolean is_aa, Int4 chain,
                              Boolean is_ooframe, ValNodePtr mask_loc);

HitPtr BXMLSeqAlignToHits(SeqAlignPtr seqalign, Boolean ungapped, 
                          Boolean is_ooframe, ValNodePtr mask_loc);

PSIXmlPtr PSIXmlInit(AsnIoPtr aip, CharPtr program, CharPtr database, 
                     BLAST_OptionsBlkPtr options, BioseqPtr query, Int4 flags);
    
/* Close printing of this XML output, but only reset the ASN.1 output stream,
   so it is ready for the next output. */
void PSIXmlReset(PSIXmlPtr psixp);
/* Finish printing XML output and close the output stream */
void PSIXmlClose(PSIXmlPtr psixp);
void MBXmlClose(PSIXmlPtr mbxp, ValNodePtr other_returns, Boolean ungapped);

HitPtr SeqAlignToHits PROTO((SeqAlignPtr seqalign, Boolean ungapped));

#ifdef __cplusplus
/* { */ }
#endif

#endif /* XMLBLAST_H */
