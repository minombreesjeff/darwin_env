/* ingenext.h

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
* File Name:  ingenext.h
*
* Author:  Fasika Aklilu
*
* Version Creation Date:   8/9/01
*
* $Revision: 6.5 $
*
* File Description: 
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* ==========================================================================
*/

#ifndef _INGENEXT_
#define _INGENEXT_

#ifdef __cplusplus
extern "C" {
#endif

#include <ingenwin.h>
#include <dotviewer.h>
#include <spidey.h>
#include <qblastapi.h>
#include <alignmgr2.h>

#define Ing_NUM_DBS 20
#define MAX_BLASTN_LEN 8000
#define MAX_BLASTP_LEN 4000
#define MAX_BLASTX_LEN 3000
#define MAX_TBLASTN_LEN 2000
#define OVERLAP_SPACE 1000
#define POST_PROCESS_BANDALGN  1
#define POST_PROCESS_NONE      2



/******************************************************
 *
 *  typedefs
 *
******************************************************/

typedef struct dotinfo{
  SeqAlignPtr sap;
  SeqIdPtr    sip1;
  SeqIdPtr    sip2;
  TexT from1;
  TexT to1;
  TexT from2;
  TexT to2;
  TexT wordsize;
  TexT numhits;
  TexT strand;
} DotInfo;

typedef struct ig_blastdlgdata {
  ValNode *pvnSips;               /* list of sips in structure */
  LisT bsp_list;          /* listbox of bsps */
  PopuP db_list;          /* listbox of databases */
  ButtoN bGap;   /* should the alignment be gapped? */
  TexT tMax;
  TexT tExpect;
  TexT wordsize;
  ButtoN     maskrep;
  ButtoN     masksimple;
  ButtoN     megablast;
  SeqAlign *salp;  /* resulting seqalign */
}Ing_BlastDlgData;



typedef struct ig_blast2info
{
  BioseqPtr  bsp1;
  BioseqPtr  bsp2;
  CharPtr    path;
  CharPtr    GI_list;
  GrouP      localorglobal;
  GrouP      progname;
  GrouP      gapped;
  TexT       from;
  TexT       to;
  TexT       eval;
  TexT       wordsize;
  ButtoN     maskrep;
  ButtoN     masksimple;
  ButtoN     megablast;
  IngGenomeViewerPtr igvp;
} IngBlast2Info, PNTR IngBlast2InfoPtr;


typedef struct ig_spideyform
{
  BioseqPtr  bsp1;
  BioseqPtr  bsp2;
  CharPtr    path;
  CharPtr    GI_list;
  ButtoN     inters;
  PopuP      org;
  TexT       idcutoff;
  TexT       lencutoff;
  TexT       firstpasseval;
  TexT       secondpasseval;
  TexT       thirdpasseval;
  TexT       numreturns;
  TexT       from;
  TexT       to;
} IngSpideyForm , PNTR IngSpideyFormPtr;


extern  Uint2 primID_cntr;

static CharPtr ig_nucdbs [] = {
  "nr", "est", "est_human", "est_mouse", "est_others", "gss","htgs", "pat", "yeast",  "mito", "vector", "ecoli", "pdb", "drosoph", "month", "sts", "kabat",
  "epd",  "alu", NULL
};

static CharPtr ig_nucdbsnames [] = {
  "nr", "est", "est_human", "est_mouse", "est_others", "gss (single-pass genomic)", "htgs", "patent", "yeast", "mito", "vector", "ecoli", "pdb (3-D prot)", "Drosophila", "month", "sts",  "kabat (immunologic)", "epd (eukaryotic promoter)",   "Alu repeats", NULL
};


/******************************************************
 *
 *  Function Declarations
 *
******************************************************/


#ifdef __cplusplus
}
#endif

#endif /* ndef _GENVIEW_ */

