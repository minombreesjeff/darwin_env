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
* File Name: urkptpf.h
*
* Author(s): John Kuzio
*
* Version Creation Date: 98-01-01
*
* $Revision: 6.20 $
*
* File Description: patterns and profiles header
*
* Modifications:
* --------------------------------------------------------------------------
* Date       Name        Description of modification
* --------------------------------------------------------------------------
* $Log: urkptpf.h,v $
* Revision 6.20  1999/07/13 13:07:05  sicotte
* prefix functions SeqAlignSort* with URK to resolve toolkit conflicts urkptpf.c
*
* Revision 6.19  1998/11/16 14:29:54  kuzio
* flagBoundaryCondition
*
* Revision 6.18  1998/09/16 17:46:46  kuzio
* cvs logging
*
*
* ==========================================================================
*/

#ifndef _URKPTPF__
#define _URKPTPF__

#include <objres.h>
#include <objseq.h>
#include <objsset.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAXMATCH     1024
#define MAXPATSIZE   4096
#define MAXPROFSIZE  128
#define MAXCHUNKSIZE 32768

typedef struct compilepattern
{
  Char    name[256];
  Int4    min;
  Int4    max;
  Char    res[64];
  Boolean flagPalindrome;
  Boolean flagNTerm, flagCTerm;
  struct  compilepattern PNTR prev;
  struct  compilepattern PNTR next;
  struct  compilepattern PNTR prevpattern;
  struct  compilepattern PNTR nextpattern;
} ComPat, PNTR ComPatPtr;

typedef struct compileprofile
{
  Char    name[256];
  FloatHi score[64];
  Int4    iscore[64];
  struct  compileprofile PNTR prev;
  struct  compileprofile PNTR next;
  struct  compileprofile PNTR prevprofile;
  struct  compileprofile PNTR nextprofile;
} ComProf, PNTR ComProfPtr;

/* patterns and profiles */

extern SeqAlignPtr MeldStrands (SeqAlignPtr sap);
extern SeqAlignPtr SeparateStrands (SeqAlignPtr sap);

/* patterns */

extern ValNodePtr ReadPatternNames (CharPtr filename);
extern ComPatPtr ReadRENPattern (CharPtr filename, Boolean flagNeedCutPos,
                                 ValNodePtr namelist);
extern ComPatPtr ReadPrositePattern (CharPtr filename, Boolean flagStringent,
                                     Int4 cutoff, CharPtr taxon,
                                     ValNodePtr namelist);
extern Boolean IsProsite (CharPtr inbuff);
extern CharPtr PrositeToGBPattern (CharPtr pspat);

extern CharPtr ReadPattern (CharPtr filename);
extern void CleanPattern (CharPtr inbuff);

extern ComPatPtr ComPatNew (ComPatPtr currcpp);
extern ComPatPtr ComPatFree (ComPatPtr headcpp);
extern ComPatPtr ComPatDup (ComPatPtr cppold);

extern ComPatPtr CompilePattern (CharPtr pattern, Int2 moltype);
extern ComPatPtr InvertPattern (ComPatPtr cppold);
extern void PalindromeCheck (ComPatPtr cpph);

extern SeqAlignPtr PatternMatch (Uint1Ptr seq, Int4 seqpos, Uint1 strand,
                                 SeqIdPtr sip, ComPatPtr cpp,
                                 Int4 mismatch, Int2 topology,
                                 Boolean flagFindAllMatches);
extern SeqAlignPtr PatternMatchBioseq (BioseqPtr bsp, ComPatPtr cpp,
                                       Int4 mismatch);

/* profiles */

extern ComProfPtr ReadProfile (CharPtr filename);

extern ComProfPtr ComProfNew (ComProfPtr currppp);
extern ComProfPtr ComProfFree (ComProfPtr headppp);
extern ComProfPtr ComProfDup (ComProfPtr pppold);
extern ComProfPtr ComProfDupThis (ComProfPtr pppthis);
extern void ComProfLink (ComProfPtr PNTR ppphead, ComProfPtr pppdup);

extern void IntegerProfile (ComProfPtr ppp);
extern ComProfPtr InvertProfile (ComProfPtr pppold);
extern FloatHi ProfScoreMax (ComProfPtr ppp);
extern Int4 ProfLenMax (ComProfPtr ppp);
extern Int4 CatenateProfile (ComProfPtr ppp, Int4 proflen, Int4 icount);

extern SeqAlignPtr ProfileMatch (Uint1Ptr seq, Int4 seqpos, FloatHi cutoff,
                                 Uint1 strand, SeqIdPtr sip, ComProfPtr ppp,
                                 Int2 topology, Boolean flagFindAllMatches,
                                 Boolean flagBoundaryCondition);
extern SeqAlignPtr ProfileMatchBioseq (BioseqPtr bsp, ComProfPtr ppp,
                                       ComProfPtr ippp, FloatHi cutoff,
                                       Boolean flagBoundaryCondition);
extern SeqAlignPtr IntProfileMatchBioseq (BioseqPtr bsp, ComProfPtr ppp,
                                          ComProfPtr ippp, Int4 cutoff,
                                          Boolean flagBoundaryCondition);

/* SeqAligns SeqLocs */

extern SeqLocPtr MatchSa2Sl (SeqAlignPtr PNTR saph);
extern void EmbedFragLengthInfo (SeqAlignPtr saph, Int4 length);
extern void EmbedMolecularWeightInfo (SeqAlignPtr saph, BioseqPtr bsp);
extern void URK_SeqAlignSortByStart (SeqAlignPtr PNTR sap);
extern void URK_SeqAlignSortByLength (SeqAlignPtr PNTR sap);
extern void URK_SeqAlignSortByMolWt (SeqAlignPtr PNTR sap);
extern void URK_SeqAlignSortByScore (SeqAlignPtr PNTR sap);

#ifdef __cplusplus
}
#endif

#endif
